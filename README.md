# nvstat
A lightweight and extremely simple command-line utility for monitoring and recording NVIDIA GPU metrics in real-time. Its primary use is for collecting GPU metrics such as SM utilization and power consumption in the background while workloads run. Other metrics available through [NVML](https://developer.nvidia.com/management-library-nvml) are trivial to add; I will probably do so where necessary for my own purposes.

## Features
- Real-time monitoring of GPU metrics
- Interactive and non-interactive display modes
- Configurable refresh interval
- CSV output support for data logging
- Clean, formatted display of metrics

## Prerequisites
- NVIDIA GPU
- CUDA Toolkit
- NVML (NVIDIA Management Library)
- GCC compiler
- Make

## Building
To build the project:
1. Set the `CUDA_HOME` and `NVML_SPECIAL_LIB_PATH` environment variables
2. Build with make
    ```bash
    make
    ```

## Usage

```
./gpu_monitor [-i] [-t interval] [-o output_file] [-h]
```

- `-i`: Interactive mode with a refreshing display
- `-t <ms>`: Set update interval in milliseconds (default: 500ms)
- `-o <file>`: Save metrics to a CSV file
- `-h`: Display help message

### Display Modes

1. **Standard Mode** (default): Displays metrics in a formatted table where new rows are continuously added
2. **Interactive Mode** (`-i`): Full-screen display with live updates

### CSV Output

When using the `-o` option, data is logged in CSV format with:
- Timestamp
- SM Utilization (%)
- Power Usage (W)

## Limitations
Currently, nvstat only works with a single GPU. Multi-GPU support will be added soon.