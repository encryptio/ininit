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

f1 = 20
f2 = 20000
T = 30

b = f2/f1
a = f1
freq = makefn(100, function () return b^(gettime()/T) * a end)
sweep = osc_sine(0.25, freq)

output_openal(sweep, sweep)
run(T)

