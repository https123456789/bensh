project('bensh', 'c')

include_dir = include_directories('include')
sources = files('src/main.c', 'src/command.c', 'src/environment.c')

executable(
  'bensh',
  sources,
  include_directories: include_dir
)
