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

freq = table_add({ 440, table_multiply({ 100, osc_sine(0, 1) }) })
osc = osc_sawtooth(0, freq)

output_openal(osc, osc)
run(3)

