#ifndef base64_h
#define base64_h

#include <stdio.h>

#if __cplusplus
extern "C"
{
#endif

    int base64_encode(const char *indata, int inlen, char *outdata, int *outlen);
    int base64_decode(const char *indata, int inlen, char *outdata, int *outlen);
    char *base64_encode_file(const unsigned char *bindata, char *base64, int binlength);
#if __cplusplus
}
#endif

#endif

/* -----End of file----- */
