deps = [
  glib_dep,
  gobject_dep,
  gio_dep,
  gio_unix_dep,
]

executable(
  'gnome-introspect-portal',
  sources: ['portal.c'] + window_activity_dep,
  dependencies : [deps],
  install : true,
)
