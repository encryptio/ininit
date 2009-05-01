fr = makefn(20, function (x) return x*40 + 100 end, control_brownian(0.0001))
width = makefn(20, function (x) return (x+1)/2 end, control_brownian(0.0001))
wave = osc_square(0, fr, width)

output_openal(wave, wave)
run(10)

