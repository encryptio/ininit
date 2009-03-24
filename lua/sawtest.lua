freq = makefn(1, function(x) return x*100+440 end, osc_sine(0, 1));
osc = osc_sawtooth(0, freq)

saver(osc, "audio/sawtest-lua.au")
run(3)

