#include "Reverb.h"
#include <od/config.h>
#include <hal/ops.h>
Reverb::Reverb() : DSP()
{
  UI uiParams(this);

  addInput(minL);
  addInput(minR);
  addOutput(moutL);
  addOutput(moutR);
  addParameter(mBand Width);
  addParameter(mDamping);
  addParameter(mDecay);
  addParameter(mDry/Wet);
  DSP.init(globalConfig.sampleRate);
  DSP.buildUserInterface(&uiParams);
}
Reverb::~Reverb()
{ }
void Reverb::process()
{
  FAUSTFLOAT* inputs[] = { minL.buffer(), minR.buffer() };
  FAUSTFLOAT* outputs[] = { moutL.buffer(), moutR.buffer() };
  *ffBand Width = CLAMP(mBand Width.target(), 0, 1);
  *ffDamping = CLAMP(mDamping.target(), 0, 1);
  *ffDecay = CLAMP(mDecay.target(), 0, 1);
  *ffDry/Wet = CLAMP(mDry/Wet.target(), 0, 1);
  DSP.compute(FRAMELENGTH, inputs, outputs);
}

