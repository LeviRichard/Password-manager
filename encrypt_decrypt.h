#ifndef ENCRYPT_DECRYPT_H
#define ENCRYPT_DECRYPT_H

#define KEY_LEN 32
#define IV_LEN 16

int encrypt_password(char *plaintext, char *master_password, char **ciphertext_base64);
int decrypt_password(char *ciphertext_base64, char *master_password, char **plaintext_out);

#endif
