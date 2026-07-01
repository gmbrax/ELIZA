#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GROUPS 16

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

typedef  struct {
    PatternToken* pattern;
    int pattern_size;
    char** responses;
    int response_count;
}ElizaRule;

typedef struct {
    char*  word;
    int  weight;
    ElizaRule* rules;
    int rules_count;
}ElizaKeyword;


typedef struct {
    int group_count;
    int group_start[MAX_GROUPS];
    int group_end[MAX_GROUPS];
}MatchResult;

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

char* resp_i_am[] = {
    "Why do you say you are (1)?"
};


ElizaRule rules_i[] = {
    { pat_i_am, 4, resp_i_am, 1 }
};

ElizaKeyword keywords_array[] = {
    {"i", 1, rules_i, 1},
    {"you", 2, NULL, 0},
    {"computer", 6, NULL, 0},
    {"sad", 8, NULL, 0},
    {"mom", 10, NULL, 0}
};




ElizaContext* initialize_context() {
    ElizaContext* context = malloc(sizeof(ElizaContext));
    if (context == NULL) return NULL;
    context->word_count = 0;
    return context;
}

void reset_context(ElizaContext* context) {
    context->word_count = 0;
    }

void free_context(ElizaContext* context) {
    free(context);
}

void add_word(ElizaContext* context, char* word) {
    context->words[context->word_count] = word;
    context->word_count++;
}

int get_input(char *input_buffer, size_t buffer_size) {
    return fgets(input_buffer, buffer_size, stdin) != NULL;
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


ElizaKeyword* keyword_scanner(ElizaContext* context, ElizaKeyword keywords[],  unsigned int keyword_count){
    ElizaKeyword *best_word = NULL;
    int best_word_weight = -1;
    for (int keyword_idx = 0; keyword_idx < keyword_count ; keyword_idx++) {
       for (int token_idx = 0; token_idx < context->word_count; token_idx++) {
           if (strcmp(context->words[token_idx], keywords[keyword_idx].word) == 0  && (best_word_weight < keywords[keyword_idx].weight)){
               best_word = &keywords[keyword_idx];
               best_word_weight = keywords[keyword_idx].weight;

           }
       }
    }
    return best_word;
}
int pattern_match(PatternToken pattern[], int pattern_size, int current_pattern_index, ElizaContext* context,
    int current_phrase_index, MatchResult* match_result, int group_index) {
    if (current_pattern_index == pattern_size)
        return (current_phrase_index == context->word_count);

    PatternToken current_pattern = pattern[current_pattern_index];

    if (current_pattern.type == TOKEN_LITERAL) {
        if (current_phrase_index < context->word_count
            && strcmp(context->words[current_phrase_index], current_pattern.word) == 0) {
            return pattern_match(pattern, pattern_size, current_pattern_index + 1, context, current_phrase_index + 1,match_result, group_index);
            }
        return 0;
    }

    if (current_pattern.type == TOKEN_WILDCARD) {
        for (int gobble_index = 0; gobble_index <= (context->word_count - current_phrase_index); gobble_index++) {
            if (pattern_match(pattern, pattern_size, current_pattern_index + 1, context,
                              current_phrase_index + gobble_index, match_result, group_index + 1)) {
                match_result->group_start[group_index] = current_phrase_index;
                match_result->group_end[group_index]   = current_phrase_index + gobble_index;
                if (group_index + 1 > match_result->group_count)
                    match_result->group_count = group_index + 1;
                return 1;
                              }
        }
        return 0;
    }

    return 0;
}


void response_generator(ElizaContext* context, MatchResult* match_result,
                        PatternToken pattern[], int pattern_size,
                        char* out, size_t out_size) {
    if (pattern_match(pattern, pattern_size, 0, context, 0, match_result, 0)) {
        char group_text[256] = "";
        for (int w = match_result->group_start[1]; w < match_result->group_end[1]; w++) {
            strcat(group_text, context->words[w]);
            strcat(group_text, " ");
        }
        snprintf(out, out_size, "ELIZA:> Why do you say you are %s?\n", group_text);
    } else {
        snprintf(out, out_size, "ELIZA:> Tell me more\n");
    }
}

int ELIZALoop(ElizaContext* context) {

    bool is_running = true;
    char input_buffer[512];
    while (is_running) {
        reset_context(context);
        MatchResult match_result = {0};
        if (!get_input(input_buffer, sizeof(input_buffer))) {
            is_running = false;
        } else {
            clean_newline(input_buffer);                    // ← AQUI, antes de tudo
            if (strcmp(input_buffer, "/quit") == 0) {
                is_running = false;
            } else if (input_buffer[0] == '\0') {           // ← vazio idiomático
                printf("ELIZA:>Tell me more\n");
            }
            else {
                lowercase_string(input_buffer);             // (clean_newline já foi)
                remove_punctuation(input_buffer);
                tokenize(context, input_buffer);

                size_t keyword_count = sizeof(keywords_array)/sizeof(keywords_array[0]);
                ElizaKeyword* found = keyword_scanner(context, keywords_array, keyword_count);
                if (found == NULL) {
                    printf("ELIZA:>Tell me more\n");
                } else {
                    char response[512];
                    response_generator(context, &match_result, pat_i_am,  sizeof(pat_i_am)/sizeof(pat_i_am[0]), response, sizeof(response));
                    printf("%s", response);


                }
            }
        }
    }
    free_context(context);
    return 0;
}

int main(void) {
    ElizaContext* context = initialize_context();
    if (context == NULL) return 1;
    return ELIZALoop(context);
}
