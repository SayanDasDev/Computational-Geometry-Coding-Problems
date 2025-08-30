project(
  'csearch',
  'cpp',
  version: '0.1',
  license: 'GPL-2.0-or-later',
  default_options: ['warning_level=3', 'c_std=c23', 'cpp_std=c++23'],
)

thread_dep = dependency('threads')
gtest_dep = dependency('gtest')

subdir('src')
# subdir('test')
# subdir('bench')

#prj_inc = include_directories('include')
#prj_deps = []
#prj_libs = []


prj_exe = executable(
  'csearch',
  main_srcs,
  #include_directories : prj_inc,
  #dependencies : prj_deps,
  #link_with : prj_libs
)
