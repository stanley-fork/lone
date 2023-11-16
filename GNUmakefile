# SPDX-License-Identifier: AGPL-3.0-or-later

MAKEFLAGS += --no-builtin-variables --no-builtin-rules

CC := cc
LD := ld
CFLAGS := -Wall -Wextra -Wpedantic -Wno-unused-function -Wno-unused-parameter -Os

ifdef TARGET
  ifndef UAPI
    $(error UAPI must be defined when cross compiling)
  endif

  TARGET.triple := $(TARGET)-unknown-linux-elf
  override CC := clang -target $(TARGET.triple)
else
  TARGET := $(shell uname -m)
endif

source_to_object = $(patsubst $(directories.source)/%.c,$(directories.build.objects)/%.o,$(1))
source_to_prerequisite = $(patsubst $(directories.source)/%.c,$(directories.build.prerequisites)/%.d,$(1))
source_to_tool = $(patsubst $(directories.source.tools)/%.c,$(directories.build.tools)/%,$(1))

ARCH := $(TARGET)

directories.build := build/$(ARCH)
directories.build.tools := $(directories.build)/tools
directories.build.objects := $(directories.build)/objects
directories.build.objects.tools := $(directories.build.objects)/tools
directories.build.prerequisites := $(directories.build)/prerequisites
directories.build.include := $(directories.build)/include
directories.create :=

directories.include := include architecture/$(ARCH)/include $(directories.build.include)
directories.source := source
directories.source.tools := $(directories.source)/tools

files.sources.all := $(shell find $(directories.source) -type f)
files.sources.tools := $(filter $(directories.source.tools)/%,$(files.sources.all))
files.sources.lone := $(filter-out $(directories.source.tools)/%,$(files.sources.all))

targets.phony :=
targets.NR.list := $(directories.build)/NR.list
targets.NR.c := $(directories.build.include)/lone/NR.c
targets.NR := $(targets.NR.list) $(targets.NR.c)
targets.objects.all := $(call source_to_object,$(files.sources.all))
targets.objects.lone := $(call source_to_object,$(files.sources.lone))
targets.objects.tools := $(call source_to_object,$(files.sources.tools))
targets.lone := $(directories.build)/lone
targets.prerequisites := $(call source_to_prerequisite,$(files.sources.all))
targets.tools := $(call source_to_tool,$(files.sources.tools))

directories.create += $(dir $(targets.lone) $(targets.objects.all) $(targets.prerequisites) $(targets.NR) $(targets.tools))

flags.whole_program := -fvisibility=hidden

ifdef LTO
  flags.lto := -flto
  ifeq ($(CC),gcc)
    flags.whole_program += -fwhole-program
  endif
endif

flags.definitions := -D LONE_ARCH=$(ARCH)
flags.include_directories := $(foreach directory,$(directories.include),-I $(directory))
flags.system_include_directories := $(if $(UAPI),-isystem $(UAPI))
flags.prerequisites_generation = -MMD -MF $(call source_to_prerequisite,$(<))
flags.common := -static -ffreestanding -nostdlib -fno-omit-frame-pointer -fshort-enums $(flags.lto)
flags.object = $(flags.system_include_directories) $(flags.include_directories) $(flags.prerequisites_generation) $(flags.definitions) $(flags.common)
flags.executable = $(flags.common) $(flags.whole_program) -fuse-ld=$(LD) -Wl,-elone_start

$(directories.build.objects)/%.o: $(directories.source)/%.c | directories
	$(strip $(CC) $(flags.object) $(CFLAGS) -o $@ -c $<)

$(targets.lone): $(targets.objects.lone) | directories
	$(strip $(CC) $(flags.executable) $(CFLAGS) -o $@ $^)

$(call source_to_object,source/lone/modules/linux.c): $(targets.NR.c)

$(targets.NR.c): $(targets.NR.list) scripts/NR.generate
	scripts/NR.generate < $< > $@

$(targets.NR.list): scripts/NR.filter
	$(CC) -E -dM -include linux/unistd.h - < /dev/null | scripts/NR.filter > $@

targets.phony += lone
lone: $(targets.lone)

targets.phony += clean
clean:
	rm -rf $(directories.build)

targets.phony += test
test: $(targets.lone)
	scripts/test.bash $<

targets.phony += directories
directories:
	mkdir -p $(sort $(directories.create))

.PHONY: $(targets.phony)
.DEFAULT_GOAL := lone

sinclude $(targets.prerequisites)
