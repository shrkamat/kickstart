project('app', ['c', 'cpp'],
  version : '0.1.0',
  meson_version : '>= 1.1.0',
  default_options : [
    'warning_level=3',
    'c_std=c17',
    'cpp_std=c++20',
    'buildtype=debugoptimized',
  ],
)

app_srcs = files(
  'src/main.cpp',
)

executable('app', app_srcs,
  install : true,
)

subdir('tests')
