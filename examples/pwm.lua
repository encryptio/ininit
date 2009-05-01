basefreq = 440 * 2^(-40/12)

for ch=0,1 do
    waves = {}

    for i=1,5 do
        fr = table_add({ basefreq, table_multiply({ control_brownian(0.0001), 1.5 }) })
        width = table_multiply({ table_add({ control_brownian(0.0001), 1 }), 0.5 })
        wave = osc_square(0, fr, width)

        table.insert(waves, wave)
    end

    out = table_average(waves)

    -- stupid lowpass filter
    ov = 0
    mix = 0.8
    out = makefn(1, function (v) ov = ov*mix + v*(1-mix); return ov end, out)

    if ch == 0 then
        left = out
    else
        right = out
    end
end

output_openal(left, right)
run(10)

