#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#define JSMN_PARENT_LINKS
#include "jsmn.h"


static bool jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0)
    {
        return true;
    }
    return false;
}
static int jsonint (const char *json, jsmntok_t *tok)
{
    return strtol(json + tok->start, NULL, 10);
}

static bool jsonbool(const char *json, jsmntok_t *tok)
{
    return ((tok->type == JSMN_PRIMITIVE) && (strncmp(json + tok->start, "true", tok->end - tok->start) == 0));
}

const char *readfile(const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        //FIXME: debug output
        fprintf(stderr, "Expected file \"%s\" not found", path);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    fseek(file, 0, SEEK_SET);
    //ACHTUNG -- memory allocation. Approach with care
    char *buffer = malloc(len + 1);
    if (buffer == NULL)
    {
        //FIXME: debug output
        fprintf(stderr, "Unable to allocate memory for file");
        fclose(file);
        return NULL;
    }
    fread(buffer, 1, len, file);
    buffer[len] = '\0';

    return (const char *)buffer;
}

const char *extractToken(const char *json_string, jsmntok_t *token)
{
    char *result = calloc(token->size+1, 1);
    strncpy(result, json_string+token->start, token->size);
    return result;
}

int main (void)
{
    const char *json_string = readfile("./building_data.json");
    jsmn_parser parser;
    jsmntok_t *tokens;
    if (json_string == NULL)
    {
        return -1;
    }
    jsmn_init(&parser);

    uint32_t token_count = jsmn_parse(&parser, json_string, strlen(json_string), NULL, strlen(json_string));
    jsmn_init(&parser);
    tokens = calloc(token_count, sizeof(jsmntok_t));
    int parse_result = jsmn_parse(&parser, json_string, strlen(json_string), tokens, token_count);
    if (jsoneq(json_string, &tokens[1], "buildings"))
    {
        for (int i = 1; i < token_count; i++)
        {
            char *next_tok = json_string + tokens[i].start;
            if (tokens[i].parent == 2)
            {
                for (int j = i; j < token_count; j++)
                {
                    if (tokens[j].parent == i)
                    {
                        if (jsoneq(json_string, &tokens[j], "id"))
                        {
                            printf ("Building ID %d \n",
                                    jsonint(json_string, &tokens[j+1]));
                        }
                        if (jsoneq(json_string, &tokens[j], "building_required"))
                        {
                            printf ("Building Required %d \n",
                                    jsonint(json_string, &tokens[j+1]));
                        }
                        if (jsoneq(json_string, &tokens[j], "slot_count"))
                        {
                            printf ("Slot count %d \n",
                                    jsonbool(json_string, &tokens[j+1]));
                        }
                        if (jsoneq(json_string, &tokens[j], "can_enter"))
                        {
                            printf ("Can Enter %d \n",
                                    jsonbool(json_string, &tokens[j+1]));
                        }
                        if (jsoneq(json_string, &tokens[j], "center_x"))
                        {
                            printf ("Center X %d \n",
                                    jsonint(json_string, &tokens[j+1]));
                        }
                        if (jsoneq(json_string, &tokens[j], "center_y"))
                        {
                            printf ("Center Y %d \n",
                                    jsonint(json_string, &tokens[j+1]));
                        }
                        if (jsoneq(json_string, &tokens[j], "description_id"))
                        {
                            printf ("Description ID %d \n",
                                    jsonint(json_string, &tokens[j+1]));
                        }
                        if (jsoneq(json_string, &tokens[j], "flags"))
                        {
                            printf ("Flags %d \n",
                                    jsonint(json_string, &tokens[j+1]));
                        }
                        if (jsoneq(json_string, &tokens[j], "health"))
                        {
                            printf ("HP %d \n",
                                    jsonint(json_string, &tokens[j+1]));
                        }
                        if (jsoneq(json_string, &tokens[j], "name_id"))
                        {
                            printf ("Name ID %d \n",
                                    jsonint(json_string, &tokens[j+1]));
                        }
                        if (jsoneq(json_string, &tokens[j], "race"))
                        {
                            printf ("Race %d \n",
                                    jsonint(json_string, &tokens[j+1]));
                        }
                    }
                }
            }
        }
    }
    free(json_string);
    free(tokens);
    return 0;
}
