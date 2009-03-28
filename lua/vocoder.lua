input = input_sndfile(0, "/Users/ckastorff/soundofvocoder.wav", 1)

freqs = { 150, 150 * 2^(-2/12), 150, 150 * 2^(-2/12), 150 * 2^(2/12), 150 * 2^(3/12), 150 * 2^(2/12), 150 * 2^(-2/12) }
fr = makefn(40, function () return (freqs[(math.floor(getcurrentsample()/getsamplerate() / 0.35) % #freqs) + 1] or freqs[1]) end);
--fr = control_frequencyestimator(input);

ov = 150
mix = 0.9998
fr = makefn(1, function (v) ov = ov*mix + v*(1-mix); return ov end, fr)


base = makefn(1, function (saw, white) return saw*0.7 + white*0.2 end, osc_sawtooth(0, fr), osc_white())
--base = osc_white()
--base = makefn(1, function(snd, white) return snd*0.7 + white*0.03 end, input_sndfile(0, "/Users/ckastorff/code/c/ininit/audio/pwm.au", 1), osc_white())

bands = {}

for fr_i = 1, 200 do
    fr = 100 * 1.125^fr_i
    if fr < getsamplerate()/4 then
        ibp = filter_bandpass(input, fr, 0.9987)
        bbp = filter_bandpass(base,  fr, 0.9987)

        ienv = control_envelopefollower(ibp, 0.015)
        benv = control_envelopefollower(bbp, 0.015)

        thisband = makefn(1, function (ie, be, sig) return (be > 0.0001 and ie/be*sig or 0) end, ienv, benv, bbp)

        table.insert(bands, thisband)
    end
end

out = signals_table_add(bands)

saver(input, base, out, "audio/vocoder.au")
run(2)

