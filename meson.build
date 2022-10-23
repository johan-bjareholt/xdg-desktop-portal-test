project('gnome-introspect-portal', 'c',
  version : '0.0.0',
  default_options : [ 'warning_level=1',
                      'werror=true',
                      'buildtype=debugoptimized' ])

glib_dep = dependency('glib-2.0')
gobject_dep = dependency('gobject-2.0')
gio_dep = dependency('gio-2.0')
gio_unix_dep = dependency('gio-unix-2.0')

gnome = import('gnome')

subdir('data')
subdir('src')
