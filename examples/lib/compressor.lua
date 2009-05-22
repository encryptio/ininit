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

