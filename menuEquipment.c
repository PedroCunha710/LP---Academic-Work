/*
 * File:   menuEquipment.c
 * Author: Pedro Cunha
 *
 * Created on 20 de dezembro de 2024, 13:50
 *
 * @brief In this file we will declare the functions of the equipments.
 * This file contains the functions developed by equipments, such as CRUD and reports
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "menuEquipment.h"
#include "menuBooking.h"

#define EQUIPMENT_FILE "equipment.bin"

void saveEquipment(equipmentManager *equipments){
    if (equipments->counter==0){
        puts(NOT_SAVED_EQUIPMENT);
        return;
    }
    FILE* fp=fopen(EQUIPMENT_FILE,"wb");

    if(!fp){
        perror(ERROR_SAVING_EQUIPMENT);
        logMessage(LOG_ERROR, "EQUIPMENT", "Failed to open equipment.bin for writing.");
        exit (EXIT_FAILURE);
    }

    //WRITE EQUIPMENT COUNTER
    fwrite(&equipments->counter,sizeof(int),1,fp);

    //WRITE EQUIPMENT LIST
    for (int i=0;i<equipments->counter;i++){
    fwrite(&equipments->equipmentList[i],sizeof(Equipment),1,fp);
    }

    fclose(fp);

    puts(SAVED_EQUIPMENT);
    logMessage(LOG_INFO, "EQUIPMENT", "Equipment saved to disk.");
}

// Recomputes the next unique/sequential id from the highest id currently in memory.
static void recalculateNextEquipmentId(equipmentManager *equipments){
    int maxId = 0;
    for (int i = 0; i < equipments->counter; i++){
        if (equipments->equipmentList[i].id > maxId){
            maxId = equipments->equipmentList[i].id;
        }
    }
    equipments->nextId = maxId + 1;
}

void loadEquipment(equipmentManager *equipments){
    //OPENING FILE
    FILE* fp=fopen(EQUIPMENT_FILE,"rb");

    if(!fp){
        puts(NOT_LOADED_EQUIPMENT);
        turnOnEquipment(equipments);
        return;
    }
    if(fread(&equipments->counter,sizeof(int),1,fp)!=1){
        equipments->counter=0;
    }
    equipments->capacity= (equipments->counter > 0) ? equipments->counter : 20;
    equipments->equipmentList = malloc(equipments->capacity * sizeof(Equipment));

    if(!equipments->equipmentList){
        puts(ALLOC_MEMORY_LOAD);
        logMessage(LOG_ERROR, "EQUIPMENT", "Memory allocation failed while loading equipment.");
        fclose(fp);
        exit (EXIT_FAILURE);
    }
    for(int i = 0;i<equipments->counter;i++){
    if (fread(&equipments->equipmentList[i], sizeof(Equipment), 1, fp) != 1) {
        puts("ERROR READING!!");
        logMessage(LOG_ERROR, "EQUIPMENT", "Error reading equipment list from disk.");
        free(equipments->equipmentList);
        turnOnEquipment(equipments);
        fclose (fp);
        return;
        }
    }
    recalculateNextEquipmentId(equipments);
    fclose(fp);
    puts(LOADED_EQUIPMENT);
    logMessage(LOG_INFO, "EQUIPMENT", "Equipment loaded from disk.");
}

void turnOnEquipment(equipmentManager *equipments){
    equipments->counter=0;
    equipments->capacity=20;
    equipments->nextId=1;
    equipments->equipmentList = malloc(equipments->capacity *sizeof(Equipment));
    if (equipments->equipmentList==NULL){
        perror(TURN_ON_EQUIPMENT);
        logMessage(LOG_ERROR, "EQUIPMENT", "Memory allocation failed while initializing equipment.");
        exit(EXIT_FAILURE);
    }
}

void reallocEquipment(equipmentManager *equipments){
    if (equipments->counter >=equipments->capacity){
        equipments->capacity*=2;
        Equipment *newList = realloc(equipments->equipmentList,equipments->capacity *sizeof(Equipment));
        if(!newList){
            perror("ERROR REALLOCATING MEMORY!!");
            logMessage(LOG_ERROR, "EQUIPMENT", "Memory reallocation failed.");
            free(equipments->equipmentList);
            exit(EXIT_FAILURE);
        }
        equipments->equipmentList=newList;
    }
}

void freeEquipment(equipmentManager *equipments){
    if(equipments->equipmentList != NULL){
        free(equipments->equipmentList);
        equipments->equipmentList=NULL;
    }
    equipments->capacity=0;
    equipments->counter=0;
}

const char * typeOfEquipment(equipmentType type){
    switch(type){
        case AUDIOVISUAL:
            return "AUDIOVISUAL";
        case FURNITURE:
            return "FURNITURE";
        case LIGHTING:
            return "LIGHTING";
        case SOUND_SISTEM:
            return "SOUND_SISTEM";
        case FILMING_EQUIPMENT:
            return "FILMING_EQUIPMENT";
        default:
            return "UNKNOWN";
    }
}

const char * statusOfEquipment(equipmentStatus status){
    switch (status){
        case AVALIABLE:
            return "AVALIABLE";
        case RESERVED:
            return "RESERVED";
        case UNDER_MAINTENANCE:
            return "UNDER MAINTENACE";
        default:
            return "UNKNOWN";
    }
}

int equipmentReserved(equipmentManager *equipments, int equipment_id) {
    for (int i = 0; i < equipments->counter; i++) {
        if (equipments->equipmentList[i].id == equipment_id) {
            return equipments->equipmentList[i].equipmentReserved;
        }
    }
    return 0;
}


void createEquipment(equipmentManager *equipments){
    reallocEquipment(equipments);
    equipments->counter++;

    equipments->equipmentList= realloc(equipments->equipmentList,equipments->counter * sizeof(Equipment));

    if(!equipments->equipmentList){
        perror("ERROR REALLOCATING MEMORY (CREATING EQUIPMENT)!");
        logMessage(LOG_ERROR, "EQUIPMENT", "Memory reallocation failed while creating equipment.");
        exit(EXIT_FAILURE);
    }
    Equipment *newEquipment=&equipments->equipmentList[equipments->counter-1];
    newEquipment->id=equipments->nextId++; // Unique and sequential: never reused, even after deletions.

    //INFORMATIONS
    cleanBuffer();

    //NAME
    readStrg(newEquipment->name,MAX_NAME,"EQUIPMENT NAME->");

    //TYPE
    puts("EQUIPMENT TYPE:\n0-AUDIOVISUAL\n1-FURNITURE\n2-LIGHTING\n3-SOUND SISTEM\n4-FILMING EQUIPMENT");
    newEquipment->type= (equipmentType)verify(0,4,"EQUIPMENT TYPE->");

    //QUANTITY
    newEquipment->quantity = verify(0,50,"EQUIPMENT QUANTITY->");

    //STATUS
    puts("EQUIPMENT STATUS:\n0-AVALIABLE\n1-RESERVED\n2-UNDER MAINTENANCE");
    newEquipment->status = (equipmentStatus)verify(0, 2, "EQUIPMENT STATUS->");

    newEquipment->usingEquipment = 0;
    newEquipment->equipmentReserved = 0;
    newEquipment->isActive = 1;

    puts(CREATE_EQUIPMENT);
    logMessage(LOG_INFO, "EQUIPMENT", "Equipment created.");
}

// Finds equipment by ID (searches both active and inactive equipment)
int findEquipment(equipmentManager *equipments, int id){
    for (int i = 0; i< equipments->counter; i++){
        if(equipments->equipmentList[i].id == id){
            return i;
        }
    }
    return -1;
}

void listEquipment(equipmentManager *equipments){
    if(equipments->counter == 0||equipments->equipmentList==NULL){
        puts(NOT_EQUIPMENT_SHOW);
        return;
    }
    puts("-----EQUIPMENTS LIST-----");
    int shown = 0;
    for(int i=0;i<equipments->counter;i++){
        Equipment *equipment =&equipments->equipmentList[i];
        if (!equipment->isActive){
            continue; // Inactive equipment does not appear in regular listings.
        }
        shown++;
        printf("----- ID: %d -----\nNAME: %s\nTYPE: %s\nQUANTITY: %d\nEQUIPMENT RESERVED: %d\nEQUIPMENT STATUS: %s\n---------------\n",
                equipment->id,equipment->name,typeOfEquipment(equipment->type),equipment->quantity,equipment->usingEquipment,statusOfEquipment(equipment->status));
    }
    if (shown == 0){
        puts("NO ACTIVE EQUIPMENT TO SHOW!");
    }
}

// Lists inactive (soft-deleted) equipment, so it can be reactivated if needed
void listInactiveEquipment(equipmentManager *equipments){
    int shown = 0;
    puts("-----INACTIVE EQUIPMENT-----");
    for (int i = 0; i < equipments->counter; i++){
        Equipment *equipment = &equipments->equipmentList[i];
        if (equipment->isActive){
            continue;
        }
        shown++;
        printf("----- ID: %d ----- \nNAME: %s\nTYPE: %s\n---------------\n",
               equipment->id, equipment->name, typeOfEquipment(equipment->type));
    }
    if (shown == 0){
        puts("NO INACTIVE EQUIPMENT!");
    }
}

void updateEquipment(equipmentManager *equipments){
    if(equipments->counter == 0){
        puts(NOT_EQUIPMENT_UPDATE);
        return;
    }
    int id=verify(1,100000,"ENTER THE ID TO UPDATE THE EQUIPMENT:");

    for(int i=0;i<equipments->counter;i++){
        if(equipments->equipmentList[i].id==id){
            Equipment *equipment= &equipments->equipmentList[i];
            printf("UPDATING EQUIPMENT: \n-----%s----- \n",equipment->name);
        //INFORMATIONS
            cleanBuffer();
        //NEW NAME
            char newName[MAX_NAME];
            readStrg(newName,MAX_NAME,"NEW NAME: ");
            if(strlen(newName)> 0){
            strcpy(equipment->name,newName); //COPY THE NEW NAME TO THE STRING (equipmentList)
            }
        //QUANTITY
        int newQuantity = verify(0,200,"NEW QUANTIY (0 TO KEEP): ");
        if(newQuantity>0){
             equipment->quantity=newQuantity;
            }
        //TYPE
        int newType = verify (-1,4,"NEW TYPE (-1 TO KEEP): ");
        if (newType > -1){
            equipment->type=(equipmentType)newType;
            }
        //STATUS
        int newStatus = verify (-1,2,"NEW STATUS (-1 TO KEEP):");
        if(newStatus > -1){
            equipment->status=(equipmentStatus)newStatus;
        }
        puts(UPDATE_EQUIPMENT);
        logMessage(LOG_INFO, "EQUIPMENT", "Equipment updated.");
        return;
        }
    }
    puts(ID_NOT_FOUND);
}

//FUNCTION TO DELETE EQUIPMENT
//If the equipment has reservations pointing to it, it is marked Inactive instead of being
//physically removed, to preserve referential integrity (per assignment requirements).
void deleteEquipment(equipmentManager *equipments, ReservationList *reservations){
    if(equipments->counter==0){
        puts(NOT_EQUIPMENT_DELETE);
        return;
    }
    int id = verify(1,100000,"ID OF THE EQUIPMENT YOU WANT TO DELETE: ");

    int index = findEquipment(equipments, id);
    if (index == -1){
        printf("NO EQUIPMENT WITH ID %d!\n",id);
        return;
    }

    if (!equipments->equipmentList[index].isActive){
        puts("THIS EQUIPMENT IS ALREADY INACTIVE!");
        return;
    }

    if (equipmentHasReservations(reservations, id)){
        equipments->equipmentList[index].isActive = 0;
        printf("EQUIPMENT\n -----%s-----\n HAS RESERVATIONS ASSOCIATED, SO IT WAS MARKED AS INACTIVE INSTEAD OF DELETED.\n", equipments->equipmentList[index].name);
        logMessage(LOG_INFO, "EQUIPMENT", "Equipment marked as inactive (has associated reservations).");
        return;
    }

    printf("DELETING EQUIPMENT\n -----%s-----\n",equipments->equipmentList[index].name);

    for(int j=index;j <equipments->counter - 1 ;j++){
        equipments->equipmentList[j]=equipments->equipmentList[j+1];
    }
    //REALLOC MEMORY
    equipments->counter--;
    if (equipments->counter > 0){
        Equipment *newList =realloc(equipments->equipmentList,equipments->counter * sizeof(Equipment));
        if (!newList){
            perror("ERROR ALLOCATING MEMORY (DELETE EQUIPMENT)!");
            logMessage(LOG_ERROR, "EQUIPMENT", "Memory reallocation failed while deleting equipment.");
            exit (EXIT_FAILURE);
        }
        equipments->equipmentList = newList;
    }
    puts(DELETE_EQUIPMENT);
    logMessage(LOG_INFO, "EQUIPMENT", "Equipment deleted.");
}

//FUNCTION TO REACTIVATE AN INACTIVE EQUIPMENT
void reactivateEquipment(equipmentManager *equipments){
    int index = findEquipment(equipments, verify(1, 100000, "ID OF THE EQUIPMENT YOU WANT TO REACTIVATE: "));

    if (index == -1){
        puts(ID_NOT_FOUND);
        return;
    }

    if (equipments->equipmentList[index].isActive){
        puts("THIS EQUIPMENT IS ALREADY ACTIVE!");
        return;
    }

    equipments->equipmentList[index].isActive = 1;
    puts("EQUIPMENT REACTIVATED SUCCESSFULLY!");
    logMessage(LOG_INFO, "EQUIPMENT", "Equipment reactivated.");
}

//FUNCTION TO MANUALLY DEACTIVATE AN EQUIPMENT (WITHOUT DELETING IT)
void deactivateEquipment(equipmentManager *equipments){
    int index = findEquipment(equipments, verify(1, 100000, "ID OF THE EQUIPMENT YOU WANT TO DEACTIVATE: "));

    if (index == -1){
        puts(ID_NOT_FOUND);
        return;
    }

    if (!equipments->equipmentList[index].isActive){
        puts("THIS EQUIPMENT IS ALREADY INACTIVE!");
        return;
    }

    equipments->equipmentList[index].isActive = 0;
    puts("EQUIPMENT DEACTIVATED SUCCESSFULLY!");
    logMessage(LOG_INFO, "EQUIPMENT", "Equipment manually deactivated.");
}


//REPORT EQUIPMENTS FUNCTIONS
void mostUsedEquipment(equipmentManager *equipments){
    if(equipments->counter==0){
        puts("NO EQUIPMENTS AVALIABLE!!");
        return;
    }
    Equipment* mostUsed=&equipments->equipmentList[0];

    for (int i=1;i<equipments->counter;i++){
        if(equipments->equipmentList[i].usingEquipment > mostUsed->usingEquipment){
            mostUsed=&equipments->equipmentList[i];
        }
    }
        puts("----------MOST USED EQUIPMENT----------");
        printf("NAME: %s | USED: %d \n",mostUsed->name,mostUsed->usingEquipment);
}

void leastUsedEquipment(equipmentManager *equipments){
    if(equipments->counter==0){
        puts("NO EQUIPMENTS AVALIABLE!!");
        return;
    }
    Equipment* leastUsed=&equipments->equipmentList[0];

    for (int i=1;i<equipments->counter;i++){
        if(equipments->equipmentList[i].usingEquipment < leastUsed->usingEquipment){
            leastUsed=&equipments->equipmentList[i];
        }
    }
    puts("----------LEAST USED EQUIPMENT----------");
    printf("NAME: %s | USED: %d \n",leastUsed->name,leastUsed->usingEquipment);
}

float calculateUsageRate(Equipment *equipment) {
    if (equipment->quantity == 0) return 0.0;
    return (float)equipment->usingEquipment / equipment->quantity * 100;
}

void detailedUsageReport(equipmentManager *equipments) {
    puts("----------DETAILED EQUIPMENT USAGE REPORT----------");

    // Relatório de todos os equipamentos
    for (int i = 0; i < equipments->counter; i++) {
        Equipment *equipment = &equipments->equipmentList[i];
        float usageRate = calculateUsageRate(equipment); // Cálculo da taxa de utilização
        printf("ID: %d | NAME: %s | TYPE: %s | STATUS: %s | QUANTITY: %d | USED: %d | USAGE RATE: %.2f%%\n",
               equipment->id, equipment->name, typeOfEquipment(equipment->type),
               statusOfEquipment(equipment->status), equipment->quantity,
               equipment->usingEquipment, usageRate);
    }
}

int isEquipmentAvailable(Equipment *equipment) {
    if (equipment->status == UNDER_MAINTENANCE) {
        printf("EQUIPMENT %s IS UNDER MAINTENACE AND CANNOT BE USED!\n", equipment->name);
        return 0;
    }
    return 1;
}
