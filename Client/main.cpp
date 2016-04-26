/* 
 * File:   main.cpp
 * Author: bx
 *
 * Created on October 14, 2015, 5:08 PM
 */
#include "paws_client.h"
#include <unistd.h>
#include <iostream>

#define log_report_cli(...) printf("" __VA_ARGS__)

/*
 * Main test functions:
 * Terminal Information acquisition has been coded in the paws_client.cpp file. 
 */
int main()
{
    int status = -1;
    PAWS_client paws_cli;
    printf("before staring\n");
    if(paws_cli.start_paws() != 0)
    {
        log_report_cli("PAWS start error\n");
        return -1;
    }
    
    std::cout << "please choose an operation" << std::endl;
    std::cout << "1. send initialization request" << std::endl;
    std::cout << "2. send registration request" << std::endl;
    std::cout << "3. send spectrum query without notification" << std::endl;
    std::cout << "4. send spectrum query with notification" << std::endl;
    std::cout << "5. perform the whole sequence without notification" << std::endl;
    std::cout << "6. perform the whole sequence with notification" << std::endl;
    int choice = -1;
    std::cin >> choice;
        
    if (choice == 1)
        status = paws_cli.init_request();
    else if (choice == 2)
        status = paws_cli.register_request();
    else if (choice == 3)
    {
        paws_cli.is_enable_notification(false);
        status = paws_cli.avail_spec_query();               
    }
    else if (choice == 4)
    {        
        paws_cli.is_enable_notification(true);
        status = paws_cli.avail_spec_query();               
    }
    else if (choice == 5)
    {   
        paws_cli.is_enable_notification(false);
        if (status = paws_cli.init_request() == 0)
        {
            if(status = paws_cli.register_request() ==0 )
            {
                paws_cli.avail_spec_query();               
            }
        }
    } 
    else if (choice == 6)
    {   
        paws_cli.is_enable_notification(true);
        if (status = paws_cli.init_request() == 0)
        {
            if(status = paws_cli.register_request() ==0 )
            {
                status = paws_cli.avail_spec_query();               
            }
        }
    }                 
    if (status != 0)
        std::cout << "error occurs" << std::endl;       
            
    std::cout << "press any letter to stop" << std::endl;
    char letter;
    std::cin >> letter;
    return 0;    
}