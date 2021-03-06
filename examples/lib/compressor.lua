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

function compressor(input, threshold, ratio)
    local env = control_envelopefollower(input, 0.003)
    local comp = makefn(1, function (e, v)
        local dBin = math.log10(e) * 10
        local dBout
        if dBin > threshold then
            dBout = (dBin-threshold) * ratio + threshold
        else
            dBout = dBin
        end
        return v * (10^(dBout/10)/e)
    end, env, input)
    return comp
end

