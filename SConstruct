env = Environment(CCFLAGS=['-Wall', '-Wno-unused-variable', '-O2', '-ffast-math'], CPPPATH=['.', '/sw/include'], LIBPATH=['/sw/lib'])

for i in Split('helpers die osc/sawtooth saver filter/bandpass osc/sine distort/atan control/adsr'):
    env.Object(i + '.c')

env.Program(target='app/testsaw', source=Split('osc/sawtooth.o saver.o helpers.o die.o filter/bandpass.o app/testsaw.c'))
env.Program(target='app/testsine', source=Split('osc/sine.o saver.o helpers.o die.o app/testsine.c'))
env.Program(target='luabind/luabind', source=Split('die.o luabind/main.c luabind/bind.c saver.o osc/sawtooth.o helpers.o'), LIBS=['lua'])

