# Archiver

A cross platform file compression and decompression utility using zlib with progress indicators and SIGINT (Ctrl+C) handling.

## Features

- File compression using zlib deflate algorithm
- File decompression with zlib inflate algorithm
- Visual progress indicators during operations
- Support for large files
  
## Dependencies

- [zlib](https://www.zlib.net/) - Compression library
- [indicators](https://github.com/p-ranav/indicators) - Progress bars and indicators
- C++17 compiler
- CMake 3.8+
- vcpkg package manager

## Building

### Prerequisites

1. Install [vcpkg](https://github.com/microsoft/vcpkg)
2. Set the `VCPKG_ROOT` environment variable to your vcpkg installation directory

### Build Steps (Windows)

```bash
# Clone the repository
git clone https://github.com/ArturBobotcko/archiver.git
cd archiver
```
```bash
# Create a build directory
mkdir build
```

```bash
# Build the project
cmake -B build -S .
cd build
```

```bash
# Debug version
cmake --build . --config Debug
```

```bash
# Release version
cmake --build . --config Release
```

### Build Steps (Linux)

```bash
# Clone the repository
git clone https://github.com/ArturBobotcko/archiver.git
cd archiver
```
```bash
# Create a build directory
mkdir build
```
```bash
# Build the project
sudo -E cmake -B build/ -S .
cd build
sudo make
```
## Usage

The archiver supports two main operations:

### Compression

To compress a file:

```bash
./archiver a <original_file> <archive_file>
```

Example:
```bash
./archiver a document.pdf document.pdf.gz
```

### Decompression

To extract a compressed file:

```bash
./archiver e <archive_file> <output_file>
```

Example:
```bash
./archiver e document.pdf.gz document_extracted.pdf
```