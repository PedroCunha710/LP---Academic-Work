
/*
 * File:   menuClient.h
 * Author: Henrique Lopes
 *
 * Created on 20 de dezembro de 2024, 13:52
 *
 * @brief In this header file we will declare the functions of the clients to use in other pages.
 * This file contains the functions of the clients to use in other pages.
 */

#ifndef MENUCLIENT_H
#define MENUCLIENT_H

#include <time.h>
// Constants
/** @def MAX_CLIENT_NAME
 * Maximum length for a client's name.
 */
#define MAX_CLIENT_NAME 100

/** @def MAX_CLIENT_EMAIL
 * Maximum length for a client's email.
 */
#define MAX_CLIENT_EMAIL 50

/** @def MSG_ERROR_EMPTY_LIST
 * Error message when the client list is empty.
 */
#define MSG_ERROR_EMPTY_LIST "The client list is empty!"

/** @def MSG_ERROR_CLIENT_EXISTS
 * Error message when the client is already registered.
 */
#define MSG_ERROR_CLIENT_EXISTS "The client is already registered!"

/** @def MSG_GET_CLIENT_NAME
 * Prompt message for entering the client's name.
 */
#define MSG_GET_CLIENT_NAME "Enter the client's name: "

/** @def MSG_GET_CLIENT_NIF
 * Prompt message for entering the client's NIF (9 digits).
 */
#define MSG_GET_CLIENT_NIF "Enter the client's NIF (9 digits): "

/** @def MSG_GET_CLIENT_EMAIL
 * Prompt message for entering the client's email.
 */
#define MSG_GET_CLIENT_EMAIL "Enter the client's email: "

/** @def MSG_GET_CLIENT_PHONE
 * Prompt message for entering the client's phone.
 */
#define MSG_GET_CLIENT_PHONE "Enter the client's phone: "

/** @def MSG_GET_CLIENT_ID
 * Prompt message for entering the client's ID.
 */
#define MSG_GET_CLIENT_ID "Enter the client's id: "

// Declaração antecipada de ReservationList

// Structures
typedef struct {
    int id;
    char name[MAX_CLIENT_NAME];
    int nif;
    char email[MAX_CLIENT_EMAIL];
    int phone;
    struct {
        int day, month, year;
    } registration_date;
    int isActive; // 1 = active (shown in regular listings), 0 = inactive (kept for referential integrity)
} Client;

typedef struct {
    Client *clients;
    int capacity;
    int total;
    int nextId; // Next sequential, unique id to assign. Never reused, even after deletions.
} ClientList;


// Funções que usam ReservationList

//Functions

/**
 * @brief This function saves the record in clients, for example Create, List, Update and Remove.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @return This function does not return any value. Its only save the clients.
 */
void saveClients(ClientList *list);

/**
 * @brief This function load the record in clients, for example Create, List, Update and Remove.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @return This function does not return any value. Its only load the clients.
 */
void loadClients(ClientList *list);

/**
 * @brief This function realloc the clients.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @return This function does not return any value. Just Realloc the clients.
 */
void reallocClients(ClientList *list);

/**
 * @brief This function initializes the clients.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @return This function does not return any value. Just inicialize the clients.
 */
void turnOnClient(ClientList *list);

// Function Prototypes

/**
 * @brief This function initializes the clients List.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @return This function does not return any value. Just inicialize the clients list.
 */
void initializeClientList(ClientList *list, int capacity);

/**
 * @brief This function free the clients.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @return This function does not return any value. Just free the clients.
 */
void freeClientList(ClientList *list);

/**
 * @brief This function create the clients.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @return This function does not return any value. Just create the clients.
 */
void createClient(ClientList *list);

/**
 * @brief This function list the active clients.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @return This function does not return any value. Just list the clients. Inactive clients are hidden.
 */
void listClients(ClientList *list);

/**
 * @brief This function lists the inactive (soft-deleted) clients, so they can be reactivated if needed.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @return This function does not return any value. Just list the inactive clients.
 */
void listInactiveClients(ClientList *list);

/**
 * @brief This function find the clients.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @param id is used to find the id of the client.
 * @return This function does return the index of the client if found or -1 if client is not found.
 */
int findClient(ClientList *list, int id);

/**
 * @brief This function checks if a NIF is already registered by an active client.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @param nif is the fiscal number to check.
 * @return This function returns 1 if the NIF is already registered, or 0 otherwise.
 */
int nifExists(ClientList *list, int nif);

/**
 * @brief This function update the clients.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @return This function does not return any value. Just update the clients.
 */
void updateClient(ClientList *list);

/**
 * @brief This function reactivates a client previously marked as inactive.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @return This function does not return any value. Just reactivate the client.
 */
void reactivateClient(ClientList *list);

/**
 * @brief This function manually deactivates a client (marks it Inactive) without deleting it,
 * regardless of whether it has reservations. Useful to temporarily suspend a client.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @return This function does not return any value. Just deactivate the client.
 */
void deactivateClient(ClientList *list);

// Reports
/**
 * @brief This function show the total clients reports.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @return This function does not return any value. Just show the total clients reports.
 */
void totalClientsReport(ClientList *list);

/**
 * @brief This function show the total available clients.
 * @param ClientList are used to send the adress of the mains variables "list" to the following functions.
 * @return This function does not return any value. Just show the total available clients.
 */
void totalAvailableClients(ClientList *list);

#endif /* MENUCLIENT_H */
