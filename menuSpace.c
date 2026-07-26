/* 
 * File:   menuSpace.c
 * Author: Pedro Cunha 
 *
 * Created on 20 de dezembro de 2024, 13:49
 * 
 * @brief In this file we will declare the functions of the spaces.
 * This file contains the functions developed by spaces, such as CRUD and reports.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "menus.h"
#include "menuSpace.h"
#include "menuEquipment.h"
#include "menuBooking.h"


#define SPACE_FILE "space.bin"

void saveSpace(geralSpaces *spaces){
    FILE* fp = fopen(SPACE_FILE,"wb");
    if (!fp){
        perror("ERROR OPENING FILE");
        exit (EXIT_FAILURE);
    }
    
        if (spaces->counter==0){
        puts(NOT_SAVED_SPACE);
        return;
    }
    
    //WRITE SPACES COUNTER
    fwrite(&spaces->counter,sizeof(int),1,fp);
    
    //WRITE SPACES LIST
    fwrite(spaces->spaceList,sizeof(Space),spaces->counter,fp);
    
    fclose(fp);
    puts(SAVED_SPACE);    
        
}

void loadSpace(geralSpaces *spaces){
    //OPENING FILE
    FILE* fp = fopen(SPACE_FILE,"rb");
    
    if (!fp) {
        puts(NOT_LOADED_SPACE);
        turnOnSpaces(spaces);
        return;
    }
    
    if(fread(&spaces->counter,sizeof(int),1,fp)!=1){
        spaces->counter = 0;
    }
   
    spaces->capacity= spaces->counter > 0 ? spaces->counter : 10; //IF SPACE COUNTER > 0, SPACES COUNTER=SPACES CAPACITY. ELSE SPACE COUNTER = 10
    spaces->spaceList =malloc(spaces->capacity * sizeof(Space)); 
    
    if (!spaces->spaceList) {
        perror(ALLOC_MEMORY_LOAD);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

      if (fread(spaces->spaceList, sizeof(Space), spaces->counter, fp) != (size_t)spaces->counter) {
        printf("ERROR READING SPACE LIST!!");
        free(spaces->spaceList);
        turnOnSpaces(spaces);
    }

    
    fclose(fp);
    puts(LOADED_SPACE);
}


//FUNCTION TO CONVERT THE NUMBER OF ENUM FOR STRING
const char * typeOfSpace(spaceType type) {
    switch (type) {
        case AUDITORIUM: 
            return "AUDITORIUM";
        case MEETING_ROOM: 
            return "MEETING_ROOM";
        case EXTERNAL_SPACE: 
            return "EXTERNAL_SPACE";
        case PARTY_ROOM: 
            return "PARTY_ROOM";
        case CONFERENCE_ROOM: 
            return "CONFERENCE_ROOM";
        default: 
            return "UNKNOWN";
    }
}

void turnOnSpaces(geralSpaces *spaces){
    spaces->counter=0;
    spaces->capacity=MAX_SPACES;
    spaces->spaceList = malloc(spaces->capacity *sizeof(Space));
    if (spaces->spaceList==NULL){
        perror(TURN_ON_SPACE);
        exit(EXIT_FAILURE);
    }
}

void reallocSpaces(geralSpaces *spaces){
    if (spaces->counter >= spaces->capacity){
        spaces->capacity *= 2;
        Space *newList = realloc(spaces->spaceList,spaces->capacity *sizeof(Space));
        if (!newList){
            perror("ERROR REALLOCATING MEMORY!!");
            free(spaces->spaceList);
            exit (EXIT_FAILURE);
        }
        spaces->spaceList = newList;
    }
}

void freeSpaces(geralSpaces *space){
    if(space->spaceList != NULL){
        free(space->spaceList);
        space->spaceList=NULL;
    }
    space->capacity=0;
    space->counter=0;
}

int spaceOccupied(geralSpaces *spaces, int space_id) {
    for (int i = 0; i < spaces->counter; i++) {
        if (spaces->spaceList[i].id == space_id) {
            return spaces->spaceList[i].spaceOccupied; // Retorna 1 se ocupado, 0 se disponível
        }
    }
    return 0; // Caso o espaço não seja encontrado, considera que não está ocupado
}

//FUNCTION TO CREATE SPACE (USING POINTERS TO MOFIFY WHEN CREATE A NEW SPACE)
void createSpace(geralSpaces *spaces){
    reallocSpaces(spaces);
    spaces->counter++;
    
    spaces->spaceList = realloc(spaces->spaceList,spaces->counter * sizeof(Space));
    if(!spaces->spaceList){
        perror(ALLOC_MEMORY_LOAD);
        exit(EXIT_FAILURE);
    }
    
    Space *newSpace=&spaces->spaceList[spaces->counter - 1];
    newSpace->id=spaces->counter;
    
    //INFORMATIONS:
    cleanBuffer();
    //NAME
    readStrg(newSpace->name,MAX_NAME,"SPACE NAME:");
    
    //CAPACITY
    newSpace->capacity=verify(1,200,"SPACE CAPACITY (1 TO 200):");
    
    //TYPE
    puts("SPACE TYPE:\n0-AUDITORIUM\n1-METTING ROOM\n2-EXTERNAL SPACE\n3-PARTY ROOM\n4-CONFERENCE ROOM");
    newSpace->type = (spaceType)verify(0,4,"SPACE TYPE->");
    
    newSpace->reservations = 0; 
    
    puts(CREATE_SPACE);
}

//FUNCTION TO SEARCH THE SPACES CREATED
void listSpace(geralSpaces *spaces){
    if(spaces->spaceList == NULL || spaces->counter == 0){
        puts("NO SPACE CREATED! TRY TO CREATE ONE!");
        return;
    }
    puts("-----LIST SPACES-----");
    for (int i = 0; i < spaces->counter; i++){
        Space *space = &spaces->spaceList[i];
        printf("----ID %d---- \n NAME: %s \n CAPACITY: %d \n TYPE: %s \n RESERVATIONS: %d \n STATUS: %s\n-------------\n",
               space->id,space->name,space->capacity,typeOfSpace(space->type),space->reservations,(space->spaceOccupied==1)?"RESERVED":"AVALIABLE");
    }
}

//FUNCTION TO UPDATE EXISTING SPACES
void updateSpace(geralSpaces *spaces){
    if(spaces->counter==0){
        puts("NO SPACES TO UPDATE! TRY TO CREATE ONE!!");
        return;
    }
        
    int id=verify(1,1000,"ENTER THE ID TO UPDATE THE SPACE: ");

    
    for(int i=0;i<spaces->counter;i++){
        if(spaces->spaceList[i].id==id){
            Space *space=&spaces->spaceList[i];
        printf("UPDATING SPACE: \n-----%s----- \n",space->name);
        
        //NEW NAME
        cleanBuffer();
        char newName[MAX_NAME];
        readStrg(newName,MAX_NAME,"NEW NAME: ");
        if(strlen(newName)> 0){
            strcpy(space->name,newName); //COPY THE NEW NAME TO THE STRING (spaceList)
  
        }
        
        //NEW CAPACITY
        int newCapacity=verify(0,200,"NEW CAPACITY (0 TO KEEP): ");
        if(newCapacity>0){
            space->capacity=newCapacity;
        }                   
    
        //NEW TYPE OF SPACE
        int newType=verify(-1,4,"NEW TYPE (-1 TO KEEP): ");
        if(newType>=0){
        space->type=(spaceType)newType;
        }
    
    puts(UPDATE_SPACE);
    return;
        } 
    }
        puts("ID NOT FOUND!!");
    
}


//FUNCTION TO DELETE SPACES
void deleteSpace(geralSpaces *spaces){
    if (spaces->counter==0){
        puts("NO SPACES TO DELETE!");
        return;
    }
    int id=verify(1,100,"ID OF THE SPACE YOU WANT DELETE: ");
    
    for (int i=0;i<spaces->counter;i++){
        if(spaces->spaceList[i].id==id){
            printf("DELETE SPACE\n -----%s-----\n",spaces->spaceList[i].name);
            
            for(int j=i;j<spaces->counter-1;j++){
                spaces->spaceList[j]=spaces->spaceList[j+1];
            }
            //REALLOC MEMORY
            spaces->counter--;
            reallocSpaces(spaces);
            puts(DELETE_SPACE);
        }
            return;
  }
    printf("NO SPACE WITH ID %d!\n",id);
}


//REPORTS FUNCTIONS

//FUNCTION TO REPORT THE TOTAL OF AVALIABLE SPACES
void totalAvaliableSpaces(geralSpaces *spaces){
    puts("----------TOTAL SPACES AVALIABLE----------");
    printf("SPACES AVALIABLE: %d",spaces->counter);
    
}

//FUNCTION TO REPORT THE TYPE OF SPACES AVALIABLE
void spacesByType(geralSpaces *spaces){
    if (spaces->counter==0){
        puts("NO SPACES AVALIABLE!! TRY TO CREATE ONE!!");
        return;
    }
    int typeCounter[5]={0}; //ARRAY WITH 5 POSITIONS AND ALL START WITH VALUE 0
    
    for (int i=0;i<spaces->counter;i++){
        typeCounter[spaces->spaceList[i].type]++;
    }
    
    //REPORT RESULTS
    puts("----------SPACES BY TYPE----------");
    printf("AUDITORIUM: %d\n",typeCounter[AUDITORIUM]);
    printf("MEETING ROOM: %d\n",typeCounter[MEETING_ROOM]);
    printf("EXTERNAL SPACE: %d\n",typeCounter[EXTERNAL_SPACE]);
    printf("PARTY ROOM: %d\n",typeCounter[PARTY_ROOM]);
    printf("CONFERENCE ROOM: %d\n",typeCounter[CONFERENCE_ROOM]);
    
}

//REPORT THE MOST SPACE RESERVED
/**
 * @brief This function show the most spaces reserved.
 * @param geralSpaces are used to send the adress of the mains variables "spaces" to the following functions.
 * @return This function does not return any value. Just show the most spaces reserved.
 */
void mostSpaceReservated(geralSpaces *spaces,ReservationList *reservations){
    if (spaces->counter==0){
        puts("NO SPACES AVALIABLE!!");
        return;
    }
    
    Space* mostReservated=&spaces->spaceList[0];
    
    for (int i=1;i<spaces->counter;i++){
        if(spaces->spaceList[i].reservations > mostReservated->reservations){
            mostReservated=&spaces->spaceList[i];
        }
    }
    
    puts("----------MOST RESERVATED SPACE----------");
    printf("NAME: %s | RESERVATIONS: %d \n",mostReservated->name,mostReservated->reservations);
       
}

//REPORT THE LEAST SPACE RESERVED

/**
 * @brief This function show least the most spaces reserved.
 * @param geralSpaces are used to send the adress of the mains variables "spaces" to the following functions.
 * @return This function does not return any value. show the least spaces reserved.
 */

void leastSpaceReservated(geralSpaces *spaces,ReservationList *reservations){
    if (spaces->counter==0){
        puts("NO SPACES AVALIABLE!! NO CREATED SPACE!!");   
        return;
    }
    Space* leastReservated=&spaces->spaceList[0];
    
    for(int i=1;i < spaces->counter; i++){
        if (spaces->spaceList[i].reservations < leastReservated->reservations){
            leastReservated = &spaces->spaceList[i];
        }
    }
    puts("----------LEAST RESERVATED SPACE----------");
    printf("NAME: %s | RESERVATIONS: %d \n",leastReservated->name,leastReservated->reservations);
    
}


