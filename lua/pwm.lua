for ch=0,1 do
    waves = {}

    for i=1,30 do
        fr = makefn(i, function (x) return x*2 + 40 end, control_brownian(0.0001))
        width = makefn(i, function (x) return (x+1)/2 end, control_brownian(0.0001))
        wave = osc_square(0, fr, width)

        table.insert(waves, wave)
    end

    out = signals_table_average(waves)

    ov = 0
    mix = 0.8
    out = makefn(1, function (v) ov = ov*mix + v*(1-mix); return ov end, out)

    if ch == 0 then
        left = out
    else
        right = out
    end
end

saver(left, right, "audio/pwm.au")
run(30)
