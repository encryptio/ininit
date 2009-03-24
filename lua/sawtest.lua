freq = makefn(1, function(x) return x*100+440 end, sawtooth(0, 1));
osc = sawtooth(0, freq)
saver(osc, "audio/sawtest-lua.au")
runsamples(getsamplerate()*3);

