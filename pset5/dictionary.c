/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include "dictionary.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>

char temp[LENGTH + 1];

typedef struct node{
    
   
    char value[LENGTH + 1];

    struct node *next;
    

    
} node;




void add_to_hash_table(char* name);

int hash_func(char* name);

node *first = NULL;

node* hashtable[25];



/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
   
    int temp_index = hash_func( (char*) word);
    
    node *temp = hashtable[temp_index];
    
    while (temp != NULL)
    {   
        if (strcasecmp(word, temp->value) == 0)
            return true;
        temp = temp->next;
    
    } 
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    
    FILE* dict = fopen(dictionary, "r");
    
    if (dict == NULL)
    {   
        return false;
    }
    
    int index = 0;
    
    char word[LENGTH + 1];
    
    int c = fgetc(dict);
    
    while(c != EOF)
    {
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            word[index] = c;
            index++;
            
        }
        c = fgetc(dict); 
        
        if (c == '\n' )
        {   
            if (isalpha(word[0]))
            {    
                word[index] = '\0';
            }
            if (word[0] == '\0')
                break;
            index = 0;
            
            strncpy(temp, word, LENGTH + 1);
            
            add_to_hash_table(temp);
           
            
        }
        
    }
    fclose(dict);
    return true;
    
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    
    unsigned int counter = 0;
    
    for (int a = 0; a < 26; a++)
    {
        node *tempr = hashtable[a];
        
        while (tempr != NULL)
        {
            counter += 1;
            
            tempr = tempr->next;
        }
    }
    
    return counter;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    for (int a = 0; a < 26; a++)
    {
        node *temprr = hashtable[a];
        
        while (temprr != NULL)
        {
            hashtable[a] = hashtable[a]->next;
            
            free(temprr);
            
            temprr = hashtable[a];
        }
    }
    

    return true;
}


/**
 * Return an hash number from 0 to 25 for hash table.
 */


int hash_func(char* name)
{
    
    char first = tolower(name[0]);
    
   
    return (int) first - 97;
    
}




/**
 * Add word from a dictionary to a hash table.
 */
 
 
void add_to_hash_table(char* name)
{
    
    node *newptr = malloc(sizeof(node));
    
    if (newptr == NULL)
    {
        return;
    }
    
    strncpy(newptr->value, name, LENGTH + 1);
    
    newptr->next = NULL;
    
    int index = hash_func(name);
    
    
    if (hashtable[index] == NULL)
    {   
        
        first = newptr;
        
        hashtable[index] = first;
        
    }   
    
    
    else
    {   

        if (hash_func(first->value) == hash_func(name))
        {
            first->next = newptr;
        
            first = newptr;
        }    
        
        else
        {
            first = hashtable[index];
            while (first->next != NULL)
            {
                first = first->next;
            }
            
            first->next = newptr;
            first = newptr;
        }
        
        
    }
    
    
   return;
} 