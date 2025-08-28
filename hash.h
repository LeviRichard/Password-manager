#ifndef HASH_H
#define HASH_H

#include <openssl/evp.h>

#define HASH_OUTPUT_SIZE 32 //SHA-256 Output is 32 bytes


void hash_sha256(char * input, unsigned char * output);

#endif
