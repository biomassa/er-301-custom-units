# ER-301 Custom Units - Agent Development Guide

This repository contains custom DSP units for the ER-301 Eurorack module, built with C++, Lua, and SWIG bindings. Cross-compiled to ARM (am335x) via Docker.

## Repository Structure

```
mods/           # C++ DSP units with SWIG bindings (lojik, sloop, strike, polygon, granulator, wf259, tapedelay)
common/         # Shared DSP utilities, graphics, and UI components
Accents/        # Additional example units
harmonic/       # Pure Lua units (middle-layer)
er-301/         # Submodule: base ER-301 SDK headers
scripts/        # Build system scripts
testing/        # Build artifacts and test packages
```

## Build Commands

### Standard Build
```bash
make -j all              # Build all modules
make -j <mod>           # Build single module (lojik, sloop, strike, polygon, granulator, wf259, tapedelay)
```

### Development Workflow
```bash
make <mod>-install       # Build and copy to emulator (~/.od/front/ER-301/packages)
make emu                 # Build emulator and run
```

### Docker Cross-Compilation
```bash
make testing             # Build for testing profile (includes debug symbols)
make release             # Build for release profile
make <mod>-install-sd    # Copy release package to SD card
```

### Cleanup
```bash
make clean               # Remove build artifacts
```

### Package Inspection
```bash
make <mod>-list          # List package contents
make <mod>-missing       # Check for undefined symbols (ARM only)
```

## Critical: Package Versioning

**ALWAYS increment `PKGVERSION`** in `<mod>/mod.mk` before every compilation. The ER-301 package manager caches packages and will silently ignore updates without version changes.

```makefile
PKGVERSION = 1.2.3  # Increment this before each build
```

## C++ Code Style

### File Organization
- Header files: `<ModuleName>.h` with `#pragma once`
- Implementation: `<ModuleName>.cpp` (minimal - delegates to header)
- SWIG bindings: `<modulename>.cpp.swig`
- Namespace matches module name: `namespace strike`, `namespace lojik`

### Class Structure Pattern
```cpp
#pragma once
#include <od/objects/Object.h>
#include <od/config.h>
#include <hal/simd.h>

namespace strike {
  class Strike : public od::Object {
    public:
      Strike(bool stereo) { /* constructor */ }
      virtual ~Strike() { }

#ifndef SWIGLUA
      virtual void process();
      void processInternal() { /* actual DSP */ }
#endif

      od::Inlet  mTrig   { "Trig" };
      od::Outlet mOut    { "Out" };
      od::Option mBendMode { "Bend Mode", DEFAULT_VALUE };
    private:
      int mChannelCount = 1;
      std::vector<filter::biquad::Filter<2>> mFilter;
  };
}
```

### Member Naming
- Member variables: `mPrefix` (e.g., `mChannelCount`, `mFilter`)
- Inlets/Outlets: `mIn`, `mOut`, `mTrig`, `mEnv`
- Static constants: `mPrefix` with `const`

### Processing Loop Conventions
```cpp
void processInternal() {
  float *in  = mIn.buffer();
  float *out = mOut.buffer();
  
  for (int i = 0; i < FRAMELENGTH; i += 4) {  // 4-sample SIMD blocks
    // SIMD operations using float32x4_t
  }
}
```

### NEON SIMD Intrinsics
- **Include**: `#include <hal/simd.h>` (NOT `<arm_neon.h>`)
- **Loading**: `vld1q_f32(ptr + i)`
- **Storing**: `vst1q_f32(ptr + i, value)`
- **Broadcast**: `vdupq_n_f32(value)`
- **Operations**: `vmulq_f32`, `vaddq_f32`, `vcgtq_f32`, etc.

### Performance Rules
1. **Hoist registers**: Pull `this->` members into local variables before loops
2. **Use `__restrict__`**: Prevent pointer aliasing for performance
3. **Subnormal bias**: Add `+ 1e-9f` to recursive accumulations to avoid subnormal lockups
4. **Block-rate decimation**: Execute control-rate math once per 4-sample block, not per sample
5. **Hermite interpolation**: Use C1-continuous cubic for delay modulation

### Error Handling
- No exceptions (embedded environment)
- Use `CHKNULL()`, `CHK()` macros from SDK
- Validate buffer pointers before use

## SWIG Bindings Pattern

`<modulename>.cpp.swig`:
```swig
%module modulename_libmodulename
%include <od/glue/mod.cpp.swig>

%{
#undef SWIGLUA
#include <Unit1.h>
#include <Unit2.h>
#define SWIGLUA
%}

%include <Unit1.h>
%include <Unit2.h>
```

## Lua Code Style

### Unit Structure
```lua
local app = require "app"
local mod = require "modulename.libmodulename"
local Class = require "Base.Class"
local Unit = require "Unit"
local GainBias = require "Unit.ViewControl.GainBias"
local UnitShared = require "common.assets.UnitShared"

local MyUnit = Class {}
MyUnit:include(Unit)
MyUnit:include(UnitShared)

function MyUnit:init(args)
  args.title = "My Unit"
  args.mnemonic = "mu"
  Unit.init(self, args)
end

function MyUnit:onLoadGraph(channelCount)
  local ctrl = self:addGainBiasControl("param")
  local op = self:addObject("op", mod.MyUnit())
  connect(ctrl, "Out", op, "In")
  self:addMonoBranch("param", ctrl, "In", ctrl, "Out")
end

function MyUnit:onLoadViews()
  return {
    param = GainBias {
      button = "param",
      branch = self.branches.param,
      gainbias = self.objects.param,
      range = self.objects.paramRange,
      biasMap = Encoder.getMap("[0,1]"),
      initialBias = 0.5
    }
  }, {
    expanded = { "param" },
    collapsed = {}
  }
end

return MyUnit
```

### Key Patterns
- Use `UnitShared` for common control helpers
- `addGainBiasControl(name)` for continuous parameters
- `addComparatorControl(name, mode)` for gates/triggers
- `connect(src, outPort, dest, inPort)` for routing
- `self:addMonoBranch(name, obj, inPort, obj, outPort)` for exposed controls

## Adding a New Unit

1. **Create C++ header**: `mods/<module>/<UnitName>.h`
2. **Create C++ impl**: `mods/<module>/<UnitName>.cpp`
3. **Update SWIG**: Add `#include` and `%include` to `mods/<module>/<module>.cpp.swig`
4. **Create Lua UI**: `mods/<module>/assets/<UnitName>.lua`
5. **Update toc.lua**: Add unit registration
6. **Increment PKGVERSION**: In `mods/<module>/mod.mk`
7. **Build**: `make -j <module>`

## DSP Reference

See `DSP_OBJECTS.md` for available core DSP objects and their parameters. Common objects:
- `GainBias`, `ConstantOffset`, `Comparator`, `ParameterAdapter`
- `Delay`, `DopplerDelay`, `MicroDelay`
- `SlewLimiter`, `EnvelopeFollower`, `ADSR`
- `SineOscillator`, `SawtoothOscillator`, `TriangleOscillator`
- `LadderFilter`, `Equalizer3`

## Common DSP Utilities (in `common/`)

- `dsp/filter.h`: Biquad, SVF, one-pole filters (SIMD)
- `dsp/osc.h`: Oscillator shapes and AD envelopes
- `dsp/pitch.h`: V/oct scaling helpers
- `dsp/mix.h`: Mixing utilities
- `graphics/composites/`: ScaleList, Fader, Keyboard, etc.
