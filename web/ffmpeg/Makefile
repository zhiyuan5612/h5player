SRCS := $(shell find ../ffmpeg-src/libavcodec -name "*.c" -not -name "*_template.c")
SRCS += $(shell find ../ffmpeg-src/libavutil -name "*.c" -not -name "*_template.c")
SRCS += h264bsd_decoder.c

CFLAGS := -I../ffmpeg-src/ -O2 -w -std=c99 -Wno-implicit-function-declaration
LDFLAGS := -lm

OBJS := $(SRCS:%.c=%.o)

ARGS := -O3 -D_ERROR_PRINT -s ALLOW_MEMORY_GROWTH=1 --memory-init-file 0 --llvm-opts 3 --llvm-lto 3 \
		-s EXPORTED_FUNCTIONS="['HEAP8', 'HEAP16', 'HEAP32','_malloc','_free','_memcpy','_h264bsdAlloc','_h264bsdFree','_h264bsdInit','_h264bsdDecode','_h264bsdShutdown','_h264bsdNextOutputPicture','_h264bsdPicWidth','_h264bsdPicHeight','_h264bsdCroppingParams']" \
		-s NO_EXIT_RUNTIME=1 -s NO_FILESYSTEM=1 -s NO_BROWSER=1 -s INVOKE_RUN=0 -s DOUBLE_MODE=0 -s PRECISE_I64_MATH=0 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 -s ALIASING_FUNCTION_POINTERS=1 -s DISABLE_EXCEPTION_CATCHING=1 

Decoder.js: $(OBJS)
	emcc $(ARGS) -s WASM=1 -o $@ $^
	
.PHONY: clean
clean:
	rm -f Decoder.js $(OBJS)
