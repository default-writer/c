# TODO

## improvements

- file functions
- error handling

```bash
diff --git a/src/vm/types/file/file_v1.c b/src/vm/types/file/file_v1.c
index 51557bd9..ec3517d9 100644
--- a/src/vm/types/file/file_v1.c
+++ b/src/vm/types/file/file_v1.c
@@ -4,7 +4,7 @@
  * Created:
  *   11 December 2023 at 9:06:14 GMT+3
  * Modified:
- *   March 4, 2025 at 10:49:05 AM GMT+3
+ *   March 5, 2025 at 11:41:44 AM GMT+3
  *
  */
 /*
@@ -127,20 +127,55 @@ static void file_free(u64 ptr) {
     type_desctructor(*data_ptr);
 }
 
+static void file_error(const char* func, const char* file, int line) {
+    perror(func);
+    fprintf(stderr, "%s:%d: %s#\n", file, line - 1, func); /* NOLINT: fprintf */
+}
+
 static u64 file_data(u64 ptr) {
     pointer_ptr* data_ptr = CALL(virtual)->read_type(ptr, id);
     if (data_ptr == 0 || *data_ptr == 0) {
         return 0;
     }
     struct file_handler* handler = CALL(pointer)->read(*data_ptr);
+    if (handler->file == 0) {
+        return 0;
+    }
     FILE* f = handler->file;
-    fseek(f, 0, SEEK_END); /* NOLINT */
+    u64 seek = (u64)fseek(f, 0, SEEK_END); /* NOLINT */
+    if (seek != 0) {
+        file_error("fseek()", __FILE__, __LINE__);
+        fclose(handler->file);
+        handler->file = 0;
+        return 0;
+    }
     u64 size = (u64)ftell(f);
-    fseek(f, 0, SEEK_SET);
+    if (size == 0xffffffffffffffff) {
+        file_error("ftell()", __FILE__, __LINE__);
+        fclose(handler->file);
+        handler->file = 0;
+        return 0;
+    }
+    seek = (u64)fseek(f, 0, SEEK_SET);
+    if (seek != 0) {
+        file_error("fseek()", __FILE__, __LINE__);
+        fclose(handler->file);
+        handler->file = 0;
+        return 0;
+    }
     u64 data_size = size + 1;
     u64 data_handle = CALL(data)->alloc(data_size);
     void* file_data = CALL(data)->unsafe(data_handle);
-    fread(file_data, 1, size, handler->file);
+    size_t read_bytes = fread(file_data, 1, size, handler->file);
+    if (read_bytes < size) {
+        u64 error = (u64)ferror(handler->file);
+        if (error) {
+            file_error("fread()", __FILE__, __LINE__);
+            fclose(handler->file);
+            handler->file = 0;
+            return 0;
+        }
+    }
     return data_handle;
 }
```
