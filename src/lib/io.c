#include "../../include/io.h"
#include "../../include/fs.h"
#include "../../include/string.h"

#include <stdarg.h>

void fprintf(uint32_t desc, const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	while(*fmt != 0) {
		char c = *fmt;

		if(c == '%') {
			fmt++;
			char dataType = *fmt;

			if(dataType == 's') {
				char* str = va_arg(args, char*);
				uint64_t len = strlen(str);

				fwrite(desc, len, (void*)str);
			} else if(dataType == 'c') {
				char ch = va_arg(args, int);
				fwrite(desc, 1, &ch);
			} else if(dataType == 'd') {
				char buf[512] = {0};
				int n = va_arg(args, int);
				utos(n, 10, buf);

				fwrite(desc, strlen(buf), buf);
			} else if(dataType == 'x') {
				char buf[512] = {0};
				int n = va_arg(args, int);
				utos(n, 16, buf);

				fwrite(desc, strlen(buf), buf);
			}

		} else {
			fwrite(desc, 1, (void*)fmt);
		}
		fmt++;
	}
}
