#pragma once

#include <od/objects/Object.h>
#include <od/config.h>
#include <vector>
#include <cmath>
#include <sstream>
#include <cstdint>

namespace tapedelay {
  class TapeDelay : public od::Object {
  public:
    TapeDelay(bool stereo) {
      mChannelCount = stereo ? 2 : 1;
      
      for (int channel = 0; channel < mChannelCount; channel++) {
        std::ostringstream inName;
        inName << "In" << channel + 1;
        addInputFromHeap(new od::Inlet { inName.str() });

        std::ostringstream outName;
        outName << "Out" << channel + 1;
        addOutputFromHeap(new od::Outlet { outName.str() });
      }

      addInput(mTime);
      addInput(mTimeR);
      addInput(mTimeR);
      addInput(mFeedback);
      addInput(mWow);
      addInput(mFlutter);
      addInput(mDrive);
      addInput(mMix);
      addInput(mFilterCutoff);
      addInput(mFilterRes);

      mSampleRate = globalConfig.sampleRate;
      mMaxDelayInSamples = (int)(2.0f * mSampleRate); // 2 seconds max
      
      mBuffer1.resize(mMaxDelayInSamples + 16, 0.0f);
      if (stereo) {
        mBuffer2.resize(mMaxDelayInSamples + 16, 0.0f);
      }
    }

    virtual ~TapeDelay() { }

#ifndef SWIGLUA
    virtual void process();

    od::Inlet mTime { "Time L" };
    od::Inlet mTimeR { "Time R" };
    od::Inlet mFeedback { "Feedback" };
    od::Inlet mWow { "Wow" };
    od::Inlet mFlutter { "Flutter" };
    od::Inlet mDrive { "Drive" };
    od::Inlet mMix { "Mix" };
    od::Inlet mFilterCutoff { "Cutoff" };
    od::Inlet mFilterRes { "Res" };
#endif

  private:
    int mChannelCount = 1;
    float mSampleRate = 48000.0f;
    
    int mMaxDelayInSamples = 0;
    std::vector<float> mBuffer1;
    std::vector<float> mBuffer2;
    int mWritePos = 0;

    float mWowSmoothed = 0.0f;
    float mFlutterSmoothed = 0.0f;
    uint32_t mRngState = 123456789;
    
    // Tape mechanic physics phases
    float mPhase1 = 0.0f;
    float mPhase2 = 0.2f;
    float mPhase3 = 0.5f;
    float mPhase4 = 0.7f;

    inline float std_abs(float x) { return x < 0.0f ? -x : x; }
    inline float fast_sin(float p) {
        float x = (p - 0.5f) * 2.0f;
        float sine = 4.0f * x * (1.0f - std_abs(x));
        return sine * 0.225f * (std_abs(sine) - 1.0f) + sine;
    }
    
    // Fast pseudo-random number generator (-1.0 to 1.0)
    inline float fast_randf() {
        mRngState ^= mRngState << 13;
        mRngState ^= mRngState >> 17;
        mRngState ^= mRngState << 5;
        return (float)((int32_t)mRngState) * 4.65661287e-10f; // multiply by 1 / (2^31 - 1)
    }
    
    // SVF State (Left/Right)
    float svf_ic1eq[2] = {0.0f, 0.0f};
    float svf_ic2eq[2] = {0.0f, 0.0f};
    
    // Fast sigmoid approximation (Cubic) to vastly save CPU over tanhf
    inline float fast_sigmoid(float x) {
        float y = x;
        if (y > 1.4142135f) y = 1.4142135f;
        else if (y < -1.4142135f) y = -1.4142135f;
        return y - (y * y * y) * 0.16666666f;
    }

    // helper for cubic interpolation
    float cubicInterpolate(float y0, float y1, float y2, float y3, float mu) {
      float a0, a1, a2, a3, mu2;
      mu2 = mu * mu;
      a0 = y3 - y2 - y0 + y1;
      a1 = y0 - y1 - a0;
      a2 = y2 - y0;
      a3 = y1;
      return (a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3);
    }
  };
}
