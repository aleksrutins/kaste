# kaste

A simple library for storing data in GNOME applications.

Reference:

## `Kaste.Bucket`
Represents a bucket of data

**Methods**

### `static Kaste.Bucket *new(string rdns, boolean shared)`
Opens or creates a `Kaste.Bucket` with the given RDNS. `shared` is reserved for future plans, and should be set to false.

### `string get_path()`
Returns the storage path of this bucket.

### `string get_resource_path(string resource)`
Returns the storage path of the specified resource, whether or not it exists.

### `string read(string resource, GType t)`
Read a resource.

### `boolean write(string resource, GObject *value)`
Write a resource.

### `GFileEnumerator *list_contents()`
Lists the contents of a bucket.
