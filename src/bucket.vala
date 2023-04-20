namespace Kaste {
    public class Bucket : Object {
        public string rdns { get; set; }
        public bool shared { get; set; }

        public string path {
            owned get {
                var rdns_path = this.rdns.replace(".", "/");
                var path = new PathBuf.from_path(Environment.get_user_data_dir());
                path.push("bucket");
                path.push(rdns_path);
                return path.free_to_path();
            }
        }

        public Bucket(string rdns, bool shared) {
            this.rdns = rdns;
            this.shared = shared;
        }

        construct {
            if(this.shared) {
                warning("Shared buckets are not yet implemented, and will have no effect");
            }
            var path = this.path;
            var file = File.new_for_path(path);

            try {
                file.make_directory_with_parents();
            } catch(Error e) {
                if(e.code == IOError.EXISTS) {
                    assert(file.query_file_type(NOFOLLOW_SYMLINKS) == DIRECTORY);
                }
            }
        }

        public string get_resource_path(string resource) {
            var path = new PathBuf.from_path(this.path);
            path.push(resource);
            return path.free_to_path();
        }

        public T read<T>(string resource) {
            try {
                string contents;
                var path = get_resource_path(resource);
                FileUtils.get_contents(path, out contents);
                return Json.gobject_from_data(typeof(T), contents);
            } catch(Error e) {
                return null;
            }
        }

        public size_t write(string resource, Object obj) {
            try {
                size_t length;
                var data = Json.gobject_to_data(obj, out length);
                var file = File.new_for_path(get_resource_path(resource));
                file.replace_contents(data.data, null, false, NONE, null);
                return length;
            } catch(Error e) {
                return 0;
            }
        }

        public FileEnumerator list_contents() throws Error {
            return File.new_for_path(this.path).enumerate_children("standard::*", NOFOLLOW_SYMLINKS);
        }
    }
}
