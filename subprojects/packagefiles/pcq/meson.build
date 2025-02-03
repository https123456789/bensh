project('pcq', 'c', version: '0.9.0')

libpcq = library('pcq', 'pcq.c', version: '0.9.0')

libpcq_dep = declare_dependency(
  link_with: libpcq,
  include_directories: include_directories('.')
)
