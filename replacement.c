//
//  replacement.c
//  VM
//
//  Created by Mark Quimpo on 12/9/17.
//  Copyright © 2017 Mark Quimpo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "vm.h"
#include "math.h"
#include "disk.h"
#include "pagetable.h"

//Global variables
bool keepgoing = true;
int indexforframe = -1;
int lowestfreq = 1;
int highestfreq = 1;
float tempoldframe;
float tempnewframe;

//Installations
void delleastused();
void getlowestfreq();
float freqincrement(float value);

//For LinkedList data structure
struct Node
{
    float recentlyused;
    struct Node *linkedlist;
};

struct Node *head = NULL;
struct Node *tail = NULL;

//When called this will set the lowestFrequecy among the items in the list
void getlowestfreq()
{
    //varibles
    bool boolflag = true;
    struct Node *temp = head;
    
    int roundedtemp = (int)temp->recentlyused;
    
    if(roundedtemp != 1)//Enter if roundTemp isn't 1
    {
        lowestfreq = roundedtemp;
    }
    
    //Will loop through the entire list of items until the end(null)
    while(temp != NULL)
    {
        int roundedtemp = (int)temp->recentlyused;
        if(roundedtemp == 1)
        {
            lowestfreq = 1;
            boolflag = false;
        }
        if(roundedtemp < lowestfreq && boolflag != false)
        {
            lowestfreq = roundedtemp;
        }
        temp = temp->linkedlist;
    }
    if(boolflag == false) //Enters if there was a 1 present
    {
        //Overkill but that's okay
        lowestfreq = 1;
    }
}

//When called this method will delete an item within the list. It takes
//it takes in the item that should be deleted.
int deleteanynode(float usedrecently)
{
    int placement = -1;
    
    struct Node *currentlyused =
    (struct Node*)malloc(sizeof(struct Node));
    struct Node *temp =
    (struct Node*)malloc(sizeof(struct Node));
    
    currentlyused = head;
    temp = NULL;
    if(head == tail)
    {
        head = tail = NULL;
    }
    //Nothing in list
    if((tail)->recentlyused == usedrecently)
    {
        return -1;  //return error
    }
    //The first item to be inserted (least recently added)
    if((head)->recentlyused == usedrecently)
    {
        //Call delete leaset used method since it deals with least recently added
        delleastused();
        return 0; //return error
    }
    
    while(currentlyused->linkedlist->recentlyused != usedrecently)
    {
        if(currentlyused->linkedlist == NULL)
        {
            return -1;
        }
        currentlyused = currentlyused->linkedlist;
    }
    temp = currentlyused->linkedlist;
    placement = temp->recentlyused;
    currentlyused->linkedlist = temp->linkedlist;
    free(temp);
    return placement;
}

//When called this method will delete the item at the tail of the list
//Least Recently Used
void delleastused()
{
    struct Node* temp = head;
    if(head == NULL)
    {
        printf("Queue is Empty\n");
    }
    else
    {
        if(head == tail)
        {
            head = tail = NULL;
        }
        else
        {
            framereference = head->recentlyused;
            head = head->linkedlist;
        }
        free(temp);
    }
}

//This method will insert a new item to the front of the list.
void insertnode(float x)
{
    struct Node *temp = (struct Node*)malloc(sizeof(struct Node));
    
    temp->recentlyused = x;
    temp->linkedlist = NULL;
    
    if(head == NULL && tail == NULL)
    {
        head = tail = temp;
    }
    else
    {
        tail->linkedlist = temp;
        tail = temp;
    }
}

//When called this method will look at least frequency used and determibe if that is the
//item that should be replaced.
int lookaleastfreqused()
{
    //variables
    float getleastused = 0;
    int counting = 0;
    
    //Will continue to loop
    while(keepgoing)
    {
        //Enters if there hasn't been a hit yet
        if(highestfreq == 1)
        {
            //Sets the least used item in list
            tempoldframe = head->recentlyused;
            //So we loop can exit & hit the right next if statement
            keepgoing = false;
        }
        
        //Enters when highest priority is least used item
        if(keepgoing == true)
        {
            //Sets the least used item in list
            tempoldframe = head->recentlyused;
            //Deletes least used item in list
            delleastused();
            //Inserts the least used item in list again but at the tail
            insertnode(tempoldframe);
        }
        //Enters when least used has also lowest priority
        else
        {
            //Setting pulled tempoldframe to the least frequently used item
            tempoldframe = head->recentlyused;
            
            //Osolates just the frequency to use to substract it later
            int intversionleastused = (int)tempoldframe;
            
            //gets the .0 part of the float
            float frametemp = tempoldframe - intversionleastused;
            //gets the frame by times 10 by 0.? spot
            int returnthisframe = 10 *frametemp;
            
            //Delete it from list
            delleastused();
            //Add it to the tail of list
            insertnode(1 + frametemp);
            return returnthisframe;
        }
    }
    return 0;
}

//This method will generate a random number in betweeon 0 and MAX_FRAME.
int randompagereplacement()
{
    int next = rand() % MAX_FRAME;
    return next;
}

//If physical memory is full, call this to find a victim page.
int pagereplacement()
{
    int frame = 0;
    
    if(replacementpolicy == RANDOM )
    {
        frame = randompagereplacement();
    }
    return frame;
}

















