# edt — EverydayTools

A mini library of convenient tools for everyday use: small, dependency-free
C++20 building blocks that keep getting rewritten from scratch otherwise.
Header-only. Everything lives in the `edt::` namespace and is included as
`#include "edt/<area>/<file>.hpp"`.

## What's inside

| Area | Highlights |
|------|-----------|
| `bitset/` | Flat bitset views — `BitsetAdapter<T, Extent = 1>` is a zero-cost view over a single word **or** a `std::span` of words (a static-extent view is just a pointer); plus `FixedBitset`, `DynamicBitset`, `BitIterator`, `EnumBitset`. All `constexpr`. |
| `branchless/` | `IntIf(cond, a, b)` and `SwapIf(cond, a, b)` — branchless integer select/swap (unlike the ternary, always branch-free). |
| `math/` | `Matrix`, vector aliases, `FloatRange`/`IntRange`, surface points. |
| `template/` | Compile-time metaprogramming — type/value lists, `StaticFor`, signature & specialization traits, tagged identifiers, overload sets. |
| aliases | `int_aliases.hpp` (`u8`-`u64`, `i8`-`i64`) and `float_aliases.hpp` (`f32`, `f64`). Each lives in its own namespace so a consumer can hoist it wherever it wants with a using-directive. |
| misc | `algorithm/`, `constexpr/`, `concepts/`, `exception/`, `functional/`, `pointer/`, `preprocessor/`, `ranges/`, `time/`, and top-level `guid.hpp`, `delegate.hpp`, `observable.hpp`, `frame_rate_counter.hpp`, `size.hpp`. |

## Using it (yae)

edt is a first-class [yae](https://github.com/Sunday111/yae) package. Declare it
in your project's `*.package.json`:

```json
{ "link": "https://github.com/Sunday111/edt main", "packages": ["edt"] }
```

and list the `edt` module in the `Dependencies` of any `*.module.json` that uses
it. Then `#include "edt/..."` directly.

## Building & testing

Built through yae — the root `CMakeLists.txt` is yae-generated, so there is no
separate standalone CMake build:

```sh
yae build edt_tests
yae run edt_tests
```

CI (`.github/workflows/tests.yml`) runs the same on every push. The repo carries
its own `.clang-format` / `.clang-tidy`; `yae format` and `yae tidy` must stay
clean.

## Conventions

Files and directories are `snake_case`; the public include root is `edt/`.
Integer domains use exact-width unsigned types; bit ranges are half-open
`[begin, end)`. Value parameters are not marked `const`.
