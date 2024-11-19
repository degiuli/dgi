/*--
The MIT License (MIT)

Copyright (c) 2012-2024 Fabio Lourencao De Giuli (http://degiuli.github.io)
Copyright (c) 2012-2024 De Giuli Informatica Ltda. (http://www.degiuli.com.br)

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

#include <string>
#include <iostream>
#include <cctype>
#include <random>
#include <chrono>
#include <vector>
#include "dgirndpwdgen.h"

size_t get_random(size_t min, size_t max)
{
	std::random_device rd;
	std::seed_seq seedSeq{ rd(), rd(), rd(), rd(), rd(), rd() };
    std::mt19937 gen{ seedSeq };
    std::uniform_int_distribution<size_t> uid(min, max);
    return uid(gen);
}

std::string MakePassword(size_t size, std::string possibles)
{
    //start password with invalid character
    std::string passwords(size,'\xff');
    for(size_t i=0; i<size; i++)
    {
        //get position
        auto pos = get_random(0, possibles.size());
        passwords[i] = possibles[pos];
        if(i>0)
        {
            //password should not have character repeated
            if ((passwords[i] == passwords[i - 1]) ||
            //password should not have following characters - e.g. i-1 = 0x31, i = 0x32 or i-1 = 0x31, i = 0x33
                (passwords[i] == (passwords[i - 1] - 1)) ||
                (passwords[i] == (passwords[i - 1] - 2)) ||
            //password should not have following characters - e.g. i-1 = 0x32, i = 0x31 or i-1 = 0x33, i = 0x31
                (passwords[i] == (passwords[i - 1] + 1)) ||
                (passwords[i] == (passwords[i - 1] + 2)))
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
    std::cout << "\n" << '\n';
    return passwords;
}

int main(int argc, char* argv[])
{
    size_t size{ 0 };
    std::string type;
    std::string possibles;
    std::string password;

    std::cout << "DGI Random Password Generator\n";
    std::cout << "Version " << dgirndpwdgen_VERSION_MAJOR 
              << "." << dgirndpwdgen_VERSION_MINOR
              << "." << dgirndpwdgen_VERSION_PATCH << '\n';
    std::cout << "-----------------------------\n\n";

    if(argc == 1)
    {
        std::cout << "Password Size: ";
        std::cin >> size;
        std::cout << '\n';
    
        std::cout << "Password Type:\n"
                  << "\tN - Numbers\r\n"
                  << "\tA - Alphabetic\r\n"
                  << "\tX - AlphaNumeric\r\n"
                  << "\tF - Full AlpahNumeric with special characters\r\n"
                  << "\tS - Full AlpahNumeric with special characters (no white space)\n";
    
        std::cin >> type;
        std::cout << '\n';
    }
    else if(argc != 3)
    {
        std::cout << "USAGE: dgirndpwdgen [password size] [password type]\n";
        std::cout << "Password Type:\n"
                  << "\tN - Numbers\r\n"
                  << "\tA - Alphabetic\r\n"
                  << "\tX - AlphaNumeric\r\n"
                  << "\tF - Full AlpahNumeric with special characters\r\n"
                  << "\tS - Full AlpahNumeric with special characters (no white space)\n\n";
        return 1;
    }
    else
    {
        std::cout << "Parameters: [" << argv[1];
        std::cout << "] and [" << argv[2][0] << "]\n";
        
        char *end = nullptr;
        auto size_value = std::strtoul(argv[1], &end, 10);
        size = static_cast<size_t>(size_value);
        
        type = argv[2][0];
        std::cout << "Running using password size " << size;
        std::cout << "and password type " << type << '\n';
    }
        
    if (size > 0)
    {
        bool run{ true };
        switch (std::toupper(type[0]))
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
                    possibles = "abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNOPQRSTUVWXYZ0123456789 ,.;/<>:?\\|\'\"!@#$%�&*()-_=+[]{}~^�`";
                }
                break;
            case 'S':
                {
                    type = "FullNoSpace";
                    possibles = "abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNOPQRSTUVWXYZ0123456789,.;/<>:?\\|\'\"!@#$%�&*()-_=+[]{}~^�`";
                }
                break;
            default:
                {
                    std::cout << "Invalid Type " << type.c_str() << "\n\n";
                    run = false;
                }
                break;
        }
        if(run)
        {
            //generate the code based on the paramenters
            std::chrono::time_point<std::chrono::system_clock> start, end;
            start = std::chrono::system_clock::now();
            
            password = MakePassword(size, possibles);
            
            end = std::chrono::system_clock::now();

            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
            std::cout << "Time elapesed " << (float)(elapsed/1000) << "s\n\n";
        }
    }

    //is there invalid character in the password?
    //if so, error
    auto find_res = password.find('\xff');
    if (find_res == std::string::npos)
    {
        std::cout << "New " << type << " Password " << password << "\n\n";
    }
    else
    {
        std::cout << "Unable to create new acceptable " << type << " password\n\n";
    }

    std::cout << "-----------------------------\n";
    std::cout << "DGI Random Password Generator\n\n";
    return 0;
}
