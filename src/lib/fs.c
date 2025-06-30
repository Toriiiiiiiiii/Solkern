#include "../../include/fs.h"

fs_node_t* descriptors[NDESCRIPTORS] = { (fs_node_t*)0 };

int getNextFreeDescriptor() {
    int i = 0;
    while(descriptors[i]) {
        i++;
    }

    return i;
}

void setDescriptor(uint32_t desc, fs_node_t *node) {
    descriptors[desc] = node;
}

fs_node_t *getNodeByDescriptor(uint32_t desc) {
    return descriptors[desc];
}

int fwrite(uint32_t desc, uint32_t n, void* data) {
    if(!descriptors[desc]) return 1;

    fs_node_t *f = getNodeByDescriptor(desc);
    return f->write(n, data);
}

int fread(uint32_t desc, uint32_t n, void* buf) {
    if(!descriptors[desc]) return 1;

    fs_node_t *f = getNodeByDescriptor(desc);
    return f->read(n, buf);
}
