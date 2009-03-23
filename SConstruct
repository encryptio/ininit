env = Environment(CCFLAGS=['-Wall', '-O2'], CPPPATH=['.'])

for i in Split('helpers die osc/sawtooth saver filter/bandpass'):
    env.Object(i + '.c')

env.Program(target='app/testsaw', source=Split('osc/sawtooth.o saver.o helpers.o die.o filter/bandpass.o app/testsaw.c'))

