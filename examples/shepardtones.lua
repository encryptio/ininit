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

-- some constants. try pow=2, mid=8 for a more standard shepard tone
local pow = 1.5
local mid = 30

local t = {} -- a table to hold each partial
local mult = 1/math.sqrt(2*math.pi) -- a handy constant
for k=1,10 do
    -- 10 partials, each with this frequency and amplitude
    local freq = makefn(50, function ()
        local v = - gettime()/15 + k
        while v > 10 do v = v - 10 end
        while v < 1  do v = v + 10 end
        return mid*pow^v
    end)

    local amp = makefn(50, function (f)
        local a = math.log(f) - math.log(mid*pow^5)
        a = a * 1.3
        return mult * math.exp(-a^2/2) -- gaussian window around mid*pow^5
    end, freq)

    table.insert(t, table_multiply({ amp, osc_sine(0, freq) }) )
end

-- take all the partials into one signal
local signal = table_average(t)

-- and play it forever
output_openal(signal, signal)
while true do run(1) end

