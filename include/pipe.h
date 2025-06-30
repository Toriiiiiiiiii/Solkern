#ifndef PIPE_H
#define PIPE_H

#include "fs.h"

int mkpipe(const char* path, int (*write)(uint32_t n, void* data), int (*read)(uint32_t n, void* buf));

#endif
