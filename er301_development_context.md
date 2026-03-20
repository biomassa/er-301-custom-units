# ER-301 Custom Units Development Guide

This document captures architectural and structural context for developing bespoke units for the ER-301 Eurorack module.

## Repository and Build System
- The codebase uses `odevices/er-301` as a submodule to provide the base ER-301 headers and environment.
- The build is orchestrated via `make` using a Docker container (`tomjfiset/er-301-am335x-build-env:1.1.2`) for cross-compiling to the `am335x` ARM architecture.
- Modules are grouped in `mods/` and `harmonic/`.
- Building a module generally uses `make -j <mod>`. A `mod.mk` file is included in each module directory which pulls in `scripts/mod-builder.mk`.

## C++ Low-Level Units (`/mods`)
The core DSP happens in C++ for maximum performance on the ARM processor.
- Components inherit from `od::Object` (e.g., `class Strike : public od::Object`).
- **Ports:** Inputs, outputs, and options are defined using `od::Inlet`, `od::Outlet`, and `od::Option` within the class.
- **Processing:** Audio is processed in blocks of `FRAMELENGTH` loops. The `process()` method delegates to an internal block-processing loop.
- **SWIG Bindings:** C++ classes are bound to Lua using SWIG. Each module directory contains a `.cpp.swig` file (e.g., `strike.cpp.swig`) which declares the headers to be exposed to the Lua environment. `SWIGLUA` macros are used in the headers to hide implementation details from SWIG.

## Lua High/Middle-Level Units (`/harmonic` and `/mods`)
Lua is used for the middle-layer logic, UI, and connecting the low-level DSP blocks into a functional unit.
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

## Gotchas & Lessons Learned (TapeDelay Build)
- **`toc.lua` Syntax Constraints**: The emulator requires a very specific syntax for `toc.lua`. It must return a flat table with fields `name`, `title`, `author`, and the actual unit definitions **must** be inside an array named `units`. Attempting to return a nested dictionary (e.g., `return { mymodule = { ... } }`) will result in 0 units being parsed successfully.
- **Includes & Dependencies**: When building C++ modules that depend on headers from the `er-301` core repository (such as `SoundFileWriter.h` which includes `FileWriter.h`), you may encounter missing includes like `<cstdint>`. It is safe and easiest to patch the submodule header directly if needed.
- **`globalConfig` Namespace**: The `globalConfig` variable (used to get `globalConfig.sampleRate`) is defined in `od/config.h` in the global namespace, so it should be referenced directly as `globalConfig`, not `od::globalConfig`.
- **Docker vs Native Build**: Running `make -j <mod>` locally without Docker uses the host toolchain and builds for `x86_64` (or whatever architecture the native host is). This is perfect for testing on the `make emu` emulator locally. Building for the real hardware requires `ARCH=am335x` within the Docker build environment.
- **Version Management**: Always increment the `PKGVERSION` inside the `mod.mk` file on each new build iteration (e.g., from `1.0.0` to `1.0.1`). This prevents version collisions and ensures the unit updates correctly when installing new `.pkg` packages over old ones.
