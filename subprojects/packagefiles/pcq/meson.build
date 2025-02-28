project('pcq', 'c', version: '0.9.0')

libpcq = static_library('pcq', 'pcq.c')

libpcq_dep = declare_dependency(
  link_with: libpcq,
  include_directories: include_directories('.')
)
