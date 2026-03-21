/* ------------------------------------------------------------
name: "Reverb"
Code generated with Faust 2.83.1 (https://faust.grame.fr)
Compilation options: -a /tmp/faust2er301.8s6hI3/reverb/reverb.h -lang cpp -fpga-mem-th 4 -ct 1 -cn Dspreverb -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __Dspreverb_H__
#define  __Dspreverb_H__


#include <string.h>
#include <od/objects/Object.h>

#define FAUSTFLOAT float

#ifndef SWIGLUA

#ifndef __FAUST_301_BOILERPLATE__
#define __FAUST_301_BOILERPLATE__

class dsp {};

class ParametersSetter {
public:
  virtual void addParam(const char* label, FAUSTFLOAT* zone) = 0;
};

struct Meta {
  virtual void declare(const char* key, const char* value) {};
};

struct Soundfile {
  FAUSTFLOAT** fBuffers;
  int* fLength;   // length of each part
  int* fSR;       // sample rate of each part
  int* fOffset;   // offset of each part in the global buffer
  int fChannels;  // max number of channels of all concatenated files
};

class UI {
public:
  UI(ParametersSetter *_ps) {
    ps = _ps;
  }

  // -- widget's layouts
  virtual void openTabBox(const char* label) {}
  virtual void openHorizontalBox(const char* label) {}
  virtual void openVerticalBox(const char* label) {}
  virtual void closeBox() {}

  // -- active widgets
  virtual void addButton(const char* label, FAUSTFLOAT* zone) {
    ps->addParam(label, zone);
  }
  virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) {
    ps->addParam(label, zone);
  }
  virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    ps->addParam(label, zone);
  }
  virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    ps->addParam(label, zone);
  }
  virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    ps->addParam(label, zone);
  }

  // -- passive widgets
  virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
  virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}

  // -- soundfiles
  virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}

  // -- metadata declarations
  virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) {}

private:
  ParametersSetter *ps;
};
#endif

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>

#ifndef FAUSTCLASS 
#define FAUSTCLASS Dspreverb
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif


class Dspreverb : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int IOTA0;
	float fVec0[1024];
	float fRec5[2];
	float fVec1[8192];
	FAUSTFLOAT fHslider1;
	float fRec4[2];
	FAUSTFLOAT fHslider2;
	float fVec2[4096];
	float fRec2[2];
	float fVec3[4096];
	FAUSTFLOAT fHslider3;
	float fRec15[3];
	float fVec4[256];
	float fRec13[2];
	float fVec5[128];
	float fRec11[2];
	float fVec6[512];
	float fRec9[2];
	float fVec7[512];
	float fRec7[2];
	float fRec0[2];
	float fVec8[1024];
	float fRec19[2];
	float fVec9[8192];
	float fRec18[2];
	float fVec10[2048];
	float fRec16[2];
	float fVec11[2048];
	float fRec1[2];
	int fSampleRate;
	
 public:
	Dspreverb() {
	}
	
	Dspreverb(const Dspreverb&) = default;
	
	virtual ~Dspreverb() = default;
	
	Dspreverb& operator=(const Dspreverb&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.22.0");
		m->declare("compile_options", "-a /tmp/faust2er301.8s6hI3/reverb/reverb.h -lang cpp -fpga-mem-th 4 -ct 1 -cn Dspreverb -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("er301_in1", "inL");
		m->declare("er301_in2", "inR");
		m->declare("er301_out1", "outL");
		m->declare("er301_out2", "outR");
		m->declare("filename", "reverb.dsp");
		m->declare("name", "Reverb");
		m->declare("reverbs.lib/dattorro_rev:author", "Jakob Zerbian");
		m->declare("reverbs.lib/dattorro_rev:licence", "MIT-style STK-4.3 license");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "1.5.1");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.2.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/version", "1.6.0");
	}

	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.25f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.25f);
		fHslider2 = static_cast<FAUSTFLOAT>(0.8f);
		fHslider3 = static_cast<FAUSTFLOAT>(0.6f);
	}
	
	virtual void instanceClear() {
		IOTA0 = 0;
		for (int l0 = 0; l0 < 1024; l0 = l0 + 1) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			fRec5[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 8192; l2 = l2 + 1) {
			fVec1[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			fRec4[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 4096; l4 = l4 + 1) {
			fVec2[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
			fRec2[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 4096; l6 = l6 + 1) {
			fVec3[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 3; l7 = l7 + 1) {
			fRec15[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 256; l8 = l8 + 1) {
			fVec4[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			fRec13[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 128; l10 = l10 + 1) {
			fVec5[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			fRec11[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 512; l12 = l12 + 1) {
			fVec6[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			fRec9[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 512; l14 = l14 + 1) {
			fVec7[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			fRec7[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec0[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 1024; l17 = l17 + 1) {
			fVec8[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec19[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 8192; l19 = l19 + 1) {
			fVec9[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec18[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2048; l21 = l21 + 1) {
			fVec10[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
			fRec16[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2048; l23 = l23 + 1) {
			fVec11[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
			fRec1[l24] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual Dspreverb* clone() {
		return new Dspreverb(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("Reverb");
		ui_interface->addHorizontalSlider("Band Width", &fHslider3, FAUSTFLOAT(0.6f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("Damping", &fHslider1, FAUSTFLOAT(0.25f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("Decay", &fHslider2, FAUSTFLOAT(0.8f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("Dry/Wet", &fHslider0, FAUSTFLOAT(0.25f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = static_cast<float>(fHslider0);
		float fSlow1 = 1.0f - fSlow0;
		float fSlow2 = static_cast<float>(fHslider1);
		float fSlow3 = 1.0f - fSlow2;
		float fSlow4 = static_cast<float>(fHslider2);
		float fSlow5 = static_cast<float>(fHslider3);
		float fSlow6 = 0.5f * fSlow5;
		float fSlow7 = 1.0f - fSlow5;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = static_cast<float>(input0[i0]);
			float fTemp1 = 0.7f * fRec5[1] + fRec1[1];
			fVec0[IOTA0 & 1023] = fTemp1;
			fRec5[0] = fVec0[(IOTA0 - 908) & 1023];
			float fRec6 = -(0.7f * fTemp1);
			fVec1[IOTA0 & 8191] = fRec6 + fRec5[1];
			fRec4[0] = fSlow2 * fRec4[1] + fSlow3 * fVec1[(IOTA0 - 4217) & 8191];
			float fTemp2 = fSlow4 * fRec4[0] - 0.5f * fRec2[1];
			fVec2[IOTA0 & 4095] = fTemp2;
			fRec2[0] = fVec2[(IOTA0 - 2656) & 4095];
			float fRec3 = 0.5f * fTemp2;
			fVec3[IOTA0 & 4095] = fRec3 + fRec2[1];
			float fTemp3 = static_cast<float>(input1[i0]);
			fRec15[0] = fSlow7 * fRec15[2] + fSlow6 * (fTemp0 + fTemp3);
			float fTemp4 = fRec15[0] - 0.75f * fRec13[1];
			fVec4[IOTA0 & 255] = fTemp4;
			fRec13[0] = fVec4[(IOTA0 - 142) & 255];
			float fRec14 = 0.75f * fTemp4;
			float fTemp5 = fRec14 + fRec13[1] - 0.75f * fRec11[1];
			fVec5[IOTA0 & 127] = fTemp5;
			fRec11[0] = fVec5[(IOTA0 - 107) & 127];
			float fRec12 = 0.75f * fTemp5;
			float fTemp6 = fRec12 + fRec11[1] - 0.625f * fRec9[1];
			fVec6[IOTA0 & 511] = fTemp6;
			fRec9[0] = fVec6[(IOTA0 - 379) & 511];
			float fRec10 = 0.625f * fTemp6;
			float fTemp7 = fRec10 + fRec9[1] - 0.625f * fRec7[1];
			fVec7[IOTA0 & 511] = fTemp7;
			fRec7[0] = fVec7[(IOTA0 - 277) & 511];
			float fRec8 = 0.625f * fTemp7;
			fRec0[0] = fRec7[1] + fRec8 + fSlow4 * fVec3[(IOTA0 - 2656) & 4095];
			float fTemp8 = 0.7f * fRec19[1] + fRec0[1];
			fVec8[IOTA0 & 1023] = fTemp8;
			fRec19[0] = fVec8[(IOTA0 - 672) & 1023];
			float fRec20 = -(0.7f * fTemp8);
			fVec9[IOTA0 & 8191] = fRec20 + fRec19[1];
			fRec18[0] = fSlow2 * fRec18[1] + fSlow3 * fVec9[(IOTA0 - 4453) & 8191];
			float fTemp9 = fSlow4 * fRec18[0] - 0.5f * fRec16[1];
			fVec10[IOTA0 & 2047] = fTemp9;
			fRec16[0] = fVec10[(IOTA0 - 1800) & 2047];
			float fRec17 = 0.5f * fTemp9;
			fVec11[IOTA0 & 2047] = fRec17 + fRec16[1];
			fRec1[0] = fRec8 + fRec7[1] + fSlow4 * fVec11[(IOTA0 - 1800) & 2047];
			output0[i0] = static_cast<FAUSTFLOAT>(fSlow0 * fRec0[0] + fSlow1 * fTemp0);
			output1[i0] = static_cast<FAUSTFLOAT>(fSlow0 * fRec1[0] + fSlow1 * fTemp3);
			IOTA0 = IOTA0 + 1;
			fRec5[1] = fRec5[0];
			fRec4[1] = fRec4[0];
			fRec2[1] = fRec2[0];
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fRec13[1] = fRec13[0];
			fRec11[1] = fRec11[0];
			fRec9[1] = fRec9[0];
			fRec7[1] = fRec7[0];
			fRec0[1] = fRec0[0];
			fRec19[1] = fRec19[0];
			fRec18[1] = fRec18[0];
			fRec16[1] = fRec16[0];
			fRec1[1] = fRec1[0];
		}
	}

};
#endif

class Reverb :
    public od::Object
#ifndef SWIGLUA
    , public ParametersSetter
#endif
{
public:
  Reverb();
  ~Reverb();

#ifndef SWIGLUA
  virtual void process();
  od::Inlet minL{"inL"};
  od::Inlet minR{"inR"};
  od::Outlet moutL{"outL"};
  od::Outlet moutR{"outR"};
  od::Parameter mBand Width{"Band Width"};
  od::Parameter mDamping{"Damping"};
  od::Parameter mDecay{"Decay"};
  od::Parameter mDry/Wet{"Dry/Wet"};
  virtual void addParam(const char* label, FAUSTFLOAT* zone) {
    if (strcmp(label, "Band Width") == 0) {
      ffBand Width = zone;
    }
    if (strcmp(label, "Damping") == 0) {
      ffDamping = zone;
    }
    if (strcmp(label, "Decay") == 0) {
      ffDecay = zone;
    }
    if (strcmp(label, "Dry/Wet") == 0) {
      ffDry/Wet = zone;
    }
  }

private:
  FAUSTFLOAT* ffBand Width;
  FAUSTFLOAT* ffDamping;
  FAUSTFLOAT* ffDecay;
  FAUSTFLOAT* ffDry/Wet;
  DspReverb DSP;

#endif

};


#endif
