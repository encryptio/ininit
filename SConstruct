# Copyright (c) 2009 Jack Christopher Kastorff
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions, and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * The name Chris Kastorff may not be used to endorse or promote
#       products derived from this software without specific prior written
#       permission.

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

    uname = os.popen("uname").read().rstrip()

    # openbsd forgets some includes
    if uname == 'OpenBSD':
        env.MergeFlags( '-pthread -lossaudio' )




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



Mkdir('docs/temp')
docfiles = []
for i in sourcefiles:
    docfiles += env.MakeDocs('docs/temp/'+i, i+'.h')
docfiles += env.MakeDocs('docs/temp/luabind/staticbindings', 'luabind/staticbindings.c')

env.Cat('docs/lua.txt', docfiles)

