#include <stdio.h>
#include <string.h>

void vulnerable_function() {
    char buffer[16];  // Small stack-allocated buffer

    printf("Buffer address: %p\n", buffer);

    // Deliberate overflow: copying 128 bytes into 16-byte buffer
    char long_input[128];
    memset(long_input, 'A', sizeof(long_input)-1);  // Fill with 'A'
   long_input[127] = '\0';

    strcpy(buffer, long_input);  // No bounds check!
    printf("Buffer content: %s\n", buffer);
}

int main() {
    vulnerable_function();
    printf("Returned safely from vulnerable_function\n");
    return 0;
}
