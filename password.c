#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void mask_password(char *input, int max_len) {
    struct termios oldt, newt;
    int i = 0;
    char ch;

    // Get the terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);  // Disable echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    
    while (i < max_len - 1) {
        ch = getchar();  // Read a character
        if (ch == '\n') break;  // Break on Enter
        input[i++] = ch;
        printf("*");  // Print an asterisk for each character
    }
    input[i] = '\0';  // Null-terminate the string

    // Restore the terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");  // Move to the next line
}
