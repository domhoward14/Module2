#include <stdio.h>
#include <stdlib.h>
#include "ArrayList.c"
#include <string.h>

typedef struct Token
{
    char *word;
    int sym;
}Token;

void errorCheck();
void printLexemeList();
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
void printLexemeTable();
int isNextGts();
int isNextEqual();





char buffer[256];
FILE *codeFile;
FILE *output;
FILE *lexemeTable;
FILE *lexemeList;
int structIndex = 0;
int getter;
int codeCount = 0;
Token *tokenArray;
ArrayList *codeArray;
int tokenArrayCount = 0;

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
    printCleanInput();
    cleanArrayList();
    findToken();
    //errorCheck();
    printLexemeTable();
    printLexemeList();
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
            if(x == '*' && prev == '/')
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
    put(codeArray, "end");
    put(codeArray, ".");
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
    int i = 0;
   // while(i < tokenArrayCount)
   // {
    //    printf("%s\t", tokenArray[i].word);
    //    printf("%d\n", tokenArray[i].sym);
   //     i ++;
    //}

   //printf("The code account is %d", codeCount);
   //printArrayList(codeArray);



   // printf("%d\n", tokenArrayCount);
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
    getter = 0;
    char *string;
    char bos;
    while(getter < codeArray ->size)
    {
            string = get(codeArray, getter);
            bos = string[0];
            if(isLetter(bos))
            {
                if(wordSwitch(string))
                {
                    getter ++;
                    continue;
                }
                else
                {
                    putIdentifierToken(string);
                    getter ++;
                    continue;
                }
            }
            else
            {
                if(isNumber(string))
                {
                    getter ++;
                    continue;
                }
                else
                    symbolSwitch(string);
                    getter ++;
            }
    }
    int i = 0;
    while(i < tokenArrayCount)
    {
        printf("%s\t", tokenArray[i].word);
        printf("%d\n", tokenArray[i].sym);
        i ++;
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
            if(isNextEqual())
            {
                putSymbolToken("<=", 12);
                getter ++;
            }
            else if(isNextGts())
            {
                putSymbolToken("<>", 10);
                getter ++;
            }
            else
                putSymbolToken("<", 11);
            break;
        case '>':
            if(isNextEqual())
            {
                putSymbolToken(">=", 13);
                getter ++;
            }
            else
                putSymbolToken(">", 13);
            break;
        case ';':
            putSymbolToken(";", 18);
            break;
        case ':':
            putSymbolToken(":=", 20);
            getter ++;
            break;
        default:
            printf("Invalid symbol.");
    }
}


void putIdentifierToken(char *string)
{
    tokenArray[structIndex].word = string;
    tokenArray[structIndex].sym = 2;
    structIndex ++;
    tokenArrayCount ++;
    printf("The count for the tokenArray is %d\n", tokenArrayCount);
}

void putNumToken(char *string)
{
    tokenArray[structIndex].word = string;
    tokenArray[structIndex].sym = 3;
    structIndex ++;
    tokenArrayCount ++;
    printf("The count for the tokenArray is %d\n", tokenArrayCount);
}

void putReserveToken(char *string, int symbol)
{
    tokenArray[structIndex].word = string;
    tokenArray[structIndex].sym = symbol;
    structIndex ++;
    tokenArrayCount ++;
    printf("The count for the tokenArray is %d\n", tokenArrayCount);
}

void putSymbolToken(char *string, int symbol)
{
    tokenArray[structIndex].word = string;
    tokenArray[structIndex].sym = symbol;
    structIndex ++;
    tokenArrayCount ++;
    printf("The count for the tokenArray is %d\n", tokenArrayCount);
}


void printLexemeTable()
{
    lexemeTable = fopen("lexemetable.txt", "w");
    fprintf(lexemeTable, "lexeme\t");
    fprintf(lexemeTable,"token type\n");
    int i = 0;
    while(i < tokenArrayCount)
    {
        fprintf(lexemeTable, "%s\t", tokenArray[i].word);
        fprintf(lexemeTable, "%d\n", tokenArray[i].sym);
        i ++;
    }
    fclose(lexemeTable);
}

void printLexemeList()
{
    int i = 0;
    lexemeList = fopen("lexemelist.txt", "w");
    while(i < tokenArrayCount)
    {
        fprintf(lexemeList, "%d ", tokenArray[i].sym);
        if(tokenArray[i].sym == 3 || tokenArray[i].sym == 2)
            fprintf(lexemeList, "%s ", tokenArray[i].word);
        i ++;
    }
    fclose(lexemeList);
}


int isNextEqual()
{
    char *string = get(codeArray, structIndex + 1);
    if(string[0] == '=')
        return 1;
    return 0;
}

int isNextGts()
{
    char *string = get(codeArray, structIndex + 1);
    if(string[0] == '>')
        return 1;
    return 0;
}

void errorCheck()
{
    int i = 0;
    int limit = tokenArrayCount;
    char *word;
    char letter;
    while(i < limit)
    {

        word = tokenArray[i].word;
        letter = word[1];
        if(isNumber(word) && isLetter(letter))
        {
            printf("invalid variable. Can not start with a number.");
            exit(1);
        }
        i ++;
    }

}
