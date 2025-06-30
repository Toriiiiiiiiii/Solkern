#ifndef PIPE_H
#define PIPE_H

#include "fs.h"

int mkpipe(const char* path, void (*write)(uint32_t n, void* data), void (*read)(uint32_t n, void* buf));

#endif
