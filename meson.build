project('bensh', 'c')

libpcq_proj = subproject('pcq')
libpcq_dep = libpcq_proj.get_variable('libpcq_dep')

include_dir = include_directories('include')
sources = files('src/main.c', 'src/command.c', 'src/environment.c', 'src/lang.c', 'src/sigdefs.c')

executable(
  'bensh',
  sources,
  include_directories: include_dir,
  dependencies: libpcq_dep
)
