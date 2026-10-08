fs = import('fs')

gtest_dep = dependency('gtest_main')

test_srcs = [
  'sample_test.cpp',
]

# Register every TEST()/TEST_F() case as its own meson test, similar to CMake's
# gtest_discover_tests. Meson must know tests at configure time, so we scan the
# sources instead of querying the binary. fs.read() makes meson reconfigure
# whenever a source changes, so new cases are picked up automatically.
# Limitation: only single-line TEST(Suite, Name) / TEST_F(Suite, Name) headers
# are found; TEST_P / TYPED_TEST are not expanded.
foreach src : test_srcs
  exe = executable(fs.stem(src), src,
    dependencies : gtest_dep,
  )

  foreach line : fs.read(src).split('\n')
    line = line.strip()
    foreach macro : ['TEST(', 'TEST_F(']
      if line.startswith(macro)
        parts = line.split(macro)[1].split(')')[0].split(',')
        name = parts[0].strip() + '.' + parts[1].strip()
        test(name, exe,
          args : ['--gtest_filter=' + name],
          protocol : 'gtest',
          suite : fs.stem(src),
        )
      endif
    endforeach
  endforeach
endforeach
