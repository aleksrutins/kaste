#include "kaste.h"

#include <gio/gio.h>
#include <json-glib/json-glib.h>

typedef enum {
  PROP_RDNS = 1,
  PROP_SHARED,
  N_PROPERTIES
} KasteBucketProperty;

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

struct _KasteBucket {
  GObject parent_instance;

  gchar *rdns;
  gboolean shared;
};

G_DEFINE_TYPE (KasteBucket, kaste_bucket, G_TYPE_OBJECT)

static void
kaste_bucket_set_property (GObject      *object,
                           guint         property_id,
                           const GValue *value,
                           GParamSpec   *pspec)
{
  KasteBucket *self = KASTE_BUCKET (object);

  switch ((KasteBucketProperty)property_id) {
  case PROP_RDNS:
    g_free(self->rdns);
    self->rdns = g_value_dup_string (value);
    break;
  case PROP_SHARED:
    self->shared = g_value_get_boolean (value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    break;
  }
}

static void
kaste_bucket_get_property (GObject    *object,
                           guint       property_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
  KasteBucket *self = KASTE_BUCKET (object);

  switch ((KasteBucketProperty)property_id) {
  case PROP_RDNS:
    g_value_set_string (value, self->rdns);
    break;
  case PROP_SHARED:
    g_value_set_boolean (value, self->shared);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    break;
  }
}
static void
kaste_bucket_class_init (KasteBucketClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = kaste_bucket_set_property;
  object_class->get_property = kaste_bucket_get_property;

  obj_properties[PROP_RDNS] =
    g_param_spec_string ("rdns",
                         "Reverse DNS ID",
                         "The RDNS of the bucket (e.g. com.rutins.Trilogy.connections)",
                         NULL,
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  obj_properties[PROP_SHARED] =
    g_param_spec_boolean ("shared",
                          "Shared",
                          "Whether this bucket is public or private",
                          FALSE,
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, N_PROPERTIES, obj_properties);
}

static void
kaste_bucket_init(KasteBucket *self)
{
  g_autofree gchar *path = kaste_bucket_get_path (self);
  g_autoptr (GFile) file = g_file_new_for_path (path);
  GError *error = NULL;

  if(self->shared) {
    g_warning ("Shared buckets are not yet implemented; this option will be ignored");
  }

  g_file_make_directory_with_parents (file, NULL, &error);

  if (error != NULL && error->code == G_IO_ERROR_EXISTS) {
    g_assert (g_file_query_file_type (file, G_FILE_QUERY_INFO_NONE, NULL) != G_FILE_TYPE_DIRECTORY);
  }

  if (error != NULL) g_error_free(error);
}

KasteBucket *
kaste_bucket_new (gchar    *rdns,
                  gboolean  shared)
{
  return g_object_new (KASTE_TYPE_BUCKET,
                       "rdns", rdns,
                       "shared", shared,
                       NULL);
}

gchar *kaste_bucket_get_path(KasteBucket *self) {
  g_auto (GPathBuf) path;
  g_autoptr (GString) rdns = g_string_new(self->rdns);
  g_string_replace (rdns, ".", "/", 0);

  g_path_buf_init_from_path (&path, getenv ("XDG_DATA_HOME"));

  g_path_buf_push (&path, "bucket");
  g_path_buf_push (&path, rdns->str);

  return g_path_buf_to_path (&path);
}

gchar *
kaste_bucket_get_resource_path (KasteBucket *self,
                                gchar       *resource)
{
  g_auto (GPathBuf) result;
  g_path_buf_init_from_path (&result, kaste_bucket_get_path (self));
  g_path_buf_push (&result, resource);
  return g_path_buf_to_path (&result);
}

/* And now, the interesting bit */

GObject *
kaste_bucket_read (KasteBucket *self,
                   gchar       *resource,
                   GType        t)
{
  gsize length;
  g_autofree gchar *data;
  GObject *obj;
  g_autofree gchar *path = kaste_bucket_get_resource_path (self, resource);
  GError *err = NULL;

  g_file_get_contents (path, &data, &length, &err);

  if(err != NULL) {
    g_error_free (err);
    return NULL;
  }

  obj = json_gobject_from_data (t, data, length, &err);

  if(err != NULL) {
    g_error_free (err);
    return NULL;
  }

  return obj;
}

gboolean
kaste_bucket_write (KasteBucket *self,
                    gchar       *resource,
                    GObject     *value)
{
  gsize length;
  g_autofree gchar *data = json_gobject_to_data (value, &length);
  g_autofree gchar *path = kaste_bucket_get_resource_path (self, resource);
  GError *err = NULL;

  g_file_set_contents (path, data, length, &err);

  if(err != NULL) {
    g_error_free (err);
    return FALSE;
  }

  return TRUE;
}
