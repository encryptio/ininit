freq = table_add({ 440, table_multiply({ 100, osc_sine(0, 1) }) })
osc = osc_sawtooth(0, freq)

output_openal(osc, osc)
run(3)

