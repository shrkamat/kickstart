
all: app
.PHONY: app

app: build/build.ninja
	@meson compile -C build
	@size -t build/app

build/build.ninja: meson.build
	meson setup build --wrap-mode=forcefallback

.PHONY: check
check: build/build.ninja
	meson test -C build --print-errorlogs

# Files to lint are listed in .clang-tidy-include (subprojects/ is excluded).
.PHONY: lint
lint: build/build.ninja
	ninja -C build clang-tidy

# Runs clang-tidy --fix directly rather than meson's clang-tidy-fix target,
# which needs clang-apply-replacements (not shipped in the pip clang-tidy).
# Directories come from the "<dir>/**/*" lines in .clang-tidy-include.
LINT_DIRS := $(wildcard $(shell sed -n 's|/\*\*/\*$$||p' .clang-tidy-include))
LINT_SRCS = $(shell find $(LINT_DIRS) -type f \( -name '*.c' -o -name '*.cpp' \))

.PHONY: lint-fix
lint-fix: build/build.ninja
	clang-tidy -p build --quiet --fix $(LINT_SRCS)

.PHONY: distclean
distclean:
	rm -rf build
	rm -rf subprojects/googletest-*

