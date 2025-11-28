project(
  'triangulate',
  'cpp',
  version: '0.1',
  license: 'GPL-2.0-or-later',
  default_options : ['warning_level=3', 'cpp_std=c++14'],
)

subdir('src')

dependencies = [
]

prj_exe = executable(
  'triangulate',
  main_srcs,
  install : true,
  dependencies : dependencies,
)
