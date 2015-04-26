/*--
The MIT License (MIT)

Copyright (c) 2012-2013 De Giuli Informática Ltda. (http://www.degiuli.com.br)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
--*/

#include <windows.h>
#include <string>
#include <iostream>
#include <cctype>
#include <random>
#include <chrono>

unsigned short get_random(size_t min, size_t max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> uid(min, max);
    return uid(gen);
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
        unsigned short pos = get_random(0, possibles.size());
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
    std::chrono::time_point<std::chrono::system_clock> start, end;

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
              << "\tF - Full AlpahNumeric with special characters\r\n"
              << "\tS - Full AlpahNumeric with special characters (no white space)" << std::endl;
    std::cin >> type;
    std::cout << std::endl;
    
    if(size>0)
    {
        bool run = true;
        switch(std::toupper(type[0]))
        {
            case 'N':
                {
                    type = "Numeric";
                    possibles = "0123456789";
                }
                break;
            case 'A':
                {
                    type = "Alphabetic";
                    possibles = "abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNOPQRSTUVWXYZ";
                }
                break;
            case 'X':
                {
                    type = "AlphaNumeric";
                    possibles = "abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNOPQRSTUVWXYZ0123456789";
                }
                break;
            case 'F':
                {
                    type = "Full";
                    possibles = "abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNOPQRSTUVWXYZ0123456789 ,.;/<>:?\\|\'\"!@#$%¨&*()-_=+[]{}~^´`";
                }
                break;
            case 'S':
                {
                    type = "FullNoSpace";
                    possibles = "abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNOPQRSTUVWXYZ0123456789,.;/<>:?\\|\'\"!@#$%¨&*()-_=+[]{}~^´`";
                }
                break;
            default:
                {
                    std::cout << "Invalid Type " << type.c_str() << "\n" << std::endl;
                    run = false;
                }
                break;
        }
        if(run)
        {
            //generate the code based on the paramenters
            start = std::chrono::system_clock::now();
            password = MakePassword(size,possibles);
            end = std::chrono::system_clock::now();
        }
    }

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    std::cout << "Time elapesed " << (float)(elapsed/1000) << "s\n" << std::endl;

    //is there invalid character in the password?
    //if so, error
    auto find_res = password.find('\xff');
    if(find_res==std::string::npos)
    {
        std::cout << "New " << type << " Password " << password << "\n" << std::endl;
    }
    else
    {
        std::cout << "Unable to create acceptable new " << type << " password\n" << std::endl;
    }

    std::cout << "-----------------------------" << std::endl;
    std::cout << "DGI Random Password Generator\n" << std::endl;
    return 0;
}
