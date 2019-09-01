//
//  inputgen.c
//  VM
//
//  Created by Mark Quimpo on 12/9/17.
//  Copyright © 2017 Mark Quimpo. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "vm.h"


//Main fuction - is used to generete the request with number of arguments
//and return 0 upon completion and success.
int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("usage: ./input_gen number_of_request\n");
        return -1;
    }
    
    int req_num = atoi(argv[1]);
    int i;
    int pid;
    int page;
    int offset;
    int addr;
    char type;
    
    //To generate unique random numbers each time ran
    srand(time(NULL));
    
    //Main loop to generate the correct number of request
    for(i = 0; i < req_num; i++)
    {
        pid = rand() % MAX_PID;
        page = rand() % MAX_PAGE;
        offset = rand() % 0xFF;
        
        if(rand() % 2 == 0)
        {
            type = 'R';
        }
        else
        {
            type = 'W';
        }
        
        //Create the hex value for the address
        addr = (page << 8) + offset;
        
        printf("%d %c 0x%x\n", pid, type, addr);
    }
    
    return 0;
}

