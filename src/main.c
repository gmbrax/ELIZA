#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void get_input(char *input_buffer, size_t buffer_size) {
    fgets(input_buffer, buffer_size, stdin);
}

char* copy_string(char *input_buffer, size_t buffer_size) {
    char* copied_buffer = malloc(buffer_size + 1);
    memcpy(copied_buffer, input_buffer, buffer_size + 1);
    return copied_buffer;

}

void free_string_buffer(char *input_buffer) {
    free(input_buffer);
    }

void clean_newline(char *input_buffer) {
    input_buffer[strcspn(input_buffer, "\n")] = '\0';
}

void lowercase_string(char* input_buffer) {

    for (int char_index = 0; input_buffer[char_index]; char_index++)
    {
        input_buffer[char_index] = tolower(input_buffer[char_index]);
    }

}

void remove_punctuation(char* input_buffer) {

    for (int char_index = 0; input_buffer[char_index]; char_index++) {
        if (ispunct(input_buffer[char_index])) {
            input_buffer[char_index] = ' ';
        }
    }

}

int main(void) {
    char input_buffer[512];
    get_input(input_buffer, sizeof(input_buffer));
    clean_newline(input_buffer);
    lowercase_string(input_buffer);
    remove_punctuation(input_buffer);

    char* copied_buffer = copy_string(input_buffer, strlen(input_buffer));
    printf("Input: %s\n", copied_buffer);
    printf("Hello, World!\n");
    free_string_buffer(copied_buffer);
    return 0;
}
