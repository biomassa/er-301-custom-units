#include <TapeDelay.h>
#include <hal/ops.h> // often needed for vector ops if used, but we'll use scalar
#include <util/math.h>

namespace tapedelay {

  void TapeDelay::process() {
    float *in1 = getInput(0)->buffer();
    float *out1 = getOutput(0)->buffer();
    
    float *in2 = nullptr;
    float *out2 = nullptr;
    if (mChannelCount == 2) {
      in2 = getInput(1)->buffer();
      out2 = getOutput(1)->buffer();
    }

    const float *time1 = mTime.buffer();
    const float *timeR = mTimeR.buffer();
    const float *feedback = mFeedback.buffer();
    const float *wow = mWow.buffer();
    const float *flutter = mFlutter.buffer();
    const float *drive = mDrive.buffer();
    const float *mix = mMix.buffer();

    // LFO frequencies (approximate tape wow/flutter speeds)
    const float wowFreq = 0.7f; // Hz
    const float wowPhaseInc = wowFreq * (2.0f * M_PI / mSampleRate);
    
    // Base flutter frequency
    const float flutterFreq = 12.0f; // Hz
    const float flutterPhaseInc1 = flutterFreq * (2.0f * M_PI / mSampleRate);
    const float flutterPhaseInc2 = flutterFreq * 1.43f * (2.0f * M_PI / mSampleRate);
    const float flutterPhaseInc3 = flutterFreq * 2.17f * (2.0f * M_PI / mSampleRate);

    for (int i = 0; i < FRAMELENGTH; i++) {
        // Base delay time in seconds
        float t1 = time1[i];
        if (t1 < 0.0f) t1 = 0.0f;
        if (t1 > 1.9f) t1 = 1.9f;
        
        float t2 = mChannelCount == 2 ? timeR[i] : t1; 
        if (t2 < 0.0f) t2 = 0.0f;
        if (t2 > 1.9f) t2 = 1.9f;

        // Advance LFOs
        mWowPhase += wowPhaseInc;
        if (mWowPhase > 2.0f * M_PI) mWowPhase -= 2.0f * M_PI;
        
        mFlutterPhase1 += flutterPhaseInc1;
        if (mFlutterPhase1 > 2.0f * M_PI) mFlutterPhase1 -= 2.0f * M_PI;
        mFlutterPhase2 += flutterPhaseInc2;
        if (mFlutterPhase2 > 2.0f * M_PI) mFlutterPhase2 -= 2.0f * M_PI;
        mFlutterPhase3 += flutterPhaseInc3;
        if (mFlutterPhase3 > 2.0f * M_PI) mFlutterPhase3 -= 2.0f * M_PI;

        // Generate LFO signals
        float wowSignal = sinf(mWowPhase);
        
        // Complex flutter using multiple inharmonic sine waves 
        // to simulate scraping and multiple mechanical rollers
        float flutterSignal = (sinf(mFlutterPhase1) + 
                               0.5f * sinf(mFlutterPhase2) + 
                               0.25f * sinf(mFlutterPhase3)) / 1.75f;

        // Modulate delay time
        // Wow modulates up to 0.02s (~20ms) max 
        // Flutter modulates up to 0.005s (~5ms) max
        float modTime1 = t1 + (wow[i] * 0.02f * wowSignal) + (flutter[i] * 0.005f * flutterSignal);
        if (modTime1 < 0.001f) modTime1 = 0.001f;
        if (modTime1 > 1.99f) modTime1 = 1.99f;
        float delaySamples1 = modTime1 * mSampleRate;

        // Saturation drive (unipolar 0 to 1)
        float drv = drive[i];
        if (drv < 0.0f) drv = 0.0f;
        if (drv > 1.0f) drv = 1.0f;
        // Tape saturation gain curve: 1.0 (clean) to 6.0 (driven)
        float drvGain = 1.0f + drv * 5.0f;

        // Wet/Dry mix
        float mixAmount = mix[i];
        if (mixAmount < 0.0f) mixAmount = 0.0f;
        if (mixAmount > 1.0f) mixAmount = 1.0f;

        float fbk = feedback[i];
        if (fbk > 1.5f) fbk = 1.5f;
        if (fbk < -1.5f) fbk = -1.5f;
        
        // Attenuate feedback as drive goes up to prevent runaway overdrive
        float effectiveFbk = fbk * (1.0f - 0.5f * drv);

        // Channel 1
        float readPos1 = (float)mWritePos - delaySamples1;
        while (readPos1 < 0.0f) readPos1 += mMaxDelayInSamples;
        int rp1 = (int)readPos1;
        float rFrac1 = readPos1 - rp1;

        int r0 = (rp1 - 1 + mMaxDelayInSamples) % mMaxDelayInSamples;
        int r1 = rp1;
        int r2 = (rp1 + 1) % mMaxDelayInSamples;
        int r3 = (rp1 + 2) % mMaxDelayInSamples;

        float d0 = mBuffer1[r0];
        float d1 = mBuffer1[r1];
        float d2 = mBuffer1[r2];
        float d3 = mBuffer1[r3];

        float outSample1 = cubicInterpolate(d0, d1, d2, d3, rFrac1);
        
        // Wet/Dry mix with master tanh limiter
        out1[i] = tanhf(in1[i] * (1.0f - mixAmount) + outSample1 * mixAmount);

        // Feedback path with saturation (tanh)
        float inSample1 = in1[i] + outSample1 * effectiveFbk;
        float saturated1 = tanhf(inSample1 * drvGain);
        
        mBuffer1[mWritePos] = saturated1;

        // Channel 2
        if (mChannelCount == 2) {
            float modTime2 = t2 + (wow[i] * 0.02f * wowSignal) + (flutter[i] * 0.005f * flutterSignal);
            if (modTime2 < 0.001f) modTime2 = 0.001f;
            if (modTime2 > 1.99f) modTime2 = 1.99f;
            float delaySamples2 = modTime2 * mSampleRate;

            float readPos2 = (float)mWritePos - delaySamples2;
            while (readPos2 < 0.0f) readPos2 += mMaxDelayInSamples;
            int rp2 = (int)readPos2;
            float rFrac2 = readPos2 - rp2;

            int rc0 = (rp2 - 1 + mMaxDelayInSamples) % mMaxDelayInSamples;
            int rc1 = rp2;
            int rc2 = (rp2 + 1) % mMaxDelayInSamples;
            int rc3 = (rp2 + 2) % mMaxDelayInSamples;

            float outSample2 = cubicInterpolate(mBuffer2[rc0], mBuffer2[rc1], mBuffer2[rc2], mBuffer2[rc3], rFrac2);
            // Wet/Dry mix with master tanh limiter
            out2[i] = tanhf(in2[i] * (1.0f - mixAmount) + outSample2 * mixAmount);

            float inSample2 = in2[i] + outSample2 * effectiveFbk;
            float saturated2 = tanhf(inSample2 * drvGain);
            
            mBuffer2[mWritePos] = saturated2;
        }

        mWritePos++;
        if (mWritePos >= mMaxDelayInSamples) {
            mWritePos = 0;
        }
    }
  }
}
