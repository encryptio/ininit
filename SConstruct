env = Environment(CCFLAGS=['-Wall', '-O2'], CPPPATH=['.'])

for i in Split('helpers die osc/sawtooth saver filter/bandpass osc/sine'):
    env.Object(i + '.c')

env.Program(target='app/testsaw', source=Split('osc/sawtooth.o saver.o helpers.o die.o filter/bandpass.o app/testsaw.c'))
env.Program(target='app/testsine', source=Split('osc/sine.o saver.o helpers.o die.o app/testsine.c'))

