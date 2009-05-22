f1 = 20
f2 = 20000
T = 30

b = f2/f1
a = f1
freq = makefn(100, function () return b^(gettime()/T) * a end)
sweep = osc_sine(0.25, freq)

output_openal(sweep, sweep)
run(T)

