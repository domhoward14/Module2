#include <stdio.h>
#include <stdlib.h>
#include "ArrayList.c"
#include <string.h>

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
void putSymbolToken(char *string, int symbol);
void putReserveToken(char *string, int symbol);
void putNumToken(char *string);
void putIdentifierToken(char *string);
int symbolSwitch(char *string);
int isNumber(char *string);
int wordSwitch(char *string);
void findToken();




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
    cleanArrayList();
    findToken();
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
    findToken();
    printf("%d, %s", tokenArray[4].sym, tokenArray[4].word );
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

void findToken()
{
    structIndex = 0;
    char * string;
    char bos;
    while(structIndex < codeArray ->size)
    {
            string = get(codeArray, structIndex);
            bos = string[0];
            if(isLetter(bos))
            {
                if(wordSwitch(string))
                {
                    structIndex ++;
                    continue;
                }
                else
                {
                    putIdentifierToken(string);
                    structIndex ++;
                    continue;
                }
            }
            else
            {
                if(isNumber(string))
                {
                    putNumToken(string);
                    structIndex ++;
                    continue;
                }
                else
                    symbolSwitch(string);
                    structIndex ++;
            }
    }
}

int isLetter(char letter)
{
    if(letter >= 65 && letter <= 90)
        return 1;
    else if(letter >= 97 && letter <= 122)
        return 1;
    else
        return 0;
}

int wordSwitch(char *string)
{
    char bos = string[0];
    switch(bos)
    {
        case 'c':
            if(strcmp (string, "const") == 0)
            {
                putReserveToken("const", 28);
                break;
            }
            if(strcmp (string, "call") == 0)
            {
                putReserveToken("call", 27);
                break;
            }
            putIdentifierToken(string);
            break;

        case 'v':
            if(strcmp (string, "var") == 0)
            {
                putReserveToken("var", 29);
                break;
            }
            putIdentifierToken(string);
            break;

        case 'p':
            if(strcmp (string, "procedure") == 0)
            {
                putReserveToken("procedure", 30);
                break;
            }
            putIdentifierToken(string);
            break;

        case 'b':
            if(strcmp (string, "begin") == 0)
            {
                putReserveToken("begin", 21);
                break;
            }
            putIdentifierToken(string);
            break;

        case 'e':
            if(strcmp (string, "end") == 0)
            {
                putReserveToken("end", 22);
                break;
            }

            if(strcmp (string, "else") == 0)
            {
                putReserveToken("else", 33);
                break;
            }
            putIdentifierToken(string);
            break;

        case 'i':
            if(strcmp (string, "if") == 0)
            {
                putReserveToken("if", 23);
                break;
            }
            putIdentifierToken(string);
            break;

        case 't':
            if(strcmp (string, "then") == 0)
            {
                putReserveToken("then", 24);
                break;
            }
            putIdentifierToken(string);
            break;

        case 'w':
            if(strcmp (string, "while") == 0)
            {
                putReserveToken("while", 25);
                break;
            }

            if(strcmp (string, "write") == 0)
            {
                putReserveToken("write", 31);
                break;
            }
            putIdentifierToken(string);
            break;

        case 'd':
            if(strcmp (string, "do") == 0)
            {
                putReserveToken("do", 26);
                break;
            }
            putIdentifierToken(string);
            break;

        case 'r':
            if(strcmp (string, "read") == 0)
            {
                putReserveToken("read", 32);
                break;
            }
            putIdentifierToken(string);
            break;

        case 'o':
            if(strcmp (string, "odd") == 0)
            {
                putReserveToken("odd", 8);
                break;
            }
            putIdentifierToken(string);
            break;

        default :
            putIdentifierToken(string);

    }
}

int isNumber(char *string)
{
    int number = string[0];

    if(number >= 48 && number <= 57)
    {
        putNumToken(string);
        return 1;
    }
    return 0;

}

int symbolSwitch(char *string)
{
    char symbol = string[0];
    switch(symbol)
    {
        case '+':
            putSymbolToken("+", 4);
            break;
        case '-':
            putSymbolToken("-", 5);
            break;
        case '*':
            putSymbolToken("*", 6);
            break;
        case '/':
            putSymbolToken("/", 7);
            break;
        case '=':
            putSymbolToken("=", 9);
            break;
        case '(':
            putSymbolToken("(", 15);
            break;
        case ')':
            putSymbolToken(")", 16);
            break;
        case ',':
            putSymbolToken(",", 17);
            break;
        case '.':
            putSymbolToken(".", 19);
            break;
        case '<':
            putSymbolToken("<", 11);
            break;
        case '>':
            putSymbolToken(">", 13);
            break;
        case ';':
            putSymbolToken(";", 18);
            break;
        case ':':
            putSymbolToken(":=", 9);
            break;
    }
}


void putIdentifierToken(char *string)
{
    tokenArray[structIndex].word = string;
    tokenArray[structIndex].sym = 2;
}

void putNumToken(char *string)
{
    tokenArray[structIndex].word = string;
    tokenArray[structIndex].sym = 3;
}

void putReserveToken(char *string, int symbol)
{
    tokenArray[structIndex].word = string;
    tokenArray[structIndex].sym = symbol;
}

void putSymbolToken(char *string, int symbol)
{
    tokenArray[structIndex].word = string;
    tokenArray[structIndex].sym = symbol;
}
