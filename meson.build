project('bensh', 'c')

libmpc_proj = subproject('mpc')
libmpc_dep = libmpc_proj.get_variable('libmpc_dep')

include_dir = include_directories('include')
sources = files('src/main.c', 'src/command.c', 'src/environment.c')

executable(
  'bensh',
  sources,
  include_directories: include_dir,
  dependencies: libmpc_dep
)
