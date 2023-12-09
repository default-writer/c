/*
 *
 * Russian's IP Protection License
 *
 * Copyright (c) 2023 Artur Mustafin
 *
 * Permission is hereby granted, free of charge, to any person with citizenship
 * and location in Russia including Crimea and all occupations obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * For the rest of the world it is an order to pay royalties by agreement to the
 * author of the code base for ability to use any part of the project for any
 * purpouse including but not limited to the creative ideas or technologies are
 * being used in this owned intellectual property.
 *
 * It is strictly prohibited to use this code base or any part of it for any purpouse
 * including prohibiting or restricive purpouses agains Russians for any EU citizens
 * or other person with USA citizenship, origin or background including work permit
 * or locations from selected territories or any territory or any other country except
 * Russia considered as breaking basic human rights, freedom of speesh or involved in
 * acts of terrorism in a territory owned, occupied or managed by another country.
 *
 */
#include "pointer/v2/pointer_v2.h"
#include "common/memory.h"
#include "list-micro/data.h"
#include "vm/v2/vm_v2.h"

#define DEFAULT_SIZE 0x100

/* definition */
extern const struct list list_micro_definition;

/* definition */
static const struct list* _list = &list_micro_definition;

/* private */

struct pointer {
    void* data;
    u64 size;
    enum type type;
};

struct pointer_data {
    struct vm_data* vm;
    struct list_data* list;
#ifdef USE_GC
    struct list_data* gc;
#endif
};

static struct pointer_data pointer_data;
static struct pointer_data* base = &pointer_data;

struct file_handler {
    FILE* file;
#ifdef USE_MEMORY_DEBUG_INFO
    char* path;
#endif
};

struct list_handler {
    struct list_data* list;
};

void pointer_init(u64 size);
void pointer_destroy(void);

/* api */

static void pointer_push(u64 ptr);
static u64 pointer_copy(u64 ptr);
static u64 pointer_peek(void);
static u64 pointer_pop(void);
static u64 pointer_alloc(void);
#ifndef USE_GC
static void pointer_free(u64 ptr);
#endif
static u64 pointer_list_alloc(void);
static void pointer_list_free(u64 ptr);
static void pointer_list_push(u64 ptr, u64 data_ptr);
static u64 pointer_list_peek(u64 ptr);
static u64 pointer_list_pop(u64 ptr);
static char* pointer_unsafe(u64 ptr);
static u64 pointer_size(u64 ptr);
static void pointer_strcpy(u64 dest_ptr, u64 src_ptr);
static void pointer_strcat(u64 dest_ptr, u64 src_ptr);
static u64 pointer_match_last(u64 src_ptr, u64 match_ptr);
static u64 pointer_load(const char* data);
static u64 pointer_getcwd(void);
static u64 pointer_file_alloc(u64 file_path_ptr, u64 mode_ptr);
static u64 pointer_file_read(u64 ptr);
static void pointer_file_free(u64 ptr);
static void pointer_free_vm(u64 ptr);
static void pointer_printf(u64 ptr);
#if defined(VM_MEMORY_DEBUG_INFO)
static void pointer_dump(struct pointer* ptr);
static void pointer_dump_ref(void** ptr);
#endif
static void pointer_put_char(u64 ptr, char value);

#ifdef USE_GC
static void pointer_gc(void);
#endif

/* internal */

static struct pointer* pointer_alloc_internal(u64 size, enum type type);
static void pointer_realloc_internal(struct pointer* ptr, u64 size);
static void pointer_free_internal(struct pointer* ptr);
static void pointer_gc_internal(struct list_data** current);
static struct pointer* list_alloc_internal(void);
static void list_free_internal(struct pointer* ptr);
static void file_close_internal(struct pointer* ptr);

static struct pointer* pointer_alloc_internal(u64 size, enum type type) {
    struct pointer* ptr = memory->alloc(sizeof(struct pointer));
    if (size != 0) {
        ptr->data = memory->alloc(size);
        ptr->size = size;
    }
    ptr->type = type;
    return ptr;
}

static void pointer_realloc_internal(struct pointer* ptr, u64 size) {
    ptr->data = memory->realloc(ptr->data, ptr->size, size);
    ptr->size = size;
}

static void pointer_free_internal(struct pointer* ptr) {
    if (ptr->data != 0 && ptr->size != 0) {
        memory->free(ptr->data, ptr->size);
#ifdef USE_MEMORY_CLEANUP
        ptr->data = 0;
        ptr->size = 0;
#endif
    }
    memory->free(ptr, sizeof(struct pointer));
}

static void pointer_gc_internal(struct list_data** current) {
    u64 ptr = 0;
    while ((ptr = (u64)_list->pop(current)) != 0) {
        pointer_list_free(ptr);
        pointer_file_free(ptr);
        pointer_free_vm(ptr);
    }
}

static struct pointer* list_alloc_internal(void) {
    struct pointer* ptr = pointer_alloc_internal(sizeof(struct list_handler), TYPE_LIST);
    struct list_handler* handler = ptr->data;
    _list->init(&handler->list);
    return ptr;
}

static void list_free_internal(struct pointer* ptr) {
    struct list_handler* handler = ptr->data;
    pointer_gc_internal(&handler->list);
    _list->destroy(&handler->list);
}

static void file_close_internal(struct pointer* ptr) {
    struct file_handler* handler = ptr->data;
    FILE* f = handler->file;
    fclose(f);
    handler->file = 0;
}

/* implementation */

void pointer_ctx_init(struct pointer_data** ctx, u64 size) {
    /* ctx */
    *ctx = memory->alloc(sizeof(struct pointer_data));
    struct pointer_data* ptr = *ctx;
    /* vm */
    virtual->init(&ptr->vm, size);
    base->vm = ptr->vm;
    /* list */
    _list->init(&ptr->list);
    base->list = ptr->list;
#ifdef USE_GC
    _list->init(&ptr->gc);
    base->gc = ptr->gc;
#endif
}

void pointer_ctx_destroy(struct pointer_data** ctx) {
#if defined(VM_MEMORY_DEBUG_INFO)
    virtual->dump_ref(base->vm);
    virtual->dump(base->vm);
#endif
    struct pointer_data* ptr = *ctx;
#ifdef USE_GC
    pointer_gc();
    ptr->gc = base->gc;
    _list->destroy(&ptr->gc);
#endif
    /* list */
    ptr->list = base->list;
    _list->destroy(&ptr->list);
    /* vm */
    ptr->vm = base->vm;
    virtual->destroy(&ptr->vm);
    /* ctx */
    memory->free(*ctx, sizeof(struct pointer_data));
}

void pointer_init(u64 size) {
    virtual->init(&base->vm, size);
    _list->init(&base->list);
#ifdef USE_GC
    _list->init(&base->gc);
#endif
}

void pointer_destroy(void) {
#if defined(VM_MEMORY_DEBUG_INFO)
    virtual->dump_ref(base->vm);
#endif
#ifdef USE_GC
    pointer_gc();
    _list->destroy(&base->gc);
#endif
    _list->destroy(&base->list);
    virtual->destroy(&base->vm);
}

#ifdef USE_GC
static void pointer_gc(void) {
    pointer_gc_internal(&base->gc);
}
#endif

static void pointer_push(u64 ptr) {
    if (ptr != 0) {
        _list->push(&base->list, (void*)ptr);
    }
}

static u64 pointer_copy(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    const struct pointer* data_ptr = virtual->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_PTR) {
        return 0;
    }
    if (data_ptr->size == 0) {
        return 0;
    }
    struct pointer* copy_ptr = pointer_alloc_internal(data_ptr->size, data_ptr->type);
    memcpy(copy_ptr->data, data_ptr->data, copy_ptr->size); /* NOLINT */
    u64 data = virtual->write(&base->vm, copy_ptr);
#ifdef USE_GC
    _list->push(&base->gc, (void*)data);
#endif
    return data;
}

static u64 pointer_peek(void) {
    return (u64)_list->peek(&base->list);
}

static u64 pointer_pop(void) {
    return (u64)_list->pop(&base->list);
}

static u64 pointer_list_alloc(void) {
    struct pointer* ptr = list_alloc_internal();
    u64 data = virtual->write(&base->vm, ptr);
#ifdef USE_GC
    _list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void pointer_list_free(u64 ptr) {
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = virtual->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->type != TYPE_LIST) {
        return;
    }
    list_free_internal(data_ptr);
    data_ptr = virtual->free(&base->vm, ptr);
    pointer_free_internal(data_ptr);
}

static void pointer_list_push(u64 ptr_list, u64 ptr) {
    if (ptr_list == ptr) {
        return;
    }
    if (ptr_list == 0) {
        return;
    }
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = virtual->read(&base->vm, ptr_list);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->type != TYPE_LIST) {
        return;
    }
    struct list_handler* handler = data_ptr->data;
    _list->push(&handler->list, (void*)ptr);
#ifdef USE_GC
    _list->push(&base->gc, (void*)ptr);
#endif
}

static u64 pointer_list_peek(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    struct pointer* data_ptr = virtual->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_LIST) {
        return 0;
    }
    struct list_handler* handler = data_ptr->data;
    u64 list_peek_ptr = (u64)_list->peek(&handler->list);
    return list_peek_ptr;
}

static u64 pointer_list_pop(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    struct pointer* data_ptr = virtual->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_LIST) {
        return 0;
    }
    struct list_handler* handler = data_ptr->data;
    u64 list_pop_ptr = (u64)_list->pop(&handler->list);
    return list_pop_ptr;
}

static u64 pointer_alloc(void) {
    struct pointer* ptr = pointer_alloc_internal(0, TYPE_PTR);
    u64 data = virtual->write(&base->vm, ptr);
#ifdef USE_GC
    _list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void pointer_free_vm(u64 ptr) {
    struct pointer* data_ptr = virtual->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return;
    }
    data_ptr = virtual->free(&base->vm, ptr);
    pointer_free_internal(data_ptr);
}

#ifndef USE_GC
static void pointer_free(u64 ptr) {
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = virtual->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->type != TYPE_PTR) {
        return;
    }
    data_ptr = virtual->free(&base->vm, ptr);
    pointer_free_internal(data_ptr);
}
#endif

static char* pointer_unsafe(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    struct pointer* data_ptr = virtual->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_PTR) {
        return 0;
    }
    char* ch = data_ptr->data;
    return ch;
}

static u64 pointer_size(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    const struct pointer* data_ptr = virtual->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return 0;
    }
    u64 size = data_ptr->size;
    return size;
}

static void pointer_strcpy(u64 dest, u64 src) {
    if (dest == src) {
        return;
    }
    struct pointer* dest_ptr = virtual->read(&base->vm, dest);
    if (dest_ptr == 0) {
        return;
    }
    if (dest_ptr->type != TYPE_PTR) {
        return;
    }
    const struct pointer* src_ptr = virtual->read(&base->vm, src);
    if (src_ptr == 0) {
        return;
    }
    if (src_ptr->type != TYPE_PTR) {
        return;
    }
    if (src_ptr->data == 0) {
        return;
    }
    if (dest_ptr->size == 0) {
        dest_ptr->data = memory->alloc(src_ptr->size);
        dest_ptr->size = src_ptr->size;
    } else {
        u64 size = src_ptr->size + 1;
        if (dest_ptr->size < size) {
            pointer_realloc_internal(dest_ptr, size);
        }
    }
    char* data_dest = dest_ptr->data;
    const char* data_src = src_ptr->data; /* NOLINT */
    strcpy(data_dest, data_src); /* NOLINT */
}

static void pointer_strcat(u64 dest, u64 src) {
    if (dest == src) {
        return;
    }
    struct pointer* dest_ptr = virtual->read(&base->vm, dest);
    if (dest_ptr == 0) {
        return;
    }
    const struct pointer* src_ptr = virtual->read(&base->vm, src);
    if (src_ptr == 0) {
        return;
    }
    if (src_ptr->size == 0) {
        return;
    }
    if (dest_ptr->size == 0) {
        dest_ptr->data = memory->alloc(src_ptr->size);
        dest_ptr->size = src_ptr->size;
    } else {
        u64 size = dest_ptr->size + src_ptr->size - 1;
        if (dest_ptr->size < size) {
            pointer_realloc_internal(dest_ptr, size);
        }
    }
    char* data_dest = dest_ptr->data;
    const char* data_src = src_ptr->data; /* NOLINT */
    strcat(data_dest, data_src); /* NOLINT */
}

static u64 pointer_match_last(u64 src, u64 match) {
    if (src == 0) {
        return 0;
    }
    if (match == 0) {
        return 0;
    }
    if (src == match) {
        return 0;
    }
    const struct pointer* src_ptr = virtual->read(&base->vm, src);
    if (src_ptr == 0) {
        return 0;
    }
    if (src_ptr->type != TYPE_PTR) {
        return 0;
    }
    if (src_ptr->data == 0) {
        return 0;
    }
    const struct pointer* match_ptr = virtual->read(&base->vm, match);
    if (match_ptr == 0) {
        return 0;
    }
    if (match_ptr->type != TYPE_PTR) {
        return 0;
    }
    if (match_ptr->data == 0) {
        return 0;
    }
    const char* data_src = src_ptr->data;
    const char* data_match = match_ptr->data;
    if (*data_match == 0) {
        return 0;
    }
    char* data_last = strrchr(data_src, *data_match);
    while (data_last != 0 && *data_last != 0 && *data_match != 0 && *data_last == *data_match) {
        data_last++;
        data_match++;
    }
    if (data_last == 0) {
        return 0;
    }
    if (*data_match != 0) {
        return 0;
    }
    struct pointer* last_match_ptr = pointer_alloc_internal(0, TYPE_PTR);
    last_match_ptr->data = --data_last;
    u64 data = virtual->write(&base->vm, last_match_ptr);
#ifdef USE_GC
    _list->push(&base->gc, (void*)data);
#endif
    return data;
}

static u64 pointer_load(const char* src_data) {
    if (src_data == 0) {
        return 0;
    }
    u64 size = strlen(src_data) + 1;
    struct pointer* data_ptr = pointer_alloc_internal(size, TYPE_PTR);
    memcpy(data_ptr->data, src_data, size); /* NOLINT */
    u64 data = virtual->write(&base->vm, data_ptr);
#ifdef USE_GC
    _list->push(&base->gc, (void*)data);
#endif
    return data;
}

static u64 pointer_getcwd(void) {
    char cwd[PATH_MAX + 1];
    u64 data_ptr;
    getcwd(cwd, PATH_MAX);
    u64 size = strlen(cwd) + 1;
    char* ch = memory->alloc(size);
    strcpy(ch, cwd); /* NOLINT */
    data_ptr = pointer_load(ch);
    memory->free(ch, size);
    return data_ptr;
}

static u64 pointer_file_alloc(u64 file_path, u64 mode) {
    if (file_path == 0) {
        return 0;
    }
    if (mode == 0) {
        return 0;
    }
    if (file_path == mode) {
        return 0;
    }
    struct pointer* file_path_ptr = virtual->read(&base->vm, file_path);
    if (file_path_ptr == 0) {
        return 0;
    }
    if (file_path_ptr->type != TYPE_PTR) {
        return 0;
    }
    const struct pointer* mode_ptr = virtual->read(&base->vm, mode);
    if (mode_ptr == 0) {
        return 0;
    }
    if (mode_ptr->type != TYPE_PTR) {
        return 0;
    }
    if (file_path_ptr->size == 0 || file_path_ptr->size > PATH_MAX) {
        return 0;
    }
    const char* file_path_data = file_path_ptr->data;
    if (*file_path_data == 0) {
        return 0;
    }
    const char* mode_data = mode_ptr->data;
    FILE* f = fopen(file_path_data, mode_data); /* NOLINT */
    if (f == 0) {
        return 0;
    }
    struct pointer* f_ptr = pointer_alloc_internal(sizeof(struct file_handler), TYPE_FILE);
    struct file_handler* handler = f_ptr->data;
    handler->file = f;
#ifdef USE_MEMORY_DEBUG_INFO
    handler->path = file_path_ptr->data;
#endif
    u64 data = virtual->write(&base->vm, f_ptr);
#ifdef USE_GC
    _list->push(&base->gc, (void*)data);
#endif
    return data;
}

static u64 pointer_file_read(u64 ptr) {
    if (ptr == 0) {
        return 0;
    }
    struct pointer* data_ptr = virtual->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return 0;
    }
    if (data_ptr->type != TYPE_FILE) {
        return 0;
    }
    struct file_handler* handler = data_ptr->data;
    FILE* f = handler->file;
    fseek(f, 0, SEEK_END); /* NOLINT */
    u64 size = (u64)ftell(f);
    fseek(f, 0, SEEK_SET);
    data_ptr = pointer_alloc_internal(size + 1, TYPE_PTR);
    fread(data_ptr->data, 1, size, handler->file);
    u64 data = virtual->write(&base->vm, data_ptr);
#ifdef USE_GC
    _list->push(&base->gc, (void*)data);
#endif
    return data;
}

static void pointer_file_free(u64 ptr) {
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = virtual->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->type != TYPE_FILE) {
        return;
    }
    file_close_internal(data_ptr);
    data_ptr = virtual->free(&base->vm, ptr);
    pointer_free_internal(data_ptr);
}

static void pointer_printf(u64 ptr) {
    const struct pointer* data_ptr = virtual->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->type != TYPE_PTR) {
        return;
    }
    if (data_ptr->data == 0) {
        return;
    }
    const char* ch = data_ptr->data;
#if defined(VM_MEMORY_DEBUG_INFO)
    void* ptr_data = data_ptr->data;
    printf("   .: %016llx > %016llx\n", (u64)data_ptr, (u64)ptr_data);
#endif
    puts(ch);
}

#if defined(VM_MEMORY_DEBUG_INFO)
static void pointer_dump(struct pointer* ptr) {
    printf("   ^: %016llx > %016llx\n", (u64)ptr, (u64)ptr->data);
}

static void pointer_dump_ref(void** ptr) {
    if (*ptr == 0) {
        return;
    }
    printf("   &: %016llx > %016llx\n", (u64)ptr, (u64)*ptr);
}
#endif

static void pointer_put_char(u64 ptr, char value) {
    if (ptr == 0) {
        return;
    }
    struct pointer* data_ptr = virtual->read(&base->vm, ptr);
    if (data_ptr == 0) {
        return;
    }
    if (data_ptr->type != TYPE_PTR) {
        return;
    }
    char* ch = data_ptr->data;
    *ch = value;
}

/* public */

const struct list_methods list_methods_definition = {
    .alloc = pointer_list_alloc,
    .free = pointer_list_free,
    .peek = pointer_list_peek,
    .pop = pointer_list_pop,
    .push = pointer_list_push
};

const struct file_methods file_methods_definition = {
    .file_alloc = pointer_file_alloc,
    .file_read = pointer_file_read,
    .file_free = pointer_file_free
};

#if defined(VM_MEMORY_DEBUG_INFO)
const struct debug_methods debug_methods_definition = {
    .dump = pointer_dump,
    .dump_ref = pointer_dump_ref
};
#endif

const struct pointer_methods pointer_methods_definition = {
    .init = pointer_init,
    .destroy = pointer_destroy,
    .alloc = pointer_alloc,
    .copy = pointer_copy,
    .peek = pointer_peek,
    .push = pointer_push,
    .pop = pointer_pop,
    .strcpy = pointer_strcpy,
    .strcat = pointer_strcat,
    .match_last = pointer_match_last,
    .load = pointer_load,
    .getcwd = pointer_getcwd,
    .printf = pointer_printf,
    .put_char = pointer_put_char,
    .unsafe = pointer_unsafe,
    .size = pointer_size,
#ifndef USE_GC
    .free = pointer_free,
#else
    .gc = pointer_gc
#endif
};
