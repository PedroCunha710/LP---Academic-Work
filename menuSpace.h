/* 
 * File:   menuSpace.h
 * Author: Pedro Cunha 
 *
 * Created on 20 de dezembro de 2024, 13:51
 * 
 * @brief In this header file we will declare the functions of the spaces to use in other pages.
 * This file contains the functions of the spaces to use in other pages.
 */


#ifndef MENUSPACE_H
#define MENUSPACE_H

/**
 * @def MAX_NAME
 * Maximum length for a name.
 */
#define MAX_NAME 50

/**
 * @def MAX_SPACES
 * Maximum number of spaces allowed in the system.
 */
#define MAX_SPACES 10

/**
 * @def ID_NOT_FOUND
 * Message displayed when an ID is not found.
 */
#define ID_NOT_FOUND "ID NOT FOUND"

/** @def NOT_SAVED_SPACE
 * Message displayed when there are no spaces to save.
 */
#define NOT_SAVED_SPACE "NO SPACES TO SAVE!!"

/** @def NOT_SPACE_SHOW
 * Message displayed when there are no spaces to show.
 */
#define NOT_SPACE_SHOW "NO SPACES TO SHOW!"

/** @def NOT_SPACE_UPDATE
 * Message displayed when there are no spaces to update.
 */
#define NOT_SPACE_UPDATE "NO SPACES TO UPDATE"

/** @def NOT_SPACE_DELETE
 * Message displayed when there are no spaces to delete.
 */
#define NOT_SPACE_DELETE "NO SPACES TO DELETE!"

/** @def ERROR_SAVING_SPACE
 * Message displayed when there is an error saving spaces.
 */
#define ERROR_SAVING_SPACE "ERROR SAVING SPACE!!"

/** @def SAVED_SPACE
 * Message displayed when spaces are saved successfully.
 */
#define SAVED_SPACE "SPACE SAVED SUCESSFULLY"

/**
 * @def NOT_LOADED_SPACE
 * Message displayed when no space files are found.
 */
#define NOT_LOADED_SPACE "NO SPACE FILES FOUND! INICIALIZATING EMPTY LIST"

/**
 * @def ALLOC_MEMORY_LOAD
 * Message displayed when there is an error allocating memory to load spaces.
 */
#define ALLOC_MEMORY_LOAD "ERROR ALLOCATING MEMORY!"

/**
 * @def LOADED_SPACE
 * Message displayed when spaces are loaded successfully.
 */
#define LOADED_SPACE "SPACES LOADED SUCESSFULLY"

/**
 * @def TURN_ON_SPACE
 * Message displayed when there is an error turning on the space.
 */
#define TURN_ON_SPACE "ERROR ALLOCATING MEMORY FOR SPACE!"

/**
 * @def CREATE_SPACE
 * Message displayed when space is created successfully.
 */
#define CREATE_SPACE "\nSPACE CREATED SUCESSFULLY!"

/**
 * @def DELETE_SPACE
 * Message displayed when space is deleted successfully.
 */
#define DELETE_SPACE "SPACE DELETED SUCESSFULLY!"

/**
 * @def UPDATE_SPACE
 * Message displayed when space is updated successfully.
 */
#define UPDATE_SPACE "SPACE UPDATED SUCESSFULLY"

typedef enum{
    AUDITORIUM, 
    MEETING_ROOM, 
    EXTERNAL_SPACE,
    PARTY_ROOM,
    CONFERENCE_ROOM,
    UNKNOWN
} spaceType;

//SPACE STRUCT
typedef struct{
    int id;
    char name[MAX_NAME];
    int capacity;
    int spaceOccupied;
    spaceType type;
    int reservations;
    
} Space;

typedef struct{
    int counter;
    int capacity;
    Space *spaceList;
}geralSpaces;

//FUNCTIONS
/**
 * @brief This function saves the record in spaces, for example Create, List, Update and Remove.
 * @param geralSpaces are used to send the adress of the mains variables "spaces" to the following functions.
 * @return This function does not return any value. Its only save the spaces.
 */
void saveSpace(geralSpaces *spaces);

/**
 * @brief This function load the record in spaces, for example Create, List, Update and Remove.
 * @param geralSpaces are used to send the adress of the mains variables "spaces" to the following functions.
 * @return This function does not return any value. Its only load the spaces.
 */
void loadSpace(geralSpaces *spaces);

//OTHER FUNCTIONS
/**
 * @brief This function initializes the spaces.
 * @param geralSpaces are used to send the adress of the mains variables "spaces" to the following functions.
 * @return This function does not return any value. Just inicialize the spaces.
 */
void turnOnSpaces(geralSpaces *spaces);

/**
 * @brief This function realloc the spaces.
 * @param geralSpaces are used to send the adress of the mains variables "spaces" to the following functions.
 * @return This function does not return any value. Just Realloc the spaces.
 */
void reallocSpaces(geralSpaces *spaces);

/**
 * @brief This function free the spaces.
 * @param geralSpaces are used to send the adress of the mains variables "spaces" to the following functions.
 * @return This function does not return any value. Just free the spaces.
 */
void freeSpaces(geralSpaces *spaces);

/**
 * @brief This function search if spaces is occupied.
 * @param geralSpaces are used to send the adress of the mains variables "spaces" to the following functions.
 * @param space_id show if space is occupied to the reserve.
 * @return This function does return 1 if space is occupied or 0 if space is avaiable.
 */
int spaceOccupied(geralSpaces *spaces, int space_id);


//FUNCTIONS TO OPERATIONS CREATE,LIST,UPDATE,DELETE

/**
 * @brief This function create the spaces.
 * @param geralSpaces are used to send the adress of the mains variables "spaces" to the following functions.
 * @return This function does not return any value. Just create the spaces.
 */
void createSpace(geralSpaces *spaces);

/**
 * @brief This function list the spaces.
 * @param geralSpaces are used to send the adress of the mains variables "spaces" to the following functions.
 * @return This function does not return any value. Just list the spaces.
 */
void listSpace(geralSpaces *spaces);

/**
 * @brief This function update the spaces.
 * @param geralSpaces are used to send the adress of the mains variables "spaces" to the following functions.
 * @return This function does not return any value. Just update the spaces.
 */
void updateSpace(geralSpaces *spaces);

/**
 * @brief This function delete the spaces.
 * @param geralSpaces are used to send the adress of the mains variables "spaces" to the following functions.
 * @return This function does not return any value. Just delete the spaces.
 */
void deleteSpace(geralSpaces *spaces);

//PROTOTIPS REPORTS FUCTIONS
/**
 * @brief This function show the total avaliable spaces.
 * @param geralSpaces are used to send the adress of the mains variables "spaces" to the following functions.
 * @return This function does not return any value. Just show the total avaliable spaces.
 */
void totalAvaliableSpaces(geralSpaces *spaces);

/**
 * @brief This function show spaces by type.
 * @param geralSpaces are used to send the adress of the mains variables "spaces" to the following functions.
 * @return This function does not return any value. Just spaces by type.
 */
void spacesByType(geralSpaces *spaces);


#endif /* MENUSPACE_H */
