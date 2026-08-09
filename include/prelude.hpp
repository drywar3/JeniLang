#ifndef PRELUDE_HPP
#define PRELUDE_HPP

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <new>

// ============================================================================
// 1. FUNDAMENTAL TYPES & PLACEMENT NEW
// ============================================================================

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

using usize = size_t;
using isize = ptrdiff_t;
using byte  = uint8_t;

// Custom Move & Forward (STL replacement)
template <typename T> struct RemoveReference {
    using Type = T;
};
template <typename T> struct RemoveReference<T &> {
    using Type = T;
};
template <typename T> struct RemoveReference<T &&> {
    using Type = T;
};

template <typename T>
constexpr typename RemoveReference<T>::Type &&move(T &&arg) noexcept {
    return static_cast<typename RemoveReference<T>::Type &&>(arg);
}

template <typename T>
constexpr T &&forward(typename RemoveReference<T>::Type &arg) noexcept {
    return static_cast<T &&>(arg);
}

template <typename T>
constexpr T &&forward(typename RemoveReference<T>::Type &&arg) noexcept {
    return static_cast<T &&>(arg);
}

template <typename T> void swap(T &a, T &b) {
    T tmp = move(a);
    a     = move(b);
    b     = move(tmp);
}

template <typename T> constexpr T min(T a, T b) { return (a < b) ? a : b; }

template <typename T> constexpr T max(T a, T b) { return (a > b) ? a : b; }

// ============================================================================
// 2. DIAGNOSTICS & CONTROL FLOW MACROS
// ============================================================================

#define PANIC(msg, ...)                                                        \
    do {                                                                       \
        fprintf(stderr, "[PANIC] %s:%d: " msg "\n", __FILE__, __LINE__,        \
                ##__VA_ARGS__);                                                \
        abort();                                                               \
    } while (0)

#define TODO(msg, ...)                                                         \
    do {                                                                       \
        fprintf(stderr, "[TODO] %s:%d: " msg "\n", __FILE__, __LINE__,         \
                ##__VA_ARGS__);                                                \
        abort();                                                               \
    } while (0)

#define UNREACHABLE()                                                          \
    do {                                                                       \
        fprintf(stderr, "[UNREACHABLE] %s:%d reached unreachable code.\n",     \
                __FILE__, __LINE__);                                           \
        abort();                                                               \
    } while (0)

// Defer macro implementation
template <typename F> struct Defer {
    F fn;
    Defer(F fn) : fn(fn) {}
    ~Defer() { fn(); }
};

struct DeferHelper {
    template <typename F> Defer<F> operator+(F fn) { return Defer<F>(fn); }
};

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)
#define defer auto CONCAT(_defer_var_, __LINE__) = DeferHelper() + [&]()

// ============================================================================
// 3. ALLOCATOR INTERFACE & CONCRETE ALLOCATORS
// ============================================================================

struct Allocator {
    void *ctx;
    void *(*alloc_fn)(void *ctx, usize size, usize align);
    void (*free_fn)(void *ctx, void *ptr, usize size, usize align);
    void *(*realloc_fn)(void *ctx, void *ptr, usize old_size, usize new_size,
                        usize align);

    void *alloc(usize size, usize align = alignof(max_align_t)) const {
        return alloc_fn ? alloc_fn(ctx, size, align) : nullptr;
    }

    void free(void *ptr, usize size, usize align = alignof(max_align_t)) const {
        if (free_fn && ptr)
            free_fn(ctx, ptr, size, align);
    }

    void *realloc(void *ptr, usize old_size, usize new_size,
                  usize align = alignof(max_align_t)) const {
        if (realloc_fn)
            return realloc_fn(ctx, ptr, old_size, new_size, align);
        if (new_size == 0) {
            free(ptr, old_size, align);
            return nullptr;
        }
        void *new_ptr = alloc(new_size, align);
        if (new_ptr && ptr) {
            memcpy(new_ptr, ptr, min(old_size, new_size));
            free(ptr, old_size, align);
        }
        return new_ptr;
    }
};

// C Standard Library Allocator
namespace impl
{
    inline void *c_alloc(void *, usize size, usize) { return malloc(size); }
    inline void c_free(void *, void *ptr, usize, usize) { ::free(ptr); }
    inline void *c_realloc(void *, void *ptr, usize, usize new_size, usize) {
        return ::realloc(ptr, new_size);
    }
} // namespace impl

inline Allocator c_allocator() {
    return Allocator{nullptr, impl::c_alloc, impl::c_free, impl::c_realloc};
}

// Bump Arena Allocator
struct Arena {
    u8 *buffer;
    usize capacity;
    usize offset;
    Allocator backing;

    static Arena create(usize cap, Allocator backing_alloc = c_allocator()) {
        Arena arena;
        arena.capacity = cap;
        arena.offset   = 0;
        arena.backing  = backing_alloc;
        arena.buffer   = (u8 *)backing_alloc.alloc(cap);
        return arena;
    }

    void destroy() {
        if (buffer) {
            backing.free(buffer, capacity);
            buffer   = nullptr;
            capacity = 0;
            offset   = 0;
        }
    }

    void reset() { offset = 0; }

    Allocator allocator() {
        return Allocator{
            this,
            [](void *ctx, usize size, usize align) -> void * {
                Arena *self       = (Arena *)ctx;
                usize current_ptr = (usize)(self->buffer + self->offset);
                usize padding     = (align - (current_ptr % align)) % align;
                if (self->offset + padding + size > self->capacity)
                    return nullptr;
                self->offset += padding;
                void *ptr = self->buffer + self->offset;
                self->offset += size;
                return ptr;
            },
            [](void *, void *, usize, usize) { /* Arena free is a no-op */ },
            [](void *ctx, void *ptr, usize old_size, usize new_size,
               usize align) -> void * {
                Arena *self = (Arena *)ctx;
                if (ptr &&
                    (u8 *)ptr + old_size == self->buffer + self->offset) {
                    isize diff = (isize)new_size - (isize)old_size;
                    if ((isize)self->offset + diff <= (isize)self->capacity) {
                        self->offset += diff;
                        return ptr;
                    }
                }
                void *new_ptr = self->allocator().alloc(new_size, align);
                if (new_ptr && ptr)
                    memcpy(new_ptr, ptr, min(old_size, new_size));
                return new_ptr;
            }};
    }
};

// ============================================================================
// 4. STRING VIEW
// ============================================================================

struct StringView {
    const char *data = "";
    usize len        = 0;

    StringView() = default;
    StringView(const char *str) : data(str), len(str ? strlen(str) : 0) {}
    StringView(const char *str, usize length) : data(str), len(length) {}

    char operator[](usize idx) const { return data[idx]; }

    bool equals(StringView other) const {
        if (len != other.len)
            return false;
        return memcmp(data, other.data, len) == 0;
    }

    bool starts_with(StringView prefix) const {
        if (prefix.len > len)
            return false;
        return memcmp(data, prefix.data, prefix.len) == 0;
    }

    bool ends_with(StringView suffix) const {
        if (suffix.len > len)
            return false;
        return memcmp(data + (len - suffix.len), suffix.data, suffix.len) == 0;
    }

    StringView substr(usize start, usize count = (usize)-1) const {
        if (start >= len)
            return StringView("", 0);
        usize actual_count = min(count, len - start);
        return StringView(data + start, actual_count);
    }

    isize find_first(char c) const {
        for (usize i = 0; i < len; ++i) {
            if (data[i] == c)
                return (isize)i;
        }
        return -1;
    }

    isize find_last(char c) const {
        for (isize i = (isize)len - 1; i >= 0; --i) {
            if (data[i] == c)
                return i;
        }
        return -1;
    }
};

inline bool operator==(StringView a, StringView b) { return a.equals(b); }
inline bool operator!=(StringView a, StringView b) { return !a.equals(b); }

// ============================================================================
// 5. DYNAMIC ARRAY (CONTAINER)
// ============================================================================

template <typename T> struct Array {
    T *data         = nullptr;
    usize count     = 0;
    usize capacity  = 0;
    Allocator alloc = c_allocator();

    Array(Allocator allocator = c_allocator()) : alloc(allocator) {}

    ~Array() {
        clear();
        if (data)
            alloc.free(data, capacity * sizeof(T));
    }

    Array(const Array &)            = delete;
    Array &operator=(const Array &) = delete;

    Array(Array &&other) noexcept
        : data(other.data), count(other.count), capacity(other.capacity),
          alloc(other.alloc) {
        other.data     = nullptr;
        other.count    = 0;
        other.capacity = 0;
    }

    Array &operator=(Array &&other) noexcept {
        if (this != &other) {
            clear();
            if (data)
                alloc.free(data, capacity * sizeof(T));
            data           = other.data;
            count          = other.count;
            capacity       = other.capacity;
            alloc          = other.alloc;
            other.data     = nullptr;
            other.count    = 0;
            other.capacity = 0;
        }
        return *this;
    }

    T &operator[](usize idx) { return data[idx]; }
    const T &operator[](usize idx) const { return data[idx]; }

    void reserve(usize new_cap) {
        if (new_cap <= capacity)
            return;
        T *new_data = (T *)alloc.realloc(data, capacity * sizeof(T),
                                         new_cap * sizeof(T), alignof(T));
        if (!new_data)
            PANIC("Out of memory in Array::reserve");
        data     = new_data;
        capacity = new_cap;
    }

    void push(const T &val) {
        if (count >= capacity)
            reserve(capacity == 0 ? 8 : capacity * 2);
        new (&data[count]) T(val);
        count++;
    }

    void push(T &&val) {
        if (count >= capacity)
            reserve(capacity == 0 ? 8 : capacity * 2);
        new (&data[count]) T(move(val));
        count++;
    }

    T pop() {
        if (count == 0)
            PANIC("Pop from empty Array");
        count--;
        T val = move(data[count]);
        data[count].~T();
        return val;
    }

    void clear() {
        for (usize i = 0; i < count; ++i)
            data[i].~T();
        count = 0;
    }

    T *begin() { return data; }
    T *end() { return data + count; }
    const T *begin() const { return data; }
    const T *end() const { return data + count; }
};

// ============================================================================
// 6. DYNAMIC STRING
// ============================================================================

struct String {
    Array<char> buffer;

    String(Allocator alloc = c_allocator()) : buffer(alloc) {
        buffer.push('\0');
    }

    String(StringView sv, Allocator alloc = c_allocator()) : buffer(alloc) {
        append(sv);
    }

    String(const char *str, Allocator alloc = c_allocator())
        : String(StringView(str), alloc) {}

    void append(StringView sv) {
        if (buffer.count > 0)
            buffer.pop(); // Remove null terminator
        buffer.reserve(buffer.count + sv.len + 1);
        for (usize i = 0; i < sv.len; ++i)
            buffer.push(sv.data[i]);
        buffer.push('\0');
    }

    const char *c_str() const { return buffer.data; }
    usize len() const { return buffer.count > 0 ? buffer.count - 1 : 0; }
    StringView view() const { return StringView(c_str(), len()); }
    operator StringView() const { return view(); }
};

template <typename Key, typename Value> struct HashMap {
  public:
    HashMap(Allocator allocator = c_allocator())
        : m_buckets(allocator.alloc(sizeof(Bucket *) * BUCKET_COUNT)),
          m_allocator(allocator) {
        memset(m_buckets, 0, sizeof(Bucket *) * BUCKET_COUNT);
    }

    HashMap(HashMap const &)            = delete;
    HashMap &operator=(HashMap const &) = delete;

  private:
    struct Bucket {
        Key key;
        Value value;
        bool is_set;
        Bucket *next;
    };

    Bucket **m_buckets;
    static constexpr usize BUCKET_COUNT = 64;
    Allocator m_allocator;
};

// ============================================================================
// 7. PATH MANIPULATORS
// ============================================================================

namespace path
{
    inline bool is_sep(char c) { return c == '/' || c == '\\'; }

    inline StringView parent(StringView p) {
        if (p.len == 0)
            return StringView(".");
        isize idx     = p.find_last('/');
        isize idx_win = p.find_last('\\');
        idx           = max(idx, idx_win);

        if (idx == -1)
            return StringView(".");
        if (idx == 0)
            return StringView(p.data, 1);
        return p.substr(0, (usize)idx);
    }

    inline StringView filename(StringView p) {
        isize idx     = p.find_last('/');
        isize idx_win = p.find_last('\\');
        idx           = max(idx, idx_win);

        if (idx == -1)
            return p;
        return p.substr((usize)idx + 1);
    }

    inline StringView extension(StringView p) {
        StringView fname = filename(p);
        isize idx        = fname.find_last('.');
        if (idx <= 0)
            return StringView("", 0);
        return fname.substr((usize)idx);
    }

    inline String join(StringView a, StringView b,
                       Allocator alloc = c_allocator()) {
        String res(alloc);
        res.append(a);
        if (a.len > 0 && !is_sep(a[a.len - 1])) {
            res.append(StringView("/"));
        }
        if (b.len > 0 && is_sep(b[0])) {
            b = b.substr(1);
        }
        res.append(b);
        return res;
    }
} // namespace path

// ============================================================================
// 8. FILE SYSTEM HELPERS
// ============================================================================

namespace fs
{
    inline bool read_entire_file(StringView filepath, Array<u8> &out_bytes) {
        // StringView to null-terminated C string on stack/heap if necessary
        char path_buf[512];
        const char *cpath = path_buf;
        if (filepath.len < sizeof(path_buf)) {
            memcpy(path_buf, filepath.data, filepath.len);
            path_buf[filepath.len] = '\0';
        } else {
            String temp(filepath);
            cpath = temp.c_str();
        }

        FILE *f = fopen(cpath, "rb");
        if (!f)
            return false;
        defer { fclose(f); };

        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        fseek(f, 0, SEEK_SET);

        if (size < 0)
            return false;

        out_bytes.clear();
        out_bytes.reserve((usize)size);
        out_bytes.count = (usize)size;

        usize read_bytes = fread(out_bytes.data, 1, (usize)size, f);
        return read_bytes == (usize)size;
    }

    inline bool write_entire_file(StringView filepath, const void *data,
                                  usize size) {
        char path_buf[512];
        const char *cpath = path_buf;
        if (filepath.len < sizeof(path_buf)) {
            memcpy(path_buf, filepath.data, filepath.len);
            path_buf[filepath.len] = '\0';
        } else {
            String temp(filepath);
            cpath = temp.c_str();
        }

        FILE *f = fopen(cpath, "wb");
        if (!f)
            return false;
        defer { fclose(f); };

        usize written = fwrite(data, 1, size, f);
        return written == size;
    }
} // namespace fs

#endif // PRELUDE_HPP
