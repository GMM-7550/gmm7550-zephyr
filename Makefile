
blinky := zephyr/samples/basic/blinky
board  := gmm7550
west   := uv run west

build_dir ?= build
top_dir   ?= $(shell $(west) topdir)

app ?= $(top_dir)/$(blinky)

all: build

.PHONY: all build clean

build:
	${west} build --board $(board) \
	--build-dir $(build_dir) \
	$(app) \
	-- -DBOARD_ROOT=`pwd`

clean:
	rm -rf $(build_dir)
