// ConsoleEXE.cpp : Defines the entry point for the console 
// application.
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
const int ciSize = 5;

typedef struct SmallStruct
{
    short shShort1;
    short shShort2;
} SmallStruct;

typedef struct BigStruct
{
    long lLong1;
    long lLong2;
} BigStruct;

void ArrayOverrunCrash(void);
void OversizedCastCrash(void);
void PrematureDeleteCrash(void);

int main(int argc, char* argv[])
{
    bool bExit = false;
    char sInput[ciSize];
    //========
    printf("Welcome to Crash Test Dummy.\n");
    do
    {
        printf("\nEnter 'a' for array overrun.\n");
        printf("Enter 'c' for oversized cast.\n");
        printf("Enter 'd' for premature delete.\n");
        printf("Enter 'x' to exit.>>");
        scanf("%s", sInput);
        switch (sInput[0])
        {
        case 'a':
        case 'A':
            ArrayOverrunCrash();
            break;
        case 'c':
        case 'C':
            OversizedCastCrash();
            break;
        case 'd':
        case 'D':
            PrematureDeleteCrash();
            break;
        case 'x':
        case 'X':
            bExit = true;
            break;
        default:;
            // Do nothing
        }
    }
    while (! bExit); 
    return 0;
}

void ArrayOverrunCrash(void)
{
    char *sASCII = new char[ciSize];
for (int i = 0; i <= ciSize; i++)  
{
    // sASCII[ciSize] is outside array. 
    sASCII[i] = (char)rand();
}
    delete [] sASCII;
}
