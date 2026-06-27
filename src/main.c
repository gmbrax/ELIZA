#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct context {
    char*  words[128];
    int   word_count;
}ElizaContext;



ElizaContext* initialize_context() {
    ElizaContext* context = malloc(sizeof(ElizaContext));
    if (context == NULL) return NULL;
    context->word_count = 0;
    return context;
}

void free_context(ElizaContext* context) {
    free(context);
}

void add_word(ElizaContext* context, char* word) {
    context->words[context->word_count] = word;
    context->word_count++;
}

void get_input(char *input_buffer, size_t buffer_size) {
    fgets(input_buffer, buffer_size, stdin);
}

char* copy_string(char *input_buffer) {
    char* copied_buffer = strdup(input_buffer);
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

void tokenize( ElizaContext* context, char* input_buffer) {
    char* token = strtok(input_buffer, " ");
    while (token != NULL) {
        if (context->word_count < 128) {
            add_word(context, token);
        }
        token = strtok(NULL, " ");
    }
}


char* keyword_scanner(ElizaContext* context, char* keywords[], int keyword_count){

    for (int keyword_idx = 0; keyword_idx < keyword_count ; keyword_idx++) {
       for (int token_idx = 0; token_idx < context->word_count; token_idx++) {
           if (strcmp(context->words[token_idx], keywords[keyword_idx]) == 0) {
               return context->words[token_idx];
           }
       }
    }
    return NULL;
}

int main(void) {
    char* keywords[] = {"i","you","computer","sad","mom"};
    ElizaContext* context = initialize_context();
    if (context == NULL) return 1;
    char input_buffer[512];
    get_input(input_buffer, sizeof(input_buffer));
    clean_newline(input_buffer);
    lowercase_string(input_buffer);
    remove_punctuation(input_buffer);
    char* copied_buffer = copy_string(input_buffer);
    tokenize(context , input_buffer);

    for (int token_idx = 0; token_idx < context->word_count; token_idx++) {
        printf("%s\n", context->words[token_idx]);
    }


    size_t keyword_count = sizeof(keywords)/sizeof(keywords[0]);
    char* achado = keyword_scanner(context, keywords, keyword_count);
    if (achado == NULL) {
        printf("I dont Know\n");
    }else{printf("Achado: %s\n",achado);}

    free_string_buffer(copied_buffer);
    free_context(context);
    return 0;
}
