//
//  pagetable.c
//  VM
//
//  Created by Mark Quimpo on 12/9/17.
//  Copyright © 2017 Mark Quimpo. All rights reserved.
//

#include <stdio.h>
#include "vm.h"
#include "disk.h"
#include "pagetable.h"

//Global variables
int frameindex;
int framereference;
PT pagetable;
Invert inverttable[MAX_FRAME];

//Declared Methods
int hittest(int pid, int pagenum, char type);
int pagefaulthandler(int pid, int pagenum, char type, bool *hit);
int memorymanagementunit(int pid, int addr, char type, bool *hit);
void printpagetablestats();
void pagetableinit();


int hittest(int pid, int pagenum, char type)
{
    int framenum;
    if(pagetable.entry[pid][pagenum].valid == false) //If there was a miss
    {
        pagetable.stats.misscount = pagetable.stats.misscount + 1; //Increment miss count
        return -1;
    }
    
    //Increment hit count
    pagetable.stats.hitcount = pagetable.stats.hitcount + 1;
    if(replacementpolicy == 2 || replacementpolicy == 3)
    {
        framereference = pagetable.entry[pid][pagenum].framenum;
        pagereplacement();
    }
    if(type == 'W')
    {
        pagetable.entry[inverttable[pagetable.entry[pid][pagenum].framenum].pid][inverttable[pagetable.entry[pid][pagenum].framenum].page].dirty = true;
    }
    return pagetable.entry[pid][pagenum].framenum;
}

int pagefaulthandler(int pid, int pagenum, char type, bool *hit)
{
    *hit = false; //false because pagefaulthandler is being called
    
    if(frameindex == MAX_FRAME) //Full
    {
    
        int victimframe = pagereplacement();
        int victimPID = inverttable[victimframe].pid;
        int victimpagenum = inverttable[victimframe].page;
        
        //-------Assign new valid and check dirty for victim--------
        //Assign victim valid to falsee
        pagetable.entry[inverttable[victimframe].pid][inverttable[victimframe].page].valid = false;
        //Check victim dirty bit to possible write to disk if dirty is true
        if(pagetable.entry[inverttable[victimframe].pid][inverttable[victimframe].page].dirty == true)
        {
            //Increment write since it's dirty bit
            diskwrite(pagetable.entry[inverttable[victimframe].pid][inverttable[victimframe].page].framenum, victimPID, victimPID);
            //Assign Dirty to old address
            pagetable.entry[inverttable[victimframe].pid][inverttable[victimframe].page].dirty = false;
        }
        
        //-------Assign Frame, Valid, Dirty to Replacement--------
        //Assign replacement framenum
        pagetable.entry[pid][pagenum].framenum = victimframe;
        //Assign Valid
        pagetable.entry[pid][pagenum].valid = true;
        //Adding the pid and pagenum to inverttable struct
        inverttable[pagetable.entry[pid][pagenum].framenum].pid = pid;
        inverttable[pagetable.entry[pid][pagenum].framenum].page = pagenum;
        
        //Assign Dirty (optional)
        if(type == 'W')
        {
            //Increment Write
            diskread(pagetable.entry[pid][pagenum].framenum, pid, pagenum);
            //Set Dirty Bit
            pagetable.entry[pid][pagenum].dirty = true;
        }
        else
        {
            //Increment Reads
            diskread(pagetable.entry[pid][pagenum].framenum, pid, pagenum);
            pagetable.entry[pid][pagenum].dirty = false;
        }
        return pagetable.entry[pid][pagenum].framenum;
    }
    //Else: Enters if PT is not full
    else
    {
        //-------Assign Frame, Valid, Dirty to Entry--------------
        //Assign valid
        pagetable.entry[pid][pagenum].valid = true;
        //Assign frame number
        pagetable.entry[pid][pagenum].framenum = frameindex;
        
        //Adding the pid and pageno to inverttable struct
        inverttable[pagetable.entry[pid][pagenum].framenum].pid = pid;
        inverttable[pagetable.entry[pid][pagenum].framenum].page = pagenum;
        
        //Assign Dirty (optional)
        if(type == 'W')
        {
            //Increment Write
            diskread(pagetable.entry[pid][pagenum].framenum, pid, pagenum);
            //Set Dirty bit
            pagetable.entry[pid][pagenum].dirty = true;
        }
        else
        {
            //Increment Reads
            diskread(pagetable.entry[pid][pagenum].framenum, pid, pagenum);
            pagetable.entry[pid][pagenum].dirty = false;
        }
        
        //Increment Frame Index
        frameindex = frameindex + 1;
        return pagetable.entry[pid][pagenum].framenum;
    }
}

int memorymanagementunit(int pid, int addr, char type, bool *hit)
{
    int framenum;
    int pagenum = (addr >> 8);
    int offset = addr - (pagenum < 8);
    
    if(pagenum > MAX_PAGE)
    {
        printf("Invalid page numer (MAX_PAGE = 0x%x): pid %d, addr %x\n", MAX_PAGE, pid, addr);
        return -1;
    }
    if(pid > MAX_PID)
    {
        printf("Invalid Pid (MAX_PID = %d): pid %d, addr %x\n", MAX_PID, pid, addr);
        return -1;
    }
    
    //Call to hittest
    framenum = hittest(pid, pagenum, type);
    if(framenum > -1)
    {
        *hit = true;
        return (framenum << 8) + offset;
    }
    
    //Call to pagefaulthandler - missed have to put into Physical Memory
    framenum = pagefaulthandler(pid, pagenum, type, hit);
    //Return the address
    return(framenum << 8) + offset;
}

void printpagetablestats()
{
    int req = pagetable.stats.hitcount + pagetable.stats.misscount;
    int hit = pagetable.stats.hitcount;
    int miss = pagetable.stats.misscount;
    
    printf("Request: %d\n", req);
    printf("Hit: %d (%.2f%%)\n", hit, (float) hit * 100 / (float)req);
    printf("Miss: %d (%.2f%%)\n", miss, (float)miss * 100 / (float)req);
}

void pagetableinit()
{
    int i;
    int j;
    
    //Initializing hit and miss count to zero
    pagetable.stats.hitcount = 0;
    pagetable.stats.misscount = 0;
    
    //Initialize the multidimensional array
    for(i = 0; i < MAX_PID; i++)
    {
        for(j = 0; j < MAX_PAGE; j++)
        {
            pagetable.entry[i][j].valid = false;
        }
    }
}
