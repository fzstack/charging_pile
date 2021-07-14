# for module compiling
import os
Import('RTT_ROOT')
from building import *

cwd = GetCurrentDir()

src = Glob('*.c')
path = [
    cwd
]

# USE_UPPER_END
CPPDEFINES = [ ]
LINKFLAGS = ''
CCFLAGS = ''

if GetDepend(['USE_UPPER_END']):
    CPPDEFINES += ['UPPER_END']

if GetDepend(['USE_LOWER_END']):
    CPPDEFINES += ['LOWER_END']
    # LINKFLAGS += ' --specs=nano.specs'
    CCFLAGS += ' -fno-exceptions'

objs = [DefineGroup('Root', src, depend = [''], CPPPATH = path, CPPDEFINES = CPPDEFINES, CCFLAGS = CCFLAGS, LINKFLAGS = LINKFLAGS)]
list = os.listdir(cwd)

for d in list:
    path = os.path.join(cwd, d)
    if os.path.isfile(os.path.join(path, 'SConscript')):
        objs = objs + SConscript(os.path.join(d, 'SConscript'))

Return('objs')
