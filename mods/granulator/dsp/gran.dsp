import("stdfaust.lib");

process = vgroup("gran", environment {
    declare name "gran";
    declare author "Antigravity";

    // ER-301 mappings
    declare er301_in1 "In";
    declare er301_out1 "Out";

    // UI Bindings
    grainSize_sec = hslider("GrainSize", 0.05, 0.001, 2.0, 0.001);
    speed_hz      = hslider("Speed",     10.0, 1.0, 400.0, 0.001);
    prob          = hslider("Probability",0.7, 0.0, 1.0,   0.01);
    envShape      = hslider("EnvShape",   0.0, 0.0, 3.0, 1.0);

    // Master trigger ticking at Speed
    master_phasor = +(speed_hz / ma.SR) ~ fmod(_, 1.0);
    tick          = master_phasor < master_phasor';

    // Apply Probability
    rand_val      = (no.noise + 1.0) * 0.5 : ba.sAndH(tick);
    valid_tick    = tick & (rand_val <= prob);

    // Voice allocator
    voice_count   = 8;
    counter       = +(valid_tick) ~ _ ;

    // Bounds and allocations
    grainSize_samps = max(1.0, grainSize_sec * ma.SR);
    max_delay       = 131072;

    // Envelope shape functions
    // phase = normalized position in grain [0..1)
    hann_env(phase)     = 0.5 * (1.0 - cos(2.0 * ma.PI * phase));
    blackman_env(phase) = 0.42 - 0.5 * cos(2.0 * ma.PI * phase)
                          + 0.08 * cos(4.0 * ma.PI * phase);
    triangle_env(phase) = 1.0 - abs(2.0 * phase - 1.0);
    tukey_env(phase)    = hann_env(phase) * (phase < 0.25)
                        + 1.0 * (phase >= 0.25) * (phase < 0.75)
                        + hann_env(phase) * (phase >= 0.75);

    // Select envelope based on integer shape parameter
    grain_env(phase, shape) = hann_env(phase)     * (shape < 0.5)
                            + blackman_env(phase)  * (shape >= 0.5) * (shape < 1.5)
                            + triangle_env(phase)  * (shape >= 1.5) * (shape < 2.5)
                            + tukey_env(phase)     * (shape >= 2.5);

    voice(i, in_sig) = out_sig
    with {
        trig = valid_tick & ((counter % voice_count) == i);

        rand_delay = ((no.noise + 1.0) * 0.5 : ba.sAndH(trig)) * (max_delay - grainSize_samps);
        safe_delay = max(0.0, min(float(max_delay), rand_delay));

        grain_audio = in_sig : de.fdelay(max_delay, safe_delay);

        timer = (+(1) : *(1 - trig)) ~ _ ;

        valid_window = timer < grainSize_samps;
        phase = timer / grainSize_samps;
        env = grain_env(phase, envShape) * valid_window;

        out_sig = grain_audio * env;
    };

    process(x) = x <: (voice(0), voice(1), voice(2), voice(3),
                        voice(4), voice(5), voice(6), voice(7)) :> _ ;

}.process);