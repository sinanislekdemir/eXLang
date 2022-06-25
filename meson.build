project('exlang', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])

add_global_arguments('-DLINUX_APP', language : 'cpp')
add_global_arguments('-DMAX_SUB_COMMANDS=1024', language: 'cpp')
add_global_arguments('-DMAX_SUBS=32', language: 'cpp')
add_global_arguments('-DPROG_SUBS=32', language: 'cpp')
add_global_arguments('-DMAX_PROGS=1', language: 'cpp')
add_global_arguments('-DMAX_MEM=1024000', language: 'cpp')
add_global_arguments('-DMAX_CMDS=10240', language: 'cpp')

cc = meson.get_compiler('cpp')

inc = include_directories('include')
inc_modules = include_directories('lib/modules/src')
subdir('include')
subdir('src')
subdir('lib/modules/src')


m_dep = cc.find_library('m', required: true)

executable('exlang',
           'src/main.cpp',
           install : true,
           link_with: [exlang_lib, exlang_modules_lib],
           dependencies: [m_dep],
           include_directories: [inc, inc_modules]
           )
