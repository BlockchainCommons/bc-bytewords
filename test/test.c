//
//  test.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>

#include "../src/bc-bytewords.h"
#include "../src/crc32.h"
#include "test-utils.h"

static void testcrc32() {
    char* input = "Hello, world!";
    uint32_t checksum = crc32((const uint8_t*)input, strlen(input));
    assert(checksum == 0xebe6c6e6);
}

static bool test_encode(bw_style style, const uint8_t input[], size_t input_len, const char* expected) {
    bool success = false;

    char* encoded = bytewords_encode(style, input, input_len);
    // printf("%s\n", encoded);
    if(!equal_strings(encoded, expected)) {
        goto finish;
    }

    success = true;

finish:
    free(encoded);
    return success;
}

static bool test_decode(bw_style style, const char* encoded, uint8_t input[], size_t input_len) {
    bool success = false;

    uint8_t* decoded = NULL;
    size_t decoded_len;
    if (!bytewords_decode(style, encoded, &decoded, &decoded_len)) {
        goto finish;
    }
    if (!equal_uint8_buffers(input, input_len, decoded, decoded_len)) {
        goto finish;
    }

    success = true;

finish:
    free(decoded);
    return success;
}

void test1() {
    uint8_t input[] = {0, 1, 2, 128, 255};
    size_t input_len = 5;
    assert(test_encode(bw_standard, input, input_len, "able acid also lava zero jade need echo taxi"));
    assert(test_encode(bw_uri, input, input_len, "able-acid-also-lava-zero-jade-need-echo-taxi"));
    assert(test_encode(bw_minimal, input, input_len, "aeadaolazojendeoti"));

    assert(test_decode(bw_standard, "able acid also lava zero jade need echo taxi", input, input_len));
    assert(test_decode(bw_uri, "able-acid-also-lava-zero-jade-need-echo-taxi", input, input_len));
    assert(test_decode(bw_minimal, "aeadaolazojendeoti", input, input_len));

    // bad checksum
    assert(!test_decode(bw_standard, "able acid also lava zero jade need echo wolf", input, input_len));
    assert(!test_decode(bw_uri, "able-acid-also-lava-zero-jade-need-echo-wolf", input, input_len));
    assert(!test_decode(bw_minimal, "aeadaolazojendeowf", input, input_len));

    // too short
    assert(!test_decode(bw_standard, "wolf", input, input_len));
    assert(!test_decode(bw_standard, "", input, input_len));
}

void test2() {
    uint8_t input[] = {
        245, 215,  20, 198, 241, 235,  69,  59, 209, 205,
        165,  18, 150, 158, 116, 135, 229, 212,  19, 159,
         17,  37, 239, 240, 253,  11, 109, 191,  37, 242,
         38, 120, 223,  41, 156, 189, 242, 254, 147, 204,
         66, 163, 216, 175, 191,  72, 169,  54,  32,  60,
        144, 230, 210, 137, 184, 197,  33, 113,  88,  14,
        157,  31, 177,  46,   1, 115, 205,  69, 225, 150,
         65, 235,  58, 144,  65, 240, 133,  69, 113, 247,
         63,  53, 242, 165, 160, 144,  26,  13,  79, 237,
        133,  71,  82,  69, 254, 165, 138,  41,  85,  24
    };
    size_t input_len = 100;

    char* encoded =
    "yank toys bulb skew when warm free fair tent swan "
    "open brag mint noon jury lion view tiny brew note "
    "body data webs what zone bald join runs data whiz "
    "days keys user diet news ruby whiz zoom menu surf "
    "flew omit trip pose runs fund part even crux fern "
    "math visa tied loud redo silk curl jugs hard beta "
    "next cost puma drum acid junk swan free very mint "
    "flap warm fact math flap what list free jugs yell "
    "fish epic whiz open numb math city belt glow wave "
    "list fuel grim free zoom open love diet gyro cats "
    "fizz holy city puff";
    assert(test_encode(bw_standard, input, input_len, encoded));
    assert(test_decode(bw_standard, encoded, input, input_len));

    char* encoded_minimal =
    "yktsbbswwnwmfefrttsnonbgmtnnjylnvwtybwne"
    "bydawswtzebdjnrsdawzdsksurdtnsrywzzmmusf"
    "fwottppersfdptencxfnmhvatdldroskcljshdba"
    "ntctpadmadjksnfevymtfpwmftmhfpwtltfejsyl"
    "fhecwzonnbmhcybtgwweltflgmfezmonledtgocs"
    "fzhycypf";
    assert(test_encode(bw_minimal, input, input_len, encoded_minimal));
    assert(test_decode(bw_minimal, encoded_minimal, input, input_len));
}

int main() {
    testcrc32();
    test1();
    test2();

    return 0;
}
