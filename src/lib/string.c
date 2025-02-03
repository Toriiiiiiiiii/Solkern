#include <string.h>

void memcpy(void *dest, void *src, unsigned long n) {
    for(int i = 0; i < n; ++i) {
        ((char*)dest)[i] = ((char*)src)[i];
    }
}

void memset(char *dest, char val, unsigned long n) {
    for(int i = 0; i < n; ++i) {
        dest[i] = val;
    }
}
