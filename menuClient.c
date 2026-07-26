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
        exit(EXIT_FAILURE);
    }

    // Write total number of clients
    fwrite(&list->total, sizeof(int), 1, fp);

    // Write client data
    fwrite(list->clients, sizeof(Client), list->total, fp);

    fclose(fp);
    puts("CLIENTS SAVED SUCCESSFULLY!");
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
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    if (fread(list->clients, sizeof(Client), list->total, fp) != (size_t)list->total) {
        printf("ERROR READING CLIENT LIST!");
        free(list->clients);
        initializeClientList(list, 10);
    }

    fclose(fp);
    puts("CLIENTS LOADED SUCCESSFULLY!");
}

// Function to reallocate memory for the client list
void reallocClients(ClientList *list) {
    if (list->total >= list->capacity) {
        list->capacity *= 2;
        Client *newList = realloc(list->clients, list->capacity * sizeof(Client));
        if (!newList) {
            perror("ERROR REALLOCATING MEMORY!");
            free(list->clients);
            exit(EXIT_FAILURE);
        }
        list->clients = newList;
    }
}

void turnOnClient(ClientList *list){
    list->total=0;
    list->capacity=10;
    list->clients = malloc(list->capacity *sizeof(ClientList));
    if (list->clients==NULL){
        perror("ERROR ALLOCATING MEMORY FOR CLIENTS!");
        exit(EXIT_FAILURE);
    }
}

// Initialize client list
void initializeClientList(ClientList *list, int capacity) {
    list->clients =(Client *) malloc(capacity * sizeof(Client));
    if (list->clients == NULL) {
        perror("Error allocating memory for client list");
        exit(EXIT_FAILURE);
    }
    list->capacity = capacity;
    list->total = 0;
}

// Free client list
void freeClientList(ClientList *list) {
    free(list->clients);
    list->clients = NULL;
    list->total = 0;
    list->capacity = 0;
}

// Add a client
// Função para criar um novo cliente
void createClient(ClientList *list) {
    if (list->total == list->capacity) {
        list->capacity *= 2;  // Aumenta a capacidade se necessário
        list->clients = realloc(list->clients, list->capacity * sizeof(Client));
        if (list->clients == NULL) {
            perror("Error resizing memory for client list");
            return;
        }
    }

    Client *newClient = &list->clients[list->total];  // Novo cliente a ser inserido
    
    // Atribui o ID de forma sequencial, começando de 1
    newClient->id = list->total + 1;

    cleanBuffer();  // Limpa o buffer antes de ler uma nova entrada

    printf(MSG_GET_CLIENT_NAME);
    fgets(newClient->name, MAX_CLIENT_NAME, stdin);
    newClient->name[strcspn(newClient->name, "\n")] = '\0';  // Remove o '\n' do final do nome

    printf(MSG_GET_CLIENT_NIF);
    scanf("%d", &newClient->nif);

    // Verifica se o cliente já existe com o mesmo NIF (identificador único para cada cliente)
    

    printf(MSG_GET_CLIENT_EMAIL);
    scanf("%s", newClient->email);

    printf(MSG_GET_CLIENT_PHONE);
    scanf("%d", &newClient->phone);

    newClient->registration_date.day = registDay();  // Preenche a data de registro (placeholder)
    newClient->registration_date.month = registMonth();  // Preenche a data de registro (placeholder)
    newClient->registration_date.year = registYear();  // Preenche a data de registro (placeholder)

    list->total++;  // Incrementa o total de clientes
}


// List clients
void listClients(ClientList *list) {
    if (list->total == 0) {
        puts(MSG_ERROR_EMPTY_LIST);
        return;
    }

    for (int i = 0; i < list->total; i++) {
        Client *client = &list->clients[i];
        printf("Client %d:\n", i + 1);
        printf("  Name: %s\n", client->name);
        printf("  NIF: %d\n", client->nif);
        printf("  Email: %s\n", client->email);
        printf("  Phone: %d\n", client->phone);
        printf("  Registration Date: %02d/%02d/%04d\n",
               client->registration_date.day,
               client->registration_date.month,
               client->registration_date.year);
        printf("  ID: %d\n", client->id);
    }
}

// Find client by ID
int findClient(ClientList *list, int id) {
    for (int i = 0; i < list->total; i++) {
        if (list->clients[i].id == id) {  // Busca pelo ID
            puts("CLIENT FOUND!");
            return i;
        }
    }
    puts("ID NOT FOUND!");  // Se o ID não for encontrado
    return -1;
}

// Update client
void updateClient(ClientList *list) {
    int id;
    printf(MSG_GET_CLIENT_ID);
    scanf("%d", &id);

    int index = findClient(list, id);
    if (index == -1) {
        puts("Client not found!");
        return;
    }

    Client *client = &list->clients[index];
    printf("Updating information for client %s:\n", client->name);
    cleanBuffer();
    printf(MSG_GET_CLIENT_NAME);
    fgets(client->name, MAX_CLIENT_NAME, stdin);
    client->name[strcspn(client->name, "\n")] = '\0'; // Remove '\n'

    printf(MSG_GET_CLIENT_EMAIL);
    scanf("%s", client->email);

    printf(MSG_GET_CLIENT_PHONE);
    scanf("%d", &client->phone);

    puts("Client updated successfully!");
}

// Remove client
void removeClient(ClientList *list) {
    int id;
    printf(MSG_GET_CLIENT_ID);  // Solicita o ID do cliente
    scanf("%d", &id);

    int index = findClient(list, id);  // Procura pelo ID
    if (index == -1) {
        puts("Client not found!");  // Se não encontrar o cliente
        return;
    }

    // Remove o cliente deslocando os elementos
    for (int i = index; i < list->total - 1; i++) {
        list->clients[i] = list->clients[i + 1];  // Desloca o cliente para a posição anterior
    }

    list->total--;  // Diminui o total de clientes
    puts("Client removed successfully!");
}

// Reports
void totalClientsReport(ClientList *list) {
    printf("Total registered clients: %d\n", list->total);
}


// Relatório de Total de Clientes
void totalAvailableClients(ClientList *list) {
    puts("----------TOTAL CLIENTS----------");
    printf("Clients Available: %d", list->total);
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

    int minReservations = __INT_MAX__;
    Client *leastReserved = NULL;

    for (int i = 0; i < clients->total; i++) {
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
