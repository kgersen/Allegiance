#pragma once

BOOL JPEG_process_headers(void *file, int *width, int *height);
BOOL JPEG_decompress(void *pbits);
void JPEG_cleanup(void);

enum {
	ERR_INVALID_JPEG_FILE,
	ERR_EXTENDED_JPEG_FORMAT_NOT_SUPPORTED,
	ERR_COULDNT_ALLOCATE_MEMORY,
};


