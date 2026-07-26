/* 
 * File:   menuBooking.c
 * Author: Henrique Lopes
 *
 * Created on 20 de dezembro de 2024, 13:50
 * 
 * @brief In this file we will declare the functions of the reservations.
 * This file contains the functions developed by reservations, such as CRUD and reports.
 */

#include <stdio.h>
#include <stdlib.h>
#include "menus.h"
#include "input.h"
#include "menuEquipment.h"
#include "menuSpace.h"
#include "menuClient.h"    
#include "menuBooking.h"

#define BOOKING_FILE "reservations.bin"


// Initialize the reservation list
void initializeReservationList(ReservationList *list, int initialCapacity) {
    list->reservations = malloc(initialCapacity * sizeof(Reservation));
    if (!list->reservations) {
        perror("Error allocating memory for the reservation list.");
        exit(EXIT_FAILURE);
    }
    list->capacity = initialCapacity;
    list->total = 0;
}

// Free the memory allocated for the reservation list
void freeReservationList(ReservationList *list) {
    free(list->reservations);
    list->reservations = NULL;
    list->capacity = 0;
    list->total = 0;
}
void reallocReservations(ReservationList *list) {
    if (list->total == list->capacity) {
        int newCapacity = list->capacity > 0 ? list->capacity * 2 : 1;
        list->reservations = realloc(list->reservations, newCapacity * sizeof(Reservation));
        if (!list->reservations) {
            perror("Erro ao redimensionar a lista de reservas");
            exit(EXIT_FAILURE);
        }
        list->capacity = newCapacity;
    }
}

void turnOnBooking(ReservationList *list){
    list->total=0;
    list->capacity=10;
    list->reservations = malloc(list->capacity *sizeof(Reservation));
    if (list->reservations==NULL){
        perror("ERROR ALLOCATING MEMORY FOR RESERVATIONS!");
        exit(EXIT_FAILURE);
    }
}

int findSpace(geralSpaces *spaces, int space_id) {
    for (int i = 0; i < spaces->counter; i++) {
        if (spaces->spaceList[i].id == space_id) {
            return i;
        }
    }
    return -1;
}

// Add a reservation to the list

void createReservation(ReservationList *reservations, ClientList *clients, geralSpaces *spaces, equipmentManager *equipments) {
    // Verificar se há espaço suficiente para uma nova reserva
    reallocReservations(reservations);

    // Alocar espaço para a nova reserva
    Reservation *newReservation = &reservations->reservations[reservations->total];

    // Atribuir um ID único para a reserva (baseado no contador total)
    newReservation->reservation_id = reservations->total + 1; // ou use qualquer outro método para atribuir um ID único

    // Limpar o buffer para a entrada de dados
    cleanBuffer();

    // Obter o ID do cliente
    printf("CLIENT ID: ");
    if (scanf("%d", &newReservation->client_id) != 1 || newReservation->client_id <= 0) {
        fprintf(stderr, "ERROR: INVALID CLIENT ID.\n");
        return;
    }

    // Verificar se o cliente existe
    if (findClient(clients, newReservation->client_id) == -1) {
        fprintf(stderr, "ERROR: CLIENT NOT FOUND.\n");
        return;
    }

    // Obter o ID do espaço
    printf("SPACE ID: ");
    if (scanf("%d", &newReservation->space_id) != 1 || newReservation->space_id <= 0) {
        fprintf(stderr, "ERROR: INVALID SPACE ID.\n");
        return;
    }

    // Verificar se o espaço existe
    if (findSpace(spaces, newReservation->space_id) == -1) {
        fprintf(stderr, "ERROR: SPACE NOT FOUND.\n");
        return;
    }
    
    // Verificar se o espaço já está reservado
    if (spaceOccupied(spaces, newReservation->space_id)) {
        fprintf(stderr, "ERROR: THIS SPACE IS ALREADY RESERVED.\n");
        return;
    }

 // Obter o ID do equipamento
    printf("EQUIPMENT ID: ");
    if (scanf("%d", &newReservation->equipment_id) == -1) {
        fprintf(stderr, "ERROR: INVALID EQUIPMENT ID.\n");
        return;
    }

    // Verificar se o equipamento existe
    int equipmentIndex = -1;
    for (int j = 0; j < equipments->counter; j++) {
        if (equipments->equipmentList[j].id == newReservation->equipment_id) {
            equipmentIndex = j;
            break;
        }
    }

    if (equipmentIndex == -1) {
        fprintf(stderr, "ERROR: EQUIPMENT NOT FOUND.\n");
        return;
    }
    
      // Verificar se o equipamento está em manutenção
    if (equipments->equipmentList[equipmentIndex].status == UNDER_MAINTENANCE) {
        fprintf(stderr, "ERROR: EQUIPMENT IS UNDER MAINTENANCE AND CANNOT BE RESERVED.\n");
        return;
    }

    // Verificar se o equipamento já foi completamente reservado
    if (equipments->equipmentList[equipmentIndex].quantity - equipments->equipmentList[equipmentIndex].usingEquipment == 0) {
        fprintf(stderr, "ERROR: THIS EQUIPMENT IS ALREADY FULLY RESERVED.\n");
        return;
    }

    // Solicitar o número de equipamentos a serem reservados
    printf("NUMBER OF EQUIPMENTS TO RESERVE (MAX %d): ", equipments->equipmentList[equipmentIndex].quantity);
    if (scanf("%d", &newReservation->equipment_count) != 1 || newReservation->equipment_count < 0 || 
        newReservation->equipment_count > (equipments->equipmentList[equipmentIndex].quantity - equipments->equipmentList[equipmentIndex].usingEquipment)) {
        fprintf(stderr, "ERROR: INVALID EQUIPMENT COUNT.\n");
        return;
    }

    // Atualiza a quantidade disponível de equipamentos
    equipments->equipmentList[equipmentIndex].usingEquipment += newReservation->equipment_count;

    // Verificar se a quantidade de equipamentos ficou completamente reservada
    if (equipments->equipmentList[equipmentIndex].usingEquipment == equipments->equipmentList[equipmentIndex].quantity) {
        equipments->equipmentList[equipmentIndex].equipmentReserved = 1;
        equipments->equipmentList[equipmentIndex].status = RESERVED; // Alterar o status para RESERVED
    printf("SUCCESS: EQUIPMENT FULLY RESERVED.\n");
} else {
    printf("SUCCESS: EQUIPMENT RESERVED. REMAINING QUANTITY: %d\n", 
           equipments->equipmentList[equipmentIndex].quantity - equipments->equipmentList[equipmentIndex].usingEquipment);// Marca como totalmente reservado
    }

    // Obter a data e hora da reserva
    printf("RESERVATION DATE (DD MM YYYY HH MM): ");
    if (scanf("%d %d %d %d %d", &newReservation->date.day, &newReservation->date.month, &newReservation->date.year,
              &newReservation->date.hour, &newReservation->date.minute) != 5) {
        fprintf(stderr, "ERROR: INVALID DATE FORMAT.\n");
        return;
    }

    // Validar a duração da reserva (1 a 24 horas)
    newReservation->duration_hours = verify(1, 24, "DURATION (1 TO 24 HOURS):");

    // Escolher o status da reserva
    puts("RESERVATION STATUS:\n1 - PENDING\n2 - CONFIRMED\n3 - CANCELED");
    newReservation->status = verify(1, 3, "STATUS:");

    // Obter o número de participantes
    newReservation->participants = verify(1, 200, "NUMBER OF PARTICIPANTS (1 TO 200):");
    
    // Marcar o espaço como reservado
    for (int i = 0; i < spaces->counter; i++) {
        if (spaces->spaceList[i].id == newReservation->space_id) {
            spaces->spaceList[i].spaceOccupied = 1; // Marca o espaço como reservado
            spaces->spaceList[i].reservations++;
            break;
        }
    }

    // Atualizar o contador de reservas
    reservations->total++;

    // Realocar memória para a lista de reservas (caso necessário)
    reallocReservations(reservations);

    // Mensagem de sucesso
    puts("\nRESERVATION CREATED SUCCESSFULLY!");
}

void cancelReservation(ReservationList *reservations, geralSpaces *spaces, equipmentManager *equipments, int reservation_id) {
    int index = searchReservation(reservations, reservation_id);
    if (index == -1) {
        printf("Reservation not found.\n");
        return;
    }

    Reservation *reservation = &reservations->reservations[index];

    // Alterar o status do espaço para disponível
    for (int i = 0; i < spaces->counter; i++) {
        if (spaces->spaceList[i].id == reservation->space_id) {
            spaces->spaceList[i].spaceOccupied = 0; // Espaço liberado
            if (spaces->spaceList[i].reservations > 0) {
                spaces->spaceList[i].reservations--; // Reduz o número de reservas associadas
            }
            break;
        }
    }

    // Alterar o status dos equipamentos para disponíveis
    for (int i = 0; i < equipments->counter; i++) {
        if (equipments->equipmentList[i].id == reservation->equipment_id) {
            equipments->equipmentList[i].usingEquipment -= reservation->equipment_count;

            // Verificar se todos os equipamentos estão liberados
            if (equipments->equipmentList[i].usingEquipment == 0) {
                equipments->equipmentList[i].status = AVALIABLE;
                printf("Equipment ID %d is now fully available.\n", equipments->equipmentList[i].id);
            }
            break;
        }
    }

    // Atualizar status da reserva para cancelada
    reservation->status = STATUS_CANCELED;

    printf("Reservation canceled, and associated space and equipment are now available.\n");
}

// List all reservations
void listReservations(ReservationList *list) {
    if (list->total == 0) {
        printf("No reservations registered.\n");
        return;
    }

    for (int i = 0; i < list->total; i++) {
        Reservation *r = &list->reservations[i];
        printf("Reservation ID: %d, Client ID: %d, Space ID: %d, Equipment ID: %d, Date: %02d/%02d/%04d %02d:%02d, "
               "Duration: %d hours, Status: %d, Participants: %d\n",
               r->reservation_id, r->client_id, r->space_id,r->equipment_id, r->date.day, r->date.month,
               r->date.year, r->date.hour, r->date.minute, r->duration_hours, r->status,
               r->participants);
    }
}

// Find a reservation by ID
int searchReservation(ReservationList *list, int reservation_id) {
    for (int i = 0; i < list->total; i++) {
        if (list->reservations[i].reservation_id == reservation_id) {
            return i;
        }
    }
    return -1;
}

// Update reservation details
void updateReservation(ReservationList *list, int reservation_id) {
   
    reservation_id = verify(1, 1000, "ENTER RESERVATION TO UPDATE: ");
     
    int index = searchReservation(list, reservation_id);
    if (index == -1) {
        printf("Reservation with ID %d not found.\n", reservation_id);
        return;
    }

    Reservation *reservation = &list->reservations[index];
    printf("Updating reservation ID %d:\n", reservation->reservation_id);
    printf("New Status (1.Pending, 2.Confirmed, 3.Canceled): ");
    scanf("%d", &reservation->status);
    
    cleanBuffer();
    printf("Update successful!\n");
}

// Remove a reservation
void deleteReservation(ReservationList *reservations,geralSpaces *spaces, equipmentManager *equipments, int reservation_id) {
    
    reservation_id = verify(1, 1000, "ENTER RESERVATION TO DELETE: ");
    
    int index = searchReservation(reservations, reservation_id);
    if (index == -1) {
        printf("Reservation with ID %d not found.\n", reservation_id);
        return;
    }

    for (int i = index; i < reservations->total - 1; i++) {
        reservations->reservations[i] = reservations->reservations[i + 1];
    }
    
    cancelReservation(reservations,spaces,equipments,reservation_id);
    reservations->total--;
    cleanBuffer();
    printf("Reservation removed successfully!\n");
}

// List reservations by status
void listReservationsByStatus(ReservationList *list, int status) {
    printf("Reservations with status %d:\n", status);
    for (int i = 0; i < list->total; i++) {
        if (list->reservations[i].status == status) {
            printf("Reservation ID: %d\n", list->reservations[i].reservation_id);
        }
    }
}

// Save reservations to a binary file
void saveReservations(ReservationList *list) {
    FILE *file = fopen(BOOKING_FILE, "wb");
    if (!file) {
        perror("Error opening file to save reservations.");
        return;
    }

    fwrite(&list->total, sizeof(int), 1, file);
    fwrite(list->reservations, sizeof(Reservation), list->total, file);

    fclose(file);
    printf("Reservations saved successfully to '%s'.\n", BOOKING_FILE);
}

// Load reservations from a binary file
void loadReservations(ReservationList *reservations) {
    // Abrindo o arquivo binário
    FILE *fp = fopen(BOOKING_FILE, "rb");
    
    if (!fp) {
        puts("NO RESERVATION FILE FOUND! INITIALIZING EMPTY LIST");
        initializeReservationList(reservations, 10); // Inicializa com capacidade padrão
        return;
    }

    // Lendo o contador de reservas (total)
    if (fread(&reservations->total, sizeof(int), 1, fp) != 1) {
        perror("ERROR READING RESERVATION COUNT");
        initializeReservationList(reservations, 10); // Inicializa uma lista vazia
        fclose(fp);
        return;
    }

    // Definindo a capacidade inicial
    reservations->capacity = reservations->total > 0 ? reservations->total : 10;
    reservations->reservations = malloc(reservations->capacity * sizeof(Reservation));
    
    if (!reservations->reservations) {
        perror("ERROR ALLOCATING MEMORY FOR RESERVATIONS");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    // Lendo as reservas do arquivo
    if (fread(reservations->reservations, sizeof(Reservation), reservations->total, fp) != (size_t)reservations->total) {
        puts("ERROR READING RESERVATION LIST! INITIALIZING EMPTY LIST");
        free(reservations->reservations);
        initializeReservationList(reservations, 10); // Inicializa uma lista vazia
    } else {
        puts("RESERVATIONS LOADED SUCCESSFULLY!");
    }

    fclose(fp);
}


void totalReservations(ReservationList *reservations) {
    puts("----------TOTAL RESERVATIONS----------");
    printf("Total Reservations: %d\n", reservations->total);
}

void reservationsByStatus(ReservationList *reservations) {
    if (reservations->total == 0) {
        puts("NO RESERVATIONS AVAILABLE!");
        return;
    }

    int statusCount[4] = {0}; // Array to count reservations by status (index 0 unused)
    for (int i = 0; i < reservations->total; i++) {
        statusCount[reservations->reservations[i].status]++;
    }

    // Report results
    puts("----------RESERVATIONS BY STATUS----------");
    printf("PENDING: %d\n", statusCount[STATUS_PENDING]);
    printf("CONFIRMED: %d\n", statusCount[STATUS_CONFIRMED]);
    printf("CANCELED: %d\n", statusCount[STATUS_CANCELED]);
}
void mostParticipantsReservation(ReservationList *reservations) {
    if (reservations->total == 0) {
        puts("NO RESERVATIONS AVAILABLE!");
        return;
    }

    Reservation *mostParticipants = &reservations->reservations[0];
    for (int i = 1; i < reservations->total; i++) {
        if (reservations->reservations[i].participants > mostParticipants->participants) {
            mostParticipants = &reservations->reservations[i];
        }
    }

    puts("----------RESERVATION WITH MOST PARTICIPANTS----------");
    printf("CLIENT ID: %d | PARTICIPANTS: %d\n", mostParticipants->client_id, mostParticipants->participants);
}

void leastParticipantsReservation(ReservationList *reservations) {
    if (reservations->total == 0) {
        puts("NO RESERVATIONS AVAILABLE!");
        return;
    }

    Reservation *leastParticipants = &reservations->reservations[0];
    for (int i = 1; i < reservations->total; i++) {
        if (reservations->reservations[i].participants < leastParticipants->participants) {
            leastParticipants = &reservations->reservations[i];
        }
    }

    puts("----------RESERVATION WITH LEAST PARTICIPANTS----------");
    printf("CLIENT ID: %d | PARTICIPANTS: %d\n", leastParticipants->client_id, leastParticipants->participants);
}
