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
void printCleanInput();
int isEnd();
void skipComment();
void printTest();

char buffer[256];
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
    //printTest();
    printCleanInput();
    return 0;
}

//function responsible for retrieving data from the input file and then loading it to the code array for
//code output and further token processing.
void load()
{
    codeCount = 0;
    char symbolBuffer[2];
    int i = 0;
    int x, prev = 0, codeIndex, endSwitch = 0, commentSwitch = 0;
    if(!codeFile)
    {
        printf("Error in opening the file.");
        exit(0);
    }
    while (isEnd() != 1)
    {
        x = fgetc(codeFile);
        //printf("%d", codeCount);
        //printf("this iteration of x is %c \n", x);
        char tempString [10];

        if(isSymbol(x))
        {
            if(x == '*')
            {
                //skip over comment and continue
                skipComment();
                prev = 33;
                continue;
            }

            if(!isSymbol(prev))
            {
                codeCount += 2;
               //printf("\nthe string is %c%c%c\n", buffer[0],buffer[1],buffer[2]);
               put(codeArray, buffer);
              // printf("%s", buffer);
            }

            symbolBuffer[0] = x;
            symbolBuffer[1] = '\0';
            put(codeArray, symbolBuffer);
            endSwitch = isEnd();
            i = 0;
        }
        else
        {
            codeCount ++;
            buffer[i] = x;
            buffer[i+1] = '\0';
            i++;
        }
        prev = x;
        //printf("%d\n", codeCount);

    }
    put(codeArray, "end.");
    //printArrayList(codeArray);
    fclose(codeFile);

}

int isSymbol(int x)
{
    if((x>=0 && x<=47) || (x>=58 && x<=64) || (x>= 91 && x<=96) || (x>=123 && x<=126))
    {
        return 1;
    }
    return 0;
}

void printCleanInput()
{
    int i = 0;
    char* tempString;
   //printf("the count code is %d", codeCount);
    while(i < codeCount)
    {
        int temp;
        tempString = get(codeArray, i);
        temp = tempString[0];
       if((tempString = get(codeArray, i)) == "\n")
            fprintf(output, "\n");
        if(temp != 47)
            fprintf(output, "%s", tempString);
        i ++;
    }
    fclose(output);
}

int isEnd ()
{
    //printf("code cont is %d", codeCount);
    //printf("\n1-> %d. 2-> %d, 3-> %d\n", buffer[0], buffer[1], buffer[2]);
    if (buffer[0] == 'e' && buffer[1] == 'n' && buffer[2] == 'd')
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
}
void printTest()
{

    printf("the index is %d\n", get(codeArray, 20)[0]);
    printf("the index is %d\n", get(codeArray, 20)[1]);
    printf("the index is %d\n", get(codeArray, 20)[2]);
    printf("the index is %s\n", get(codeArray, 20));

}

