namespace GLib {
    [Version(since = "2.76")]
    [Compact]
    [CCode(copy_function = "g_path_buf_copy", free_function = "g_path_buf_free")]
    public class PathBuf {
        public PathBuf();
        public PathBuf.from_path(string path);
        public bool equal(PathBuf v2);
        public string free_to_path();
        public bool pop();
        public unowned PathBuf push(string path);
        public bool set_extension(string extension);
        public bool set_filename(string file_name);
        public string? to_path();
    }
}
