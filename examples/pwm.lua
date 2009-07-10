--[[
Copyright (c) 2009 Jack Christopher Kastorff

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions, and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * The name Chris Kastorff may not be used to endorse or promote
      products derived from this software without specific prior written
      permission.
--]]

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

