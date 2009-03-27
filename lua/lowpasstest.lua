stop = getsamplerate() * 0.4

ch = { 1, 0, 1, 0, 1, 1, 1, 0 }
i = 0
chlen = getsamplerate()*0.054
trigger = makefn(100, function () i = i + 100; return ch[(math.floor(i/chlen) % #ch) + 1] end)

mainfreq = 110
frch = {}
for n=1,8 do table.insert(frch, mainfreq * 2^( 0/12)) end
for n=1,8 do table.insert(frch, mainfreq * 2^(-2/12)) end
for n=1,8 do table.insert(frch, mainfreq * 2^( 3/12)) end
for n=1,4 do table.insert(frch, mainfreq * 2^( 5/12)) end
table.insert(frch, mainfreq * 2^( 5/12))
table.insert(frch, mainfreq * 2^( 6/12))
table.insert(frch, mainfreq * 2^( 5/12))
table.insert(frch, mainfreq * 2^(-2/12))
frlen = chlen * 4
basefr = makefn(100, function () return frch[(math.floor(i/frlen) % #frch) + 1] end)

ampctl = control_adsr(trigger, 0.004, 0.08, 0.3, 0.3)
lpctl  = control_adsr(trigger, 0.03,  0.2,  0.08, 0.1)
lpfr   = makefn(1, function (ct) return ct*getsamplerate()/4 end, lpctl)

base = makefn(1, function (o, a) return o*a*0.3 end, osc_sawtooth(0, basefr), ampctl)
filtered = filter_lowpass(base, lpfr, 0.9)

saver(ampctl, lpctl, base, filtered, "audio/lowpasstest.au")
run(frlen * #frch * 2 / getsamplerate())

