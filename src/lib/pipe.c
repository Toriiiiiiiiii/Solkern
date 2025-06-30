#include "../../include/pipe.h"
#include "../../include/liballoc.h"

int mkpipe(const char* path, int (*write)(uint32_t n, void* data), int (*read)(uint32_t n, void* buf)) {
    fs_node_t* node = (fs_node_t*)malloc(sizeof(fs_node_t));

    node->path = path;
    node->type = FS_PIPE;
    node->size = 0;
    node->write = write;
    node->read = read;

    // TODO: make it an actual tree
    node->parent = NULL;
    node->children = NULL;

    int descriptor = getNextFreeDescriptor();
    setDescriptor(descriptor, node);

    return descriptor;
}
