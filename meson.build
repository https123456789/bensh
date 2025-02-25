project('bensh', 'c')

libpcq_proj = subproject('pcq')
libpcq_dep = libpcq_proj.get_variable('libpcq_dep')

unity_subproject = subproject('unity')
unity_dep = unity_subproject.get_variable('unity_dep')
unity_gen_runner = unity_subproject.get_variable('gen_test_runner')

include_dir = include_directories('include')
sources = files('src/command.c', 'src/environment.c', 'src/lang.c', 'src/sigdefs.c')
tests = ['test_lang', 'test_command']

executable(
  'bensh',
  sources: sources + ['src/main.c'],
  include_directories: include_dir,
  dependencies: libpcq_dep
)

foreach test_name : tests
  runner = unity_gen_runner.process('tests' / test_name + '.c')
  t = executable(
    test_name,
    sources: sources + ['tests' / test_name + '.c', runner],
    include_directories: include_dir,
    dependencies: [libpcq_dep, unity_dep],
    build_by_default: false
  )
  test(test_name, t, should_fail: false)
endforeach
