/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 */
#include "tomcrypt.h"

/**
  @file hmac_test.c
  HMAC support, self-test, Tom St Denis/Dobes Vandermeer/Steffen Jaeckel
*/

#ifdef LTC_HMAC

#define LTC_HMAC_BLOCKSIZE hash_descriptor[hash].blocksize

/*
    TEST CASES SOURCE:

Network Working Group                                          P. Cheng
Request for Comments: 2202                                          IBM
Category: Informational                                        R. Glenn
                                                                   NIST
                                                         September 1997

                 Test Cases for HMAC-MD5 and HMAC-SHA-1

*******************************************************************************

Network Working Group                                            J. Kapp
Request for Comments: 2286                           Reaper Technologies
Category: Informational                                    February 1998

            Test Cases for HMAC-RIPEMD160 and HMAC-RIPEMD128

*******************************************************************************

Network Working Group                                         M. Nystrom
Request for Comments: 4231                                  RSA Security
Category: Standards Track                                  December 2005

     Identifiers and Test Vectors for HMAC-SHA-224, HMAC-SHA-256,
                     HMAC-SHA-384, and HMAC-SHA-512
*/

/**
  HMAC self-test
  @return CRYPT_OK if successful, CRYPT_NOP if tests have been disabled.
*/
int hmac_test(void)
{
 #ifndef LTC_TEST
    return CRYPT_NOP;
 #else
    unsigned char digest[MAXBLOCKSIZE];
    int i;

    static const unsigned char hmac_test_case_keys[][136] = {
        { /* 1 */
            0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
            0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
            0x0b, 0x0b, 0x0b, 0x0b
        },
#ifdef LTC_TEST_EXT
        { /* 2 */
            0x4a, 0x65, 0x66, 0x65
        },
        { /* 4 */
            0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
            0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14,
            0x15, 0x16, 0x17, 0x18, 0x19
        },
        { /* 5 */
            0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
            0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,
            0x0c, 0x0c, 0x0c, 0x0c
        },
        { /* 3, 6, 7 */
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,

            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,

            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,

            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa
        }
#endif /* LTC_TEST_EXT */
    };


    static const unsigned char hmac_test_case_data[][153] = {
        {
            "Hi There"
        },
#ifdef LTC_TEST_EXT
        {
            "what do ya want for nothing?"
        },
        {
            0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
            0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
            0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
            0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
            0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd
        },
        {
            0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
            0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
            0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
            0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
            0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd
        },
        {
            "Test With Truncation"
        },
        {
            "Test Using Larger Than Block-Size Key - Hash Key First"
        },
        {
            "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data"
        },
        {
            "This is a test using a larger than block-size key and a larger than block-size data. The key needs to be hashed before being used by the HMAC algorithm."
        }
#endif /* LTC_TEST_EXT */
    };

    static const struct hmac_test_case {
        const char *num;
        const char *algo;
        const unsigned char *key;
        unsigned long keylen;
        const unsigned char *data;
        unsigned long datalen;
        unsigned char digest[MAXBLOCKSIZE];
    } cases[] = {
        /*
        RFC 2202 3. Test Cases for HMAC-SHA-1
        */
        { "rfc2202 3.1", "sha1",
            hmac_test_case_keys[0], 20,
            hmac_test_case_data[0], 8,
            {0xb6, 0x17, 0x31, 0x86, 0x55, 0x05, 0x72, 0x64,
             0xe2, 0x8b, 0xc0, 0xb6, 0xfb, 0x37, 0x8c, 0x8e,
             0xf1, 0x46, 0xbe, 0x00} },

#ifdef LTC_TEST_EXT
        { "rfc2202 3.2", "sha1",
            hmac_test_case_keys[1], 4,
            hmac_test_case_data[1], 28,
            {0xef, 0xfc, 0xdf, 0x6a, 0xe5, 0xeb, 0x2f, 0xa2,
             0xd2, 0x74, 0x16, 0xd5, 0xf1, 0x84, 0xdf, 0x9c,
             0x25, 0x9a, 0x7c, 0x79} },

        { "rfc2202 3.3", "sha1",
            hmac_test_case_keys[4], 20,
            hmac_test_case_data[2], 50,
            {0x12, 0x5d, 0x73, 0x42, 0xb9, 0xac, 0x11, 0xcd,
             0x91, 0xa3, 0x9a, 0xf4, 0x8a, 0xa1, 0x7b, 0x4f,
             0x63, 0xf1, 0x75, 0xd3} },

        { "rfc2202 3.4", "sha1",
            hmac_test_case_keys[2], 25,
            hmac_test_case_data[3], 50,
            {0x4c, 0x90, 0x07, 0xf4, 0x02, 0x62, 0x50, 0xc6,
             0xbc, 0x84, 0x14, 0xf9, 0xbf, 0x50, 0xc8, 0x6c,
             0x2d, 0x72, 0x35, 0xda} },

        { "rfc2202 3.5", "sha1",
            hmac_test_case_keys[3], 20,
            hmac_test_case_data[4], 20,
            {0x4c, 0x1a, 0x03, 0x42, 0x4b, 0x55, 0xe0, 0x7f, 0xe7, 0xf2,
             0x7b, 0xe1, 0xd5, 0x8b, 0xb9, 0x32, 0x4a, 0x9a, 0x5a, 0x04} },

        { "rfc2202 3.6", "sha1",
            hmac_test_case_keys[4], 80,
            hmac_test_case_data[5], 54,
            {0xaa, 0x4a, 0xe5, 0xe1, 0x52, 0x72, 0xd0, 0x0e,
             0x95, 0x70, 0x56, 0x37, 0xce, 0x8a, 0x3b, 0x55,
             0xed, 0x40, 0x21, 0x12} },

        { "rfc2202 3.7", "sha1",
            hmac_test_case_keys[4], 80,
           hmac_test_case_data[6], 73,
            {0xe8, 0xe9, 0x9d, 0x0f, 0x45, 0x23, 0x7d, 0x78, 0x6d,
             0x6b, 0xba, 0xa7, 0x96, 0x5c, 0x78, 0x08, 0xbb, 0xff, 0x1a, 0x91} },
#endif /* LTC_TEST_EXT */

        /*
        RFC 2202 2. Test Cases for HMAC-MD5
        */
        { "rfc2202 2.1", "md5",
            hmac_test_case_keys[0], 16,
            hmac_test_case_data[0], 8,
            {0x92, 0x94, 0x72, 0x7a, 0x36, 0x38, 0xbb, 0x1c,
             0x13, 0xf4, 0x8e, 0xf8, 0x15, 0x8b, 0xfc, 0x9d}  },

#ifdef LTC_TEST_EXT
        { "rfc2202 2.2", "md5",
            hmac_test_case_keys[1], 4,
            hmac_test_case_data[1], 28,
            {0x75, 0x0c, 0x78, 0x3e, 0x6a, 0xb0, 0xb5, 0x03,
             0xea, 0xa8, 0x6e, 0x31, 0x0a, 0x5d, 0xb7, 0x38} },

        { "rfc2202 2.3", "md5",
            hmac_test_case_keys[4], 16,
            hmac_test_case_data[2], 50,
            {0x56, 0xbe, 0x34, 0x52, 0x1d, 0x14, 0x4c, 0x88,
             0xdb, 0xb8, 0xc7, 0x33, 0xf0, 0xe8, 0xb3, 0xf6} },

        { "rfc2202 2.4", "md5",
            hmac_test_case_keys[2], 25,
            hmac_test_case_data[3], 50,
            {0x69, 0x7e, 0xaf, 0x0a, 0xca, 0x3a, 0x3a, 0xea,
             0x3a, 0x75, 0x16, 0x47, 0x46, 0xff, 0xaa, 0x79} },

        { "rfc2202 2.5", "md5",
            hmac_test_case_keys[3], 16,
            hmac_test_case_data[4], 20,
            {0x56, 0x46, 0x1e, 0xf2, 0x34, 0x2e, 0xdc, 0x00,
             0xf9, 0xba, 0xb9, 0x95, 0x69, 0x0e, 0xfd, 0x4c} },

        { "rfc2202 2.6", "md5",
            hmac_test_case_keys[4], 80,
            hmac_test_case_data[5], 54,
            {0x6b, 0x1a, 0xb7, 0xfe, 0x4b, 0xd7, 0xbf, 0x8f,
             0x0b, 0x62, 0xe6, 0xce, 0x61, 0xb9, 0xd0, 0xcd} },

        { "rfc2202 2.7", "md5",
            hmac_test_case_keys[4], 80,
           hmac_test_case_data[6], 73,
            {0x6f, 0x63, 0x0f, 0xad, 0x67, 0xcd, 0xa0, 0xee,
             0x1f, 0xb1, 0xf5, 0x62, 0xdb, 0x3a, 0xa5, 0x3e} },
#endif /* LTC_TEST_EXT */

        /*
        RFC 2286 2. Test Cases for HMAC-RIPEMD160
        */
        { "rfc2286 2.1", "rmd160",
            hmac_test_case_keys[0], 20,
            hmac_test_case_data[0], 8,
            {0x24, 0xcb, 0x4b, 0xd6, 0x7d, 0x20, 0xfc, 0x1a,
             0x5d, 0x2e, 0xd7, 0x73, 0x2d, 0xcc, 0x39, 0x37,
             0x7f, 0x0a, 0x56, 0x68} },

#ifdef LTC_TEST_EXT
        { "rfc2286 2.2", "rmd160",
            hmac_test_case_keys[1], 4,
            hmac_test_case_data[1], 28,
            {0xdd, 0xa6, 0xc0, 0x21, 0x3a, 0x48, 0x5a, 0x9e,
             0x24, 0xf4, 0x74, 0x20, 0x64, 0xa7, 0xf0, 0x33,
             0xb4, 0x3c, 0x40, 0x69} },

        { "rfc2286 2.3", "rmd160",
            hmac_test_case_keys[4], 20,
            hmac_test_case_data[2], 50,
            {0xb0, 0xb1, 0x05, 0x36, 0x0d, 0xe7, 0x59, 0x96,
             0x0a, 0xb4, 0xf3, 0x52, 0x98, 0xe1, 0x16, 0xe2,
             0x95, 0xd8, 0xe7, 0xc1} },

        { "rfc2286 2.4", "rmd160",
            hmac_test_case_keys[2], 25,
            hmac_test_case_data[3], 50,
            {0xd5, 0xca, 0x86, 0x2f, 0x4d, 0x21, 0xd5, 0xe6,
             0x10, 0xe1, 0x8b, 0x4c, 0xf1, 0xbe, 0xb9, 0x7a,
             0x43, 0x65, 0xec, 0xf4} },

        { "rfc2286 2.5", "rmd160",
            hmac_test_case_keys[3], 20,
            hmac_test_case_data[4], 20,
            {0x76, 0x19, 0x69, 0x39, 0x78, 0xf9, 0x1d, 0x90,
             0x53, 0x9a, 0xe7, 0x86, 0x50, 0x0f, 0xf3, 0xd8,
             0xe0, 0x51, 0x8e, 0x39} },

        { "rfc2286 2.6", "rmd160",
            hmac_test_case_keys[4], 80,
            hmac_test_case_data[5], 54,
            {0x64, 0x66, 0xca, 0x07, 0xac, 0x5e, 0xac, 0x29,
             0xe1, 0xbd, 0x52, 0x3e, 0x5a, 0xda, 0x76, 0x05,
             0xb7, 0x91, 0xfd, 0x8b} },

        { "rfc2286 2.7", "rmd160",
            hmac_test_case_keys[4], 80,
            hmac_test_case_data[6], 73,
            {0x69, 0xea, 0x60, 0x79, 0x8d, 0x71, 0x61, 0x6c,
             0xce, 0x5f, 0xd0, 0x87, 0x1e, 0x23, 0x75, 0x4c,
             0xd7, 0x5d, 0x5a, 0x0a} },
#endif /* LTC_TEST_EXT */

        /*
        RFC 2286 3. Test Cases for HMAC-RIPEMD128
        */
        { "rfc2286 3.1", "rmd128",
            hmac_test_case_keys[0], 16,
            hmac_test_case_data[0], 8,
            {0xfb, 0xf6, 0x1f, 0x94, 0x92, 0xaa, 0x4b, 0xbf,
             0x81, 0xc1, 0x72, 0xe8, 0x4e, 0x07, 0x34, 0xdb} },

#ifdef LTC_TEST_EXT
        { "rfc2286 3.2", "rmd128",
            hmac_test_case_keys[1], 4,
            hmac_test_case_data[1], 28,
            {0x87, 0x5f, 0x82, 0x88, 0x62, 0xb6, 0xb3, 0x34,
             0xb4, 0x27, 0xc5, 0x5f, 0x9f, 0x7f, 0xf0, 0x9b} },

        { "rfc2286 3.3", "rmd128",
            hmac_test_case_keys[4], 16,
            hmac_test_case_data[2], 50,
            {0x09, 0xf0, 0xb2, 0x84, 0x6d, 0x2f, 0x54, 0x3d,
             0xa3, 0x63, 0xcb, 0xec, 0x8d, 0x62, 0xa3, 0x8d} },

        { "rfc2286 3.4", "rmd128",
            hmac_test_case_keys[2], 25,
            hmac_test_case_data[3], 50,
            {0xbd, 0xbb, 0xd7, 0xcf, 0x03, 0xe4, 0x4b, 0x5a,
             0xa6, 0x0a, 0xf8, 0x15, 0xbe, 0x4d, 0x22, 0x94} },

        { "rfc2286 3.5", "rmd128",
            hmac_test_case_keys[3], 16,
            hmac_test_case_data[4], 20,
            {0xe7, 0x98, 0x08, 0xf2, 0x4b, 0x25, 0xfd, 0x03,
             0x1c, 0x15, 0x5f, 0x0d, 0x55, 0x1d, 0x9a, 0x3a} },

        { "rfc2286 3.6", "rmd128",
            hmac_test_case_keys[4], 80,
            hmac_test_case_data[5], 54,
            {0xdc, 0x73, 0x29, 0x28, 0xde, 0x98, 0x10, 0x4a,
             0x1f, 0x59, 0xd3, 0x73, 0xc1, 0x50, 0xac, 0xbb} },

        { "rfc2286 3.7", "rmd128",
            hmac_test_case_keys[4], 80,
            hmac_test_case_data[6], 73,
            {0x5c, 0x6b, 0xec, 0x96, 0x79, 0x3e, 0x16, 0xd4,
             0x06, 0x90, 0xc2, 0x37, 0x63, 0x5f, 0x30, 0xc5} },
#endif /* LTC_TEST_EXT */

        /*
        RFC 4231 4. Test Vectors
        Ch. 4.6 with truncated output left out to simplify tests
        */
        { "rfc4231 4.2", "sha224",
            hmac_test_case_keys[0], 20,
            hmac_test_case_data[0], 8,
            {0x89, 0x6f, 0xb1, 0x12, 0x8a, 0xbb, 0xdf, 0x19,
             0x68, 0x32, 0x10, 0x7c, 0xd4, 0x9d, 0xf3, 0x3f,
             0x47, 0xb4, 0xb1, 0x16, 0x99, 0x12, 0xba, 0x4f,
             0x53, 0x68, 0x4b, 0x22} },

#ifdef LTC_TEST_EXT
        { "rfc4231 4.3", "sha224",
            hmac_test_case_keys[1], 4,
            hmac_test_case_data[1], 28,
            {0xa3, 0x0e, 0x01, 0x09, 0x8b, 0xc6, 0xdb, 0xbf,
             0x45, 0x69, 0x0f, 0x3a, 0x7e, 0x9e, 0x6d, 0x0f,
             0x8b, 0xbe, 0xa2, 0xa3, 0x9e, 0x61, 0x48, 0x00,
             0x8f, 0xd0, 0x5e, 0x44} },

        { "rfc4231 4.4", "sha224",
            hmac_test_case_keys[4], 20,
            hmac_test_case_data[2], 50,
            {0x7f, 0xb3, 0xcb, 0x35, 0x88, 0xc6, 0xc1, 0xf6,
             0xff, 0xa9, 0x69, 0x4d, 0x7d, 0x6a, 0xd2, 0x64,
             0x93, 0x65, 0xb0, 0xc1, 0xf6, 0x5d, 0x69, 0xd1,
             0xec, 0x83, 0x33, 0xea} },

        { "rfc4231 4.5", "sha224",
            hmac_test_case_keys[2], 25,
            hmac_test_case_data[3], 50,
            {0x6c, 0x11, 0x50, 0x68, 0x74, 0x01, 0x3c, 0xac,
             0x6a, 0x2a, 0xbc, 0x1b, 0xb3, 0x82, 0x62, 0x7c,
             0xec, 0x6a, 0x90, 0xd8, 0x6e, 0xfc, 0x01, 0x2d,
             0xe7, 0xaf, 0xec, 0x5a} },

        { "rfc4231 4.7", "sha224",
            hmac_test_case_keys[4], 131,
            hmac_test_case_data[5], 54,
            {0x95, 0xe9, 0xa0, 0xdb, 0x96, 0x20, 0x95, 0xad,
             0xae, 0xbe, 0x9b, 0x2d, 0x6f, 0x0d, 0xbc, 0xe2,
             0xd4, 0x99, 0xf1, 0x12, 0xf2, 0xd2, 0xb7, 0x27,
             0x3f, 0xa6, 0x87, 0x0e} },

        { "rfc4231 4.8", "sha224",
            hmac_test_case_keys[4], 131,
            hmac_test_case_data[7], 152,
            {0x3a, 0x85, 0x41, 0x66, 0xac, 0x5d, 0x9f, 0x02,
             0x3f, 0x54, 0xd5, 0x17, 0xd0, 0xb3, 0x9d, 0xbd,
             0x94, 0x67, 0x70, 0xdb, 0x9c, 0x2b, 0x95, 0xc9,
             0xf6, 0xf5, 0x65, 0xd1} },
#endif /* LTC_TEST_EXT */

        { "rfc4231 4.2", "sha256",
            hmac_test_case_keys[0], 20,
            hmac_test_case_data[0], 8,
            {0xb0, 0x34, 0x4c, 0x61, 0xd8, 0xdb, 0x38, 0x53,
             0x5c, 0xa8, 0xaf, 0xce, 0xaf, 0x0b, 0xf1, 0x2b,
             0x88, 0x1d, 0xc2, 0x00, 0xc9, 0x83, 0x3d, 0xa7,
             0x26, 0xe9, 0x37, 0x6c, 0x2e, 0x32, 0xcf, 0xf7} },

#ifdef LTC_TEST_EXT
        { "rfc4231 4.3", "sha256",
            hmac_test_case_keys[1], 4,
            hmac_test_case_data[1], 28,
            {0x5b, 0xdc, 0xc1, 0x46, 0xbf, 0x60, 0x75, 0x4e,
             0x6a, 0x04, 0x24, 0x26, 0x08, 0x95, 0x75, 0xc7,
             0x5a, 0x00, 0x3f, 0x08, 0x9d, 0x27, 0x39, 0x83,
             0x9d, 0xec, 0x58, 0xb9, 0x64, 0xec, 0x38, 0x43} },

        { "rfc4231 4.4", "sha256",
            hmac_test_case_keys[4], 20,
            hmac_test_case_data[2], 50,
            {0x77, 0x3e, 0xa9, 0x1e, 0x36, 0x80, 0x0e, 0x46,
             0x85, 0x4d, 0xb8, 0xeb, 0xd0, 0x91, 0x81, 0xa7,
             0x29, 0x59, 0x09, 0x8b, 0x3e, 0xf8, 0xc1, 0x22,
             0xd9, 0x63, 0x55, 0x14, 0xce, 0xd5, 0x65, 0xfe} },

        { "rfc4231 4.5", "sha256",
            hmac_test_case_keys[2], 25,
            hmac_test_case_data[3], 50,
            {0x82, 0x55, 0x8a, 0x38, 0x9a, 0x44, 0x3c, 0x0e,
             0xa4, 0xcc, 0x81, 0x98, 0x99, 0xf2, 0x08, 0x3a,
             0x85, 0xf0, 0xfa, 0xa3, 0xe5, 0x78, 0xf8, 0x07,
             0x7a, 0x2e, 0x3f, 0xf4, 0x67, 0x29, 0x66, 0x5b} },

        { "rfc4231 4.7", "sha256",
            hmac_test_case_keys[4], 131,
            hmac_test_case_data[5], 54,
            {0x60, 0xe4, 0x31, 0x59, 0x1e, 0xe0, 0xb6, 0x7f,
             0x0d, 0x8a, 0x26, 0xaa, 0xcb, 0xf5, 0xb7, 0x7f,
             0x8e, 0x0b, 0xc6, 0x21, 0x37, 0x28, 0xc5, 0x14,
             0x05, 0x46, 0x04, 0x0f, 0x0e, 0xe3, 0x7f, 0x54} },

        { "rfc4231 4.8", "sha256",
            hmac_test_case_keys[4], 131,
            hmac_test_case_data[7], 152,
            {0x9b, 0x09, 0xff, 0xa7, 0x1b, 0x94, 0x2f, 0xcb,
             0x27, 0x63, 0x5f, 0xbc, 0xd5, 0xb0, 0xe9, 0x44,
             0xbf, 0xdc, 0x63, 0x64, 0x4f, 0x07, 0x13, 0x93,
             0x8a, 0x7f, 0x51, 0x53, 0x5c, 0x3a, 0x35, 0xe2} },
#endif /* LTC_TEST_EXT */

        { "rfc4231 4.2", "sha384",
            hmac_test_case_keys[0], 20,
            hmac_test_case_data[0], 8,
            {0xaf, 0xd0, 0x39, 0x44, 0xd8, 0x48, 0x95, 0x62,
             0x6b, 0x08, 0x25, 0xf4, 0xab, 0x46, 0x90, 0x7f,
             0x15, 0xf9, 0xda, 0xdb, 0xe4, 0x10, 0x1e, 0xc6,
             0x82, 0xaa, 0x03, 0x4c, 0x7c, 0xeb, 0xc5, 0x9c,
             0xfa, 0xea, 0x9e, 0xa9, 0x07, 0x6e, 0xde, 0x7f,
             0x4a, 0xf1, 0x52, 0xe8, 0xb2, 0xfa, 0x9c, 0xb6} },

#ifdef LTC_TEST_EXT
        { "rfc4231 4.3", "sha384",
            hmac_test_case_keys[1], 4,
            hmac_test_case_data[1], 28,
            {0xaf, 0x45, 0xd2, 0xe3, 0x76, 0x48, 0x40, 0x31,
             0x61, 0x7f, 0x78, 0xd2, 0xb5, 0x8a, 0x6b, 0x1b,
             0x9c, 0x7e, 0xf4, 0x64, 0xf5, 0xa0, 0x1b, 0x47,
             0xe4, 0x2e, 0xc3, 0x73, 0x63, 0x22, 0x44, 0x5e,
             0x8e, 0x22, 0x40, 0xca, 0x5e, 0x69, 0xe2, 0xc7,
             0x8b, 0x32, 0x39, 0xec, 0xfa, 0xb2, 0x16, 0x49} },

        { "rfc4231 4.4", "sha384",
            hmac_test_case_keys[4], 20,
            hmac_test_case_data[2], 50,
            {0x88, 0x06, 0x26, 0x08, 0xd3, 0xe6, 0xad, 0x8a,
             0x0a, 0xa2, 0xac, 0xe0, 0x14, 0xc8, 0xa8, 0x6f,
             0x0a, 0xa6, 0x35, 0xd9, 0x47, 0xac, 0x9f, 0xeb,
             0xe8, 0x3e, 0xf4, 0xe5, 0x59, 0x66, 0x14, 0x4b,
             0x2a, 0x5a, 0xb3, 0x9d, 0xc1, 0x38, 0x14, 0xb9,
             0x4e, 0x3a, 0xb6, 0xe1, 0x01, 0xa3, 0x4f, 0x27} },

        { "rfc4231 4.5", "sha384",
            hmac_test_case_keys[2], 25,
            hmac_test_case_data[3], 50,
            {0x3e, 0x8a, 0x69, 0xb7, 0x78, 0x3c, 0x25, 0x85,
             0x19, 0x33, 0xab, 0x62, 0x90, 0xaf, 0x6c, 0xa7,
             0x7a, 0x99, 0x81, 0x48, 0x08, 0x50, 0x00, 0x9c,
             0xc5, 0x57, 0x7c, 0x6e, 0x1f, 0x57, 0x3b, 0x4e,
             0x68, 0x01, 0xdd, 0x23, 0xc4, 0xa7, 0xd6, 0x79,
             0xcc, 0xf8, 0xa3, 0x86, 0xc6, 0x74, 0xcf, 0xfb} },

        { "rfc4231 4.7", "sha384",
            hmac_test_case_keys[4], 131,
            hmac_test_case_data[5], 54,
            {0x4e, 0xce, 0x08, 0x44, 0x85, 0x81, 0x3e, 0x90,
             0x88, 0xd2, 0xc6, 0x3a, 0x04, 0x1b, 0xc5, 0xb4,
             0x4f, 0x9e, 0xf1, 0x01, 0x2a, 0x2b, 0x58, 0x8f,
             0x3c, 0xd1, 0x1f, 0x05, 0x03, 0x3a, 0xc4, 0xc6,
             0x0c, 0x2e, 0xf6, 0xab, 0x40, 0x30, 0xfe, 0x82,
             0x96, 0x24, 0x8d, 0xf1, 0x63, 0xf4, 0x49, 0x52} },

        { "rfc4231 4.8", "sha384",
            hmac_test_case_keys[4], 131,
            hmac_test_case_data[7], 152,
            {0x66, 0x17, 0x17, 0x8e, 0x94, 0x1f, 0x02, 0x0d,
             0x35, 0x1e, 0x2f, 0x25, 0x4e, 0x8f, 0xd3, 0x2c,
             0x60, 0x24, 0x20, 0xfe, 0xb0, 0xb8, 0xfb, 0x9a,
             0xdc, 0xce, 0xbb, 0x82, 0x46, 0x1e, 0x99, 0xc5,
             0xa6, 0x78, 0xcc, 0x31, 0xe7, 0x99, 0x17, 0x6d,
             0x38, 0x60, 0xe6, 0x11, 0x0c, 0x46, 0x52, 0x3e} },
#endif /* LTC_TEST_EXT */

        { "rfc4231 4.2", "sha512",
            hmac_test_case_keys[0], 20,
            hmac_test_case_data[0], 8,
            {0x87, 0xaa, 0x7c, 0xde, 0xa5, 0xef, 0x61, 0x9d,
             0x4f, 0xf0, 0xb4, 0x24, 0x1a, 0x1d, 0x6c, 0xb0,
             0x23, 0x79, 0xf4, 0xe2, 0xce, 0x4e, 0xc2, 0x78,
             0x7a, 0xd0, 0xb3, 0x05, 0x45, 0xe1, 0x7c, 0xde,
             0xda, 0xa8, 0x33, 0xb7, 0xd6, 0xb8, 0xa7, 0x02,
             0x03, 0x8b, 0x27, 0x4e, 0xae, 0xa3, 0xf4, 0xe4,
             0xbe, 0x9d, 0x91, 0x4e, 0xeb, 0x61, 0xf1, 0x70,
             0x2e, 0x69, 0x6c, 0x20, 0x3a, 0x12, 0x68, 0x54} },

#ifdef LTC_TEST_EXT
        { "rfc4231 4.3", "sha512",
            hmac_test_case_keys[1], 4,
            hmac_test_case_data[1], 28,
            {0x16, 0x4b, 0x7a, 0x7b, 0xfc, 0xf8, 0x19, 0xe2,
             0xe3, 0x95, 0xfb, 0xe7, 0x3b, 0x56, 0xe0, 0xa3,
             0x87, 0xbd, 0x64, 0x22, 0x2e, 0x83, 0x1f, 0xd6,
             0x10, 0x27, 0x0c, 0xd7, 0xea, 0x25, 0x05, 0x54,
             0x97, 0x58, 0xbf, 0x75, 0xc0, 0x5a, 0x99, 0x4a,
             0x6d, 0x03, 0x4f, 0x65, 0xf8, 0xf0, 0xe6, 0xfd,
             0xca, 0xea, 0xb1, 0xa3, 0x4d, 0x4a, 0x6b, 0x4b,
             0x63, 0x6e, 0x07, 0x0a, 0x38, 0xbc, 0xe7, 0x37} },

        { "rfc4231 4.4", "sha512",
            hmac_test_case_keys[4], 20,
            hmac_test_case_data[2], 50,
            {0xfa, 0x73, 0xb0, 0x08, 0x9d, 0x56, 0xa2, 0x84,
             0xef, 0xb0, 0xf0, 0x75, 0x6c, 0x89, 0x0b, 0xe9,
             0xb1, 0xb5, 0xdb, 0xdd, 0x8e, 0xe8, 0x1a, 0x36,
             0x55, 0xf8, 0x3e, 0x33, 0xb2, 0x27, 0x9d, 0x39,
             0xbf, 0x3e, 0x84, 0x82, 0x79, 0xa7, 0x22, 0xc8,
             0x06, 0xb4, 0x85, 0xa4, 0x7e, 0x67, 0xc8, 0x07,
             0xb9, 0x46, 0xa3, 0x37, 0xbe, 0xe8, 0x94, 0x26,
             0x74, 0x27, 0x88, 0x59, 0xe1, 0x32, 0x92, 0xfb} },

        { "rfc4231 4.5", "sha512",
            hmac_test_case_keys[2], 25,
            hmac_test_case_data[3], 50,
            {0xb0, 0xba, 0x46, 0x56, 0x37, 0x45, 0x8c, 0x69,
             0x90, 0xe5, 0xa8, 0xc5, 0xf6, 0x1d, 0x4a, 0xf7,
             0xe5, 0x76, 0xd9, 0x7f, 0xf9, 0x4b, 0x87, 0x2d,
             0xe7, 0x6f, 0x80, 0x50, 0x36, 0x1e, 0xe3, 0xdb,
             0xa9, 0x1c, 0xa5, 0xc1, 0x1a, 0xa2, 0x5e, 0xb4,
             0xd6, 0x79, 0x27, 0x5c, 0xc5, 0x78, 0x80, 0x63,
             0xa5, 0xf1, 0x97, 0x41, 0x12, 0x0c, 0x4f, 0x2d,
             0xe2, 0xad, 0xeb, 0xeb, 0x10, 0xa2, 0x98, 0xdd} },

        { "rfc4231 4.7", "sha512",
            hmac_test_case_keys[4], 131,
            hmac_test_case_data[5], 54,
            {0x80, 0xb2, 0x42, 0x63, 0xc7, 0xc1, 0xa3, 0xeb,
             0xb7, 0x14, 0x93, 0xc1, 0xdd, 0x7b, 0xe8, 0xb4,
             0x9b, 0x46, 0xd1, 0xf4, 0x1b, 0x4a, 0xee, 0xc1,
             0x12, 0x1b, 0x01, 0x37, 0x83, 0xf8, 0xf3, 0x52,
             0x6b, 0x56, 0xd0, 0x37, 0xe0, 0x5f, 0x25, 0x98,
             0xbd, 0x0f, 0xd2, 0x21, 0x5d, 0x6a, 0x1e, 0x52,
             0x95, 0xe6, 0x4f, 0x73, 0xf6, 0x3f, 0x0a, 0xec,
             0x8b, 0x91, 0x5a, 0x98, 0x5d, 0x78, 0x65, 0x98} },

        { "rfc4231 4.8", "sha512",
            hmac_test_case_keys[4], 131,
            hmac_test_case_data[7], 152,
            {0xe3, 0x7b, 0x6a, 0x77, 0x5d, 0xc8, 0x7d, 0xba,
             0xa4, 0xdf, 0xa9, 0xf9, 0x6e, 0x5e, 0x3f, 0xfd,
             0xde, 0xbd, 0x71, 0xf8, 0x86, 0x72, 0x89, 0x86,
             0x5d, 0xf5, 0xa3, 0x2d, 0x20, 0xcd, 0xc9, 0x44,
             0xb6, 0x02, 0x2c, 0xac, 0x3c, 0x49, 0x82, 0xb1,
             0x0d, 0x5e, 0xeb, 0x55, 0xc3, 0xe4, 0xde, 0x15,
             0x13, 0x46, 0x76, 0xfb, 0x6d, 0xe0, 0x44, 0x60,
             0x65, 0xc9, 0x74, 0x40, 0xfa, 0x8c, 0x6a, 0x58} },
#endif /* LTC_TEST_EXT */

    };

    unsigned long outlen;
    int err;
    int tested=0,failed=0;
    for(i=0; i < (int)(sizeof(cases) / sizeof(cases[0])); i++) {
        int hash = find_hash(cases[i].algo);
        if (hash == -1) continue;
        ++tested;
        outlen = sizeof(digest);
        if((err = hmac_memory(hash, cases[i].key, cases[i].keylen, cases[i].data, cases[i].datalen, digest, &outlen)) != CRYPT_OK) {
#ifdef LTC_TEST_DBG
            printf("HMAC-%s test %s, %s\n", cases[i].algo, cases[i].num, error_to_string(err));
#endif
            return err;
        }

        if(compare_testvector(digest, outlen, cases[i].digest, (size_t)hash_descriptor[hash].hashsize, cases[i].num, i)) {
            failed++;
        }
    }

    if (failed != 0) {
        return CRYPT_FAIL_TESTVECTOR;
    } else if (tested == 0) {
        return CRYPT_NOP;
    } else {
        return CRYPT_OK;
    }
 #endif
}

#endif


/* ref:         $Format:%D$ */
/* git commit:  $Format:%H$ */
/* commit time: $Format:%ai$ */
