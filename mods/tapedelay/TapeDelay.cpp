#include "TapeDelay.h"
#include <cmath>
#include <hal/simd.h>
#include <od/config.h>

namespace tapedelay {

void TapeDelay::process() {
  const float *__restrict__ in1 = getInput(0)->buffer();
  float *__restrict__ out1 = getOutput(0)->buffer();

  const float *__restrict__ in2 = nullptr;
  float *__restrict__ out2 = nullptr;
  if (mChannelCount == 2) {
    in2 = getInput(1)->buffer();
    out2 = getOutput(1)->buffer();
  }

  const float *__restrict__ time1 = mTime.buffer();
  const float *__restrict__ timeR = mTimeR.buffer();
  const float *__restrict__ feedback = mFeedback.buffer();
  const float *__restrict__ wow = mWow.buffer();
  const float *__restrict__ flutter = mFlutter.buffer();
  const float *__restrict__ drive = mDrive.buffer();
  const float *__restrict__ mix = mMix.buffer();
  const float *__restrict__ cutoff = mFilterCutoff.buffer();
  const float *__restrict__ res = mFilterRes.buffer();

  // MAP 50Hz to 8000Hz precisely (-5v to +5v map to 0.0 to 1.0)
  float p = (cutoff[0] + 5.0f) * 0.1f;
  if (p < 0.0f)
    p = 0.0f;
  if (p > 1.0f)
    p = 1.0f;
  float cutHz = 50.0f * powf(2.0f, p * 7.3219f); // 50 * 2^7.32 = 8000

  float r = res[0];
  if (r < 0.0f)
    r = 0.0f;
  if (r > 0.99f)
    r = 0.99f;
  float k = 2.0f - (2.0f * r);

  // Hardcoded pi to prevent cmath alias failures
  float g = tanf(3.14159265f * cutHz / mSampleRate);
  float a1 = 1.0f / (1.0f + g * (g + k));
  float a2 = g * a1;
  float a3 = g * a2;

  // Restore classic V1.0.15 volume behavior
  float norm_factor = sqrtf(k);

  float l_svf1_ic1 = svf_ic1eq[0];
  float l_svf1_ic2 = svf_ic2eq[0];
  float l_svf2_ic1 = svf_ic1eq[1];
  float l_svf2_ic2 = svf_ic2eq[1];
  int l_writePos = mWritePos;
  const int l_maxDelay = mMaxDelayInSamples;
  const float l_sampleRate = mSampleRate;
  const int l_channels = mChannelCount;
  float *__restrict__ l_buf1 = mBuffer1.data();
  float *__restrict__ l_buf2 = (l_channels == 2) ? mBuffer2.data() : nullptr;

  // Advance block-rate tape machine mechanical physical physics
  mPhase1 += 0.6f * FRAMELENGTH / l_sampleRate;
  if (mPhase1 > 1.0f)
    mPhase1 -= 1.0f;
  mPhase2 += 1.3f * FRAMELENGTH / l_sampleRate;
  if (mPhase2 > 1.0f)
    mPhase2 -= 1.0f;
  mPhase3 += 11.0f * FRAMELENGTH / l_sampleRate;
  if (mPhase3 > 1.0f)
    mPhase3 -= 1.0f;
  mPhase4 += 23.0f * FRAMELENGTH / l_sampleRate;
  if (mPhase4 > 1.0f)
    mPhase4 -= 1.0f;

  float mechanical_wow = fast_sin(mPhase1) * 0.5f + fast_sin(mPhase2) * 0.5f;
  float mechanical_flutter =
      fast_sin(mPhase3) * 0.6f + fast_sin(mPhase4) * 0.4f;

  float wSig_target = mechanical_wow * 0.005f;     // 5ms total swing depth
  float fSig_target = mechanical_flutter * 0.001f; // 1ms fast scrape depth

  float wSig_base = mWowSmoothed;
  float fSig_base = mFlutterSmoothed;

  float wSig_delta = (wSig_target - wSig_base) / (float)FRAMELENGTH;
  float fSig_delta = (fSig_target - fSig_base) / (float)FRAMELENGTH;

  mWowSmoothed = wSig_target;
  mFlutterSmoothed = fSig_target;

  float32x4_t norm_vec = vdupq_n_f32(norm_factor);
  float32x4_t one_v = vdupq_n_f32(1.0f);
  float32x4_t max_v = vdupq_n_f32(1.5f);
  float32x4_t min_v = vdupq_n_f32(-1.5f);
  float32x4_t mixboost_v = vdupq_n_f32(1.5f);
  float32x4_t pcoeff_v = vdupq_n_f32(0.148148f);

  float32x4_t half_v = vdupq_n_f32(0.5f);
  float32x4_t two_v = vdupq_n_f32(2.0f);
  float32x4_t two_five_v = vdupq_n_f32(2.5f);
  float32x4_t one_five_v = vdupq_n_f32(1.5f);

  for (int i = 0; i < FRAMELENGTH; i += 4) {

    float mixAmount = mix[i];
    if (mixAmount < 0.0f)
      mixAmount = 0.0f;
    if (mixAmount > 1.0f)
      mixAmount = 1.0f;
    float32x4_t mixAmount_v = vdupq_n_f32(mixAmount);

    float fbBase = feedback[i];
    if (fbBase < 0.0f)
      fbBase = 0.0f;
    if (fbBase > 1.2f)
      fbBase = 1.2f;

    float drvBase = drive[i];
    if (drvBase < 0.0f)
      drvBase = 0.0f;
    float drvGain = 1.0f + (drvBase * 5.0f);
    float effectiveFbk = fbBase * (1.0f - (drvBase * 0.3f));
    float32x4_t effectiveFbk_v = vdupq_n_f32(effectiveFbk);
    float32x4_t drvGain_v = vdupq_n_f32(drvGain);

    // --- Channel 1 ---
    // Restore continuous 48kHz audio-rate fractional offset for true Doppler
    // phase modeling
    float tmp_d0[4], tmp_d1[4], tmp_d2[4], tmp_d3[4], tmp_rf[4];

    for (int d = 0; d < 4; d++) {
      float cur_wSig = wSig_base + (i + d) * wSig_delta;
      float cur_fSig = fSig_base + (i + d) * fSig_delta;

      float t1 = time1[i + d];
      if (t1 < 0.0f)
        t1 = 0.0f;
      if (t1 > 1.9f)
        t1 = 1.9f;

      float modTime1 =
          t1 + (wow[i + d] * cur_wSig) + (flutter[i + d] * cur_fSig);
      if (modTime1 < 0.001f)
        modTime1 = 0.001f;
      if (modTime1 > 1.99f)
        modTime1 = 1.99f;
      float delaySamples1 = modTime1 * l_sampleRate;

      float rPos1 = (l_writePos + d) - delaySamples1;
      if (rPos1 < 0.0f)
        rPos1 += l_maxDelay;
      int rc = (int)rPos1;

      int rm1 = rc - 1;
      if (rm1 < 0)
        rm1 += l_maxDelay;

      tmp_d0[d] = l_buf1[rm1];
      tmp_d1[d] = l_buf1[rc];
      tmp_d2[d] = l_buf1[rc + 1];
      tmp_d3[d] = l_buf1[rc + 2];
      tmp_rf[d] = rPos1 - rc;
    }

    float32x4_t p0 = vld1q_f32(tmp_d0);
    float32x4_t p1 = vld1q_f32(tmp_d1);
    float32x4_t p2 = vld1q_f32(tmp_d2);
    float32x4_t p3 = vld1q_f32(tmp_d3);
    float32x4_t f = vld1q_f32(tmp_rf);

    // Core SIMD Hermite C1 Polynomial Fetch Integration
    float32x4_t c0 = p1;
    float32x4_t c1 = vmulq_f32(half_v, vsubq_f32(p2, p0));
    float32x4_t c2 =
        vsubq_f32(vaddq_f32(p0, vmulq_f32(two_v, p2)),
                  vaddq_f32(vmulq_f32(two_five_v, p1), vmulq_f32(half_v, p3)));
    float32x4_t c3 = vaddq_f32(vmulq_f32(half_v, vsubq_f32(p3, p0)),
                               vmulq_f32(one_five_v, vsubq_f32(p1, p2)));

    // (((c3 * f) + c2) * f + c1) * f + c0
    float32x4_t outSample1_v = vaddq_f32(
        c0,
        vmulq_f32(
            f, vaddq_f32(c1, vmulq_f32(f, vaddq_f32(c2, vmulq_f32(f, c3))))));

    float32x4_t in1_v = vld1q_f32(in1 + i);
    float32x4_t wet_v =
        vmulq_f32(outSample1_v, vmulq_f32(mixAmount_v, mixboost_v));
    float32x4_t dry_v = vmulq_f32(in1_v, vsubq_f32(one_v, mixAmount_v));
    vst1q_f32(out1 + i, vaddq_f32(dry_v, wet_v));

    float32x4_t inSample1_v =
        vaddq_f32(in1_v, vmulq_f32(outSample1_v, effectiveFbk_v));
    float32x4_t sat_in = vmulq_f32(inSample1_v, drvGain_v);
    float32x4_t x_clp = vminq_f32(max_v, vmaxq_f32(min_v, sat_in));
    float32x4_t sat_out = vmulq_f32(
        x_clp, vsubq_f32(one_v, vmulq_f32(vmulq_f32(x_clp, x_clp), pcoeff_v)));

    // Safely unroll SVF scalar process to bypass aggressive GCC pointer
    // aliasing / vector stacks
    float s0 = vgetq_lane_f32(sat_out, 0);
    float s1 = vgetq_lane_f32(sat_out, 1);
    float s2 = vgetq_lane_f32(sat_out, 2);
    float s3 = vgetq_lane_f32(sat_out, 3);

    float v3_0 = s0 - l_svf1_ic2;
    float f0 = a1 * l_svf1_ic1 + a2 * v3_0;
    float v2_0 = l_svf1_ic2 + a2 * l_svf1_ic1 + a3 * v3_0;
    l_svf1_ic1 = 2.0f * f0 - l_svf1_ic1;
    l_svf1_ic2 = 2.0f * v2_0 - l_svf1_ic2;

    float v3_1 = s1 - l_svf1_ic2;
    float f1 = a1 * l_svf1_ic1 + a2 * v3_1;
    float v2_1 = l_svf1_ic2 + a2 * l_svf1_ic1 + a3 * v3_1;
    l_svf1_ic1 = 2.0f * f1 - l_svf1_ic1;
    l_svf1_ic2 = 2.0f * v2_1 - l_svf1_ic2;

    float v3_2 = s2 - l_svf1_ic2;
    float f2 = a1 * l_svf1_ic1 + a2 * v3_2;
    float v2_2 = l_svf1_ic2 + a2 * l_svf1_ic1 + a3 * v3_2;
    l_svf1_ic1 = 2.0f * f2 - l_svf1_ic1;
    l_svf1_ic2 = 2.0f * v2_2 - l_svf1_ic2;

    float v3_3 = s3 - l_svf1_ic2;
    float f3 = a1 * l_svf1_ic1 + a2 * v3_3;
    float v2_3 = l_svf1_ic2 + a2 * l_svf1_ic1 + a3 * v3_3;
    l_svf1_ic1 = 2.0f * f3 - l_svf1_ic1;
    l_svf1_ic2 = 2.0f * v2_3 - l_svf1_ic2;

    float repack[4] = {f0, f1, f2, f3};
    float32x4_t filt_v = vmulq_f32(vld1q_f32(repack), norm_vec);
    vst1q_f32(l_buf1 + l_writePos, filt_v);
    if (l_writePos < 4)
      vst1q_f32(l_buf1 + l_writePos + l_maxDelay, filt_v);

    // --- Channel 2 ---
    if (l_channels == 2) {
      float tmp_d0_2[4], tmp_d1_2[4], tmp_d2_2[4], tmp_d3_2[4], tmp_rf_2[4];

      for (int d = 0; d < 4; d++) {
        float cur_wSig = wSig_base + (i + d) * wSig_delta;
        float cur_fSig = fSig_base + (i + d) * fSig_delta;

        float t2 = timeR[i + d];
        if (t2 < 0.0f)
          t2 = 0.0f;
        if (t2 > 1.9f)
          t2 = 1.9f;

        float modTime2 =
            t2 + (wow[i + d] * cur_wSig) + (flutter[i + d] * cur_fSig);
        if (modTime2 < 0.001f)
          modTime2 = 0.001f;
        if (modTime2 > 1.99f)
          modTime2 = 1.99f;
        float delaySamples2 = modTime2 * l_sampleRate;

        float rPos2 = (l_writePos + d) - delaySamples2;
        if (rPos2 < 0.0f)
          rPos2 += l_maxDelay;
        int rc_2 = (int)rPos2;

        int rm1_2 = rc_2 - 1;
        if (rm1_2 < 0)
          rm1_2 += l_maxDelay;

        tmp_d0_2[d] = l_buf2[rm1_2];
        tmp_d1_2[d] = l_buf2[rc_2];
        tmp_d2_2[d] = l_buf2[rc_2 + 1];
        tmp_d3_2[d] = l_buf2[rc_2 + 2];
        tmp_rf_2[d] = rPos2 - rc_2;
      }

      float32x4_t p0_2 = vld1q_f32(tmp_d0_2);
      float32x4_t p1_2 = vld1q_f32(tmp_d1_2);
      float32x4_t p2_2 = vld1q_f32(tmp_d2_2);
      float32x4_t p3_2 = vld1q_f32(tmp_d3_2);
      float32x4_t f_2 = vld1q_f32(tmp_rf_2);

      float32x4_t c0_2 = p1_2;
      float32x4_t c1_2 = vmulq_f32(half_v, vsubq_f32(p2_2, p0_2));
      float32x4_t c2_2 = vsubq_f32(
          vaddq_f32(p0_2, vmulq_f32(two_v, p2_2)),
          vaddq_f32(vmulq_f32(two_five_v, p1_2), vmulq_f32(half_v, p3_2)));
      float32x4_t c3_2 =
          vaddq_f32(vmulq_f32(half_v, vsubq_f32(p3_2, p0_2)),
                    vmulq_f32(one_five_v, vsubq_f32(p1_2, p2_2)));

      float32x4_t outSample2_v = vaddq_f32(
          c0_2,
          vmulq_f32(
              f_2, vaddq_f32(
                       c1_2,
                       vmulq_f32(f_2, vaddq_f32(c2_2, vmulq_f32(f_2, c3_2))))));

      float32x4_t in2_v = vld1q_f32(in2 + i);
      float32x4_t wet2_v =
          vmulq_f32(outSample2_v, vmulq_f32(mixAmount_v, mixboost_v));
      float32x4_t dry2_v = vmulq_f32(in2_v, vsubq_f32(one_v, mixAmount_v));
      vst1q_f32(out2 + i, vaddq_f32(dry2_v, wet2_v));

      float32x4_t inSample2_v =
          vaddq_f32(in2_v, vmulq_f32(outSample2_v, effectiveFbk_v));
      float32x4_t sat_in2 = vmulq_f32(inSample2_v, drvGain_v);
      float32x4_t x_clp2 = vminq_f32(max_v, vmaxq_f32(min_v, sat_in2));
      float32x4_t sat_out2 = vmulq_f32(
          x_clp2,
          vsubq_f32(one_v, vmulq_f32(vmulq_f32(x_clp2, x_clp2), pcoeff_v)));

      float s0_2 = vgetq_lane_f32(sat_out2, 0);
      float s1_2 = vgetq_lane_f32(sat_out2, 1);
      float s2_2 = vgetq_lane_f32(sat_out2, 2);
      float s3_2 = vgetq_lane_f32(sat_out2, 3);

      float v3_0_2 = s0_2 - l_svf2_ic2;
      float f0_2 = a1 * l_svf2_ic1 + a2 * v3_0_2;
      float v2_0_2 = l_svf2_ic2 + a2 * l_svf2_ic1 + a3 * v3_0_2;
      l_svf2_ic1 = 2.0f * f0_2 - l_svf2_ic1;
      l_svf2_ic2 = 2.0f * v2_0_2 - l_svf2_ic2;

      float v3_1_2 = s1_2 - l_svf2_ic2;
      float f1_2 = a1 * l_svf2_ic1 + a2 * v3_1_2;
      float v2_1_2 = l_svf2_ic2 + a2 * l_svf2_ic1 + a3 * v3_1_2;
      l_svf2_ic1 = 2.0f * f1_2 - l_svf2_ic1;
      l_svf2_ic2 = 2.0f * v2_1_2 - l_svf2_ic2;

      float v3_2_2 = s2_2 - l_svf2_ic2;
      float f2_2 = a1 * l_svf2_ic1 + a2 * v3_2_2;
      float v2_2_2 = l_svf2_ic2 + a2 * l_svf2_ic1 + a3 * v3_2_2;
      l_svf2_ic1 = 2.0f * f2_2 - l_svf2_ic1;
      l_svf2_ic2 = 2.0f * v2_2_2 - l_svf2_ic2;

      float v3_3_2 = s3_2 - l_svf2_ic2;
      float f3_2 = a1 * l_svf2_ic1 + a2 * v3_3_2;
      float v2_3_2 = l_svf2_ic2 + a2 * l_svf2_ic1 + a3 * v3_3_2;
      l_svf2_ic1 = 2.0f * f3_2 - l_svf2_ic1;
      l_svf2_ic2 = 2.0f * v2_3_2 - l_svf2_ic2;

      float repack2[4] = {f0_2, f1_2, f2_2, f3_2};
      float32x4_t filt2_v = vmulq_f32(vld1q_f32(repack2), norm_vec);
      vst1q_f32(l_buf2 + l_writePos, filt2_v);
      if (l_writePos < 4)
        vst1q_f32(l_buf2 + l_writePos + l_maxDelay, filt2_v);
    }

    l_writePos += 4;
    if (l_writePos >= l_maxDelay)
      l_writePos = 0;
  }

  mWritePos = l_writePos;
  svf_ic1eq[0] = l_svf1_ic1;
  svf_ic2eq[0] = l_svf1_ic2;
  svf_ic1eq[1] = l_svf2_ic1;
  svf_ic2eq[1] = l_svf2_ic2;
}
} // namespace tapedelay
