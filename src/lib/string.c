#include "../../include/string.h"
#include "../../include/liballoc.h"
#include "../../include/fs.h"

int strlen(char *s) {
    int len = 0;
    while(s[len] != '\0') {
        len++;
    }

    return len;
}

int streq(char *a, char *b) {
    if(strlen(a) != strlen(b)) return 0;

    for(int i = 0; i < strlen(a); ++i) {
        if(a[i] != b[i]) return 0;
    }

    return 1;
}

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

char *utos(uint32_t val, uint8_t base, char *buf) {
    uint32_t n = 0;
    uint32_t temp = val;

    if(val == 0) {
        buf[0] = '0';
        buf[1] = '\0';
		return buf;
    }
    else {
        while(temp > 0) {
            n++;
            temp /= base;
        }
    }

    buf[n] = '\0';

    char charmap[32] = "0123456789abcdefghijklmnopqrstu";

    unsigned int strptr = n-1;
    while(val > 0) {
        char digit = charmap[val % base];
        val /= base;

        buf[strptr] = digit;
        strptr--;
    }

	return buf;
}
