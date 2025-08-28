#ifndef PASSWORD_H
#define PASSWORD_H

#define MAX_PASSWORD_LEN 128

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void mask_password(char * input, int max_len);





#endif // !PASSWORD_H


