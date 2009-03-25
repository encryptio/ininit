input = input_sndfile(0, "/Users/ckastorff/coolstuff.wav", 1)

base = makefn(1, function (saw, white) return saw*0.8 + white*0.1 end, osc_sawtooth(0, 100), osc_white())

bands = {}

for fr_i = 1, 100 do
    fr = 100 * 1.25^fr_i
    if fr < getsamplerate()/3 then
        ibp = filter_bandpass(input, fr, 0.999)
        bbp = filter_bandpass(base,  fr, 0.999)

        ienv = control_envelopefollower(ibp)
        benv = control_envelopefollower(bbp)

        thisband = makefn(1, function (ie, be, sig) return ie/be*sig end, ienv, benv, bbp)

        table.insert(bands, thisband)
    end
end

out = signals_table_add(bands)

saver(input, base, out, "audio/vocoder.au")
run(3)

