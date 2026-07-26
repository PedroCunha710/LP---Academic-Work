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
        fclose(fp);
        exit (EXIT_FAILURE);
    }
    for(int i = 0;i<equipments->counter;i++){
    if (fread(&equipments->equipmentList[i], sizeof(Equipment), 1, fp) != 1) {
        puts("ERROR READING!!");
        free(equipments->equipmentList);
        turnOnEquipment(equipments);
        fclose (fp);
        return;
        }
    }
    fclose(fp);
    puts(LOADED_EQUIPMENT);
}

void turnOnEquipment(equipmentManager *equipments){
    equipments->counter=0;
    equipments->capacity=20;
    equipments->equipmentList = malloc(equipments->capacity *sizeof(Equipment));
    if (equipments->equipmentList==NULL){
        perror(TURN_ON_EQUIPMENT);
        exit(EXIT_FAILURE);
    }
}

void reallocEquipment(equipmentManager *equipments){
    if (equipments->counter >=equipments->capacity){
        equipments->capacity*=2;
        Equipment *newList = realloc(equipments->equipmentList,equipments->capacity *sizeof(Equipment));
        if(!newList){
            perror("ERROR REALLOCATING MEMORY!!");
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
        exit(EXIT_FAILURE);
    }
    Equipment *newEquipment=&equipments->equipmentList[equipments->counter-1];
    newEquipment->id=equipments->counter;
    
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
    
    puts(CREATE_EQUIPMENT);
}

int findEquipment(equipmentManager *equipments, int id){
    for (int i = 0; i< equipments->counter; i++){
        if(equipments->equipmentList[i].id == id){
            puts("EQUIPMENT FOUND!");
            return i;
        }
    }
    puts("ID NOT FOUND!");  // Se o ID não for encontrado
    return -1;
}
void listEquipment(equipmentManager *equipments){
    if(equipments->counter == 0||equipments->equipmentList==NULL){
        puts(NOT_EQUIPMENT_SHOW);
        return;
    }
    puts("-----EQUIPMENTS LIST-----");
    for(int i=0;i<equipments->counter;i++){
        Equipment *equipment =&equipments->equipmentList[i];
        printf("----- ID: %d -----\nNAME: %s\nTYPE: %s\nQUANTITY: %d\nEQUIPMENT RESERVED: %d\nEQUIPMENT STATUS: %s\n---------------\n",
                equipment->id,equipment->name,typeOfEquipment(equipment->type),equipment->quantity,equipment->usingEquipment,statusOfEquipment(equipment->status));
    }
}

void updateEquipment(equipmentManager *equipments){
    if(equipments->counter == 0){
        puts(NOT_EQUIPMENT_UPDATE);
    }
    int id=verify(1,1000,"ENTER THE ID TO UPDATE THE EQUIPMENT:");
    
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
        }
    }
    puts(ID_NOT_FOUND);
}

void deleteEquipment(equipmentManager *equipments){
    if(equipments->counter==0){
        puts(NOT_EQUIPMENT_DELETE);
        return;
    }
    int id = verify(1,1000,"ID OF THE EQUIPMENT YOU WANT TO DELETE: ");
    
    for(int i=0;i<equipments->counter;i++){
        if(equipments->equipmentList[i].id==id){
            printf("DELETING EQUIPMENT\n -----%s-----\n",equipments->equipmentList[i].name);
            
            for(int j=i;j <equipments->counter - 1 ;j++){
                equipments->equipmentList[j]=equipments->equipmentList[j+1];   
            }
            //REALLOC MEMORY
            equipments->counter--;
            Equipment *newList =realloc(equipments->equipmentList,equipments->counter * sizeof(Equipment));
            if (newList || equipments->counter == 0){
                equipments->equipmentList = newList;
            }else{
                perror("ERROR ALLOCATING MEMORY (DELETE EQUIPMENT)!");
               exit (EXIT_FAILURE); 
            }
            puts(DELETE_EQUIPMENT);
            return;
        }
        printf("NO EQUIPMENT WITH ID %d!\n",id);
    }
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