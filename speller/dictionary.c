// Implements a dictionary's functionality
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

//struct for hash table, to memoize(remember) the checked false words.
typedef struct falsch
{
    char hash;
    struct falsch *alpha;
    struct falsch *beta;
} falsch;

//struct for dictionary words
typedef struct node
{
    char lexi;
    struct node *jump;
    struct node *next;
} node;

// memoization falsch pointers
falsch *base;
falsch *navi = NULL;

// node pointers to the dict: load , unload , size and check functions
node *root = NULL;
node *stalker = NULL;


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    navi = base;
    bool checked = hash_check(word);
    if (checked == true)
    {
        return false;
    }
    stalker = root;
    {
        int j = 0, length = strlen(word);
        while (j <= length)
        {
            if (stalker->lexi != tolower(word[j]) )
            {
                if (stalker->jump != NULL)
                    {
                        stalker = stalker->jump;
                    }
                    else
                    {
                        hash_falsch(word);
                        return false;
                    }
            }
            else if (stalker->lexi == tolower(word[j]))
            {
                if (stalker->next != NULL)
                {
                    stalker = stalker->next;
                    j++;
                }
                else
                {
                    if (tolower(word[j]) == '\0' && stalker->lexi == '\0')
                    {
                        return true;
                    }
                    else
                    {
                        hash_falsch(word);
                        return false;
                    }
                }
            }
        }
    }
    hash_falsch(word);
    return false;    
}

int count=0;
// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    int cur;
    root = malloc(sizeof(struct node));
    base = malloc(sizeof(struct falsch));
    base->alpha = NULL;
    base->beta = NULL;
    //memo = root;
    root->jump = NULL;
    root->next = NULL;
    stalker = root;
    FILE *dict;
    dict = fopen(dictionary, "r");
    if (stalker != NULL)
    {
        while((cur = fgetc(dict)) != EOF)
        {
            stalker = root;
            while(cur != '\n')
            {
                if (stalker->lexi != cur && stalker->jump == NULL)
                {
                    stalker->jump = malloc(sizeof(struct node));
                    stalker = stalker->jump;
                    stalker->lexi = cur;
                    cur = fgetc(dict);
                    for (; cur != '\n'; cur = fgetc(dict))
                    {
                        stalker->next = malloc(sizeof(struct node));
                        stalker = stalker->next;
                        stalker->lexi = cur;
                        stalker->jump = NULL;
                    }
                    count++;
                    stalker->jump = NULL;
                    stalker->next = malloc(sizeof(struct node));
                    stalker = stalker->next;
                    stalker->lexi = '\0';
                    stalker->next = NULL;
                    stalker->jump = NULL;
                }
                else if (stalker->lexi != cur && stalker->jump != NULL )
                {
                    stalker = stalker->jump;
                }
                else
                {
                    cur = fgetc(dict);
                    if (stalker->next != NULL)
                    {
                        stalker = stalker->next;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO

    return count;
}

void free_memo(struct node *tree)
{
    if (tree != 0) 
    {
        free_memo(tree->jump);
        //free(tree->lexi); //if data was heap allocated, need to free it
        free_memo(tree->next);
        free(tree);
    }
}

void free_tree(struct falsch *tree)
{
    if (tree != 0) 
    {
        free_tree(tree->alpha);
        //free(tree->hash); //if data was heap allocated, need to free it
        free_tree(tree->beta);
        free(tree);
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    free_memo(root);
    free_tree(base);
    root = NULL;
    base = NULL;
    if (!root && !base)
    {
        return true;
    }
    return false;
}

// Returns true if the word is already checked as false(misspelled)
bool hash_check(const char *word)
{
    if (navi->alpha != NULL)
    {
        int j = 0, length = strlen(word);
        while (j <= length)
        {
            if (navi->hash != tolower(word[j]) )
            {
                if (navi->alpha != NULL)
                    {
                        navi = navi->alpha;
                    }
                    else
                    {
                        return false;
                    }
            }
            else if (navi->hash == tolower(word[j]))
            {
                if (navi->beta != NULL)
                {
                    navi = navi->beta;
                    j++;
                }
                else
                {
                    if (tolower(word[j]) == '\0' && navi->hash == '\0')
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    }
    return false;
}

void hash_falsch (const char *word)
{
    if (navi != NULL)
    {
        int j = 0, length = strlen(word);
        while (j <= length)
        {
            if (navi->hash != tolower(word[j]) && navi->alpha == NULL)
            {
                navi->alpha = malloc(sizeof(struct falsch));
                navi = navi->alpha;
                navi->hash = tolower(word[j]);
                j++;
                for (; j<=length; j++)
                {
                    navi->beta = malloc(sizeof(struct falsch));
                    navi = navi->beta;
                    navi->hash = tolower(word[j]);
                    navi->alpha = NULL;
                }
                navi->beta = NULL;
                navi->alpha = NULL;
            }
            else if (navi->hash != tolower(word[j]) && navi->alpha != NULL )
            {
                navi = navi->alpha;
            }
            else if (navi->hash == tolower(word[j]))
            {
                j++;
                if (navi->beta != NULL)
                {
                    navi = navi->beta;
                }
            }
        }
    }
}
