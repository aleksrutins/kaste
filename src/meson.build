api_version = '0.1'

kaste_sources = [
  'bucket.vala',
  'pathbuf.vapi'
]

kaste_deps = [
  dependency('gio-2.0'),
  dependency('gobject-2.0'),
  dependency('glib-2.0'),
  dependency('json-glib-1.0')
]

kaste_lib = shared_library('kaste-' + api_version,
  kaste_sources,
  vala_gir: 'Kaste-' + api_version + '.gir',
  dependencies: kaste_deps,
       install: true,
)

g_ir_compiler = find_program('g-ir-compiler')
custom_target('Kaste-' + api_version + '.typelib', command: [g_ir_compiler, '--shared-library', 'lib' + kaste_lib.name() + '.so', '--output', '@OUTPUT@', meson.current_build_dir() / 'Kaste-' + api_version + '.gir'],
              output: 'Kaste-' + api_version + '.typelib',
              depends: kaste_lib,
              install: true,
              install_dir: get_option('libdir') / 'girepository-1.0')



pkg = import('pkgconfig')
pkg.generate(
  description: 'A library for storing application data in GNOME apps',
    libraries: kaste_lib,
         name: 'kaste',
     filebase: 'kaste-' + api_version,
      version: meson.project_version(),
      subdirs: 'kaste',
     requires: 'gio-2.0',
  install_dir: join_paths(get_option('libdir'), 'pkgconfig')
)