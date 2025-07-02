#include "../../include/fs.h"
#include "../../include/liballoc.h"
#include "../../include/string.h"
#include "../../include/serial.h"

fs_node_t* descriptors[NDESCRIPTORS] = { (fs_node_t*)0 };

static fs_node_t fs_root = {
    "",
    FS_DIR, 0,
    NULL, NULL,
    NULL, NULL
};

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
    if(!descriptors[desc]) return 0;
    if(!descriptors[desc]->write) return 0;

    fs_node_t *f = getNodeByDescriptor(desc);
    return f->write(n, data);
}

int fread(uint32_t desc, uint32_t n, void* buf) {
    if(!descriptors[desc]) return 0;
    if(!descriptors[desc]->read) return 0;

    fs_node_t *f = getNodeByDescriptor(desc);
    return f->read(n, buf);
}

int fputs(uint32_t desc, char* str) {
    return fwrite(desc, strlen(str), str);
}

char *getPathSegment(char *path, char **ptr) {
    int l = 0;
    while(l < strlen(path) && path[l] != PATH_DELIM) {
        l++;
    }

    char *res = (char*)malloc(l+1);
    memcpy(res, path, l);
    res[l] = '\0';

    if(l < strlen(path))
        *ptr = path + l + 1;
    else 
        *ptr = NULL;

    return res;
}

fs_node_t *getNodeByPath(char *path) {
    if(!path) return NULL;
    if(path[0] == '\0') return &fs_root;

    fs_node_t* node = &fs_root;


    while(path && node->children) {
        // Traverse node children until a match for the segment is found
        char *newptr;
        char *segment = getPathSegment(path, &newptr);
        if(!segment || strlen(segment) == 0) break;
        path = newptr;

        fs_node_t *child = node->children;
        while(child) {
            if(streq(child->path, segment)) {
                node = child;
                free(segment);
                break;
            }

            child = child->_fs_sibling_next;
        }
    }

    return node;
}

int registerFSNode(char *path, fs_node_t* node) {
    if(!path || path[0] == '\0') return 1;

    int lastDelim = 0;
    int nDelims = 0;
    for(int i = 0; i < strlen(path); ++i) {
        if(path[i] == PATH_DELIM) {
            lastDelim = i;
            nDelims++;
        }
    }

    fs_node_t* parent = &fs_root;
    if(nDelims) {
        path[lastDelim] = '\0';
        parent = getNodeByPath(path);
    }

    if(!parent) {
        serial_puts("ERROR : Could not register FS Node -- Parent was NULL.\r\n");
        return 1;
    }

    node->parent = parent;

    if(!parent->children) {
        parent->children = node;
        return 0;
    }

    fs_node_t* child = parent->children;
    while(child->_fs_sibling_next) {
        child = child->_fs_sibling_next;
    }

    child->_fs_sibling_next = node;
    node->_fs_sibling_prev = child;
    return 0;
}

char* extractFilenameFromPath(char *path) {
    int i = strlen(path)-1;

    while(i > 0 && path[i] != PATH_DELIM) {
        i--;
    }

    return path + i + 1;
}

void initFSTree() {
    fs_node_t *devFS = (fs_node_t *)malloc(sizeof(fs_node_t));

    devFS->path = "dev";
    devFS->type = FS_DIR;
    devFS->size = 0;
    devFS->write = NULL;
    devFS->read = NULL;
    devFS->parent = NULL;
    devFS->children = NULL;

    // TODO: Proper error handling here
    registerFSNode("dev", devFS);
}

void printFSNode(int indent, fs_node_t* node) {
    for(int i = 0; i < indent; ++i) {
        fwrite(1, 1, " ");
    }

    fwrite(1, strlen(node->path), node->path);
    if(node->type == FS_DIR) fwrite(1, 1, "/");
    fwrite(1, 1, "\n");
    if(node->children) {
        fs_node_t* child = node->children;

        while(child) {
            printFSNode(indent + 2, child);
            child = child->_fs_sibling_next;
        }
    }
}

void printFSTree() {
    printFSNode(0, &fs_root);
}
