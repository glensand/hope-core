# hope-core

Core metaprogramming and utility components extracted from the `hope` ecosystem.
This library focuses on practical C++17 tools for:

- struct-to-tuple conversion ("static reflection"-style workflows),
- compile-time type containers (`type_list`, `type_map`),
- type-keyed runtime storage (`type_value_map`),
- compact switch-like dispatch helpers,
- AoS/SoA/ToA data conversion helpers.

If you want a minimal usage project, see: [hope-core-sample](https://github.com/glensand/hope-core-sample)

## Note

`hope-core` is the focused core subset of the wider [`hope`](https://github.com/glensand/hope) project.
It is intended as a lightweight, mostly header-only toolbox with no third-party dependencies.

## Requirements

- C++17 compiler
- CMake 3.11+

## Build

```bash
git clone https://github.com/glensand/hope-core.git
cd hope-core
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

## CMake Integration

The library target is `hope-core` (INTERFACE / header-only style).

```cmake
add_subdirectory(path/to/hope-core)
target_link_libraries(your_target PRIVATE hope-core)
```

Includes are available from:

```cpp
#include "hope_core/tuple/tuple_from_struct.h"
#include "hope_core/tuple/flat_tuple.h"
#include "hope_core/type_traits/type_list.h"
```

## What is inside

- `tuple/*` - flat tuple, struct-to-tuple conversion, tuple helpers
- `type_traits/type_list.h` - compile-time type list algorithms
- `type_traits/typemap.h` - compile-time key->value type mapping
- `type_traits/type_value_map.h` - runtime values keyed by compile-time types
- `switch_expression/*` - compact key/function dispatch helpers
- `type_traits/soa.h` - array-of-struct <-> struct-of-arrays conversion

## Examples

### 1) Struct to tuple (value and reference policies)

```cpp
#include "hope_core/tuple/tuple_from_struct.h"
#include <cassert>
#include <string>

struct user final {
    std::string name;
    int age;
};

int main() {
    user u{ "alice", 21 };

    // Value copy
    auto value_tuple = hope::tuple_from_struct(u);
    assert(value_tuple.get<0>() == "alice");
    assert(value_tuple.get<int>() == 21);

    // Reference view (modifies original struct)
    auto ref_tuple = hope::tuple_from_struct(u, hope::field_policy::reference{});
    ref_tuple.get<0>() = "bob";
    ref_tuple.get<1>() = 42;

    assert(u.name == "bob");
    assert(u.age == 42);
}
```

### 2) Flat tuple utilities

```cpp
#include "hope_core/tuple/flat_tuple.h"
#include <string_view>

int main() {
    auto t = hope::make_flat_tuple(10, 3.14f, std::string_view("hello"));

    // By index
    auto i = t.get<0>();

    // By type
    auto f = t.get<float>();

    // for_each over all values
    t.for_each([](const auto& value) {
        (void)value;
    });
}
```

### 3) `type_list` compile-time algorithms

```cpp
#include "hope_core/type_traits/type_list.h"
#include <type_traits>

using types = hope::type_list<int, float, bool>;

static_assert(hope::size(types{}) == 3);
static_assert(hope::contains<float>(types{}));
static_assert(hope::find<bool>(types{}) == 2);
static_assert(hope::all_of<std::is_arithmetic>(types{}));
```

### 4) `type_map` (compile-time key/value types)

```cpp
#include "hope_core/type_traits/typemap.h"
#include <type_traits>

struct i32_tag {};
struct f32_tag {};

using map_t = hope::type_map<
    hope::type_pair<i32_tag, int>,
    hope::type_pair<f32_tag, float>
>;

static_assert(std::is_same_v<map_t::get_t<i32_tag>, int>);
static_assert(std::is_same_v<map_t::get_v_t<float>, f32_tag>);
```

### 5) `type_value_map` (runtime values by compile-time key)

```cpp
#include "hope_core/type_traits/type_value_map.h"
#include <cassert>
#include <string>

int main() {
    auto values = hope::type_value_map(
        hope::tv<int>(42),
        hope::tv<std::string>("hello")
    );

    assert(values.get<int>() == 42);
    assert(values.get<std::string>() == "hello");

    values.set<int>(100);
    assert(values.get<int>() == 100);
}
```

### 6) Switch expression helper

```cpp
#include "hope_core/switch_expression/switch_expression.h"
#include <string_view>

int main() {
    int result = 0;

    auto sw = hope::switch_expression_lambda(
        "add", [&](int a, int b) { result = a + b; },
        "mul", [&](int a, int b) { result = a * b; }
    );

    sw.apply(std::string_view("mul"), 3, 4); // result = 12
    return result == 12 ? 0 : 1;
}
```

### 7) AoS -> SoA conversion

```cpp
#include "hope_core/type_traits/soa.h"
#include <vector>

struct particle final {
    float x;
    float y;
};

struct particle_soa final {
    std::vector<float> x;
    std::vector<float> y;
};

int main() {
    std::vector<particle> aos = { {1.f, 2.f}, {3.f, 4.f} };
    particle_soa soa;
    hope::soa_with_resize(aos, soa);
}
```

## Running tests

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## License

MIT
