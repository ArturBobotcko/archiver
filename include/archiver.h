#pragma once

// Define these before any includes to enable large file support
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE

#include <iostream>

#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <zlib.h>
#include <string>
#include <vector>
#include <indicators/progress_bar.hpp>
#include <indicators/block_progress_bar.hpp>
#include <thread>
#include <chrono>

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

using namespace std;
using namespace indicators;

#define CHUNK 2097152

int def(FILE* source, FILE* dest, int level);
int inf(FILE* source, FILE* dest);
void zerr(int ret);
