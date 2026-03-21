#include "gran.h"
#include <od/config.h>
#include <hal/ops.h>
#include <hal/simd.h>
#include <cstring>
#include <cmath>

// ── Fast cosine approximation ──────────────────────────────────────
// Bhaskara-I inspired, max error ~0.001, avoids libm entirely.
static inline float fast_cos(float x)
{
    const float TP  = 1.0f / (2.0f * 3.14159265f);
    x *= TP;
    x -= 0.25f + floorf(x + 0.25f);
    x *= 16.0f * (fabsf(x) - 0.5f);
    x += 0.225f * x * (fabsf(x) - 1.0f);
    return x;
}

// ── Fast soft-clip (tanh approximation) ────────────────────────────
// Pade approximant, stays in [-1,1], smooth saturation.
static inline float fast_tanh(float x)
{
    if (x < -3.0f) return -1.0f;
    if (x >  3.0f) return  1.0f;
    float x2 = x * x;
    return x * (27.0f + x2) / (27.0f + 9.0f * x2);
}

// ── LCG pseudo-random (Faust-compatible) ───────────────────────────
static inline float lcg_rand(int &state)
{
    state = 1103515245 * state + 12345;
    return 0.5f * (4.656613e-10f * (float)state + 1.0f);
}

// ── gran Constructor ───────────────────────────────────────────────
gran::gran() : DSP()
{
  UI uiParams(this);

  addInput(mIn);
  addOutput(mOut);
  addOption(mEnvShape);
  addParameter(mGrainSize);
  addParameter(mProbability);
  addParameter(mSpeed);
  DSP.init(globalConfig.sampleRate);
  DSP.buildUserInterface(&uiParams);

  // Init voice state
  memset(voiceTimer, 0, sizeof(voiceTimer));
  memset(voiceDelay, 0, sizeof(voiceDelay));
  // Park all voices beyond grain size so they don't produce output on init
  for (int v = 0; v < 8; v++) voiceTimer[v] = 999999;
  phasor = 0.0f;
  rngState = 12345;
  randVal = 0.5f;
  voiceCounter = 0;
}

gran::~gran()
{ }

// ── Optimised process() ────────────────────────────────────────────
void gran::process()
{
    float* __restrict__ in  = mIn.buffer();
    float* __restrict__ out = mOut.buffer();

    // ── Block-rate parameter extraction (once per FRAMELENGTH) ──
    const float sr            = (float)globalConfig.sampleRate;
    const float grainSizeSec  = CLAMP(mGrainSize.target(), 0.001f, 2.0f);
    const float prob          = CLAMP(mProbability.target(), 0.0f, 1.0f);
    const float speedHz       = CLAMP(mSpeed.target(), 1.0f, 400.0f);
    // Option is 1-indexed for Lua compatibility: 1=Hann, 2=Blackman, 3=Triangle, 4=Tukey
    const int   envShape      = CLAMP(mEnvShape.value(), 1, 4) - 1;

    const float grainSizeSamps = fmaxf(1.0f, grainSizeSec * sr);
    const float phasorInc      = speedHz / sr;
    const float invGrain       = 1.0f / grainSizeSamps;
    const float twoPiInvGrain  = 6.2831853f * invGrain;
    const float fourPiInvGrain = 12.566371f * invGrain;
    const float twoInvGrain    = 2.0f * invGrain;
    const float maxDelaySamps  = 131072.0f - grainSizeSamps;

    // Normalization: scale output by 1/sqrt(NUM_VOICES) for headroom
    const float normGain = 1.0f / 2.8284f; // 1/sqrt(8) ≈ 0.354

    // ── sample loop ────────────────────────────────────────────
    for (int i = 0; i < FRAMELENGTH; i++)
    {
        // Write input into circular delay buffer
        delayBuf[writePos & 0x3FFFF] = in[i];

        // ── Master phasor ──
        float prevPhasor = phasor;
        phasor = fmodf(phasor + phasorInc, 1.0f);
        int tick = (phasor < prevPhasor) ? 1 : 0;

        // ── Probability gate ──
        int validTick = 0;
        if (tick) {
            randVal = lcg_rand(rngState);
            validTick = (randVal <= prob) ? 1 : 0;
        }

        // ── Trigger voice allocation ──
        if (validTick) {
            int slot = voiceCounter & 7;
            voiceTimer[slot] = 0;
            float r = lcg_rand(rngState);
            voiceDelay[slot] = fmaxf(0.0f, fminf(131072.0f, r * maxDelaySamps));
            voiceCounter++;
        }

        // ── Accumulate output from all 8 voices ──
        float accum = 0.0f;

        for (int v = 0; v < 8; v++)
        {
            int timer = voiceTimer[v];
            if (timer >= (int)grainSizeSamps) {
                continue; // voice expired
            }

            float t = (float)timer;
            float phase = t * invGrain; // [0..1)

            // ── Envelope computation ──
            float env;
            switch (envShape) {
                case 0: { // Hann
                    float c = fast_cos(twoPiInvGrain * t);
                    env = 0.5f * (1.0f - c);
                    break;
                }
                case 1: { // Blackman
                    float c1 = fast_cos(twoPiInvGrain * t);
                    float c2 = fast_cos(fourPiInvGrain * t);
                    env = 0.42f - 0.5f * c1 + 0.08f * c2;
                    break;
                }
                case 2: { // Triangle with cosine-tapered edges
                    float tri = 1.0f - fabsf(twoInvGrain * t - 1.0f);
                    // Apply a small cosine taper at edges to avoid
                    // discontinuous derivatives (clicks/noise)
                    if (phase < 0.05f)
                        tri *= 0.5f * (1.0f - fast_cos(phase * 62.8318f));
                    else if (phase > 0.95f)
                        tri *= 0.5f * (1.0f - fast_cos((1.0f - phase) * 62.8318f));
                    env = tri;
                    break;
                }
                case 3: { // Tukey (flat top, Hann edges)
                    if (phase < 0.25f)
                        env = 0.5f * (1.0f - fast_cos(phase * 4.0f * 3.14159265f));
                    else if (phase >= 0.75f)
                        env = 0.5f * (1.0f - fast_cos((1.0f - phase) * 4.0f * 3.14159265f));
                    else
                        env = 1.0f;
                    break;
                }
                default:
                    env = 0.0f;
                    break;
            }

            // ── Delay-line read with linear interpolation ──
            float dly = voiceDelay[v];
            int   idly = (int)dly;
            float frac = dly - (float)idly;
            int   idx0 = (writePos - idly)     & 0x3FFFF;
            int   idx1 = (writePos - idly - 1) & 0x3FFFF;
            float sample = delayBuf[idx0] * (1.0f - frac)
                         + delayBuf[idx1] * frac;

            accum += sample * env;
            voiceTimer[v] = timer + 1;
        }

        // ── Normalize and soft-limit output ──
        accum *= normGain;
        out[i] = fast_tanh(accum);

        writePos++;
    }
}
