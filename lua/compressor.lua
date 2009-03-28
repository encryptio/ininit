input = input_sndfile(0, "/Users/ckastorff/compressme.wav", 1)

threshold = -10
ratio = 1/3

env = control_envelopefollower(input)
comp = makefn(1, function (e, v)
    dBin = math.log10(e) * 10
    if dBin > threshold then
        dBout = (dBin-threshold) * ratio + threshold
    else
        dBout = dBin
    end
    outvol = 10^(dBout/10)/e
    return v*outvol
end, env, input)

saver(input, env, comp, "audio/compressor.au")
run(12)

