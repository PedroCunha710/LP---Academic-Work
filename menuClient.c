/*
 * File:   menuClient.c
 * Author: Henrique Lopes
 *
 * Created on 20 de dezembro de 2024, 13:50
 *
 * @brief In this file we will declare the functions of the clients.
 * This file contains the functions developed by clients, such as CRUD and reports.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "menuClient.h"
#include "menuBooking.h"
#include "input.h"

#define CLIENT_FILE "clients.bin"

// Save client data to a file
void saveClients(ClientList *list) {
    if (list->total == 0) {
        puts("NO CLIENTS TO SAVE!");
        return;
    }

    FILE *fp = fopen(CLIENT_FILE, "wb");
    if (!fp) {
        perror("ERROR OPENING FILE");
        logMessage(LOG_ERROR, "CLIENT", "Failed to open clients.bin for writing.");
        exit(EXIT_FAILURE);
    }

    // Write total number of clients
    fwrite(&list->total, sizeof(int), 1, fp);

    // Write client data
    fwrite(list->clients, sizeof(Client), list->total, fp);

    fclose(fp);
    puts("CLIENTS SAVED SUCCESSFULLY!");
    logMessage(LOG_INFO, "CLIENT", "Clients saved to disk.");
}

// Recomputes the next unique/sequential id from the highest id currently in memory.
static void recalculateNextClientId(ClientList *list) {
    int maxId = 0;
    for (int i = 0; i < list->total; i++) {
        if (list->clients[i].id > maxId) {
            maxId = list->clients[i].id;
        }
    }
    list->nextId = maxId + 1;
}

// Load client data from a file
void loadClients(ClientList *list) {
    FILE *fp = fopen(CLIENT_FILE, "rb");
    if (!fp) {
        puts("NO CLIENT FILE FOUND! INITIALIZING EMPTY LIST.");
        initializeClientList(list, 10);
        return;
    }

    if (fread(&list->total, sizeof(int), 1, fp) != 1) {
        list->total = 0;
    }

    list->capacity = list->total > 0 ? list->total : 10;
    list->clients = malloc(list->capacity * sizeof(Client));
    if (!list->clients) {
        perror("ERROR ALLOCATING MEMORY");
        logMessage(LOG_ERROR, "CLIENT", "Memory allocation failed while loading clients.");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    if (fread(list->clients, sizeof(Client), list->total, fp) != (size_t)list->total) {
        printf("ERROR READING CLIENT LIST!");
        logMessage(LOG_ERROR, "CLIENT", "Error reading client list from disk.");
        free(list->clients);
        initializeClientList(list, 10);
        fclose(fp);
        return;
    }

    recalculateNextClientId(list);
    fclose(fp);
    puts("CLIENTS LOADED SUCCESSFULLY!");
    logMessage(LOG_INFO, "CLIENT", "Clients loaded from disk.");
}

// Function to reallocate memory for the client list
void reallocClients(ClientList *list) {
    if (list->total >= list->capacity) {
        list->capacity *= 2;
        Client *newList = realloc(list->clients, list->capacity * sizeof(Client));
        if (!newList) {
            perror("ERROR REALLOCATING MEMORY!");
            logMessage(LOG_ERROR, "CLIENT", "Memory reallocation failed.");
            free(list->clients);
            exit(EXIT_FAILURE);
        }
        list->clients = newList;
    }
}

void turnOnClient(ClientList *list){
    list->total=0;
    list->capacity=10;
    list->nextId=1;
    list->clients = malloc(list->capacity *sizeof(Client));
    if (list->clients==NULL){
        perror("ERROR ALLOCATING MEMORY FOR CLIENTS!");
        logMessage(LOG_ERROR, "CLIENT", "Memory allocation failed while initializing clients.");
        exit(EXIT_FAILURE);
    }
}

// Initialize client list
void initializeClientList(ClientList *list, int capacity) {
    list->clients =(Client *) malloc(capacity * sizeof(Client));
    if (list->clients == NULL) {
        perror("Error allocating memory for client list");
        logMessage(LOG_ERROR, "CLIENT", "Memory allocation failed while initializing client list.");
        exit(EXIT_FAILURE);
    }
    list->capacity = capacity;
    list->total = 0;
    list->nextId = 1;
}

// Free client list
void freeClientList(ClientList *list) {
    free(list->clients);
    list->clients = NULL;
    list->total = 0;
    list->capacity = 0;
}

// Find client by ID (searches both active and inactive clients)
int findClient(ClientList *list, int id) {
    for (int i = 0; i < list->total; i++) {
        if (list->clients[i].id == id) {  // Busca pelo ID
            return i;
        }
    }
    return -1;
}

// Checks if an active client is already registered with the given NIF
int nifExists(ClientList *list, int nif) {
    for (int i = 0; i < list->total; i++) {
        if (list->clients[i].isActive && list->clients[i].nif == nif) {
            return 1;
        }
    }
    return 0;
}

// Add a client
void createClient(ClientList *list) {
    reallocClients(list);

    cleanBuffer();  // Limpa o buffer antes de ler uma nova entrada

    char name[MAX_CLIENT_NAME];
    readStrg(name, MAX_CLIENT_NAME, MSG_GET_CLIENT_NAME);

    int nif = verify(100000000, 999999999, MSG_GET_CLIENT_NIF);
    if (nifExists(list, nif)) {
        puts(MSG_ERROR_CLIENT_EXISTS);
        logMessage(LOG_WARNING, "CLIENT", "Attempted to create client with a NIF already registered.");
        return;
    }

    char email[MAX_CLIENT_EMAIL];
    printf("%s", MSG_GET_CLIENT_EMAIL);
    scanf("%49s", email);

    int phone = verify(100000000, 999999999, MSG_GET_CLIENT_PHONE);

    Client *newClient = &list->clients[list->total];  // Novo cliente a ser inserido

    newClient->id = list->nextId++; // Unique and sequential: never reused, even after deletions.
    strcpy(newClient->name, name);
    newClient->nif = nif;
    strcpy(newClient->email, email);
    newClient->phone = phone;
    newClient->isActive = 1;

    newClient->registration_date.day = registDay();
    newClient->registration_date.month = registMonth();
    newClient->registration_date.year = registYear();

    list->total++;  // Incrementa o total de clientes
    puts("CLIENT CREATED SUCCESSFULLY!");
    logMessage(LOG_INFO, "CLIENT", "Client created.");
}


// List active clients
void listClients(ClientList *list) {
    if (list->total == 0) {
        puts(MSG_ERROR_EMPTY_LIST);
        return;
    }

    int shown = 0;
    for (int i = 0; i < list->total; i++) {
        Client *client = &list->clients[i];
        if (!client->isActive) {
            continue; // Inactive clients do not appear in regular listings.
        }
        shown++;
        printf("Client %d:\n", client->id);
        printf("  Name: %s\n", client->name);
        printf("  NIF: %d\n", client->nif);
        printf("  Email: %s\n", client->email);
        printf("  Phone: %d\n", client->phone);
        printf("  Registration Date: %02d/%02d/%04d\n",
               client->registration_date.day,
               client->registration_date.month,
               client->registration_date.year);
    }
    if (shown == 0) {
        puts("NO ACTIVE CLIENTS TO SHOW!");
    }
}

// List inactive (soft-deleted) clients, so they can be reactivated if needed
void listInactiveClients(ClientList *list) {
    int shown = 0;
    puts("-----INACTIVE CLIENTS-----");
    for (int i = 0; i < list->total; i++) {
        Client *client = &list->clients[i];
        if (client->isActive) {
            continue;
        }
        shown++;
        printf("Client %d: %s | NIF: %d\n", client->id, client->name, client->nif);
    }
    if (shown == 0) {
        puts("NO INACTIVE CLIENTS!");
    }
}

// Update client
void updateClient(ClientList *list) {
    int id = verify(1, 100000, MSG_GET_CLIENT_ID);

    int index = findClient(list, id);
    if (index == -1) {
        puts("Client not found!");
        return;
    }

    Client *client = &list->clients[index];
    printf("Updating information for client %s:\n", client->name);
    cleanBuffer();

    char newName[MAX_CLIENT_NAME];
    readStrg(newName, MAX_CLIENT_NAME, "NEW NAME (LEAVE EMPTY TO KEEP): ");
    if (strlen(newName) > 0) {
        strcpy(client->name, newName);
    }

    char newEmail[MAX_CLIENT_EMAIL];
    printf("%s", "NEW EMAIL (LEAVE EMPTY TO KEEP): ");
    scanf("%49s", newEmail);
    if (strlen(newEmail) > 0 && strcmp(newEmail, ".") != 0) {
        strcpy(client->email, newEmail);
    }

    int newPhone = verify(-1, 999999999, "NEW PHONE (-1 TO KEEP): ");
    if (newPhone >= 0) {
        client->phone = newPhone;
    }

    puts("Client updated successfully!");
    logMessage(LOG_INFO, "CLIENT", "Client updated.");
}

// Remove client
// If the client has reservations pointing to it, it is marked Inactive instead of being
// physically removed, to preserve referential integrity (per assignment requirements).
void removeClient(ClientList *list, ReservationList *reservations) {
    int id = verify(1, 100000, MSG_GET_CLIENT_ID);

    int index = findClient(list, id);
    if (index == -1) {
        puts("Client not found!");
        return;
    }

    if (!list->clients[index].isActive) {
        puts("THIS CLIENT IS ALREADY INACTIVE!");
        return;
    }

    if (clientHasReservations(reservations, id)) {
        list->clients[index].isActive = 0;
        printf("CLIENT %s HAS RESERVATIONS ASSOCIATED, SO IT WAS MARKED AS INACTIVE INSTEAD OF DELETED.\n", list->clients[index].name);
        logMessage(LOG_INFO, "CLIENT", "Client marked as inactive (has associated reservations).");
        return;
    }

    // Remove o cliente deslocando os elementos
    for (int i = index; i < list->total - 1; i++) {
        list->clients[i] = list->clients[i + 1];  // Desloca o cliente para a posição anterior
    }

    list->total--;  // Diminui o total de clientes
    puts("Client removed successfully!");
    logMessage(LOG_INFO, "CLIENT", "Client deleted.");
}

// Reactivate a client previously marked as inactive
void reactivateClient(ClientList *list) {
    int id = verify(1, 100000, MSG_GET_CLIENT_ID);

    int index = findClient(list, id);
    if (index == -1) {
        puts("Client not found!");
        return;
    }

    if (list->clients[index].isActive) {
        puts("THIS CLIENT IS ALREADY ACTIVE!");
        return;
    }

    list->clients[index].isActive = 1;
    puts("CLIENT REACTIVATED SUCCESSFULLY!");
    logMessage(LOG_INFO, "CLIENT", "Client reactivated.");
}

// Manually deactivates a client (marks it Inactive) without deleting it
void deactivateClient(ClientList *list) {
    int id = verify(1, 100000, MSG_GET_CLIENT_ID);

    int index = findClient(list, id);
    if (index == -1) {
        puts("Client not found!");
        return;
    }

    if (!list->clients[index].isActive) {
        puts("THIS CLIENT IS ALREADY INACTIVE!");
        return;
    }

    list->clients[index].isActive = 0;
    puts("CLIENT DEACTIVATED SUCCESSFULLY!");
    logMessage(LOG_INFO, "CLIENT", "Client manually deactivated.");
}

// Reports
void totalClientsReport(ClientList *list) {
    printf("Total registered clients: %d\n", list->total);
}


// Relatório de Total de Clientes
void totalAvailableClients(ClientList *list) {
    puts("----------TOTAL CLIENTS----------");
    int active = 0;
    for (int i = 0; i < list->total; i++) {
        if (list->clients[i].isActive) {
            active++;
        }
    }
    printf("Clients Available: %d\n", active);
}

// Relatório de Clientes com Mais Reservas
void mostActiveClient(ClientList *clientList, ReservationList *reservations) {
    int i, j;
    int maxReservations = 0;
    int mostActiveClientID = -1;

    // Check if there are any clients or reservations in the system
    if (clientList->total == 0) {
        printf("No clients registered.\n");
        return;
    }
    if (reservations->total == 0) {
        printf("No reservations recorded.\n");
        return;
    }

    // Iterate through the client array and store the position of the client
    for (i = 0; i < clientList->total; i++) {
        if (!clientList->clients[i].isActive) {
            continue;
        }
        int clientReservations = 0;
        int clientID = clientList->clients[i].id;  // Access the client's ID

        // Count the reservations made by this client
        for (j = 0; j < reservations->total; j++) {
            // Check if the reservation was made by the current client ID
            if (reservations->reservations[j].client_id == clientID) {
                clientReservations++;
            }
        }

        // Check if this client has the most reservations
        if (clientReservations > maxReservations) {
            maxReservations = clientReservations;
            mostActiveClientID = clientID;
        }
    }

    // Display the most active client
    if (mostActiveClientID != -1) {
        printf("\nMost active client:\n");
        printf("Client ID: %d\n", mostActiveClientID);
        printf("Total reservations: %d\n", maxReservations);
    } else {
        printf("There are no reservations associated with clients.\n");
    }
}

// Relatório de Clientes com Menos Reservas
void leastReservationsClient(ClientList *clients, ReservationList *reservations) {
    if (clients->total == 0) {
        puts("NO CLIENTS AVAILABLE!!");
        return;
    }

    if (reservations->total == 0) {
        puts("NO RESERVATIONS AVAILABLE!!");
        return;
    }

    int minReservations = INT_MAX;
    Client *leastReserved = NULL;

    for (int i = 0; i < clients->total; i++) {
        if (!clients->clients[i].isActive) {
            continue;
        }
        int clientReservations = 0;

        // Contar reservas associadas ao cliente atual
        for (int j = 0; j < reservations->total; j++) {
            if (reservations->reservations[j].client_id == clients->clients[i].id) {
                clientReservations++;
            }
        }

        // Atualizar cliente com menos reservas
        if (clientReservations < minReservations) {
            minReservations = clientReservations;
            leastReserved = &clients->clients[i];
        }
    }

    if (leastReserved != NULL) {
        puts("----------LEAST RESERVATIONS CLIENT----------");
        printf("NAME: %s | RESERVATIONS: %d\n", leastReserved->name, minReservations);
    } else {
        puts("NO CLIENTS WITH RESERVATIONS FOUND.");
    }
}

// Relatório detalhado: para cada cliente, total de reservas e espaços utilizados
void clientReservationDetails(ClientList *clients, ReservationList *reservations, geralSpaces *spaces) {
    if (clients->total == 0) {
        puts("NO CLIENTS AVAILABLE!!");
        return;
    }

    puts("----------CLIENT RESERVATION DETAILS----------");
    for (int i = 0; i < clients->total; i++) {
        Client *client = &clients->clients[i];
        if (!client->isActive) {
            continue;
        }

        int total = 0;
        printf("CLIENT: %s (ID %d)\n", client->name, client->id);

        for (int j = 0; j < reservations->total; j++) {
            Reservation *reservation = &reservations->reservations[j];
            if (reservation->client_id != client->id) {
                continue;
            }
            total++;

            const char *spaceName = "UNKNOWN SPACE";
            int spaceIndex = findSpace(spaces, reservation->space_id);
            if (spaceIndex != -1) {
                spaceName = spaces->spaceList[spaceIndex].name;
            }
            printf("  RESERVATION %d | SPACE: %s\n", reservation->reservation_id, spaceName);
        }

        printf("  TOTAL RESERVATIONS: %d\n\n", total);
    }
}
