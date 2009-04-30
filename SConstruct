# this is a scons buildfile. http://www.scons.org/

import os



bindingbuilder = Builder(action = "perl luabind/makebinds.pl -builtins $SOURCE -header `find . -name \\*.h | sed 's/^\\.\\///'` > $TARGET")
docbuilder = Builder(action = "perl docs/makedocs.pl $SOURCE > $TARGET")
def catgenerator(source, target, env, for_signature):
    mystr = 'cat '
    source.sort(key=str)
    for i in source:
        mystr += str(i)+' '
    mystr += '> '+str(target[0])
    return mystr
catbuilder = Builder(generator = catgenerator)


# from http://www.scons.org/wiki/UsingPkgConfig
def CheckPKG(context, name):
    context.Message( 'Checking for %s... ' % name )
    ret = context.TryAction('pkg-config --exists \'%s\'' % name)[0]
    context.Result( ret )
    return ret


env = Environment( ENV = os.environ )
env.Append( CCFLAGS=['-Wall', '-Wno-unused-variable', '-O3', '-ffast-math'] )
env.Append( CPPPATH=['.'] )
env.Append( BUILDERS={'LuaMakeBindings' : bindingbuilder, 'MakeDocs': docbuilder, 'Cat': catbuilder} )


# TODO: cleanliness and possible integration with the platform checking below
conf = Configure(env, custom_tests = { 'CheckPKG' : CheckPKG })

luapcname = ''
if conf.CheckPKG('lua'):
    luapcname = 'lua'
elif conf.CheckPKG('lua5.1'):
    luapcname = 'lua5.1'
else:
    print 'lua not found.'
    Exit(1)

env = conf.Finish()

# TODO: should this be inside the configure?
env.ParseConfig('pkg-config --cflags --libs sndfile '+luapcname)

if env['PLATFORM'] == 'darwin':
    env.Append( FRAMEWORKS=['OpenAL'] )
    env.Append( CPPDEFINES=['-DOpenAL_DARWIN'] )
else:
    # something else ... hope it has openal
    env.Append( CPPDEFINES=['-DOpenAL_NORMAL'] )
    env.ParseConfig('pkg-config --cflags --libs openal')



sourcefiles = Split('helpers osc/sawtooth saver filter/bandpass osc/sine distort/atan control/adsr control/envelopefollower input/sndfile osc/white control/frequencyestimator osc/square control/brownian filter/lowpass distort/lofimat control/totrigger osc/triangle filter/chamberlin filter/delay output/openal control/expandtrigger')


sharedcode = []
for i in sourcefiles:
    sharedcode += env.Object(i + '.c')



luabindscode = env.LuaMakeBindings('luabind/bind.c', 'luabind/staticbindings.c')

for file in Split(os.popen("find . -name \\*.h | sed 's/^\\.\\///'").read()):
    Depends(luabindscode, file)

luabindsobj = env.Object('luabind/bind.c')
Depends(luabindsobj, luabindscode)

env.Program(target='ininit', source=['luabind/main.c', luabindsobj] + sharedcode)



env.Program(target='app/testsaw', source=['app/testsaw.c', 'helpers.o', 'saver.o', 'osc/sawtooth.o', 'filter/bandpass.o'])
env.Program(target='app/testsine', source=['app/testsine.c', 'helpers.o', 'saver.o', 'osc/sine.o'])


Mkdir('docs/temp')
docfiles = []
for i in sourcefiles:
    docfiles += env.MakeDocs('docs/temp/'+i, i+'.h')
docfiles += env.MakeDocs('docs/temp/luabind/staticbindings', 'luabind/staticbindings.c')

env.Cat('docs/lua.txt', docfiles)

