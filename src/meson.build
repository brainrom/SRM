project(
    'SRM',
    'c',
    version : run_command('cat', '../VERSION', check : false).stdout(),
    meson_version: '>= 0.59.0',
    default_options: [
        'warning_level=2',
        'buildtype=release'
    ]
)

version_major = meson.project_version().split('.')[0]
version_minor = meson.project_version().split('.')[1]
version_patch = meson.project_version().split('.')[2]
version_build = run_command('cat', '../BUILD', check : false).stdout()

little_endian = target_machine.endian() == 'little'
big_endian = target_machine.endian() == 'big'

add_project_arguments([
        '-DSRM_LITTLE_ENDIAN=@0@'.format(little_endian.to_int()),
        '-DSRM_BIG_ENDIAN=@0@'.format(big_endian.to_int())
], language: 'c')

c = meson.get_compiler('c')

if get_option('buildtype') == 'custom'
    proj_args = ['-Ofast', '-g3', '-s', '-march=native', '-fno-strict-aliasing']
    add_project_arguments(proj_args, language : 'c')
endif

# ------------ INSTALL CONFIG ------------

# Install directories
headers_install_dir = join_paths(get_option('prefix'), get_option('includedir'), 'SRM')

# Public and private API headers
api_headers = ['/', '/private', '/private/modes']

foreach subdir : api_headers
    install_headers(run_command('find', './lib' + subdir, '-type', 'f', '-name', '*.h', '-maxdepth', '1', check : false).stdout().strip().split('\n'),
        install_dir : headers_install_dir + subdir)
endforeach

conf_data = configuration_data()
conf_data.set('SRM_VERSION_MAJOR', version_major.to_int())
conf_data.set('SRM_VERSION_MINOR', version_minor.to_int())
conf_data.set('SRM_VERSION_PATCH', version_patch.to_int())
conf_data.set('SRM_VERSION_BUILD', version_build.to_int())
configure_file(input : 'lib/SRMTypes.hh',
               output : 'SRMTypes.h',
               configuration : conf_data,
               install_dir : headers_install_dir)
conf_file_inc = include_directories('.')

# ------------ SETUP INCLUDE PATHS ------------

include_paths = ['./lib', conf_file_inc]

# ------------ DEPENDENCIES ------------

pkg              = import('pkgconfig')
gl_dep           = dependency('gl', version: '>= 1.2')
egl_dep          = dependency('egl', version: '>= 1.5')
glesv2_dep       = dependency('glesv2', version: '>= 3.2')
udev_dep         = dependency('libudev', version: '>= 249')
drm_dep          = dependency('libdrm', version: '>= 2.4.113')
gbm_dep          = dependency('gbm', version: '>= 23.2.1')
display_info_dep = dependency('libdisplay-info', required : false)
pthread_dep      = c.find_library('pthread')

if display_info_dep.found() and get_option('use_display_info')
add_project_arguments([
        '-DSRM_USE_LIBDISPLAYINFO'
], language: 'c')
endif

drm_headers_dep = drm_dep.partial_dependency(compile_args: true, includes: true)

# ------------ SOURCE CODE FILES ------------

headers = run_command('find', './lib', '-type', 'f', '-name', '*.h', check : false).stdout().strip().split('\n')
sources = run_command('find', './lib', '-type', 'f', '-name', '*.c', check : false).stdout().strip().split('\n')

SRM = library(
    'SRM',
    sources : sources,
    include_directories : include_paths,
    dependencies : [
        display_info_dep,
        egl_dep,
        gl_dep,
        glesv2_dep,
        udev_dep,
        pthread_dep,
        drm_dep,
        gbm_dep
    ],
    soversion: version_major,
    install : true)

pkg.generate(
    SRM,
    name: 'Simple Rendering Manager',
    description: 'C library for building DRM/KMS applications with OpenGL ES 2.0',
    version: meson.project_version(),
    subdirs: ['SRM'],
    filebase: 'SRM')

if get_option('build_examples') or get_option('build_tests')
    srm_dep = declare_dependency(
        dependencies: [],
        include_directories : include_paths,
        link_with : SRM)
endif

if get_option('build_examples')
    libinput_dep = dependency('libinput', version: '>= 1.20.0')
    libseat_dep = dependency('libseat', version: '>= 0.6.4')
    m_dep = c.find_library('m')

    subdir('examples/srm-basic')
    subdir('examples/srm-display-info')
    subdir('examples/srm-all-connectors')
    subdir('examples/srm-multi-session')
    subdir('examples/srm-direct-scanout')
endif

if get_option('build_tests')
    subdir('tests/srm-test-lists')
    subdir('tests/srm-test-memory')
    subdir('tests/srm-test-memcpy')
endif
