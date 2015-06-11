#include <stdio.h>
#include <stdlib.h>
#include "ArrayList.c"

typedef struct Token
{
    char *word;
    int sym;
}Token;

void  cleanArrayList();
void load();
int isSymbol(int x);
void printCleanInput();
int isEnd();
void skipComment();
void printTest();
int isLetter(char letter);

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
    //printCleanInput();
    //cleanArrayList();
    printTest();
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
        char tempString [256];

        if(isSymbol(x))
        {
            if(x == '*')
            {
                skipComment();
                prev = 33;
                continue;
            }

            if(!isSymbol(prev))
            {
                codeCount += 2;
               //printf("\nthe string is %c%c%c\n", buffer[0],buffer[1],buffer[2]);
               put(codeArray, buffer);
               //printf("\n%s\n", buffer);
            }

            symbolBuffer[0] = x;
            symbolBuffer[1] = '\0';
            put(codeArray, symbolBuffer);
            //printf("\n%s\n", symbolBuffer);
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
    while(i < codeArray ->size)
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
}

void cleanArrayList()
{
    char* tempString;
    int temp;
    for(int i = 0; i < codeArray ->size; i ++)
    {
        tempString = get(codeArray, i);
        temp = tempString[0];
        if(temp == 0 || temp == 10 || temp == 47 || temp == 9 || temp == 11 || temp == 32)
        {
            removeElement(codeArray,i);
            i --;
        }
    }
}
/*
void findToken()
{
    int i = 0;
    char * string;
    char bos;
    while(i < codeArray ->size)
    {
            string = get(codeArray, i);
            bos = string[0];
            if(isLetter(bos))
            {
                if(wordSwitch())
                {
                    index ++
                    continue;
                }
                else
                {
                    putIdentifierToken();
                    i ++;
                    continue;
                }
            }
            else
            {
                if(isNumber())
                {
                    makeNumToken();
                    i ++;
                    continue;
                }
                else
                    symbolSwitch();
                    i ++;
            }
    }
}
*/
int isLetter(char letter)
{
    if(letter >= 65 && letter <= 90)
        return 1;
    else if(letter >= 97 && letter <= 122)
        return 1;
    else
        return 0;
}
