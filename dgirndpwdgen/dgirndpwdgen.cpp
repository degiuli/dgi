/*++
Copyright (c) De  Giuli Informatica Ltda.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#include <windows.h>
#include <string>
#include <iostream>
#include <cctype>
#include <cstdlib>

/*
** TickCount
*/
unsigned __int64 TickCount()
{
	HMODULE hKernel32 = LoadLibrary("kernel32.dll");
	if (hKernel32 == NULL) 
		return (unsigned __int64)GetTickCount();

    typedef unsigned __int64 (WINAPI *PGetTickCount64)();
    PGetTickCount64 pGetTickCount64 = (PGetTickCount64)GetProcAddress(hKernel32,"GetTickCount64");

	if (pGetTickCount64 == NULL) 
		return (unsigned __int64)GetTickCount();
    else
        return pGetTickCount64();
}

unsigned short get_random(unsigned short min, unsigned short max)
{
    unsigned short pos = max+1;
    while(min>pos || max<pos)
    {
        unsigned __int64 tick = TickCount();
        unsigned char *lpTick = (unsigned char *)&tick;

        //Define the seed
        unsigned int x,seed=0;
        for(x=0;x<sizeof(unsigned __int64 );x++)
        {
            seed += (unsigned __int64)lpTick[x];
        }
        srand(seed);

        Sleep(1L);

        unsigned short a = (unsigned short)lpTick[0]+(unsigned short)lpTick[7];
        unsigned short b = (unsigned short)lpTick[1]+(unsigned short)lpTick[6];
        unsigned short c = (unsigned short)lpTick[2]+(unsigned short)lpTick[5];
        unsigned short d = (unsigned short)lpTick[3]+(unsigned short)lpTick[4];
        pos -= (unsigned short)((d << 4) + (c >> 4) + (b << 4) + (a >> 4) - rand());
    }
    return pos;
}

std::string MakePassword(unsigned short size,std::string possibles)
{
    if(IsDebuggerPresent())
    {
        DebugBreak();
    }

    //start password with invalid character
    std::string passwords(size,'\xff');
    for(unsigned short i=0; i<size; i++)
    {
        //get position
        unsigned short pos = get_random(0, (unsigned short)possibles.size());
        passwords[i] = possibles[pos];
        if(i>0)
        {
            //password should not have character repeated
            if((passwords[i]==passwords[i-1]) ||
            //password should not have following characters - e.g. i-1 = 0x31, i = 0x32 or i-1 = 0x31, i = 0x33
               (passwords[i]==(passwords[i-1]-1)) ||
               (passwords[i]==(passwords[i-1]-2)) ||
            //password should not have following characters - e.g. i-1 = 0x32, i = 0x31 or i-1 = 0x33, i = 0x31
               (passwords[i]==(passwords[i-1]+1)) ||
               (passwords[i]==(passwords[i-1]+2)))
            {
                std::cout << "~";
                passwords[i] = '\xff';
                i--;
            }
            else
            {
                std::cout << "*";
            }
        }
        else
        {
            std::cout << "*";
        }
    }
    std::cout << "\n" << std::endl;
    return passwords;
}

int __cdecl main(void)
{
    unsigned __int64 start = 0, end = 0;

    unsigned short size = 0;
    std::string type;
    std::string possibles;
    std::string password;

    std::cout << "DGI Random Password Generator" << std::endl;
    std::cout << "-----------------------------\n" << std::endl;

    std::cout << "Password Size: ";
    std::cin >> size;
    std::cout << std::endl;

    std::cout << "Password Type:\n"
              << "\tN - Numbers\r\n"
              << "\tA - Alphabetic\r\n"
              << "\tX - AlphaNumeric\r\n"
              << "\tF - Full AlpahNumeric with special characters" << std::endl;
    std::cin >> type;
    std::cout << std::endl;
    
    if(size>0)
    {
        switch(std::toupper(type[0]))
        {
            case 'N':
                {
                    type = "Numeric";
                    possibles = "0123456789";

                    start = TickCount();
                    password = MakePassword(size,possibles);
                    end = TickCount();
                }
                break;
            case 'A':
                {
                    type = "Alphabetic";

                    possibles = "abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNOPQRSTUVWXYZ";

                    start = TickCount();
                    password = MakePassword(size,possibles);
                    end = TickCount();
                }
                break;
            case 'X':
                {
                    type = "AlphaNumeric";

                    possibles = "abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNOPQRSTUVWXYZ0123456789";

                    start = TickCount();
                    password = MakePassword(size,possibles);
                    end = TickCount();
                }
                break;
            case 'F':
                {
                    type = "Full";

                    possibles = "abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNOPQRSTUVWXYZ0123456789 ,.;/<>:?\\|\'\"!@#$%¨&*()-_=+[]{}~^´`";

                    start = TickCount();
                    password = MakePassword(size,possibles);
                    end = TickCount();
                }
                break;
            default:
                {
                    std::cout << "Invalid Type " << type.c_str() << "\n" << std::endl;
                }
                break;
        }
    }

    auto elapsed = (end-start);
    std::cout << "Time elapesed " << (float)(elapsed/1000) << "s\n" << std::endl;

    //is there invalid character in the password?
    //if so, error
    auto find_res = password.find('\xff');
    if(find_res==-1)
    {
        std::cout << "New " << type << " Password " << password << "\n" << std::endl;
    }
    else
    {
        //std::cout << "Unable to create acceptable new " << type.c_str() << " password\n" << std::endl;
        std::cout << "Unable to create acceptable new " << type.c_str() << " password\n" << std::endl;
    }

    std::cout << "-----------------------------" << std::endl;
    std::cout << "DGI Random Password Generator\n" << std::endl;
    return 0;
}
