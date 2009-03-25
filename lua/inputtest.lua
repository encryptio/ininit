file = input_sndfile(0, "/Users/ckastorff/coolstuff.wav", 1)

filterfreq = makefn(40, function(x) return x*200+1000 end, osc_sine(0, 8))
bp = filter_bandpass(file, filterfreq, 0.996)

saver(file, bp, "audio/inputtest.au")
run(3)

