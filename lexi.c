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
void prinCleanInput();
int isEnd(char string []);
void skipComment();

FILE *codeFile;
FILE *output;
int structIndex = 0;
int codeCount = 0;
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
    codeFile = fopen("input.txt", "r");
    output = fopen("cleaninput.txt", "w");
    initArrays();
    load();
    prinCleanInput();
    return 0;
}

//function responsible for retrieving data from the input file and then loading it to the code array for
//code output and further token processing.
void load()
{
    char buffer[256];
    char symbolBuffer[2];
    int i = 0;
    int x, prev = 0, codeIndex;
    if(!codeFile)
    {
        printf("Error in opening the file.");
        exit(0);

    }
    while ((x = fgetc(codeFile)) != EOF)
    {
        //printf("this iteration of x is %c \n", x);
        char tempString [10];

        if(isSymbol(x))
        {
            if(x == '*' && prev == '/')
            {
                //skip over comment and continue
                skipComment();
                prev = 0;
                continue;
            }

            //if(!isSymbol(prev))
                put(codeArray, buffer);


            symbolBuffer[0] = x;
            symbolBuffer[1] = '\0';
            put(codeArray, symbolBuffer);
            if(isEnd(buffer))
                break;
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
    fclose(codeFile);

}

int isSymbol(int x)
{
    if((x>=32 && x<=47) || (x>=58 && x<=64) || (x>= 91 && x<=96) || (x>=123 && x<=126))
    {
        return 1;
    }
    return 0;
}

void prinCleanInput()
{
    int i = 0;
    char* tempString;
    while(i <= codeCount)
    {
       if((tempString = get(codeArray, i)) == "\n")
            fprintf(output, "\n");
        fprintf(output, "%s", tempString);
        i ++;
    }
    fclose(output);
}

int isEnd (char string [])
{
    if (string[0] == 'e' && string[1] == 'n' && string[2] == 'd')
        return 1;
    return 0;
}

void skipComment()
{
        int previousPointer = 0, currentPointer = fgetc(codeFile);
        while (previousPointer != '*' && currentPointer != '/')
        {
            previousPointer = currentPointer;
            currentPointer = fgetc(codeFile);
        }
       // fseek(output, -10, SEEK_CUR);
}
