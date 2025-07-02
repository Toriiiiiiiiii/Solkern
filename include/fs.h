#ifndef FS_H
#define FS_H

#include <stdint.h>

#define NDESCRIPTORS 256
#define PATH_DELIM '/'

typedef enum {
    FS_PIPE,
    FS_DIR,
} fs_type_t;

struct _fs_node_t;

typedef struct _fs_node_t {
    const char *path;
    fs_type_t  type;
    uint32_t   size;

    int (*write)(uint32_t n, void* data);
    int (*read)(uint32_t n, void* buf);

    struct _fs_node_t *parent;
    struct _fs_node_t *children;

    struct _fs_node_t *_fs_sibling_next;
    struct _fs_node_t *_fs_sibling_prev;
} fs_node_t;

extern fs_node_t* descriptors[NDESCRIPTORS];

int getNextFreeDescriptor();
void setDescriptor(uint32_t desc, fs_node_t* node);
fs_node_t *getNodeByDescriptor(uint32_t desc);

int fwrite(uint32_t desc, uint32_t n, void* data);
int fread(uint32_t desc, uint32_t n, void* buf);

int fputs(uint32_t desc, char* str);

fs_node_t *getNodeByPath(char *path);
int registerFSNode(char *path, fs_node_t* node);

char *extractFilenameFromPath(char *path);

void initFSTree();

void printFSTree();

#endif // FS_H
