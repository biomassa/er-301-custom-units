/* ------------------------------------------------------------
author: "Antigravity"
name: "gran"
Code generated with Faust 2.83.1 (https://faust.grame.fr)
Compilation options: -a /tmp/faust2er301.RvRro3/gran/gran.h -lang cpp -fpga-mem-th 4 -ct 1 -cn Dspgran -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __Dspgran_H__
#define  __Dspgran_H__


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
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS Dspgran
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


class Dspgran : public dsp {
	
 private:
	
	FAUSTFLOAT fHslider0;
	int fSampleRate;
	float fConst0;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	float fConst1;
	float fRec3[2];
	int iRec4[2];
	float fRec2[2];
	int iRec1[2];
	int iRec0[2];
	FAUSTFLOAT fHslider3;
	int IOTA0;
	float fVec0[262144];
	float fRec5[2];
	int iRec6[2];
	float fRec7[2];
	int iRec8[2];
	float fRec9[2];
	int iRec10[2];
	float fRec11[2];
	int iRec12[2];
	float fRec13[2];
	int iRec14[2];
	float fRec15[2];
	int iRec16[2];
	float fRec17[2];
	int iRec18[2];
	float fRec19[2];
	
 public:
	Dspgran() {
	}
	
	Dspgran(const Dspgran&) = default;
	
	virtual ~Dspgran() = default;
	
	Dspgran& operator=(const Dspgran&) = default;
	
	void metadata(Meta* m) { 
		m->declare("author", "Antigravity");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/sAndH:author", "Romain Michon");
		m->declare("basics.lib/version", "1.22.0");
		m->declare("compile_options", "-a /tmp/faust2er301.RvRro3/gran/gran.h -lang cpp -fpga-mem-th 4 -ct 1 -cn Dspgran -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("er301_in1", "In");
		m->declare("er301_out1", "Out");
		m->declare("filename", "gran.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "gran");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.5.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 1.0f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.05f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.7f);
		fHslider2 = static_cast<FAUSTFLOAT>(1e+01f);
		fHslider3 = static_cast<FAUSTFLOAT>(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
			fRec3[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
			iRec4[l1] = 0;
		}
		for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
			fRec2[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
			iRec1[l3] = 0;
		}
		for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
			iRec0[l4] = 0;
		}
		IOTA0 = 0;
		for (int l5 = 0; l5 < 262144; l5 = l5 + 1) {
			fVec0[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
			fRec5[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = l7 + 1) {
			iRec6[l7] = 0;
		}
		for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
			fRec7[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
			iRec8[l9] = 0;
		}
		for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
			fRec9[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
			iRec10[l11] = 0;
		}
		for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
			fRec11[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
			iRec12[l13] = 0;
		}
		for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
			fRec13[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = l15 + 1) {
			iRec14[l15] = 0;
		}
		for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
			fRec15[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
			iRec16[l17] = 0;
		}
		for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
			fRec17[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
			iRec18[l19] = 0;
		}
		for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
			fRec19[l20] = 0.0f;
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
	
	virtual Dspgran* clone() {
		return new Dspgran(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("gran");
		ui_interface->addHorizontalSlider("EnvShape", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(3.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("GrainSize", &fHslider0, FAUSTFLOAT(0.05f), FAUSTFLOAT(0.001f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("Probability", &fHslider1, FAUSTFLOAT(0.7f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("Speed", &fHslider2, FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f), FAUSTFLOAT(4e+02f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* output0 = outputs[0];
		float fSlow0 = std::max<float>(1.0f, fConst0 * static_cast<float>(fHslider0));
		float fSlow1 = static_cast<float>(fHslider1);
		float fSlow2 = fConst1 * static_cast<float>(fHslider2);
		float fSlow3 = 1.0f / fSlow0;
		float fSlow4 = 6.2831855f / fSlow0;
		float fSlow5 = static_cast<float>(fHslider3);
		float fSlow6 = static_cast<float>(fSlow5 >= 2.5f);
		float fSlow7 = 2.0f / fSlow0;
		float fSlow8 = static_cast<float>(fSlow5 < 2.5f) * static_cast<float>(fSlow5 >= 1.5f);
		float fSlow9 = 12.566371f / fSlow0;
		float fSlow10 = static_cast<float>(fSlow5 < 1.5f) * static_cast<float>(fSlow5 >= 0.5f);
		float fSlow11 = 0.5f * static_cast<float>(fSlow5 < 0.5f);
		float fSlow12 = 131072.0f - fSlow0;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			fRec3[0] = fSlow2 + std::fmod(fRec3[1], 1.0f);
			int iTemp0 = fRec3[0] < fRec3[1];
			iRec4[0] = 1103515245 * iRec4[1] + 12345;
			float fTemp1 = 0.5f * (4.656613e-10f * static_cast<float>(iRec4[0]) + 1.0f);
			fRec2[0] = ((iTemp0) ? fTemp1 : fRec2[1]);
			int iTemp2 = iTemp0 & (fRec2[0] <= fSlow1);
			iRec1[0] = iTemp2 + iRec1[1];
			int iTemp3 = iRec1[0] % 8;
			int iTemp4 = iTemp2 & (iTemp3 == 7);
			iRec0[0] = (iRec0[1] + 1) * (1 - iTemp4);
			float fTemp5 = static_cast<float>(iRec0[0]);
			float fTemp6 = fSlow3 * fTemp5;
			float fTemp7 = std::cos(fSlow4 * fTemp5);
			float fTemp8 = 1.0f - fTemp7;
			float fTemp9 = static_cast<float>(input0[i0]);
			fVec0[IOTA0 & 262143] = fTemp9;
			fRec5[0] = ((iTemp4) ? fTemp1 : fRec5[1]);
			float fTemp10 = std::max<float>(0.0f, std::min<float>(131072.0f, fSlow12 * fRec5[0]));
			int iTemp11 = static_cast<int>(fTemp10);
			float fTemp12 = std::floor(fTemp10);
			int iTemp13 = iTemp2 & (iTemp3 == 6);
			iRec6[0] = (iRec6[1] + 1) * (1 - iTemp13);
			float fTemp14 = static_cast<float>(iRec6[0]);
			float fTemp15 = fSlow3 * fTemp14;
			float fTemp16 = std::cos(fSlow4 * fTemp14);
			float fTemp17 = 1.0f - fTemp16;
			fRec7[0] = ((iTemp13) ? fTemp1 : fRec7[1]);
			float fTemp18 = std::max<float>(0.0f, std::min<float>(131072.0f, fSlow12 * fRec7[0]));
			int iTemp19 = static_cast<int>(fTemp18);
			float fTemp20 = std::floor(fTemp18);
			int iTemp21 = iTemp2 & (iTemp3 == 5);
			iRec8[0] = (iRec8[1] + 1) * (1 - iTemp21);
			float fTemp22 = static_cast<float>(iRec8[0]);
			float fTemp23 = fSlow3 * fTemp22;
			float fTemp24 = std::cos(fSlow4 * fTemp22);
			float fTemp25 = 1.0f - fTemp24;
			fRec9[0] = ((iTemp21) ? fTemp1 : fRec9[1]);
			float fTemp26 = std::max<float>(0.0f, std::min<float>(131072.0f, fSlow12 * fRec9[0]));
			int iTemp27 = static_cast<int>(fTemp26);
			float fTemp28 = std::floor(fTemp26);
			int iTemp29 = iTemp2 & (iTemp3 == 4);
			iRec10[0] = (iRec10[1] + 1) * (1 - iTemp29);
			float fTemp30 = static_cast<float>(iRec10[0]);
			float fTemp31 = fSlow3 * fTemp30;
			float fTemp32 = std::cos(fSlow4 * fTemp30);
			float fTemp33 = 1.0f - fTemp32;
			fRec11[0] = ((iTemp29) ? fTemp1 : fRec11[1]);
			float fTemp34 = std::max<float>(0.0f, std::min<float>(131072.0f, fSlow12 * fRec11[0]));
			int iTemp35 = static_cast<int>(fTemp34);
			float fTemp36 = std::floor(fTemp34);
			int iTemp37 = iTemp2 & (iTemp3 == 3);
			iRec12[0] = (iRec12[1] + 1) * (1 - iTemp37);
			float fTemp38 = static_cast<float>(iRec12[0]);
			float fTemp39 = fSlow3 * fTemp38;
			float fTemp40 = std::cos(fSlow4 * fTemp38);
			float fTemp41 = 1.0f - fTemp40;
			fRec13[0] = ((iTemp37) ? fTemp1 : fRec13[1]);
			float fTemp42 = std::max<float>(0.0f, std::min<float>(131072.0f, fSlow12 * fRec13[0]));
			int iTemp43 = static_cast<int>(fTemp42);
			float fTemp44 = std::floor(fTemp42);
			int iTemp45 = iTemp2 & (iTemp3 == 2);
			iRec14[0] = (iRec14[1] + 1) * (1 - iTemp45);
			float fTemp46 = static_cast<float>(iRec14[0]);
			float fTemp47 = fSlow3 * fTemp46;
			float fTemp48 = std::cos(fSlow4 * fTemp46);
			float fTemp49 = 1.0f - fTemp48;
			fRec15[0] = ((iTemp45) ? fTemp1 : fRec15[1]);
			float fTemp50 = std::max<float>(0.0f, std::min<float>(131072.0f, fSlow12 * fRec15[0]));
			int iTemp51 = static_cast<int>(fTemp50);
			float fTemp52 = std::floor(fTemp50);
			int iTemp53 = iTemp2 & (iTemp3 == 1);
			iRec16[0] = (iRec16[1] + 1) * (1 - iTemp53);
			float fTemp54 = static_cast<float>(iRec16[0]);
			float fTemp55 = fSlow3 * fTemp54;
			float fTemp56 = std::cos(fSlow4 * fTemp54);
			float fTemp57 = 1.0f - fTemp56;
			fRec17[0] = ((iTemp53) ? fTemp1 : fRec17[1]);
			float fTemp58 = std::max<float>(0.0f, std::min<float>(131072.0f, fSlow12 * fRec17[0]));
			int iTemp59 = static_cast<int>(fTemp58);
			float fTemp60 = std::floor(fTemp58);
			int iTemp61 = iTemp2 & (iTemp3 == 0);
			iRec18[0] = (iRec18[1] + 1) * (1 - iTemp61);
			float fTemp62 = static_cast<float>(iRec18[0]);
			float fTemp63 = fSlow3 * fTemp62;
			float fTemp64 = std::cos(fSlow4 * fTemp62);
			float fTemp65 = 1.0f - fTemp64;
			fRec19[0] = ((iTemp61) ? fTemp1 : fRec19[1]);
			float fTemp66 = std::max<float>(0.0f, std::min<float>(131072.0f, fSlow12 * fRec19[0]));
			int iTemp67 = static_cast<int>(fTemp66);
			float fTemp68 = std::floor(fTemp66);
			output0[i0] = static_cast<FAUSTFLOAT>((fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp67))) & 262143] * (fTemp68 + (1.0f - fTemp66)) + (fTemp66 - fTemp68) * fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp67 + 1))) & 262143]) * (fSlow11 * fTemp65 + fSlow10 * (0.08f * std::cos(fSlow9 * fTemp62) + (0.42f - 0.5f * fTemp64)) + fSlow8 * (1.0f - std::fabs(fSlow7 * fTemp62 + -1.0f)) + fSlow6 * (static_cast<float>(fTemp63 >= 0.25f) * static_cast<float>(fTemp63 < 0.75f) + 0.5f * fTemp65 * (static_cast<float>(fTemp63 < 0.25f) + static_cast<float>(fTemp63 >= 0.75f)))) * static_cast<float>(fTemp62 < fSlow0) + (fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp59))) & 262143] * (fTemp60 + (1.0f - fTemp58)) + (fTemp58 - fTemp60) * fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp59 + 1))) & 262143]) * (fSlow11 * fTemp57 + fSlow10 * (0.08f * std::cos(fSlow9 * fTemp54) + (0.42f - 0.5f * fTemp56)) + fSlow8 * (1.0f - std::fabs(fSlow7 * fTemp54 + -1.0f)) + fSlow6 * (static_cast<float>(fTemp55 >= 0.25f) * static_cast<float>(fTemp55 < 0.75f) + 0.5f * fTemp57 * (static_cast<float>(fTemp55 < 0.25f) + static_cast<float>(fTemp55 >= 0.75f)))) * static_cast<float>(fTemp54 < fSlow0) + (fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp51))) & 262143] * (fTemp52 + (1.0f - fTemp50)) + (fTemp50 - fTemp52) * fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp51 + 1))) & 262143]) * (fSlow11 * fTemp49 + fSlow10 * (0.08f * std::cos(fSlow9 * fTemp46) + (0.42f - 0.5f * fTemp48)) + fSlow8 * (1.0f - std::fabs(fSlow7 * fTemp46 + -1.0f)) + fSlow6 * (static_cast<float>(fTemp47 >= 0.25f) * static_cast<float>(fTemp47 < 0.75f) + 0.5f * fTemp49 * (static_cast<float>(fTemp47 < 0.25f) + static_cast<float>(fTemp47 >= 0.75f)))) * static_cast<float>(fTemp46 < fSlow0) + (fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp43))) & 262143] * (fTemp44 + (1.0f - fTemp42)) + (fTemp42 - fTemp44) * fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp43 + 1))) & 262143]) * (fSlow11 * fTemp41 + fSlow10 * (0.08f * std::cos(fSlow9 * fTemp38) + (0.42f - 0.5f * fTemp40)) + fSlow8 * (1.0f - std::fabs(fSlow7 * fTemp38 + -1.0f)) + fSlow6 * (static_cast<float>(fTemp39 >= 0.25f) * static_cast<float>(fTemp39 < 0.75f) + 0.5f * fTemp41 * (static_cast<float>(fTemp39 < 0.25f) + static_cast<float>(fTemp39 >= 0.75f)))) * static_cast<float>(fTemp38 < fSlow0) + (fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp35))) & 262143] * (fTemp36 + (1.0f - fTemp34)) + (fTemp34 - fTemp36) * fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp35 + 1))) & 262143]) * (fSlow11 * fTemp33 + fSlow10 * (0.08f * std::cos(fSlow9 * fTemp30) + (0.42f - 0.5f * fTemp32)) + fSlow8 * (1.0f - std::fabs(fSlow7 * fTemp30 + -1.0f)) + fSlow6 * (static_cast<float>(fTemp31 >= 0.25f) * static_cast<float>(fTemp31 < 0.75f) + 0.5f * fTemp33 * (static_cast<float>(fTemp31 < 0.25f) + static_cast<float>(fTemp31 >= 0.75f)))) * static_cast<float>(fTemp30 < fSlow0) + (fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp27))) & 262143] * (fTemp28 + (1.0f - fTemp26)) + (fTemp26 - fTemp28) * fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp27 + 1))) & 262143]) * (fSlow11 * fTemp25 + fSlow10 * (0.08f * std::cos(fSlow9 * fTemp22) + (0.42f - 0.5f * fTemp24)) + fSlow8 * (1.0f - std::fabs(fSlow7 * fTemp22 + -1.0f)) + fSlow6 * (static_cast<float>(fTemp23 >= 0.25f) * static_cast<float>(fTemp23 < 0.75f) + 0.5f * fTemp25 * (static_cast<float>(fTemp23 < 0.25f) + static_cast<float>(fTemp23 >= 0.75f)))) * static_cast<float>(fTemp22 < fSlow0) + (fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp19))) & 262143] * (fTemp20 + (1.0f - fTemp18)) + (fTemp18 - fTemp20) * fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp19 + 1))) & 262143]) * (fSlow11 * fTemp17 + fSlow10 * (0.08f * std::cos(fSlow9 * fTemp14) + (0.42f - 0.5f * fTemp16)) + fSlow8 * (1.0f - std::fabs(fSlow7 * fTemp14 + -1.0f)) + fSlow6 * (static_cast<float>(fTemp15 >= 0.25f) * static_cast<float>(fTemp15 < 0.75f) + 0.5f * fTemp17 * (static_cast<float>(fTemp15 < 0.25f) + static_cast<float>(fTemp15 >= 0.75f)))) * static_cast<float>(fTemp14 < fSlow0) + (fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp11))) & 262143] * (fTemp12 + (1.0f - fTemp10)) + (fTemp10 - fTemp12) * fVec0[(IOTA0 - std::min<int>(131073, std::max<int>(0, iTemp11 + 1))) & 262143]) * (fSlow11 * fTemp8 + fSlow10 * (0.08f * std::cos(fSlow9 * fTemp5) + (0.42f - 0.5f * fTemp7)) + fSlow8 * (1.0f - std::fabs(fSlow7 * fTemp5 + -1.0f)) + fSlow6 * (static_cast<float>(fTemp6 >= 0.25f) * static_cast<float>(fTemp6 < 0.75f) + 0.5f * fTemp8 * (static_cast<float>(fTemp6 < 0.25f) + static_cast<float>(fTemp6 >= 0.75f)))) * static_cast<float>(fTemp5 < fSlow0));
			fRec3[1] = fRec3[0];
			iRec4[1] = iRec4[0];
			fRec2[1] = fRec2[0];
			iRec1[1] = iRec1[0];
			iRec0[1] = iRec0[0];
			IOTA0 = IOTA0 + 1;
			fRec5[1] = fRec5[0];
			iRec6[1] = iRec6[0];
			fRec7[1] = fRec7[0];
			iRec8[1] = iRec8[0];
			fRec9[1] = fRec9[0];
			iRec10[1] = iRec10[0];
			fRec11[1] = fRec11[0];
			iRec12[1] = iRec12[0];
			fRec13[1] = fRec13[0];
			iRec14[1] = iRec14[0];
			fRec15[1] = fRec15[0];
			iRec16[1] = iRec16[0];
			fRec17[1] = fRec17[0];
			iRec18[1] = iRec18[0];
			fRec19[1] = fRec19[0];
		}
	}

};
#endif

class gran :
    public od::Object
#ifndef SWIGLUA
    , public ParametersSetter
#endif
{
public:
  gran();
  ~gran();

#ifndef SWIGLUA
  virtual void process();
  od::Inlet mIn{"In"};
  od::Outlet mOut{"Out"};
  od::Option mEnvShape{"EnvShape", 1};
  od::Parameter mGrainSize{"GrainSize"};
  od::Parameter mProbability{"Probability"};
  od::Parameter mSpeed{"Speed"};
  virtual void addParam(const char* label, FAUSTFLOAT* zone) {
    if (strcmp(label, "EnvShape") == 0) {
      ffEnvShape = zone;
    }
    if (strcmp(label, "GrainSize") == 0) {
      ffGrainSize = zone;
    }
    if (strcmp(label, "Probability") == 0) {
      ffProbability = zone;
    }
    if (strcmp(label, "Speed") == 0) {
      ffSpeed = zone;
    }
  }

private:
  FAUSTFLOAT* ffEnvShape;
  FAUSTFLOAT* ffGrainSize;
  FAUSTFLOAT* ffProbability;
  FAUSTFLOAT* ffSpeed;
  Dspgran DSP; // kept for init/buildUserInterface only

  // ── Hand-managed voice state (replaces Dspgran::compute) ──
  static const int NUM_VOICES = 8;
  static const int DELAY_MASK = 0x3FFFF; // 262143
  float delayBuf[262144];  // circular audio buffer (~1MB)
  int   writePos = 0;
  float phasor = 0.0f;
  int   rngState = 12345;
  float randVal = 0.5f;
  int   voiceCounter = 0;
  int   voiceTimer[NUM_VOICES];
  float voiceDelay[NUM_VOICES];

#endif

};


#endif
