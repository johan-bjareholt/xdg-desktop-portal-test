xdg-desktop-portal-test
=======================

Just a small test repo for prototyping xdg-desktop portals.

### Special permissions to introspect windows in Gnome shell

$ busctl --user call org.gnome.Shell /org/gnome/Shell/Introspect org.gnome.Shell.Introspect GetWindows
Call failed: Access denied

To fix, we need to either
- open dconf-editor and set org.gnome.shell.introspection to true
- have the org.freedesktop.impl.portal.desktop.gtk name
