#include "../../include/pipe.h"
#include "../../include/liballoc.h"

int mkpipe(const char* path, int (*write)(uint32_t n, void* data), int (*read)(uint32_t n, void* buf)) {
    fs_node_t* node = (fs_node_t*)malloc(sizeof(fs_node_t));

    node->path = extractFilenameFromPath(path);
    node->type = FS_PIPE;
    node->size = 0;
    node->write = write;
    node->read = read;
    node->parent = NULL;
    node->children = NULL;
    node->_fs_sibling_next = NULL;
    node->_fs_sibling_prev = NULL;

    if(registerFSNode(path, node)) {
        return -1;
    }

    int descriptor = getNextFreeDescriptor();
    setDescriptor(descriptor, node);

    return descriptor;
}
