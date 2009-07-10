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
]]--

fr = makefn(20, function (x) return x*40 + 100 end, control_brownian(0.0001))
width = makefn(20, function (x) return (x+1)/2 end, control_brownian(0.0001))
wave = osc_square(0, fr, width)

output_openal(wave, wave)
run(10)

