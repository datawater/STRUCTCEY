FORTIFY_SOURCE ?=1
SANITIZE_ADDRESS ?=false

CFLAGS_WARNINGS = -Wall -Wextra -pedantic -Wstrict-aliasing -Wno-pointer-arith \
					-Wno-variadic-macros -Wno-unused-command-line-argument \
					-Wno-write-strings -Wno-unknown-warning-option \
					-Wno-int-conversion -Wno-incompatible-pointer-types
CFLAGS = $(CFLAGS_WARNINGS) -std=gnu99 -D_FORTIFY_SOURCE=$(FORTIFY_SOURCE)
LIBS =

HAS_AVX=$(shell lscpu | grep "avx")
ifneq ($(HAS_AVX),)
	CFLAGS += -mavx
endif

PROFILE_DEBUG_CFLAGS := -ggdb -O0 -DDEBUG
PROFILE_RELEASE_CFLAGS := -O3 -s -flto -mtune=native -march=native -fgraphite-identity
PROFILE_SIZE_CFLAGS := -Oz -s

USE_MOLD = $(shell which mold)
ifneq ($(USE_MOLD),)
	CFLAGS += -fuse-ld=mold
endif

PROFILE ?=debug

IS_ERROR = 0
ERROR_TEXT = ok

ifeq ($(PROFILE),debug)
	CFLAGS += $(PROFILE_DEBUG_CFLAGS)
else
	ifeq ($(PROFILE),release)
		CFLAGS += $(PROFILE_RELEASE_CFLAGS)
	else
		ifeq ($(PROFILE),size)
			CFLAGS += $(PROFILE_SIZE_CFLAGS)
		else
			IS_ERROR =1
			ERROR_TEXT = [ERROR] Unknown profile "$(PROFILE)".
		endif
	endif
endif

ifeq ($(SANITIZE_ADDRESS),true)
	CFLAGS += -fsanitize=address
endif