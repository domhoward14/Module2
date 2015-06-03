#include<stdio.h>
#include<stdlib.h>
char code [1000];
FILE *codeFile;
int takingInput();
int main()
{

    takingInput();
    return 0;
}

int takingInput()
{

    codeFile = fopen("input.txt", "r");
    if(!codeFile)
    {
        printf("Error in opening the file.");
        exit(0);

    }
    int i = 0;
    int x, y;
    while ((x = fgetc(codeFile)) != EOF)
    {
        code[i] = x;
        printf("%c",code[i]);
        i ++;
    }
}
