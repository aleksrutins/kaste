api_version = '0.1'

kaste_sources = [
  'kaste.c',
  'kaste-bucket.c'
]

kaste_headers = [
  'kaste.h',
  'kaste-bucket.h'
]

version_split = meson.project_version().split('.')
version_conf = configuration_data()
version_conf.set('VERSION', meson.project_version())
version_conf.set('MAJOR_VERSION', version_split[0])
version_conf.set('MINOR_VERSION', version_split[1])
version_conf.set('MICRO_VERSION', version_split[2])

configure_file(
          input: 'kaste-version.h.in',
         output: 'kaste-version.h',
  configuration: version_conf,
        install: true,
    install_dir: join_paths(get_option('includedir'), 'kaste')
)

kaste_deps = [
  dependency('gio-2.0'),
  dependency('json-glib-1.0')
]

kaste_lib = shared_library('kaste-' + api_version,
  kaste_sources,
  dependencies: kaste_deps,
       install: true,
)

install_headers(kaste_headers, subdir: 'kaste')

pkg = import('pkgconfig')
pkg.generate(
  description: 'A shared library for ...',
    libraries: kaste_lib,
         name: 'kaste',
     filebase: 'kaste-' + api_version,
      version: meson.project_version(),
      subdirs: 'kaste',
     requires: 'gio-2.0',
  install_dir: join_paths(get_option('libdir'), 'pkgconfig')
)

gnome = import('gnome')
kaste_gir = gnome.generate_gir(kaste_lib,
              sources: kaste_headers,
            nsversion: api_version,
            namespace: 'Kaste',
        symbol_prefix: 'kaste',
    identifier_prefix: 'Kaste',
             includes: [ 'Gio-2.0' ],
              install: true,
)
