# ER-301 Core DSP Objects (Detailed)

This document provides detailed information on all `libcore` DSP objects available to Lua via SWIG, including their expected datatypes, defaults, and all possible Option macros.

## Core Data Types & Controls
- **`Inlet` / `Outlet`**: Audio or Control Voltage signals (floating-point streams at `FRAMELENGTH` boundaries). In Lua, these are usually connected using `self:tie(src, dest)` or `connect(src, ...)`.
- **`Parameter`**: Continuous floating-point state variables (e.g. `0.0f` to `1.0f`, or direct Hz/Seconds). Often assigned to `GainBias`, `Pitch`, or `Fader` UI controls in Lua.
- **`Option`**: Discrete integer-based state choices (e.g., `0`, `1`, `2`). Often assigned to `List` (dropdown menus) or `Toggle` (checkboxes) UI controls in Lua.

### `Spread`
- **Inlets (Audio/CV)**: `In`, `Spread`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`

### `RationalMultiply`
- **Inlets (Audio/CV)**: `In`, `Numerator`, `Divisor`
- **Outlets (Audio/CV)**: `Out`

### `Limiter`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Options (Integer / List / Toggle)**:
  - `Type` (Default: `LIMITER_CUBIC`) -> Possible Options: `LIMITER_CUBIC`, `LIMITER_HARD`, `LIMITER_INVSQRT`

### `Clipper`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`

### `Fold`
- **Inlets (Audio/CV)**: `In`, `Threshold`, `Upper Gain`, `Lower Gain`
- **Outlets (Audio/CV)**: `Out`

### `Rectify`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Options (Integer / List / Toggle)**:
  - `Type` (Default: `RECTIFY_POSITIVEHALF`) -> Possible Options: `RECTIFY_FULL`, `RECTIFY_NEGATIVEHALF`, `RECTIFY_POSITIVEHALF`

### `GridQuantizer`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Pre-Scale` (Default: `1.0f`)
  - `Levels` (Default: `12.0f`)
  - `Post-Scale` (Default: `1.0f`)

### `SnapToZero`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`

### `Counter`
- **Inlets (Audio/CV)**: `In`, `Reset`
- **Outlets (Audio/CV)**: `Out`
- **Options (Integer / List / Toggle)**:
  - `Wrap` (Default: `CHOICE_YES`) -> Possible Options: `CHOICE_BOTH`, `CHOICE_DOWN`, `CHOICE_LEFT`, `CHOICE_NO`, `CHOICE_RIGHT`, `CHOICE_UNKNOWN`, `CHOICE_UP`, `CHOICE_YES`
  - `Processing Rate` (Default: `PER_FRAME`) -> Possible Options: `PER_FRAME`, `PER_SAMPLE`
- **Parameters (Float / GainBias / Pitch)**:
  - `Step Size` (Default: `0.0f`)
  - `Start` (Default: `0.0f`)
  - `Finish` (Default: `0.0f`)
  - `Gain` (Default: `1.0f`)
  - `Bias` (Default: `0.0f`)
  - `Value` (Default: `0.0f`)

### `WhiteNoise`
- **Outlets (Audio/CV)**: `Out`

### `PinkNoise`
- **Outlets (Audio/CV)**: `Out`

### `VelvetNoise`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Rate` (Default: `1000.0f`)

### `RoundRobin`
- **Inlets (Audio/CV)**: `Next`, `In`
- **Options (Integer / List / Toggle)**:
  - `Processing Rate` (Default: `PER_FRAME`) -> Possible Options: `PER_FRAME`, `PER_SAMPLE`
  - `Active Output` (Default: `0`)

### `TrackAndHold`
- **Inlets (Audio/CV)**: `In`, `Track`
- **Outlets (Audio/CV)**: `Out`
- **Options (Integer / List / Toggle)**:
  - `Flavor` (Default: `TRACKNHOLD_CHOICE_HIGH`) -> Possible Options: `TRACKNHOLD_CHOICE_HIGH`, `TRACKNHOLD_CHOICE_LOW`, `TRACKNHOLD_CHOICE_MINMAX`
- **Parameters (Float / GainBias / Pitch)**:
  - `Value` (Default: `0.0f`)

### `Looper`
- **Inlets (Audio/CV)**: `In`
- **Options (Integer / List / Toggle)**:
  - `Rational` (Default: `1`)
- **Parameters (Float / GainBias / Pitch)**:
  - `Base Period` (Default: `0.0f`)
  - `Base Frequency` (Default: `0.0f`)
  - `Derived Period` (Default: `0.0f`)
  - `Derived Frequency` (Default: `0.0f`)
  - `Multiplier` (Default: `1`)
  - `Divider` (Default: `1`)

### `Clock`
- **Inlets (Audio/CV)**: `Sync`
- **Outlets (Audio/CV)**: `Out`
- **Options (Integer / List / Toggle)**:
  - `Rational` (Default: `1`)
- **Parameters (Float / GainBias / Pitch)**:
  - `Divider` (Default: `1`)
  - `Multiplier` (Default: `1`)
  - `Pulse Width` (Default: `0.5`)
  - `Period` (Default: `0.5`)
  - `Tempo` (Default: `120`)
  - `Frequency` (Default: `2`)

### `QuantizeToClock`
- **Inlets (Audio/CV)**: `In`, `Clock`
- **Outlets (Audio/CV)**: `Out`

### `ZeroCrossingDetector`
- **Inlets (Audio/CV)**: `Gate In`, `Audio In`
- **Outlets (Audio/CV)**: `Gate Out`

### `BumpMap`
- **Inlets (Audio/CV)**: `In`, `Phase`
- **Outlets (Audio/CV)**: `Out`
- **Options (Integer / List / Toggle)**:
  - `Interpolation` (Default: `INTERPOLATION_LINEAR`) -> Possible Options: `INTERPOLATION_LINEAR`, `INTERPOLATION_NONE`, `INTERPOLATION_QUADRATIC`
- **Parameters (Float / GainBias / Pitch)**:
  - `Center` (Default: `0.0f`)
  - `Width` (Default: `0.1f`)
  - `Height` (Default: `0.5f`)
  - `Fade` (Default: `0.25f`)

### `RawHead`
- **Inlets (Audio/CV)**: `Trigger`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Options (Integer / List / Toggle)**:
  - `Routing` (Default: `CHOICE_LEFT`) -> Possible Options: `CHOICE_BOTH`, `CHOICE_DOWN`, `CHOICE_LEFT`, `CHOICE_NO`, `CHOICE_RIGHT`, `CHOICE_UNKNOWN`, `CHOICE_UP`, `CHOICE_YES`
- **Parameters (Float / GainBias / Pitch)**:
  - `Speed` (Default: `1`)

### `LoopHead`
- **Inlets (Audio/CV)**: `Sync`, `Speed`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Options (Integer / List / Toggle)**:
  - `Interpolation` (Default: `INTERPOLATION_QUADRATIC`) -> Possible Options: `INTERPOLATION_LINEAR`, `INTERPOLATION_NONE`, `INTERPOLATION_QUADRATIC`
  - `Routing` (Default: `CHOICE_LEFT`) -> Possible Options: `CHOICE_BOTH`, `CHOICE_DOWN`, `CHOICE_LEFT`, `CHOICE_NO`, `CHOICE_RIGHT`, `CHOICE_UNKNOWN`, `CHOICE_UP`, `CHOICE_YES`
- **Parameters (Float / GainBias / Pitch)**:
  - `Loop Start` (Default: `0.0f`)
  - `Loop Length` (Default: `1.0f`)
  - `Fade Time` (Default: `0.01f`)
  - `Maximum Loop Duration` (Default: `5.0f`)
  - `Actual Loop Duration` (Default: `0.0f`)

### `VariSpeedHead`
- **Inlets (Audio/CV)**: `Trigger`, `Speed`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Options (Integer / List / Toggle)**:
  - `Routing` (Default: `CHOICE_LEFT`) -> Possible Options: `CHOICE_BOTH`, `CHOICE_DOWN`, `CHOICE_LEFT`, `CHOICE_NO`, `CHOICE_RIGHT`, `CHOICE_UNKNOWN`, `CHOICE_UP`, `CHOICE_YES`
  - `Interpolation` (Default: `INTERPOLATION_QUADRATIC`) -> Possible Options: `INTERPOLATION_LINEAR`, `INTERPOLATION_NONE`, `INTERPOLATION_QUADRATIC`

### `RecordHead`

### `PedalLooper`
- **Inlets (Audio/CV)**: `Left In`, `Right In`, `Record`, `Stop`, `Undo`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Options (Integer / List / Toggle)**:
  - `After Record` (Default: `0`)
  - `Pass Input` (Default: `0b11111`)
- **Parameters (Float / GainBias / Pitch)**:
  - `Feedback` (Default: `1.0f`)

### `FeedbackLooper`
- **Inlets (Audio/CV)**: `Reset`, `Punch`, `Engage`, `Feedback`, `Left In`, `Right In`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Start` (Default: `0.0f`)
  - `Fade Time` (Default: `0.005f`)

### `DubLooper`
- **Inlets (Audio/CV)**: `Reset`, `Punch`, `Engage`, `Dub`, `Left In`, `Right In`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Start` (Default: `0.0f`)
  - `Fade Time` (Default: `0.005f`)

### `GranularHead`
- **Inlets (Audio/CV)**: `Trigger`, `Speed`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Duration` (Default: `0.0f`)
  - `Start` (Default: `0.0f`)
  - `Gain` (Default: `0.0f`)
  - `Pan` (Default: `0.0f`)
  - `Squash` (Default: `0.0f`)

### `GrainStretch`
- **Inlets (Audio/CV)**: `Trigger`, `Speed`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Grain Duration` (Default: `0.1f`)
  - `Grain Pitch` (Default: `0.0f`)
  - `Grain Jitter` (Default: `0.0f`)

### `MonoGrainDelay`
- **Inlets (Audio/CV)**: `In`, `Delay`, `Speed`
- **Outlets (Audio/CV)**: `Out`

### `ADSR`
- **Inlets (Audio/CV)**: `Gate`, `Attack`, `Decay`, `Sustain`, `Release`
- **Outlets (Audio/CV)**: `Out`

### `SkewedSineEnvelope`
- **Inlets (Audio/CV)**: `Trigger`, `Level`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Skew` (Default: `0.0f`)
  - `Duration` (Default: `0.1f`)

### `PopReducer`
- **Inlets (Audio/CV)**: `In`, `Trigger`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Time` (Default: `0.0f`)

### `EnvelopeFollower`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Attack Time` (Default: `0.001f`)
  - `Release Time` (Default: `0.010f`)

### `VoltPerOctave`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`

### `PitchCircle`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Pre-Transpose` (Default: `0.0f`)
  - `Post-Transpose` (Default: `0.0f`)

### `Delay`
- **Inlets (Audio/CV)**: `Left In`, `Right In`, `Feedback`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Left Delay` (Default: `0.0f`)
  - `Right Delay` (Default: `0.0f`)
  - `Spread` (Default: `0`)

### `MicroDelay`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Delay` (Default: `0.0f`)

### `DopplerDelay`
- **Inlets (Audio/CV)**: `In`, `Feedback`, `Delay`
- **Outlets (Audio/CV)**: `Out`

### `FixedFilter`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`

### `DeadbandFilter`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Threshold` (Default: `0.1f`)

### `Equalizer3`
- **Inlets (Audio/CV)**: `In`, `Low Gain`, `Mid Gain`, `High Gain`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Low Freq` (Default: `880.0f`)
  - `High Freq` (Default: `5000.0f`)

### `LadderFilter`
- **Inlets (Audio/CV)**: `In`, `V/Oct`, `Resonance`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Fundamental` (Default: `880.0f`)

### `StereoLadderFilter`
- **Inlets (Audio/CV)**: `Left In`, `Right In`, `V/Oct`, `Resonance`, `Fundamental`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`

### `StereoLadderHPF`
- **Inlets (Audio/CV)**: `Left In`, `Right In`, `V/Oct`, `Resonance`, `Fundamental`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`

### `StereoFixedHPF`
- **Inlets (Audio/CV)**: `Left In`, `Right In`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Cutoff` (Default: `27.5`)

### `MonoConvolution`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`

### `StereoConvolution`
- **Inlets (Audio/CV)**: `Left In`, `Right In`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`

### `SlewLimiter`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Options (Integer / List / Toggle)**:
  - `Direction` (Default: `CHOICE_BOTH`) -> Possible Options: `CHOICE_BOTH`, `CHOICE_DOWN`, `CHOICE_LEFT`, `CHOICE_NO`, `CHOICE_RIGHT`, `CHOICE_UNKNOWN`, `CHOICE_UP`, `CHOICE_YES`
- **Parameters (Float / GainBias / Pitch)**:
  - `Time` (Default: `0.0f`)

### `SineOscillator`
- **Inlets (Audio/CV)**: `V/Oct`, `Sync`, `Fundamental`, `Phase`, `Feedback`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Internal Phase` (Default: `0.0f`)

### `TriangleOscillator`
- **Inlets (Audio/CV)**: `V/Oct`, `Sync`, `Fundamental`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Phase` (Default: `0.0f`)

### `SawtoothOscillator`
- **Inlets (Audio/CV)**: `V/Oct`, `Sync`, `Fundamental`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Phase` (Default: `0.0f`)

### `SingleCycle`
- **Inlets (Audio/CV)**: `V/Oct`, `Sync`, `Fundamental`, `Phase`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Internal Phase` (Default: `0.0f`)

### `Freeverb`
- **Inlets (Audio/CV)**: `Left In`, `Right In`
- **Outlets (Audio/CV)**: `Left Out`, `Right Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Size` (Default: `initialroom`)
  - `Damp` (Default: `initialdamp`)
  - `Width` (Default: `initialwidth`)

### `OverlapAddTest`
- **Inlets (Audio/CV)**: `In`
- **Outlets (Audio/CV)**: `Out`
- **Parameters (Float / GainBias / Pitch)**:
  - `Hop` (Default: `1`)
