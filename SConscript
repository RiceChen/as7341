from building import *

cwd     = GetCurrentDir()
src     = Glob('*.c')
path    = [cwd]

group = DefineGroup('as7341', src, depend = ['PKG_USING_AS7341'], CPPPATH = path)

Return('group')