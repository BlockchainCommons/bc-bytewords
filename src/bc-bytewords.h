//
//  bc-bytewords.h
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#ifndef BC_BYTEWORDS_H
#define BC_BYTEWORDS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum bw_style_e {
    bw_standard,
    bw_uri,
    bw_minimal
} bw_style;

char* bytewords_encode(bw_style style, const uint8_t* in_buf, size_t in_len);
bool bytewords_decode(bw_style style, const char* in_string, uint8_t** out_buf, size_t* out_len);

#ifdef __cplusplus
}
#endif

#endif // BC_BYTEWORDS_H
