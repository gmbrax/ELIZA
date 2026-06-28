#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct context {
    char*  words[128];
    int   word_count;
}ElizaContext;


typedef enum {
    TOKEN_LITERAL,
    TOKEN_WILDCARD
} TokenType;

typedef struct {
    TokenType type;
    char*     word;
} PatternToken;

typedef struct {
    char*  word;
    int  weight;
}ElizaKeyword;


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


char* keyword_scanner(ElizaContext* context, ElizaKeyword keywords[],  unsigned int keyword_count){
    char *best_word = NULL;
    int best_word_weight = -1;
    for (int keyword_idx = 0; keyword_idx < keyword_count ; keyword_idx++) {
       for (int token_idx = 0; token_idx < context->word_count; token_idx++) {
           if (strcmp(context->words[token_idx], keywords[keyword_idx].word) == 0  && (best_word_weight < keywords[keyword_idx].weight)){
               best_word = context->words[token_idx];
               best_word_weight = keywords[keyword_idx].weight;

           }
       }
    }
    return best_word;
}
int pattern_match(PatternToken pattern[], int pattern_size, int current_pattern_index, ElizaContext* context, int current_phrase_index) {
    if (current_pattern_index == pattern_size)
        return (current_phrase_index == context->word_count);

    PatternToken current_pattern = pattern[current_pattern_index];

    if (current_pattern.type == TOKEN_LITERAL) {
        if (current_phrase_index < context->word_count
            && strcmp(context->words[current_phrase_index], current_pattern.word) == 0) {
            return pattern_match(pattern, pattern_size, current_pattern_index + 1, context, current_phrase_index + 1);
            }
        return 0;
    }

    if (current_pattern.type == TOKEN_WILDCARD) {
        for (int gobble_index = 0; gobble_index <= (context->word_count - current_phrase_index); gobble_index++) {
            if (pattern_match(pattern, pattern_size, current_pattern_index + 1, context, current_phrase_index + gobble_index))
                return 1;
        }
        return 0;
    }

    return 0;
}

int main(void) {
    ElizaKeyword keywords_array[] = {
        {"i",1},
        {"you",2},
        {"computer",6},
        {"sad",8},
        {"mom",10}

    };

    PatternToken pat_computer_hates[] = {
        {TOKEN_WILDCARD, NULL},
        {TOKEN_LITERAL, "computer"},
        {TOKEN_LITERAL, "hates"},
        {TOKEN_WILDCARD, NULL}
    };

    PatternToken pat_i_am[] = {
        {TOKEN_WILDCARD, NULL},
        {TOKEN_LITERAL, "i"},
        {TOKEN_LITERAL, "am"},
        {TOKEN_WILDCARD, NULL},
    };

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


    size_t keyword_count = sizeof(keywords_array)/sizeof(keywords_array[0]);
    char* found = keyword_scanner(context, keywords_array, keyword_count);
    if (found == NULL) {
        printf("I dont Know\n");
    }else{printf("Achado: %s\n",found);}

    int size = sizeof(pat_i_am)/sizeof(pat_i_am[0]);
    int pattern_match_result = pattern_match(pat_i_am, size, 0, context, 0);
    printf("Matched: %i", pattern_match_result);
    free_string_buffer(copied_buffer);
    free_context(context);
    return 0;
}
