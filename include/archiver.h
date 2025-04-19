#pragma once

// Define these before any includes to enable large file support
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE

#include <iostream>

#include <fstream>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <zlib.h>
#include <string>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>
#include <atomic>
#include <csignal>
#include <indicators/cursor_control.hpp>
#include <indicators/progress_bar.hpp>
#include <indicators/block_progress_bar.hpp>
#include <indicators/indeterminate_progress_bar.hpp>

/*
	This is an ugly hack required to avoid corruption of the input and output data 
	on Windows/MS-DOS systems. Without this, those systems would assume that 
	the input and output files are text, and try to convert the end-of-line characters 
	from one standard to another. That would corrupt binary data, and in particular 
	would render the compressed data unusable. This sets the input and output to binary 
	which suppresses the end-of-line conversions. SET_BINARY_MODE() will be used later 
	on stdin and stdout, at the beginning of main().
*/

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

using namespace std;
using namespace indicators;

// 32 MB
#define CHUNK 33554432

int def(FILE* source, FILE* dest, int level);
int inf(FILE* source, FILE* dest);
void zerr(int ret);
