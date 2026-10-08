# app

A C/C++ starter project built with [Meson](https://mesonbuild.com/), using
[GoogleTest](https://github.com/google/googletest) for tests and clang-tidy for
linting.

## Requirements

- Meson >= 1.1 and Ninja
- A C17 / C++20 compiler (GCC or Clang)
- clang-tidy 21, to match the `.clang-format` used by clang-format 21
  (`pip install --user clang-tidy==21.1.6`)

GoogleTest does not need to be installed. Meson downloads and builds it from
`subprojects/gtest.wrap` on first setup.

## Build

```bash
make            # configures build/ on first run, then compiles
./build/app
```

| Target          | What it does                                             |
| --------------- | -------------------------------------------------------- |
| `make`          | Configure (if needed) and build                          |
| `make check`    | Build and run all tests                                  |
| `make lint`     | Run clang-tidy on project sources (fails on any warning) |
| `make lint-fix` | Run clang-tidy and apply its suggested fixes             |
| `make distclean`| Remove `build/` and the downloaded GoogleTest sources    |

## Tests

Tests live in `tests/`. Every `TEST()` / `TEST_F()` case is registered as its
own Meson test named `Suite.Name`, so cases can be listed, run and reported
individually, like CMake's `gtest_discover_tests`.

### Run all tests

```bash
make check
# or
meson test -C build
```

`meson test` rebuilds anything out of date before running.

### List tests

```bash
meson test -C build --list
```

```
sample_test - app:Sample.Addition
sample_test - app:Sample.StringCompare
```

Each line has the form `<suite> - <project>:<test name>`. The suite is the
test file's name without `.cpp`.

### Run a single test case

```bash
meson test -C build Sample.Addition
```

The fully qualified name `app:Sample.Addition` also works.

### Run several test cases

Pass multiple names, or use a shell-style wildcard (quote it so the shell does
not expand it):

```bash
meson test -C build Sample.Addition Sample.StringCompare
meson test -C build 'Sample.*'
```

### Run every case from one test file

```bash
meson test -C build --suite sample_test
```

### See the test's output

By default Meson only prints pass/fail. To see GoogleTest's own output:

```bash
meson test -C build -v Sample.Addition              # always show output
meson test -C build --print-errorlogs Sample.Addition # show output of failures only
```

Full logs are always written to `build/meson-logs/testlog.txt`, and per-case
results to `build/meson-logs/testlog.junit.xml` (useful for CI).

### Repeat a test (flaky test hunting)

```bash
meson test -C build --repeat 100 Sample.Addition
```

### Debug a test in gdb

```bash
meson test -C build --gdb Sample.Addition
```

### Run the test binary directly

Each test file builds into its own executable under `build/tests/`. Running it
directly gives you all of GoogleTest's flags. It does **not** rebuild first, so
run `make` beforehand.

```bash
./build/tests/sample_test                                # all cases in the file
./build/tests/sample_test --gtest_list_tests             # list cases
./build/tests/sample_test --gtest_filter=Sample.Addition # one case
./build/tests/sample_test --gtest_filter='Sample.String*' # wildcard
./build/tests/sample_test --gtest_filter='-Sample.Addition' # all but one
./build/tests/sample_test --gtest_repeat=10 --gtest_break_on_failure
```

### Add a test

- **New case in an existing file:** just add a `TEST(Suite, Name)` or
  `TEST_F(Suite, Name)`. Meson reconfigures automatically on the next
  `make check` / `meson test` and picks it up.
- **New test file:** create `tests/<name>_test.cpp` and add it to `test_srcs` in
  `tests/meson.build`. It becomes its own executable and suite.

Limitations of the automatic discovery: only `TEST(...)` / `TEST_F(...)` headers
written on a single line are found. `TEST_P` and `TYPED_TEST` cases still run
when the whole binary runs (e.g. via `--gtest_filter`), but do not get their
own Meson test entries.

## Linting

clang-tidy checks are configured in `.clang-tidy`, with every warning treated
as an error. Only the paths listed in `.clang-tidy-include` (`src/`,
`include/`, `tests/`) are linted, so code under `subprojects/` is never
checked.

```bash
make lint       # report problems
make lint-fix   # apply automatic fixes, formatted with ../.clang-format
```

`make lint-fix` exits non-zero when it found something, even if it fixed it all.
Run `make lint` afterwards to confirm the code is clean.

If `make lint` says `unknown target 'clang-tidy'`, the build directory was
created before `.clang-tidy` existed. Run `meson setup --reconfigure build`.
