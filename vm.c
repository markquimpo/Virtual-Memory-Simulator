//
//  vm.c
//  VM
//
//  Created by Mark Quimpo on 12/9/17.
//  Copyright © 2017 Mark Quimpo. All rights reserved.
//
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "vm.h"
#include "disk.h"

int replacementpolicy;

int main(int argc, char **argv)
{
    bool hit;
    int pid;
    int addr;
    char type;
    char input[256];;
    //Ensure random number generated each run
    srand(time(NULL));
    
    if(argc < 2)
    {
        fprintf(stderr, "usage: ./vm [for page replacement policy (0)Random\n");
        return -1;
    }
    
    //Replacement policy for RANDOM
    replacementpolicy = atoi(argv[1]);
    printf("Replacement Policy: %d - ", replacementpolicy);
    if(replacementpolicy == RANDOM)
    {
        printf("RANDOM\n");
    }
    else
    {
        printf("ERROR: Unknown!\n");
        return -1;
    }
    
    pagetableinit();
    
    //Main Loop
    while(fgets(input, 256, stdin))
    {
        if(sscanf(input, "%d %c 0x%x", &pid, &type, &addr) < 3)
        {
            printf("Error: Invalid format\n");
            return 0;
        }
        int physicaladdress = memorymanagementunit(pid, addr, type, &hit);
        if(hit)
        {
            printf("Hit: [%d] %c 0x%x -> 0x%x\n", pid, type, addr, physicaladdress);
        }
        else
        {
            printf("Miss: [%d] %c 0x%x -> 0x%x\n", pid, type, addr, physicaladdress);
        }
    }
    
    printf("\n\n\n");
    printpagetablestats();
    printdiskstats();

}
