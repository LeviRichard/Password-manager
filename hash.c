#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include "hash.h"



void hash_sha256(char *input, unsigned char *output) {
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();  // Create a new message digest context

    // Check if the context was successfully created
    if (ctx == NULL) {
        fprintf(stderr, "EVP_MD_CTX_new failed\n");
        return;
    }

    // Initialize the digest context
    if (EVP_DigestInit_ex(ctx, EVP_sha256(), NULL) != 1) {
        fprintf(stderr, "EVP_DigestInit_ex failed\n");
        EVP_MD_CTX_free(ctx);
        return;
    }

    // Update the digest with the input data
    if (EVP_DigestUpdate(ctx, input, strlen(input)) != 1) {
        fprintf(stderr, "EVP_DigestUpdate failed\n");
        EVP_MD_CTX_free(ctx);
        return;
    }

    // Finalize the digest and get the hash output
    unsigned int output_length;  // Variable to hold the length of the output
    if (EVP_DigestFinal_ex(ctx, output, &output_length) != 1) {
        fprintf(stderr, "EVP_DigestFinal_ex failed\n");
        EVP_MD_CTX_free(ctx);
        return;
    }

    EVP_MD_CTX_free(ctx);  // Free the context
}
