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
      addInput(mFeedback);
      addInput(mWow);
      addInput(mFlutter);
      addInput(mDrive);
      addInput(mMix);

      mSampleRate = globalConfig.sampleRate;
      mMaxDelayInSamples = (int)(2.0f * mSampleRate); // 2 seconds max
      
      mBuffer1.resize(mMaxDelayInSamples + 1, 0.0f);
      if (stereo) {
        mBuffer2.resize(mMaxDelayInSamples + 1, 0.0f);
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
#endif

  private:
    int mChannelCount = 1;
    float mSampleRate = 48000.0f;
    
    int mMaxDelayInSamples = 0;
    std::vector<float> mBuffer1;
    std::vector<float> mBuffer2;
    int mWritePos = 0;

    float mWowPhase = 0.0f;
    float mFlutterPhase1 = 0.0f;
    float mFlutterPhase2 = 0.0f;
    float mFlutterPhase3 = 0.0f;

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
