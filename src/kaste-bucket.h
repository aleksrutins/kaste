#pragma once

#ifndef KASTE_INSIDE
# error "Only <kaste.h> can be included directly."
#endif

#include "kaste-version.h"

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define KASTE_TYPE_BUCKET (kaste_bucket_get_type())
G_DECLARE_FINAL_TYPE(KasteBucket, kaste_bucket, KASTE, BUCKET, GObject)

/**
 * kaste_bucket_new:
 * @rdns: the RDNS identifier of the bucket (e.g. com.rutins.Trilogy.connections)
 * @shared: reserved for future use
 *
 * Opens the bucket identified by @rdns, or creates it if it doesn't exist.
 *
 * Returns: a new #KasteBucket
 */
KasteBucket *kaste_bucket_new (gchar         *rdns,
                               gboolean       shared);

/**
 * kaste_bucket_get_path:
 * @self: a #KasteBucket
 *
 * Returns: (transfer full): the path of this bucket's storage.
 */
gchar *kaste_bucket_get_path  (KasteBucket   *self);

/**
 * kaste_bucket_get_resource_path:
 * @self: a #KasteBucket
 * @resource: the name of a resource
 *
 * Returns: (transfer full): the path of the specified resource.
 */
gchar *kaste_bucket_get_resource_path (KasteBucket *self,
                                       gchar       *resource);

/**
 * kaste_bucket_read:
 * @self: a #KasteBucket
 * @resource: the name of the resource to read
 * @t: the type of the resource
 *
 * Reads a resource from a bucket.
 *
 * Returns: (transfer full): the object read from the resource, or NULL.
 */
GObject *kaste_bucket_read    (KasteBucket *self,
                               gchar       *resource,
                               GType        t);

/**
 * kaste_bucket_write:
 * @self: a #KasteBucket
 * @resource: the name of the resource to write to
 * @value: the value to write
 *
 * Writes a resource to a bucket.
 *
 * Returns: true on success, false on failure.
 */
gboolean kaste_bucket_write   (KasteBucket *self,
                               gchar       *resource,
                               GObject     *value);



G_END_DECLS
