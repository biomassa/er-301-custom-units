# ER-301 Custom Units Development Guide

This document captures architectural and structural context for developing bespoke units for the ER-301 Eurorack module.

## Repository and Build System
- The codebase uses `odevices/er-301` as a submodule to provide the base ER-301 headers and environment.
- The build is orchestrated via `make` using a Docker container (`tomjfiset/er-301-am335x-build-env:1.1.2`) for cross-compiling to the `am335x` ARM architecture.
- Modules (called Units in ER-301 parlance) are grouped in `mods/`, `Accents/` and `harmonic/`.
- Building a module generally uses `make -j <mod>`. A `mod.mk` file is included in each module directory which pulls in `scripts/mod-builder.mk`.
- **Version Iteration**: ALWAYS iterate the `PKGVERSION` in the module's `mod.mk` (or `Makefile`) on every new compilation. The ER-301 package manager requires incremented version numbers to correctly identify, unpack, and install updated payloads over existing module binaries.

## C++ Low-Level Units (`mods/`, `Accents/`)
The core DSP happens in C++ for maximum performance on the ARM processor.
- Components inherit from `od::Object` (e.g., `class Strike : public od::Object`).
- **Ports:** Inputs, outputs, and options are defined using `od::Inlet`, `od::Outlet`, and `od::Option` within the class.
- **`__restrict__` Pointer Aliasing**: When writing inner `for (int i=0; i<FRAMELENGTH; i++)` processing loops in C++, the object parameter variables (`this->...`) and vectors should ALWAYS be hoisted into local thread registers before the loop, and declared using `__restrict__` tags. If pointers alias, the Cortex-A8 CPU will repeatedly thrash the L1 Cache because it cannot guarantee the memory output doesn't overwrite the inputs. 
- **Explicit NEON Vectorization**: To achieve CPU usage under 10% on the Cortex-A8, it is mandatory to manually vectorize the inner loop using explicit 128-bit `float32x4_t` parallel mathematical processing. Never `#include <arm_neon.h>` directly, as it breaks the Linux x86 emulator compile environment. Instead, ALWAYS `#include <hal/simd.h>`, which provides the platform-safe wrapper logic for `arm_neon.h` intrinsics (`vld1q_f32`, `vst1q_f32`, `vmulq_f32`, etc.). **Note:** For deep reference on intrinsics and CPU pipelines, refer to the ARM Neon Programmer's Guide located at `documentation/DEN0018A_neon_programmers_guide.pdf`.
- **Auto-Vectorization Limits**: Standard `Delay.cpp` achieves high performance by reading strict 4-sample blocks using raw SIMD (`vld1q_f32`, `vst1q_f32`). Audio-rate delay sweep modulation prevents this block loading due to varying array indices. To fix this, quantize the LFO stepping to update strictly once per 4-samples (`i += 4`), which aligns the `rc0` floating points to perfect array boundaries, allowing immediate contiguous chunk loads.
- **Vectorized Interpolation**: Sweeping a delay time with Linear Interpolation generates wideband digital hash due to $C^0$ discontinuous phase derivatives. A $C^1$-continuous polynomial (like Hermite Cubic) is mandatory for pure pitch Doppler shifts. To vectorize this efficiently, sequentially accumulate 4 chronologically-adjacent taps fetched via single scalar array reads (`tmp_d_0..d3`) per temporal block, and calculate the fused polynomial natively within 128-bit vectors (`(((c3 * f) + c2) * f + ...)`).
- **SVF Vectorization Math**: Avoid defining temporary C-arrays inside tight parallel vector operations (`float sat[4]`) as standard gcc `-O3` ARM compilers unpredictably alias memory stack indices, destroying state-variable-filter stability. Manual register isolation via parallel `float` parameters (unrolling the 16 stages via native `vgetq_lane` assignments) natively skips the RAM stack entirely and guarantees mathematical integrity.
- **Physical Tape Flutter**: Do not drive Tape phase integrators with pure PRNG white-noise random walks. While mathematically simple, unconstrained noise injects violent high-frequency jitter into the Doppler pitch shift. Real capstan and pinch-rollers can be perfectly simulated summing low-frequency mechanical trigonometry (`sin(0.6Hz) + sin(11Hz)`). Calculate these once per block, and interpolate a continuous mathematical trace across the audio block. This drops processing weight effectively to zero while generating flawlessly majestic analog pitch drifts.
- **Processing:** Audio is processed in blocks of `FRAMELENGTH` loops. The `process()` method delegates to an internal block-processing loop.
- **SWIG Bindings:** C++ classes are bound to Lua using SWIG. Each module directory contains a `.cpp.swig` file (e.g., `strike.cpp.swig`) which declares the headers to be exposed to the Lua environment. `SWIGLUA` macros are used in the headers to hide implementation details from SWIG.

## Lua High/Middle-Level Units (`/harmonic` and `/mods`)
Lua is used for the middle-layer logic, UI, and connecting the low-level DSP blocks into a functional unit.
- **Available DSP Objects:** A comprehensive list of existing DSP objects exposed to Lua (along with their parameters and options) can be found in [`DSP_OBJECTS.md`](DSP_OBJECTS.md). Always check this reference to reuse existing DSP blocks before building new ones from scratch!
- **Example Units:** An extensive collection of over 60 complete example units (both C++ and Lua) can be found in the `Accents/` directory. These serve as excellent references for DSP processing, UI construction, and graph routing.
- Units are defined as classes inheriting from `Unit` (via `Base.Class`).
- **Initialization:** The `init(args)` sets up the unit's metadata like `title`, `mnemonic`, and `version`.
- **DSP Graph (`onLoadGraph(channelCount)`):** This method builds the DSP graph.
  - Controls, mixers, and oscillators are added via `self:addObject(name, class)`.
  - Audio and control signal routing is established using the `connect(src, out_port, dest, in_port)` and `tie()` functions.
  - Branches are registered with `self:addMonoBranch(...)`.
- **UI Views (`onLoadViews(objects, branches)`):** This method builds the user interface.
  - Defines the visual controls (e.g., `Pitch`, `GainBias`) using references to the initialized objects and branches.
  - Defines scaling mapping for the encoders (`Encoder.getMap`).
  - Returns the `controls` dictionary and a `views` configuration table defining what parameters are visible in expanded/collapsed views.
