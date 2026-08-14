#pragma once

#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <memory>

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

#define TRY(opt)                                                               \
    ({                                                                         \
        auto x = opt;                                                          \
        if (!x.has_value())                                                    \
            return std::nullopt;                                               \
        std::move(x.value());                                                  \
    })

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

struct SourceId {
    usize id;
    explicit SourceId(usize id) : id(id) {}
};

template <typename T> using Ref = std::reference_wrapper<T>;

template <typename T> using Box = std::unique_ptr<T>;

#define JENI_COMPILER_ASSERT(expr, msg, ...)                                   \
    do {                                                                       \
        if (!(expr)) {                                                         \
            printf("%s:%d: [JENI COMPILER ERROR]: " msg "\n", __FILE__,        \
                   __LINE__, ##__VA_ARGS__);                                   \
            printf(" --- note: this is an internal compiler error\n");         \
            abort();                                                           \
        }                                                                      \
    } while (0)