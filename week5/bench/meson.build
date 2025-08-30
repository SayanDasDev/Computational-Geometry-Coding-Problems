bm_srcs = files('bm-main.cpp')

benchmark_dep = dependency('benchmark')
thread_dep = dependency('threads')

#bm_inc = []
bm_deps = [benchmark_dep, thread_dep]
#bm_libs = []

# This executable contains all the benchmarks

bm_exe = executable(
  'run-all',
  bm_srcs,
  #include_directories: bm_inc,
  dependencies: bm_deps,
  #link_with: bm_libs,
)
