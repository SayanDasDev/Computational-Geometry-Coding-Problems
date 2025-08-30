tst_srcs = files('tst-main.cpp')

thread_dep = dependency('threads')
gtest_dep = dependency('gtest')

#tst_inc = []
tst_deps = [gtest_dep, thread_dep]
#tst_libs = []

# This executable contains all the tests

tst_exe = executable(
  'run-all',
  tst_srcs,
  #include_directories : tst_inc,
  dependencies: tst_deps,
  #link_with : tst_libs,
)

