/* 
 * @File:   menuEquipment.h
 * @Author: Pedro Cunha 
 *
 * Created on 20 de dezembro de 2024, 13:54
 * * 
 * @brief In this header file we will declare the functions of the equipment to use in other pages.
 * This file contains the functions of the equipments to use in other pages.
 */


#ifndef MENUEQUIPMENT_H
#define MENUEQUIPMENT_H

/**
 * @def MAX_NAME
 * Maximum length for a equipment name.
 */
#define MAX_NAME 50

/**
 * @def ID_NOT_FOUND
 * Message displayed when an ID is not found.
 */
#define ID_NOT_FOUND "ID NOT FOUND"

/** @def NOT_SAVED_EQUIPMENT
 * Message displayed when there are no equipment to save.
 */
#define NOT_SAVED_EQUIPMENT "NO EQUIPMENT TO SAVE!!"

/** @def NOT_EQUIPMENT_SHOW
 * Message displayed when there are no equipment to show.
 */
#define NOT_EQUIPMENT_SHOW "NO EQUIPMENTS TO SHOW!"

/** @def NOT_EQUIPMENT_UPDATE
 * Message displayed when there are no equipment to update.
 */
#define NOT_EQUIPMENT_UPDATE "NO EQUIPMENTS TO UPDATE"

/** @def NOT_EQUIPMENT_DELETE
 * Message displayed when there are no equipment to delete.
 */
#define NOT_EQUIPMENT_DELETE "NO EQUIPMENTS TO DELETE!"

/** @def ERROR_SAVING_EQUIPMENT
 * @brief Message displayed when there is an error saving equipment.
 */
#define ERROR_SAVING_EQUIPMENT "ERROR SAVING EQUIPMENTS!!"

/** @def SAVED_EQUIPMENT
 * Message displayed when equipment is saved successfully.
 */
#define SAVED_EQUIPMENT "EQUIPMENTS SAVED SUCESSFULLY"

/**
 * @def NOT_LOADED_EQUIPMENT
 * Message displayed when no equipment files are found.
 */
#define NOT_LOADED_EQUIPMENT "NO EQUIPMENT FILES FOUND! INICIALIZATING EMPTY LIST"

/**
 * @def ALLOC_MEMORY_LOAD
 * Message displayed when there is an error allocating memory to load equipment.
 */
#define ALLOC_MEMORY_LOAD "ERROR ALLOCATING MEMORY!"

/**
 * @def LOADED_EQUIPMENT
 * Message displayed when equipment is loaded successfully.
 */
#define LOADED_EQUIPMENT "EQUIPMENTS LOADED SUCESSFULLY"

/**
 * @def TURN_ON_EQUIPMENT
 * Message displayed when there is an error turning on the equipment.
 */
#define TURN_ON_EQUIPMENT "ERROR ALLOCATING MEMORY FOR EQUIPMENT!"

/**
 * @def CREATE_EQUIPMENT
 * Message displayed when equipment is created successfully.
 */
#define CREATE_EQUIPMENT "\nEQUIPMENT CREATED SUCESSFULLY!"

/**
 * @def DELETE_EQUIPMENT
 * Message displayed when equipment is deleted successfully.
 */
#define DELETE_EQUIPMENT "EQUIPMENT DELETED SUCESSFULLY!"

/**
 * @def UPDATE_EQUIPMENT
 * Message displayed when equipment is updated successfully.
 */
#define UPDATE_EQUIPMENT "EQUIPMENT UPDATED SUCESSFULLY"

//ENUMS
typedef enum{
    AUDIOVISUAL,
    FURNITURE,
    LIGHTING,
    SOUND_SISTEM,
    FILMING_EQUIPMENT,
}equipmentType;

//TYPES OF EQUIPMENT

typedef enum{
    AVALIABLE,
    RESERVED,
    UNDER_MAINTENANCE       
}equipmentStatus;

//STATUS OF EQUIPMENT
typedef struct {
    int id;
    char name[MAX_NAME];
    int quantity;
    int equipmentReserved;
    int usingEquipment;
    equipmentType type;
    equipmentStatus status;
    int isActive; // 1 = active (shown in regular listings), 0 = inactive (kept for referential integrity)
}Equipment;

typedef struct{
    int counter;
    int capacity;
    int nextId; // Next sequential, unique id to assign. Never reused, even after deletions.
    Equipment *equipmentList;
}equipmentManager;

//FUNCTIONS
/**
 * @brief This function initializes the equipments.
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @return This function does not return any value. Just inicialize the equipments.
 */
void turnOnEquipment(equipmentManager *equipments); //INCIALIZE EQUIPMENTS

/**
 * @brief This function realloc the equipments.
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @return This function does not return any value. Just realloc the equipments.
 */
void reallocEquipment(equipmentManager *equipments); //REALLOC EQUIPMENTS DINAMIC MEMORY 

/**
 * @brief This function free the equipments.
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @return This function does not return any value. Just free the equipments.
 */
void freeEquipment(equipmentManager *equipments); //FREE EQUIPMENT DINAMIC MEMORY

//FUNCTIONS FILES (SAVE AND LOAD)//
/**
 * @brief This function saves the record in equipments, for example Create, List, Update and Remove.
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @return This function does not return any value. Its only save the equipments.
 */
void saveEquipment(equipmentManager *equipments); //SAVE EQUIPMENT IN EQUIPMENT.BIN

/**
 * @brief This function load the record in equipments, for example Create, List, Update and Remove.
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @return This function does not return any value. Its only load the equipments.
 */
void loadEquipment(equipmentManager *equipments); //LOAD EQUIPMENT (FILE equipment.bin)

//FUNCTIONS CRUD (CREATE,READ,UPDATE,DELETE)//
/**
 * @brief This function create the equipments.
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @return This function does not return any value. Just create the equipments.
 */
void createEquipment(equipmentManager *equipments); //FUNCTION TO CREATE EQUIPMENT

/**
 * @brief This function list the active equipments.
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @return This function does not return any value. Just list the equipments. Inactive equipment is hidden.
 */
void listEquipment(equipmentManager *equipments); //FUNCTION TO SEARCH/LIST EQUIPMENT

/**
 * @brief This function lists the inactive (soft-deleted) equipment, so it can be reactivated if needed.
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @return This function does not return any value. Just list the inactive equipment.
 */
void listInactiveEquipment(equipmentManager *equipments);

/**
 * @brief This function update the equipments.
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @return This function does not return any value. Just update the equipments.
 */
void updateEquipment(equipmentManager *equipments); //FUNCTION TO UPDATE EQUIPMENT

/**
 * @brief This function reactivates an equipment previously marked as inactive.
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @return This function does not return any value. Just reactivate the equipment.
 */
void reactivateEquipment(equipmentManager *equipments);

/**
 * @brief This function manually deactivates an equipment (marks it Inactive) without deleting it,
 * regardless of whether it has reservations. Useful to temporarily take equipment out of service.
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @return This function does not return any value. Just deactivate the equipment.
 */
void deactivateEquipment(equipmentManager *equipments);

/**
 * @brief This function find the equipments (searches both active and inactive equipment).
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @param id is used to find the id of the equipment
 * @return This function returns the index of the equipment if found, or -1 if the id is not found.
 */
int findEquipment(equipmentManager *equipments, int id);

/**
 * @brief This function search if equipment is reserved.
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @param equipment_id show if equipment is reserved.
 * @return This function does return to the equipment_id if equipment is reserved or 0 if equipment is avaiable.
 */
int equipmentReserved(equipmentManager *equipments, int equipment_id);
//REPORT EQUIPMENT FUNCTIONS

/**
 * @brief This function show the most used equipment.
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @return This function does not return any value. Just show the most used equipment.
 */
void mostUsedEquipment(equipmentManager *equipments); //SHOW THE MOST USED EQUIPMENT

/**
 * @brief This function show the least used equipment.
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @return This function does not return any value. Just show the least used equipment.
 */
void leastUsedEquipment(equipmentManager *equipments); //SHOW THE LEAST USED EQUIPMENT

/**
 * @brief This function calcilate the usage rate.
 * @param Equipment are used to send the adress of the mains variables "equipment" to the following functions.
 * @return This function return the value of the usage rate.
 */
float calculateUsageRate(Equipment *equipment);


/**
 * @brief This function show the percentage usage of equipements.
 * @param equipmentManager are used to send the adress of the mains variables "equipments" to the following functions.
 * @return This function does not return any value. Just show the percentage usage of equipement.
 */
void detailedUsageReport(equipmentManager *equipments);


/**
 * @brief This function show if equipment is avaiable.If not, cannot be used in reservation.
 * @param Equipment are used to send the adress of the mains variables "equipment" to the following functions.
 * @return This function return 1 if equipment is avaiable or 0 if equipment is maintenance.
 */
int isEquipmentAvailable(Equipment *equipment);



#endif /* MENUEQUIPMENT_H */

