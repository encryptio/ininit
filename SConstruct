env = Environment(CCFLAGS=['-Wall', '-Wno-unused-variable', '-O2', '-ffast-math'], CPPPATH=['.'])

for i in Split('helpers die osc/sawtooth saver filter/bandpass osc/sine distort/atan'):
    env.Object(i + '.c')

env.Program(target='app/testsaw', source=Split('osc/sawtooth.o saver.o helpers.o die.o filter/bandpass.o app/testsaw.c'))
env.Program(target='app/testsine', source=Split('osc/sine.o saver.o helpers.o die.o app/testsine.c'))

