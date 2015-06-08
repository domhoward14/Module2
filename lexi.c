#include <stdio.h>
#include <stdlib.h>
#include "ArrayList.c"

typedef struct Token
{
    char *word;
    int sym;
}Token;

void load();
int isSymbol(int x);

FILE *codeFile;
int structIndex = 0;
Token *tokenArray;
ArrayList *codeArray;

void structPut(char *word, int num)
{
    if(!tokenArray)
    {
        printf("Token Array not properly initialized");
    }
    tokenArray[structIndex].word = word;
    tokenArray[structIndex].sym = num;
}

void initArrays()
{
    tokenArray = malloc(sizeof(Token)*10000);
    codeArray = createArrayList(10);
}

int main()
{
    initArrays();

    load();
    return 0;
}

void load()
{
    char buffer[256];
    char symbolBuffer[2];
    int i = 0;
    int x, prev = 0, codeIndex;
    codeFile = fopen("input.txt", "r");
    if(!codeFile)
    {
        printf("Error in opening the file.");
        exit(0);

    }
    while ((x = fgetc(codeFile)) != EOF)
    {
        if(isSymbol(x))
        {
            if(!isSymbol(prev))
                put(codeArray, buffer);
            symbolBuffer[0] = x;
            symbolBuffer[1] = '\0';
            put(codeArray, symbolBuffer);
            i = 0;
        }
        else
        {
            buffer[i] = x;
            buffer[i+1] = '\0';
            i++;
        }
        prev = x;

    }
    printArrayList(codeArray);
}

int isSymbol(int x)
{
    if((x>=32 && x<=47) || (x>=58 && x<=64) || (x>= 91 && x<=96) || (x>=123 && x<=126))
    {
        return 1;
    }
    return 0;
}
