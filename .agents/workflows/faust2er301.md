---
description: Convert Faust .dsp files into ER-301 C++ code
---

This workflow explains how to adapt and compile native Faust DSP code into C++ headers and source files ready to be used as ER-301 custom units.

1. **Prepare your `.dsp` file**:
   Ensure your `.dsp` file includes ER-301 specific metadata and complies with strict syntax requirements:
   - **Input/Output binding**: You MUST declare names for every input and output port your DSP block exposes, starting from `in1` and `out1` respectively up to the sequence of your total `process` inputs/outputs.
   - **Class Declaration Mismatch**: Your `declare name "MyUnitName";` MUST exactly correspond to the root filename of the file. For example, if your file is `gran.dsp`, it must use `declare name "gran";`. If there is a mismatch, the generated C++ will fail attempting to `#include` an inconsistent header name.
   - **UI Variables (C++ Identifiers)**: UI labels (e.g. `hslider("Parameter Name")`) MUST NOT contain spaces or numeric bracket groups `[1]`. The python extraction parses these blindly into raw C++ attributes (`FAUSTFLOAT* ffParameter Name`). Always use spaceless representations like `hslider("ParameterName")` inside the DSP file (the user-facing display label is overridden in Lua later anyway).

   ```faust
   import("stdfaust.lib");
   declare name "MyUnitName"; // <--- MUST match filename "MyUnitName.dsp"

   // ER-301 metadata
   declare er301_in1 "InL";
   declare er301_in2 "InR";
   declare er301_out1 "OutL";
   declare er301_out2 "OutR";

   // DSP Processing
   process = _,_ : ... ; 
   ```

2. **Execute the conversion tool**:
   The conversion tool relies on Python helper scripts (`faustjson2er301cpp`, `faustjson2er301arch_h`), so the `faust2er301` wrapper MUST be run from within its own `bin` directory so it can locate them.
   ```bash
   cd /home/dingus/301dev/er-301-custom-units/faust2er301/bin
   ./faust2er301 /path/to/your/file.dsp
   ```
   *Note: Passing multiple generic `.dsp` files is supported.*

3. **Incorporate generated files**:
   After successful execution, a `.cpp` and `.h` file will be generated in the same directory as your original `.dsp` file. Proceed to incorporate these as you would natively built `od::Object` sub-classes into your ER-301 custom unit `mods/` directory.

4. **Required File Architecture**:
   To natively deploy this module, ensure you create the following surrounding boilerplate to hook the C++ DSP unit into the environment:
   - `mods/<name>/mod.mk`: Needs standard GNU module variables (`include scripts/mod-builder.mk`).
   - `mods/<name>/dsp/<name>.cpp.swig`: Required to explicitly bind the generated `.h` to the SWIG interface.
   - **Makefile Inclusion**: Remember to append your module name explicitly into the overarching `PROJECTS = ...` path inside the root `Makefile`.
   - **Lua GUI bindings**: In `mods/<name>/assets/`, define a `<name>.lua` object invoking the DSP object (e.g., `lib.<name>()`) returning standard ER-301 Unit definitions, tied alongside `toc.lua`.

5. **Parameters & Controls**:
   Any Faust UI elements (e.g. `hslider`, `vslider`, `checkbox`) defined in your DSP file will have automatically been compiled into corresponding standard `od::Parameter` objects inside the generated C++ class. These can be bound in SWIG to the Lua middle-layer identically to any standard ER-301 parameter.

5. **SIMD & NEON Optimization (CRITICAL)**:
   Never rely on or enforce automatic optimization via `-vec -vs 4` keywords. Cortex-A8 compiler arrays fundamentally clash with standard `-ftree-vectorize` mechanisms, leading to scalar drops or CPU failures. **You must perform optimization yourself, to the fullest extent**, following all `er301_development_context.md` laws safely inside `<hal/simd.h>`.
   - Re-generate scalar structures (`faust -cn`).
   - Rewrite the outer sample-for-sample `for (int i=0; i < count; ++i)` calculation entirely inside the DSP `gran.h` layer to loop across 4 `(i += 4)` explicitly via standard block-load `vld1q_f32(inputs[0] + i)` mappings.
   - Separate inner recursive stages via pre-processing inline `<hal/simd.h>` assignments (`vsetq_lane_f32`, `vgetq_lane_f32`, `vmulq_f32`).
   - Constantly provide `+ 1e-9f` positive DC biases to all underlying recursive equations directly on `fRec#[]` assignments inside the loop to neutralize hardware subnormal lockups immediately inside `compute()`.

6. **Compilation and Version Bumping**:
   When invoking the `Makefile` natively (e.g., `make granulator`), **you MUST increment the `PKGVERSION` string defined in `mods/<name>/mod.mk`** (e.g. from `0.0.1` -> `0.0.2`) on every single compile. The ER-301's internal package manager will irrevocably ignore the newly built `.pkg` output if the version string hasn't stepped up, leading to hours of silent caching failures in the sandbox emulator.
