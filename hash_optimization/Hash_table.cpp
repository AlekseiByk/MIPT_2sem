#include <stdio.h>
//#include <assert.h>
#include <stdlib.h>
#include "Text.h"
#include <string.h>
#include <ctype.h>


typedef char element_t;
typedef int Size_t;
typedef unsigned int hash_t;
const int MAXSIZE = 30;
const int SIZEOFTABLE = 200001;

struct list_elem_t
{
    hash_t hash;
    element_t elem[MAXSIZE] = "";
    list_elem_t* next = NULL;
};

struct hash_table_t
{
    list_elem_t* table;
    int hashSize;
    ///KOTIKI
};

char** Create_words(char *text, int* wNumber);
extern list_elem_t* hashSearch(hash_table_t *hashTable, element_t elem[], hash_t hash);
int HashlistAdd(hash_table_t *hashTable, int number, element_t elem[], hash_t hash);
hash_t HashSolve(element_t elem[]);
void WriteTable(hash_table_t hashTable, int funcNumber);
void FillHashTable(char** input, int length, hash_table_t* table, hash_t (*hashfunc)(element_t elem[]));
void HashTableConstruct(hash_table_t* hash, int number);
void HashTableDestruct(hash_table_t* hash);

extern hash_t HashSolve_Rol(element_t elem[]);

int main() 
{
    int length = 0;
    char* input = TextFromFile("input.txt", &length);
    printf("%d\n", length);

    hash_table_t HashTable = {};
    HashTableConstruct(&HashTable, SIZEOFTABLE);

    int wNumber = 0;
    char** Index = Create_words(input, &wNumber);
    printf("%d\n", wNumber);
    fflush(0);

    for (int i = 0; i < 400; i++)
        FillHashTable(Index, wNumber, &HashTable, HashSolve_Rol);

    WriteTable(HashTable, 0);

    HashTableDestruct(&HashTable);
    free(Index);
    free(input);

    return 0;
}
/*
list_elem_t* hashSearch(hash_table_t *hashTable, element_t elem[], hash_t hash)
{
	list_elem_t* temp = hashTable -> table + (hash % (hashTable -> hashSize));
    while(temp -> next != NULL)
    {
        if (strcmp(elem, temp -> elem) == 0)
            return 0;
        else
            temp = temp -> next;
    }
	return temp;
}
*/
int HashlistAdd(hash_table_t *hashTable, element_t elem[], hash_t hash)
{
	list_elem_t* temp = hashSearch(hashTable, elem, hash);

	if (temp)
	{

		temp -> next = (list_elem_t*) calloc(1, sizeof(list_elem_t));
		strcpy(temp -> next -> elem, elem);
		temp -> next -> hash = hash;
	}

    return 0;
}

void WriteTable(hash_table_t hashTable, int funcNumber)
{
    FILE *f = fopen("results.csv", "a");
    fprintf(f, "%s; ", "hash"); //funcName
    for (int i = 0; i < hashTable.hashSize; i++)
    {
        int k = 0;
        list_elem_t* temp = hashTable.table + i;
        while(temp -> next != NULL)
        {
            //printf("%s ", temp -> elem);
            temp = temp -> next;
            k++;
        }
        fprintf(f, "%d; ", k);
    }
    fprintf(f, "\n");
    fclose(f);
}

void FillHashTable(char** input, int length, hash_table_t* hashTable, hash_t (*hashfunc)(element_t elem[]))
{
    for (int i = 0; i < length; i++)
    {
        hash_t hash = (*hashfunc)(input[i]);
        HashlistAdd(hashTable, input[i], hash);
    }
}

void HashTableConstruct(hash_table_t* hash, int number)
{
    assert(hash);

    hash -> table = (list_elem_t*) calloc(number, sizeof(list_elem_t));
    assert(hash -> table);

    hash -> hashSize = number;
}

void HashTableDestruct(hash_table_t* hash)
{
    assert(hash);
    assert(hash -> table);
    for (int i = 0; i < hash -> hashSize; i++)
    {
        list_elem_t* temp = &((*hash).table[hash -> hashSize - i - 1]);
        list_elem_t* temp1 = {};
        if (temp != nullptr)
        {
            temp = temp -> next;
            while(temp != nullptr)
            {
                temp1 = temp -> next;
                free(temp);
                temp = temp1;
            }
        }
    }   
    free(hash -> table);
    hash -> hashSize = 0;
}
/*
hash_t HashSolve_Rol(element_t elem[])
{
    hash_t hash = 0;
    int i = 0;
    while(true)
        if (elem[i] == 0)
            return hash;
        else
            hash = ((hash << 13) | (hash >> 19)) ^ elem[i++];; 
    return hash;
}
*/

char** Create_words(char *text, int* wNumber){
    assert(text != nullptr);

    char** index = (char**) calloc(500000, sizeof(index[0]));
    assert(index != nullptr);

    index[0] = text;

    int k = 0;
    while ((*text)!= '\0')
    {
        if (!(isalpha(*text) || isdigit(*text)))
        {
            *text = '\0';

            k++;
            index[k] = text + 2;

            text++;
        }
        text++;
    }

  	(*wNumber) = k + 1;

    return index;
}