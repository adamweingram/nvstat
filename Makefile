
CUDA_HOME ?= /usr/local/cuda
NVML_SPECIAL_LIB_PATH ?= $(CUDA_HOME)/targets/x86_64-linux/lib/stubs

CC = gcc
CFLAGS = -Wall -Wextra -O2
INCLUDE_FLAGS = -I$(CUDA_HOME)/include
LDFLAGS = -L$(CUDA_HOME)/lib64 -lnvidia-ml -L$(NVML_SPECIAL_LIB_PATH)

TARGET = gpu_monitor
SRCS = gpu_monitor.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS) $(INCLUDE_FLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
