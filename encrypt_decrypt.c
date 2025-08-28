#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "encrypt_decrypt.h"
#include <openssl/bio.h>
#include <openssl/buffer.h>

static void derive_key_iv(char *password, unsigned char *key, unsigned char *iv) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)password, strlen(password), hash);
    memcpy(key, hash, KEY_LEN);
    memcpy(iv, hash + 16, IV_LEN);  // use second half of hash for IV
}

static char *base64_encode(unsigned char *input, int len) {
    BIO *b64 = BIO_new(BIO_f_base64());
    BIO *mem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, mem);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(b64, input, len);
    BIO_flush(b64);
    BUF_MEM *bptr;
    BIO_get_mem_ptr(b64, &bptr);
    char *buff = malloc(bptr->length + 1);
    memcpy(buff, bptr->data, bptr->length);
    buff[bptr->length] = '\0';
    BIO_free_all(b64);
    return buff;
}

static unsigned char *base64_decode(char *input, int *out_len) {
    int len = strlen(input);
    BIO *b64 = BIO_new(BIO_f_base64());
    BIO *mem = BIO_new_mem_buf(input, len);
    b64 = BIO_push(b64, mem);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    unsigned char *buffer = malloc(len);
    *out_len = BIO_read(b64, buffer, len);
    BIO_free_all(b64);
    return buffer;
}

int encrypt_password(char *plaintext, char *master_password, char **ciphertext_base64) {
    unsigned char key[KEY_LEN], iv[IV_LEN];
    derive_key_iv(master_password, key, iv);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int out_len1, out_len2;
    int input_len = strlen(plaintext);
    unsigned char outbuf[1024];

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, outbuf, &out_len1, (unsigned char *)plaintext, input_len);
    EVP_EncryptFinal_ex(ctx, outbuf + out_len1, &out_len2);

    int total_len = out_len1 + out_len2;
    *ciphertext_base64 = base64_encode(outbuf, total_len);

    EVP_CIPHER_CTX_free(ctx);
    return 1;
}

int decrypt_password(char *ciphertext_base64, char *master_password, char **plaintext_out) {
    unsigned char key[KEY_LEN], iv[IV_LEN];
    derive_key_iv(master_password, key, iv);

    int cipher_len;
    unsigned char *ciphertext = base64_decode(ciphertext_base64, &cipher_len);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    unsigned char outbuf[1024];
    int out_len1, out_len2;

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_DecryptUpdate(ctx, outbuf, &out_len1, ciphertext, cipher_len);
    EVP_DecryptFinal_ex(ctx, outbuf + out_len1, &out_len2);

    int total_len = out_len1 + out_len2;
    outbuf[total_len] = '\0';
    *plaintext_out = strdup((char *)outbuf);

    EVP_CIPHER_CTX_free(ctx);
    free(ciphertext);
    return 1;
}
