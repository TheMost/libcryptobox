/*
 * Copyright (c) 2013 Kyle Isom <kyle@tyrfingr.is>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
 * OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 * ---------------------------------------------------------------------
 */


#include <sys/types.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>


#include <cryptobox/strongbox.h>


static unsigned char global_test_key[] = {
        0x21, 0x29, 0x32, 0x40, 0xf7, 0x28, 0x53, 0xfd,
        0x04, 0x66, 0xfd, 0x66, 0x37, 0x32, 0xad, 0xe4,
        0x0a, 0xff, 0xf0, 0x16, 0x45, 0xaa, 0x62, 0xa4,
        0xad, 0x56, 0xb9, 0x1f, 0x7f, 0x41, 0xbf, 0x3e,
        0xa0, 0x73, 0x87, 0x6c, 0x37, 0x8e, 0x8e, 0x15,
        0x09, 0xda, 0x7b, 0x2f, 0x01, 0xfc, 0x54, 0x47,
        0x80, 0xd7, 0x8a, 0x93, 0x79, 0x00, 0xce, 0x40,
        0x13, 0xca, 0xcf, 0x75, 0xf2, 0x62, 0x98, 0x89,
};

static unsigned char global_bad_key[] = {
        0xd8, 0x59, 0x27, 0x37, 0x28, 0x01, 0x82, 0x2b,
        0x17, 0x7d, 0x4d, 0x62, 0x2d, 0x33, 0x7b, 0xad,
        0x1e, 0x42, 0x78, 0xa0, 0x99, 0x7e, 0xd7, 0x7c,
        0x48, 0x8f, 0x64, 0x41, 0xfc, 0xeb, 0xab, 0xa8,
        0xe6, 0x1b, 0xd3, 0xe0, 0x6f, 0x0c, 0x0a, 0xd2,
        0xba, 0x8c, 0x50, 0x82, 0xec, 0x17, 0xac, 0xce,
        0x31, 0x42, 0x32, 0x69, 0x88, 0x1b, 0x8e, 0x9a,
        0x8e, 0xe1, 0xc8, 0x65, 0x0a, 0x1c, 0x14, 0x1b,
};

static void
print_buf(unsigned char *buf, size_t len)
{
	int		 i;

	for (i = 0; i  < len; i++) {
		if (i % 8 == 0)
			printf("\n\t");
		printf("%02hx ", buf[i]);
	}
	printf("\n");
}


static void
test_identity(void)
{
	unsigned char test_key[] = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	};
	unsigned char test_msg[] = {0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x0};

	struct strongbox_box	*box = NULL;
	unsigned char		*test_decrypted;

	box = strongbox_seal(test_msg, sizeof(test_msg), test_key);
	CU_ASSERT(box != NULL);
	test_decrypted = strongbox_open(box, test_key);
	CU_ASSERT(NULL != test_decrypted && 0 == memcmp(test_decrypted,
		  test_msg, strlen(test_msg)));
	free(test_decrypted);
        strongbox_close(box);
}


static void
test_decrypt(void)
{
        unsigned char test_box[] = {
                0xf6, 0xd8, 0x4b, 0xad, 0x35, 0x4c, 0x47, 0x6a,
                0xf2, 0xc5, 0x65, 0x2f, 0x09, 0x96, 0xa6, 0x03,
                0xa7, 0x8e, 0x8e, 0xa8, 0x31, 0x5b, 0xf2, 0x4e,
                0x17, 0x15, 0x3e, 0xe1, 0x03, 0x95, 0xd3, 0x7d,
                0x32, 0xf9, 0x60, 0x24, 0xed, 0xfd, 0x83, 0x18,
                0xdb, 0x8f, 0xb3, 0xb2, 0x9d, 0x35, 0x8d, 0x6b,
                0xc9, 0x7d, 0x1b, 0x02,
        };
        int test_box_len = sizeof test_box;

        unsigned char test_key[] = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	};
        unsigned char expected[] = {0x01, 0x02, 0x03, 0x04};
        unsigned char           *msg = NULL;
        struct strongbox_box    *box = NULL;

        box = malloc(sizeof(struct strongbox_box));
        CU_ASSERT(NULL != box);

        box->contents = malloc(test_box_len+1);
        box->len = test_box_len;
        memcpy(box->contents, test_box, box->len);

        msg = strongbox_open(box, test_key);
        CU_ASSERT(msg != NULL);
        if (NULL != msg) {
                CU_ASSERT(0 == memcmp(expected, msg, 4));
                print_buf(msg, 5);
                free(msg);
        }
        strongbox_close(box);
}


static void
test_unbox_vector(const unsigned char *box_contents, int box_len,
                  const unsigned char *expected, int expected_len)
{
        struct strongbox_box    *box = NULL;
        unsigned char           *message = NULL;

        box = malloc(sizeof(struct strongbox_box));
        CU_ASSERT(NULL != box);
        box->contents = malloc(box_len);
        box->len = box_len;
        memcpy(box->contents, box_contents, box->len);

        message = strongbox_open(box, global_test_key);
        CU_ASSERT(NULL != message);
        if (NULL != message)
                CU_ASSERT(0 == memcmp(message, expected, expected_len));
        strongbox_close(box);
        free(message);
}


static void
test_box_cycle(unsigned char *message, int message_len)
{
        struct strongbox_box    *box = NULL;
        unsigned char           *msg = NULL;

        box = strongbox_seal(message, message_len, global_test_key);
        CU_ASSERT(NULL != box);
        if (NULL != box) {
                msg = strongbox_open(box, global_test_key);
                CU_ASSERT(NULL != msg);
                if (NULL != msg)
                        CU_ASSERT(0 == memcmp(msg, message, message_len));
                free(msg);
        }
        strongbox_close(box);
}


static void
test_box_bad_cycle(unsigned char *message, int message_len)
{
        struct strongbox_box    *box = NULL;
        unsigned char           *msg = NULL;

        box = strongbox_seal(message, message_len, global_test_key);
        CU_ASSERT(NULL != box);
        if (NULL != box) {
                msg = strongbox_open(box, global_bad_key);
                CU_ASSERT(NULL == msg);
                free(msg);
        }
        strongbox_close(box);
}


static void
test_vector1(void)
{
        unsigned char expected[] = "Hello, world.";
        unsigned char box_contents[] = {
                0x50, 0x73, 0x09, 0xbc, 0xc5, 0xe7, 0xa1, 0x08,
                0x78, 0x58, 0x59, 0xec, 0x4c, 0x3e, 0x27, 0xa8,
                0xf2, 0x21, 0xa5, 0x78, 0x26, 0x15, 0x33, 0x93,
                0x42, 0x7a, 0x73, 0x77, 0x7b, 0xf2, 0x5e, 0xfa,
                0xa1, 0x6c, 0xc4, 0xbc, 0xa7, 0x40, 0xa7, 0xdf,
                0xed, 0x92, 0x5a, 0x88, 0x94, 0xb4, 0x07, 0xc1,
                0x48, 0xcb, 0x1a, 0xa0, 0x4a, 0x2a, 0xeb, 0x86,
                0x7d, 0x95, 0xba, 0xd9, 0xd6,
        };
        int     expected_len = strlen(expected);
        int     box_len = sizeof box_contents;

        test_unbox_vector(box_contents, box_len, expected, expected_len);
        test_box_cycle(expected, expected_len);
        test_box_bad_cycle(expected, expected_len);
}

static void
test_vector2(void)
{
        unsigned char expected[] = "Yes... yes. This is a fertile land, and we "
                                   "will thrive. We will rule over all this "
                                   "land, and we will call it... This Land.";
        unsigned char box_contents[] = {
                0x2d, 0x4d, 0x5f, 0x35, 0xbc, 0xe2, 0x79, 0x85,
                0xc3, 0xa6, 0x32, 0x8f, 0xc7, 0xf4, 0x3d, 0x17,
                0x64, 0xfc, 0xaf, 0x6f, 0x2b, 0x4f, 0xb3, 0x67,
                0x7b, 0xf3, 0x01, 0xd3, 0x6b, 0x45, 0x95, 0x78,
                0x12, 0x68, 0xd5, 0x0d, 0x0b, 0x68, 0x70, 0x10,
                0xec, 0x82, 0xa5, 0xd8, 0xad, 0xc1, 0x10, 0xb0,
                0xe9, 0x97, 0x86, 0x90, 0x2c, 0xc1, 0xbb, 0x04,
                0x9b, 0x42, 0x7a, 0xe4, 0x54, 0x2e, 0xe1, 0xc9,
                0x49, 0x64, 0xf4, 0xb9, 0x8b, 0x5c, 0xd5, 0x50,
                0xf1, 0xa4, 0x7a, 0x79, 0x2c, 0x92, 0x18, 0xbe,
                0xff, 0xb8, 0x88, 0x24, 0x61, 0xb5, 0x8c, 0x23,
                0xdf, 0x9d, 0x46, 0x2b, 0xf1, 0x10, 0xe0, 0x2e,
                0xd9, 0x3d, 0x9f, 0xa1, 0x9d, 0x4e, 0x6f, 0x9f,
                0x74, 0x7b, 0x8a, 0xc9, 0xa8, 0xa6, 0xf4, 0x7a,
                0x22, 0x71, 0x03, 0xe8, 0x94, 0xc3, 0xaf, 0x55,
                0x25, 0x71, 0xf3, 0xcd, 0xcc, 0x80, 0x07, 0xd8,
                0xac, 0xc1, 0x0e, 0x4d, 0xf4, 0xd2, 0x0b, 0x77,
                0x05, 0x6b, 0x52, 0xfb, 0xa1, 0x60, 0x37, 0xe0,
                0x59, 0x49, 0xe2, 0x5f, 0x6d, 0x4d, 0xc7, 0xe5,
                0xf2, 0x2a, 0xbd, 0x96, 0x8d, 0xda, 0x54, 0x25,
                0x9d, 0x71, 0xb2, 0x49, 0x40, 0x2d, 0x13, 0x03,
                0xd9, 0xbd,
        };
        int     expected_len = strlen(expected);
        int     box_len = sizeof box_contents;

        test_unbox_vector(box_contents, box_len, expected, expected_len);
        test_box_cycle(expected, expected_len);
        test_box_bad_cycle(expected, expected_len);
}


static void
test_vector3(void)
{
        unsigned char expected[] = "Ah! Curse your sudden but inevitable "
                                   "betrayal!";

        unsigned char box_contents[] = {
                0xfb, 0x6f, 0x07, 0x33, 0x2c, 0xe2, 0xfe, 0xcd,
                0x79, 0xc2, 0x26, 0xc8, 0xa1, 0x89, 0xcc, 0xff,
                0xf6, 0xa3, 0xf6, 0x45, 0xad, 0x1f, 0x1b, 0x7d,
                0x08, 0xac, 0x90, 0x60, 0xeb, 0x9a, 0xa6, 0x5b,
                0x8b, 0x70, 0xee, 0x72, 0xc4, 0x33, 0xac, 0x3f,
                0x55, 0xcd, 0x39, 0x84, 0x34, 0x8c, 0xe3, 0x48,
                0x75, 0x65, 0xd0, 0xb1, 0x4e, 0xe2, 0x44, 0x61,
                0x38, 0x61, 0x31, 0xb8, 0x69, 0x76, 0xef, 0x69,
                0x9e, 0xe8, 0xa0, 0xe8, 0x98, 0xec, 0x79, 0xbc,
                0xbb, 0xb4, 0x4f, 0x36, 0xc7, 0x06, 0xd4, 0x9d,
                0xb7, 0xe0, 0x4e, 0x46, 0x7f, 0x90, 0xfc, 0xac,
                0x77, 0x5b, 0x2f, 0x50, 0x25, 0xeb,
        };
        int     expected_len = strlen(expected);
        int     box_len = sizeof box_contents;

        test_unbox_vector(box_contents, box_len, expected, expected_len);
        test_box_cycle(expected, expected_len);
        test_box_bad_cycle(expected, expected_len);
}


static void
test_vector4(void)
{
        unsigned char expected[] = "And I'm thinkin' you weren't burdened "
                                   "with an overabundance of schooling. So "
                                   "why don't we just ignore each other until "
                                   "we go away?";
        unsigned char box_contents[] = {
                0x8f, 0x9f, 0xd5, 0xc7, 0x3d, 0x99, 0xb7, 0x6b,
                0xd7, 0xf1, 0x1d, 0x7d, 0x19, 0xa8, 0x68, 0x6b,
                0x21, 0x23, 0x47, 0x5b, 0xf0, 0x0e, 0x43, 0x9c,
                0x6b, 0xf6, 0xc5, 0x2d, 0xa1, 0x38, 0x2e, 0xd3,
                0xfe, 0xf0, 0x3e, 0x56, 0x5f, 0x45, 0xc7, 0xc4,
                0x2c, 0xa7, 0x84, 0x1c, 0xea, 0x7e, 0xab, 0xe5,
                0xee, 0xc5, 0x85, 0xfc, 0xce, 0x54, 0x0d, 0x80,
                0xc3, 0x9a, 0x6f, 0x7a, 0xd1, 0x6b, 0xef, 0xb0,
                0xc1, 0x0a, 0x3b, 0xd1, 0xa8, 0xdb, 0x46, 0xcc,
                0x41, 0xf9, 0x1a, 0xaf, 0x56, 0x7a, 0x04, 0xd4,
                0x9d, 0x39, 0x0d, 0xb5, 0xb8, 0x67, 0xcc, 0x68,
                0xec, 0x5f, 0x0b, 0x62, 0x91, 0x49, 0xd7, 0x97,
                0x35, 0x1a, 0x7a, 0xf8, 0x7b, 0xe3, 0x25, 0x6a,
                0xc8, 0x3b, 0x7d, 0x0f, 0x35, 0xb7, 0xdd, 0x93,
                0x2f, 0xb0, 0x7e, 0x51, 0x73, 0x21, 0xb6, 0x7f,
                0x00, 0xf9, 0x3e, 0xfe, 0xaa, 0x2d, 0x21, 0xba,
                0xd7, 0x58, 0xbe, 0xa9, 0x1a, 0x8f, 0x2b, 0x26,
                0x1d, 0x2f, 0x75, 0x02, 0xc5, 0x4f, 0xda, 0xad,
                0x3e, 0xae, 0xb0, 0x4b, 0x82, 0xef, 0x39, 0x2c,
                0x01, 0xb1, 0x25, 0x9f, 0x58, 0xc6, 0x9e, 0xf8,
                0x36, 0x9f, 0x25, 0x75, 0x4a, 0x87, 0x9b, 0xda,
                0xa8, 0x84, 0x2e, 0x84, 0x50, 0xbf, 0x42, 0xce,
                0x92, 0xc7,
        };
        int     expected_len = strlen(expected);
        int     box_len = sizeof box_contents;

        test_unbox_vector(box_contents, box_len, expected, expected_len);
        test_box_cycle(expected, expected_len);
        test_box_bad_cycle(expected, expected_len);
}


static void
test_vector5(void)
{
        unsigned char expected[] = "Sir, I think you have a problem with "
                                   "your brain being missing.";
        unsigned char box_contents[] = {
                0x0c, 0x81, 0xe4, 0x47, 0x32, 0x9b, 0x85, 0x32,
                0xdc, 0xd0, 0x68, 0x0b, 0x78, 0xea, 0x4a, 0xf8,
                0x02, 0x04, 0x41, 0xd3, 0xee, 0x2a, 0x69, 0xe0,
                0x12, 0x62, 0x6b, 0xff, 0x47, 0x11, 0x10, 0x07,
                0x37, 0x62, 0x1d, 0xe7, 0x0c, 0x66, 0xe6, 0x3a,
                0x0d, 0xbd, 0x50, 0x41, 0x31, 0x37, 0x4f, 0xa3,
                0xca, 0x5e, 0x40, 0x2b, 0xe6, 0xbc, 0x3d, 0x8a,
                0x9f, 0x56, 0xbc, 0xf8, 0xc6, 0xba, 0xab, 0x34,
                0xc2, 0xcb, 0xb2, 0x99, 0xc3, 0xa1, 0x90, 0xe0,
                0xf9, 0x0a, 0xd9, 0xa9, 0xc6, 0x4d, 0xbb, 0x67,
                0x3e, 0x3a, 0x14, 0xb2, 0xa5, 0xd2, 0x7c, 0x87,
                0x23, 0xab, 0x28, 0x36, 0x31, 0x48, 0x77, 0x33,
                0xc7, 0x08, 0xcc, 0x35, 0xf6, 0x38, 0x1d, 0xbe,
                0x26, 0xcc, 0x52, 0x97, 0x96, 0x95,
        };
        int     expected_len = strlen(expected);
        int     box_len = sizeof box_contents;

        test_unbox_vector(box_contents, box_len, expected, expected_len);
        test_box_cycle(expected, expected_len);
        test_box_bad_cycle(expected, expected_len);
}


static void
test_vector6(void)
{
        unsigned char expected[] = "It's the way of life in my findings that "
                                   "journeys end when and where they want to; "
                                   "and that's where you make your home.";
        unsigned char box_contents[] = {
                0x40, 0x2a, 0xb6, 0x83, 0xd5, 0xec, 0xa6, 0xcd,
                0x2b, 0xa0, 0x2a, 0xfd, 0x8b, 0x92, 0x6c, 0x39,
                0xa9, 0x11, 0x5b, 0xaa, 0x0e, 0xda, 0x7d, 0x98,
                0xc7, 0x91, 0x78, 0x48, 0x99, 0x0a, 0x04, 0xe4,
                0xdb, 0x7d, 0x8f, 0x56, 0xf9, 0x46, 0xf4, 0x26,
                0x0b, 0xbb, 0x2c, 0x2e, 0xe3, 0x17, 0xed, 0xa4,
                0x8b, 0x0d, 0x6f, 0x70, 0x4c, 0xd2, 0x19, 0xae,
                0xa3, 0x13, 0xfa, 0x57, 0x0f, 0x28, 0xe1, 0xbe,
                0x6d, 0x70, 0x0f, 0x53, 0x12, 0x55, 0x99, 0x96,
                0x90, 0xd3, 0xbe, 0x6f, 0x56, 0x29, 0xa2, 0xc5,
                0x8f, 0xf9, 0x79, 0x38, 0x9e, 0x4c, 0x3e, 0x47,
                0xe9, 0xb0, 0x1c, 0x2f, 0x1d, 0x65, 0x8a, 0xe4,
                0xc4, 0x5d, 0x84, 0x2d, 0x74, 0xf2, 0xe6, 0xfc,
                0x20, 0xe5, 0x0d, 0x78, 0xd5, 0x1f, 0x52, 0x54,
                0x65, 0x0f, 0x0f, 0xb3, 0xda, 0xf9, 0x14, 0xc8,
                0xad, 0xaa, 0x1b, 0x91, 0x39, 0xaa, 0x94, 0x69,
                0xef, 0xf4, 0x70, 0x5d, 0xbc, 0x29, 0xc7, 0x54,
                0x20, 0x60, 0x3d, 0xee, 0x3b, 0x50, 0x11, 0x57,
                0xd2, 0xdb, 0x13, 0x14, 0x58, 0x08, 0x8b, 0x77,
                0x29, 0x00, 0x0e, 0x84, 0x1e, 0x44, 0x35, 0xa8,
                0x9d, 0x73, 0x49, 0x17, 0xfe, 0xc9, 0xe2,
        };
        int     expected_len = strlen(expected);
        int     box_len = sizeof box_contents;

        test_unbox_vector(box_contents, box_len, expected, expected_len);
        test_box_cycle(expected, expected_len);
        test_box_bad_cycle(expected, expected_len);
}


static void
test_vector7(void)
{
        unsigned char expected[] = "I get confused. I remember everything. I "
				   "remember too much. And... some of it's "
				   "made up, and... some of it can't be "
				   "quantified, and... there's secrets... "
				   "and...";
        unsigned char box_contents[] = {
                0xc5, 0xe8, 0x6c, 0xb2, 0xc5, 0x2a, 0x63, 0x0b,
                0xbf, 0x13, 0x01, 0xf2, 0x7d, 0xcf, 0xd2, 0xbb,
                0x68, 0x79, 0x28, 0xff, 0x74, 0x8b, 0x0b, 0xf2,
                0x70, 0x34, 0xc1, 0x9d, 0x31, 0x24, 0xc0, 0xbc,
                0x85, 0x77, 0x00, 0xf6, 0x22, 0x95, 0x84, 0xb6,
                0x6f, 0xd8, 0x98, 0x88, 0x7a, 0x6d, 0x5c, 0x6e,
                0x3f, 0xd4, 0xc7, 0x28, 0x6f, 0xd6, 0x8f, 0xdd,
                0x56, 0xd9, 0xd4, 0x6f, 0xd9, 0x68, 0xfa, 0xfa,
                0xcd, 0x24, 0x6c, 0x7f, 0xe3, 0x43, 0x04, 0x1b,
                0x8a, 0x53, 0x5e, 0x19, 0x7a, 0x1d, 0xe1, 0x74,
                0x9c, 0x47, 0x7c, 0xb1, 0x52, 0x7f, 0x40, 0xdd,
                0x2d, 0x9f, 0x12, 0x89, 0xdd, 0x2d, 0x31, 0xef,
                0xcf, 0xc1, 0x33, 0x10, 0x63, 0xb6, 0x4f, 0xc8,
                0x36, 0x2c, 0xa2, 0x46, 0xc5, 0xd3, 0x6c, 0x72,
                0x98, 0x9f, 0x50, 0xb3, 0xf8, 0x2c, 0x26, 0xf4,
                0x17, 0x00, 0x3f, 0xcd, 0x67, 0xc5, 0x91, 0xa0,
                0xf0, 0x08, 0x90, 0x2c, 0xbd, 0x7e, 0x68, 0x03,
                0xc8, 0x93, 0x06, 0x38, 0xf8, 0x86, 0xb2, 0xab,
                0xb3, 0xf6, 0x59, 0xf1, 0xf1, 0x1e, 0x80, 0x99,
                0x9f, 0x10, 0xff, 0xd1, 0xc6, 0xdf, 0x2c, 0x1f,
                0xbb, 0xc5, 0x55, 0x01, 0x00, 0x6c, 0x76, 0x30,
                0xe8, 0x82, 0x70, 0x2f, 0x2e, 0x2b, 0x7c, 0x87,
                0x7e, 0xe4, 0x4e, 0xee, 0xee, 0xbe, 0x94, 0x55,
                0x5e, 0x09, 0x45, 0x94, 0xb3, 0xc1, 0x39, 0x3a,
                0xb1, 0xf6, 0x47, 0x5d, 0x0f, 0xd3, 0x2c, 0x60,
                0xdc, 0x53, 0x9a, 0xac, 0x91, 0x47, 0x6f, 0xff,
        };
        int     expected_len = strlen(expected);
        int     box_len = sizeof box_contents;

        test_unbox_vector(box_contents, box_len, expected, expected_len);
        test_box_cycle(expected, expected_len);
        test_box_bad_cycle(expected, expected_len);
}


static void
test_vector8(void)
{
        unsigned char expected[] = "Yeah, we're pretty much just giving each "
                                   "other significant glances and laughing "
                                   "incessantly.";
        unsigned char box_contents[] = {
                0x05, 0x18, 0xbf, 0x7c, 0xec, 0x1c, 0x04, 0x66,
                0x6f, 0x4e, 0x18, 0x32, 0x23, 0x8e, 0xe6, 0x97,
                0x81, 0x34, 0x67, 0x20, 0x7e, 0x88, 0x84, 0x1e,
                0x39, 0x4e, 0x70, 0x31, 0x24, 0x21, 0x29, 0xfa,
                0x8f, 0xe7, 0x77, 0x75, 0xa4, 0x0a, 0x44, 0x45,
                0xe8, 0x80, 0xf1, 0x8c, 0xb5, 0x36, 0xce, 0x96,
                0xa1, 0x4c, 0x92, 0xb1, 0x3e, 0x50, 0x18, 0xe0,
                0x00, 0x79, 0x0d, 0x52, 0x62, 0xd1, 0x9e, 0x02,
                0x8d, 0x6c, 0x98, 0xcd, 0x8d, 0x87, 0x71, 0x33,
                0x7d, 0xcc, 0x6c, 0x1f, 0x1b, 0xd7, 0x24, 0xb4,
                0x42, 0xa9, 0x46, 0xee, 0x52, 0xd6, 0x9b, 0xbb,
                0xaa, 0x9f, 0x4d, 0x81, 0x03, 0x6b, 0x81, 0xc8,
                0x01, 0x0b, 0x85, 0x4d, 0x56, 0x22, 0xa4, 0xb3,
                0xa1, 0x3a, 0xc8, 0xdb, 0x70, 0x84, 0x65, 0xd6,
                0xd5, 0xb9, 0x0c, 0xcc, 0xee, 0x1e, 0x6c, 0x56,
                0x05, 0x30, 0x32, 0xbc, 0xef, 0x6b, 0x92, 0x6c,
                0x11, 0xa4, 0x59, 0x6b, 0x3b, 0xde, 0x31, 0x97,
                0x75, 0xbd, 0x85, 0xf2,
        };
        int     expected_len = strlen(expected);
        int     box_len = sizeof box_contents;

        test_unbox_vector(box_contents, box_len, expected, expected_len);
        test_box_cycle(expected, expected_len);
        test_box_bad_cycle(expected, expected_len);
}


static void
test_vector9(void)
{
        unsigned char expected[] = "Jayne, go play with your rainstick.";
        unsigned char box_contents[] = {
                0xc2, 0x01, 0xc5, 0x73, 0xca, 0xd4, 0x6c, 0x49,
                0x41, 0x1b, 0x55, 0x7f, 0xd9, 0x74, 0x24, 0x27,
                0x82, 0xc0, 0x30, 0x7a, 0xc2, 0x65, 0x83, 0xed,
                0xcb, 0xf1, 0x2b, 0x84, 0x1e, 0x66, 0x84, 0x77,
                0xf6, 0x7e, 0xe7, 0x57, 0x7d, 0x28, 0xdd, 0xae,
                0xcc, 0x1e, 0x2d, 0x2c, 0x76, 0x82, 0xc7, 0xda,
                0x65, 0xfd, 0x21, 0xd3, 0x08, 0x29, 0x64, 0x38,
                0x59, 0x58, 0x76, 0x98, 0x67, 0x7c, 0x1f, 0xf3,
                0xe0, 0x36, 0x9d, 0xe3, 0xa2, 0x61, 0xd5, 0x48,
                0x1f, 0x95, 0x06, 0xf3, 0x5b, 0x84, 0x0c, 0x2a,
                0x3f, 0xe8, 0xfa,
        };
        int     expected_len = strlen(expected);
        int     box_len = sizeof box_contents;

        test_unbox_vector(box_contents, box_len, expected, expected_len);
        test_box_cycle(expected, expected_len);
        test_box_bad_cycle(expected, expected_len);
}


/*
 * init_test is called each time a test is run, and cleanup is run after
 * every test.
 */
int init_test(void)
{
	return 0;
}

int cleanup_test(void)
{
	return 0;
}


/*
 * fireball is the code called when adding test fails: cleanup the test
 * registry and exit.
 */
void
fireball(void)
{
	int	error = 0;

	error = CU_get_error();
	if (error == 0)
		error = -1;

	fprintf(stderr, "fatal error in tests\n");
	CU_cleanup_registry();
	exit(error);
}


/*
 * The main function sets up the test suite, registers the test cases,
 * runs through them, and hopefully doesn't explode.
 */
int
main(void)
{
	CU_pSuite       tsuite = NULL;
	unsigned int    fails;

	if (!(CUE_SUCCESS == CU_initialize_registry())) {
		errx(EX_CONFIG, "failed to initialise test registry");
		return EXIT_FAILURE;
	}

	tsuite = CU_add_suite("strongbox_test", init_test, cleanup_test);
	if (NULL == tsuite)
		fireball();

	if (NULL == CU_add_test(tsuite, "opening box", test_decrypt))
		fireball();
	if (NULL == CU_add_test(tsuite, "basic checks", test_identity))
		fireball();
	if (NULL == CU_add_test(tsuite, "test vector #1", test_vector1))
		fireball();
	if (NULL == CU_add_test(tsuite, "test vector #2", test_vector2))
		fireball();
	if (NULL == CU_add_test(tsuite, "test vector #3", test_vector3))
		fireball();
	if (NULL == CU_add_test(tsuite, "test vector #4", test_vector4))
		fireball();
	if (NULL == CU_add_test(tsuite, "test vector #5", test_vector5))
		fireball();
	if (NULL == CU_add_test(tsuite, "test vector #6", test_vector6))
		fireball();
	if (NULL == CU_add_test(tsuite, "test vector #7", test_vector7))
		fireball();
	if (NULL == CU_add_test(tsuite, "test vector #8", test_vector8))
		fireball();
	if (NULL == CU_add_test(tsuite, "test vector #9", test_vector9))
		fireball();

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	fails = CU_get_number_of_tests_failed();
	warnx("%u tests failed", fails);

	CU_cleanup_registry();
	return fails;
}


/*
 * This is an empty test provided for reference.
 */
void
empty_test()
{
	CU_ASSERT(1 == 0);
}
