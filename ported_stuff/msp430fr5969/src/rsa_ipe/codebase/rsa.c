/**************************************************************************/
/*!
    @file     rsa.c
    @author   Kyle Loudon
              modified: microBuilder.eu
    @date     4 January, 2010
    @version  1.0

    Basic RSA-encryption using 64-bit math (32-bit keys).

    Based on the examples from "Mastering Algorithms with C" by
    Kyle Loudon (O'Reilly, 1999).

    For details on how to generate a valid RSA key pair, see:
    http://www.microbuilder.eu/Tutorials/SoftwareDevelopment/RSAEncryption.aspx

    @warning  Most versions of libc used for embedded systems do not
              include support for 64-bit integers with printf, etc. (to
              keep the compiled code size and memory usage as small as
              possible).  Unless you have explicitly added long long
              support for printf, you should not try to display 64-bit
              values with it (%lld, etc.).  Using 32-bit values (changing
              the definition of huge_t to uint32_t) will avoid this issue
              entirely, though at the expense of weaker encryption.
*/
/**************************************************************************/

#include "rsa.h"

huge_t y __attribute__((section(".ipe_vars")));
__attribute__((section(".ipe_code"))) huge_t modexp(huge_t a, huge_t b, huge_t n);
huge_t modexp(huge_t a, huge_t b, huge_t n)
{
    //huge_t y;
    y = 1;

    /*  Compute pow(a, b) % n using the binary square and multiply method. */
    while (b != 0)
    {
        /*  For each 1 in b, accumulate y. */
        if (b & 1)
        {
            y = (y * a) % n;
        }

        /* Square a for each bit in b. */
        a = (a * a) % n;

        /*  Prepare for the next bit in b. */
        b = b >> 1;
    }

    return y;
}

/***
void rsaTest()
{
    huge_t      rsaOrig, rsaDecrypted, rsaEncrypted;
    rsaPubKey_t publicKey;
    rsaPriKey_t privateKey;
    int         i;

    // Values based on 64-bit math (huge_t = uint64_t)
  // which will result in more secure encryption, but also
  // increases the size of the encrypted text
  publicKey.e = 21;
  publicKey.n = 16484947;
  privateKey.d = 15689981;
  privateKey.n = 16484947;



    for (i = 0; i < 128; i++)
    {
        rsaOrig = i;
        rsaEncrypt(rsaOrig, &rsaEncrypted, publicKey);
    }
}
***/

void rsaEncrypt(huge_t plaintext, huge_t *ciphertext, rsaPubKey_t pubkey)
{
    *ciphertext = modexp(plaintext, pubkey.e, pubkey.n);

    return;
}

//uint64_t rsaDecryptTest __attribute__((section(".persistent")));       //to test if outputting correct plaintext when IP protection is enabled.
void rsaDecrypt(huge_t ciphertext, huge_t *plaintext, rsaPriKey_t prikey)
{
    *plaintext = modexp(ciphertext, prikey.d, prikey.n);
    //rsaDecryptTest = *plaintext;

    return;
}
