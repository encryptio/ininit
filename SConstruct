import os



bindingbuilder = Builder(action = "perl luabind/makebinds.pl -builtins $SOURCE -header `find . -name \\*.h | sed 's/^\\.\\///'` > $TARGET")

env = Environment( ENV = os.environ )
env.Append( CCFLAGS=['-Wall', '-Wno-unused-variable', '-O3', '-ffast-math'] )
env.Append( CPPPATH=['.'] )
env.Append( BUILDERS={'LuaMakeBindings' : bindingbuilder} )
env.Append( LIBS=['lua', 'sndfile'] )
env.ParseConfig('pkg-config --cflags --libs sndfile lua')



sharedcode = []
for i in Split('helpers die osc/sawtooth saver filter/bandpass osc/sine distort/atan control/adsr control/envelopefollower input/sndfile osc/white control/frequencyestimator osc/square control/brownian filter/lowpass'):
    sharedcode += env.Object(i + '.c')



luabindscode = env.LuaMakeBindings('luabind/bind.c', 'luabind/staticbindings.c')

for file in Split(os.popen("find . -name \\*.h | sed 's/^\\.\\///'").read()):
    Depends(luabindscode, file)

luabindsobj = env.Object('luabind/bind.c')
Depends(luabindsobj, luabindscode)

env.Program(target='ininit', source=['luabind/main.c', luabindsobj] + sharedcode)



env.Program(target='app/testsaw', source=['app/testsaw.c'] + sharedcode)
env.Program(target='app/testsine', source=['app/testsine.c'] + sharedcode)

