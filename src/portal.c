#include <glib.h>
#include <gio/gio.h>
#include "window-activity.h"

#define DBUS_NAME "com.bjareholt.johan.PortalTest"
#define DBUS_PATH_ROOT "/com/bjareholt/johan/PortalTest"

static void
get_windows_cb_2(GObject *obj, GAsyncResult *res, gpointer user_data)
{
  GDBusConnection *connection = G_DBUS_CONNECTION(obj);
  GDBusMethodInvocation *invocation = G_DBUS_METHOD_INVOCATION(user_data);
  GError *err = NULL;

  GVariant* reply = g_dbus_connection_call_finish (connection, res, &err);
  if (reply == NULL) {
    g_printerr("Failed to get windows: %s\n", err->message);
    // TODO: Return an error here to invocation
    //g_dbus_method_invocation_return_value(invocation, g_variant_new_tuple(&v, 1));
    return;
  }
  // unpack '(a{ta{sv}})' into 'a{ta{sv}}'
  GVariant *var = g_variant_get_child_value(reply, 0);

  GVariantIter iter;
  guint handle;
  GVariant *dict;

  GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("a(ssbb)"));
  g_variant_iter_init (&iter, var);
  while (g_variant_iter_loop (&iter, "{t@a{sv}}", &handle, &dict)) {
      gchar *title;
      gchar *appid;
      gboolean hidden;
      gboolean focused;

      g_variant_lookup(dict, "title", "s", &title);
      g_variant_lookup(dict, "app-id", "s", &appid);
      g_variant_lookup(dict, "is-hidden", "b", &hidden);
      g_variant_lookup(dict, "has-focus", "b", &focused);
      g_variant_builder_add(builder, "(ssbb)", title, appid, !hidden, focused);
  }
  GVariant *v = g_variant_builder_end(builder);
  g_dbus_method_invocation_return_value(invocation, g_variant_new_tuple(&v, 1));
}

static gboolean
get_windows_cb(WindowActivity *window_activity, GDBusMethodInvocation *invocation, gpointer user_data) {
  GDBusConnection *connection = G_DBUS_CONNECTION(user_data);
  g_dbus_connection_call (
    connection,
    "org.gnome.Shell",
    "/org/gnome/Shell/Introspect",
    "org.gnome.Shell.Introspect",
    "GetWindows",
    NULL,
    G_VARIANT_TYPE("(a{ta{sv}})"),
    G_DBUS_CALL_FLAGS_NONE,
    10,
    NULL,
    get_windows_cb_2,
    invocation
  );
  return TRUE;
}

static void
bus_acquired (GDBusConnection* connection, const gchar* name,
              gpointer user_data) {
  GError* err = NULL;
  g_print("bus acquired\n");

  WindowActivity* window_activity = window_activity_skeleton_new();
  g_object_set(window_activity, "version", 0, NULL);
  g_signal_connect_data(window_activity, "handle-get-windows", (GCallback) get_windows_cb, connection, (GClosureNotify) g_object_unref, 0);

  if (!g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON(window_activity), connection,
        DBUS_PATH_ROOT, &err)) {
    g_printerr("failed to export windowactivity on bus: %s\n", err->message);
  }
}

static void
name_acquired (GDBusConnection* connection, const gchar* name,
              gpointer user_data) {
  g_print("name acquired: %s\n", name);
}

static void
name_lost (GDBusConnection* connection, const gchar* name,
              gpointer user_data) {
  g_print("name lost: %s\n", name);
}

int
main(int argc, char** argv) {
  g_autoptr(GMainLoop) main_loop = g_main_loop_new(NULL, FALSE);

  guint bus_reg_id = g_bus_own_name (
      G_BUS_TYPE_SESSION,
      DBUS_NAME,
      G_BUS_NAME_OWNER_FLAGS_NONE,
      bus_acquired,
      name_acquired,
      name_lost,
      NULL,
      NULL
  );
  if (bus_reg_id < 0) {
    g_print("failed to call g_bus_own_name\n");
    return EXIT_FAILURE;
  }

  g_main_loop_run(main_loop);

  return EXIT_SUCCESS;
}
