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
        logMessage(LOG_ERROR, "RESERVATION", "Memory allocation failed while initializing reservation list.");
        exit(EXIT_FAILURE);
    }
    list->capacity = initialCapacity;
    list->total = 0;
    list->nextId = 1;
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
            logMessage(LOG_ERROR, "RESERVATION", "Memory reallocation failed.");
            exit(EXIT_FAILURE);
        }
        list->capacity = newCapacity;
    }
}

void turnOnBooking(ReservationList *list){
    list->total=0;
    list->capacity=10;
    list->nextId=1;
    list->reservations = malloc(list->capacity *sizeof(Reservation));
    if (list->reservations==NULL){
        perror("ERROR ALLOCATING MEMORY FOR RESERVATIONS!");
        logMessage(LOG_ERROR, "RESERVATION", "Memory allocation failed while initializing reservations.");
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

int spaceHasReservations(ReservationList *reservations, int space_id) {
    for (int i = 0; i < reservations->total; i++) {
        if (reservations->reservations[i].space_id == space_id) {
            return 1;
        }
    }
    return 0;
}

int clientHasReservations(ReservationList *reservations, int client_id) {
    for (int i = 0; i < reservations->total; i++) {
        if (reservations->reservations[i].client_id == client_id) {
            return 1;
        }
    }
    return 0;
}

int equipmentHasReservations(ReservationList *reservations, int equipment_id) {
    for (int i = 0; i < reservations->total; i++) {
        Reservation *reservation = &reservations->reservations[i];
        for (int k = 0; k < reservation->equipmentItemCount; k++) {
            if (reservation->equipmentItems[k].equipment_id == equipment_id) {
                return 1;
            }
        }
    }
    return 0;
}

// Add a reservation to the list.
// All inputs are validated into local variables first; the reservation and every side effect
// (equipment usage counters, space occupancy) are only committed once everything is valid, so a
// failed/aborted creation never leaves equipment or spaces in an inconsistent state.
void createReservation(ReservationList *reservations, ClientList *clients, geralSpaces *spaces, equipmentManager *equipments) {
    reallocReservations(reservations);

    cleanBuffer();

    // CLIENT
    int client_id;
    printf("CLIENT ID: ");
    if (scanf("%d", &client_id) != 1 || client_id <= 0) {
        fprintf(stderr, "ERROR: INVALID CLIENT ID.\n");
        cleanBuffer();
        return;
    }
    int clientIndex = findClient(clients, client_id);
    if (clientIndex == -1) {
        fprintf(stderr, "ERROR: CLIENT NOT FOUND.\n");
        return;
    }
    if (!clients->clients[clientIndex].isActive) {
        fprintf(stderr, "ERROR: CLIENT IS INACTIVE AND CANNOT MAKE RESERVATIONS.\n");
        return;
    }

    // SPACE
    int space_id;
    printf("SPACE ID: ");
    if (scanf("%d", &space_id) != 1 || space_id <= 0) {
        fprintf(stderr, "ERROR: INVALID SPACE ID.\n");
        cleanBuffer();
        return;
    }
    int spaceIndex = findSpace(spaces, space_id);
    if (spaceIndex == -1) {
        fprintf(stderr, "ERROR: SPACE NOT FOUND.\n");
        return;
    }
    if (!spaces->spaceList[spaceIndex].isActive) {
        fprintf(stderr, "ERROR: SPACE IS INACTIVE AND CANNOT BE RESERVED.\n");
        return;
    }
    if (spaceOccupied(spaces, space_id)) {
        fprintf(stderr, "ERROR: THIS SPACE IS ALREADY RESERVED.\n");
        return;
    }

    // EQUIPMENT (0 to MAX_EQUIPMENT_PER_RESERVATION different types, each with its own quantity)
    ReservationEquipmentItem tempItems[MAX_EQUIPMENT_PER_RESERVATION];
    int tempCount = 0;
    int equipmentTypesWanted = verify(0, MAX_EQUIPMENT_PER_RESERVATION,
        "HOW MANY DIFFERENT EQUIPMENT TYPES DO YOU WANT TO RESERVE (0 TO SKIP)? ");

    for (int k = 0; k < equipmentTypesWanted; k++) {
        int equipmentId = verify(1, 100000, "EQUIPMENT ID: ");
        int equipmentIndex = findEquipment(equipments, equipmentId);

        if (equipmentIndex == -1) {
            printf("ERROR: EQUIPMENT NOT FOUND. THIS ITEM WILL BE SKIPPED.\n");
            continue;
        }
        if (!equipments->equipmentList[equipmentIndex].isActive) {
            printf("ERROR: EQUIPMENT IS INACTIVE. THIS ITEM WILL BE SKIPPED.\n");
            continue;
        }
        if (equipments->equipmentList[equipmentIndex].status == UNDER_MAINTENANCE) {
            printf("ERROR: EQUIPMENT IS UNDER MAINTENANCE. THIS ITEM WILL BE SKIPPED.\n");
            continue;
        }

        // Account for other items in this same reservation already reserving the same equipment.
        int alreadyStaged = 0;
        for (int j = 0; j < tempCount; j++) {
            if (tempItems[j].equipment_id == equipmentId) {
                alreadyStaged += tempItems[j].quantity;
            }
        }

        int available = equipments->equipmentList[equipmentIndex].quantity
                       - equipments->equipmentList[equipmentIndex].usingEquipment
                       - alreadyStaged;
        if (available <= 0) {
            printf("ERROR: THIS EQUIPMENT IS ALREADY FULLY RESERVED. THIS ITEM WILL BE SKIPPED.\n");
            continue;
        }

        char prompt[64];
        snprintf(prompt, sizeof(prompt), "QUANTITY TO RESERVE (MAX %d): ", available);
        int quantity = verify(1, available, prompt);

        if (tempCount < MAX_EQUIPMENT_PER_RESERVATION) {
            tempItems[tempCount].equipment_id = equipmentId;
            tempItems[tempCount].quantity = quantity;
            tempCount++;
        }
    }

    // DATE AND TIME
    DateTime date;
    printf("RESERVATION DATE (DD MM YYYY HH MM): ");
    if (scanf("%d %d %d %d %d", &date.day, &date.month, &date.year, &date.hour, &date.minute) != 5) {
        fprintf(stderr, "ERROR: INVALID DATE FORMAT.\n");
        cleanBuffer();
        return;
    }
    cleanBuffer();
    if (date.day < 1 || date.day > 31 || date.month < 1 || date.month > 12 ||
        date.hour < 0 || date.hour > 23 || date.minute < 0 || date.minute > 59) {
        fprintf(stderr, "ERROR: DATE/TIME OUT OF RANGE.\n");
        return;
    }

    int duration = verify(1, 24, "DURATION (1 TO 24 HOURS): ");

    puts("RESERVATION STATUS:\n1 - PENDING\n2 - CONFIRMED\n3 - FINALIZED\n4 - CANCELED");
    int status = verify(1, 4, "STATUS: ");

    int participants = verify(1, 200, "NUMBER OF PARTICIPANTS (1 TO 200): ");

    // Everything validated: commit the reservation and every side effect.
    for (int k = 0; k < tempCount; k++) {
        int equipmentIndex = findEquipment(equipments, tempItems[k].equipment_id);
        equipments->equipmentList[equipmentIndex].usingEquipment += tempItems[k].quantity;
        if (equipments->equipmentList[equipmentIndex].usingEquipment >= equipments->equipmentList[equipmentIndex].quantity) {
            equipments->equipmentList[equipmentIndex].equipmentReserved = 1;
            equipments->equipmentList[equipmentIndex].status = RESERVED;
        }
    }

    spaces->spaceList[spaceIndex].spaceOccupied = 1;
    spaces->spaceList[spaceIndex].reservations++;

    Reservation *newReservation = &reservations->reservations[reservations->total];
    newReservation->reservation_id = reservations->nextId++; // Unique and sequential: never reused.
    newReservation->client_id = client_id;
    newReservation->space_id = space_id;
    newReservation->equipmentItemCount = tempCount;
    for (int k = 0; k < tempCount; k++) {
        newReservation->equipmentItems[k] = tempItems[k];
    }
    newReservation->date = date;
    newReservation->duration_hours = duration;
    newReservation->status = status;
    newReservation->participants = participants;

    reservations->total++;

    puts("\nRESERVATION CREATED SUCCESSFULLY!");
    logMessage(LOG_INFO, "RESERVATION", "Reservation created.");
}

// Releases the space and every reserved equipment item tied to a reservation, and marks it Canceled.
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

    // Liberta cada item de equipamento associado à reserva
    for (int k = 0; k < reservation->equipmentItemCount; k++) {
        int equipmentId = reservation->equipmentItems[k].equipment_id;
        int quantity = reservation->equipmentItems[k].quantity;

        for (int i = 0; i < equipments->counter; i++) {
            if (equipments->equipmentList[i].id == equipmentId) {
                equipments->equipmentList[i].usingEquipment -= quantity;
                if (equipments->equipmentList[i].usingEquipment <= 0) {
                    equipments->equipmentList[i].usingEquipment = 0;
                    equipments->equipmentList[i].equipmentReserved = 0;
                    if (equipments->equipmentList[i].status == RESERVED) {
                        equipments->equipmentList[i].status = AVALIABLE;
                    }
                }
                break;
            }
        }
    }

    // Atualizar status da reserva para cancelada
    reservation->status = STATUS_CANCELED;

    printf("Reservation canceled, and associated space and equipment are now available.\n");
    logMessage(LOG_INFO, "RESERVATION", "Reservation canceled.");
}

// List all reservations
void listReservations(ReservationList *list) {
    if (list->total == 0) {
        printf("No reservations registered.\n");
        return;
    }

    for (int i = 0; i < list->total; i++) {
        Reservation *r = &list->reservations[i];
        printf("Reservation ID: %d, Client ID: %d, Space ID: %d, Date: %02d/%02d/%04d %02d:%02d, "
               "Duration: %d hours, Status: %d, Participants: %d\n",
               r->reservation_id, r->client_id, r->space_id, r->date.day, r->date.month,
               r->date.year, r->date.hour, r->date.minute, r->duration_hours, r->status,
               r->participants);

        if (r->equipmentItemCount == 0) {
            printf("  EQUIPMENT: NONE\n");
        } else {
            for (int k = 0; k < r->equipmentItemCount; k++) {
                printf("  EQUIPMENT ID: %d | QUANTITY: %d\n",
                       r->equipmentItems[k].equipment_id, r->equipmentItems[k].quantity);
            }
        }
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

// Update reservation status. Canceling through here also releases the associated space/equipment.
void updateReservation(ReservationList *list, geralSpaces *spaces, equipmentManager *equipments) {
    int reservation_id = verify(1, 1000000, "ENTER RESERVATION TO UPDATE: ");

    int index = searchReservation(list, reservation_id);
    if (index == -1) {
        printf("Reservation with ID %d not found.\n", reservation_id);
        return;
    }

    Reservation *reservation = &list->reservations[index];
    printf("Updating reservation ID %d:\n", reservation->reservation_id);
    puts("NEW STATUS:\n1 - PENDING\n2 - CONFIRMED\n3 - FINALIZED\n4 - CANCELED");
    int newStatus = verify(1, 4, "STATUS: ");

    if (newStatus == STATUS_CANCELED && reservation->status != STATUS_CANCELED) {
        cancelReservation(list, spaces, equipments, reservation_id);
        return;
    }

    reservation->status = newStatus;
    printf("Update successful!\n");
    logMessage(LOG_INFO, "RESERVATION", "Reservation updated.");
}

// Remove a reservation (releasing its space/equipment first, then compacting the list)
void deleteReservation(ReservationList *reservations, geralSpaces *spaces, equipmentManager *equipments) {
    int reservation_id = verify(1, 1000000, "ENTER RESERVATION TO DELETE: ");

    int index = searchReservation(reservations, reservation_id);
    if (index == -1) {
        printf("Reservation with ID %d not found.\n", reservation_id);
        return;
    }

    cancelReservation(reservations, spaces, equipments, reservation_id);

    for (int i = index; i < reservations->total - 1; i++) {
        reservations->reservations[i] = reservations->reservations[i + 1];
    }
    reservations->total--;

    printf("Reservation removed successfully!\n");
    logMessage(LOG_INFO, "RESERVATION", "Reservation deleted.");
}

// Save reservations to a binary file
void saveReservations(ReservationList *list) {
    if (list->total == 0) {
        puts("NO RESERVATIONS TO SAVE!");
        return;
    }

    FILE *file = fopen(BOOKING_FILE, "wb");
    if (!file) {
        perror("Error opening file to save reservations.");
        logMessage(LOG_ERROR, "RESERVATION", "Failed to open reservations.bin for writing.");
        return;
    }

    fwrite(&list->total, sizeof(int), 1, file);
    fwrite(list->reservations, sizeof(Reservation), list->total, file);

    fclose(file);
    printf("Reservations saved successfully to '%s'.\n", BOOKING_FILE);
    logMessage(LOG_INFO, "RESERVATION", "Reservations saved to disk.");
}

// Recomputes the next unique/sequential id from the highest id currently in memory.
static void recalculateNextReservationId(ReservationList *reservations) {
    int maxId = 0;
    for (int i = 0; i < reservations->total; i++) {
        if (reservations->reservations[i].reservation_id > maxId) {
            maxId = reservations->reservations[i].reservation_id;
        }
    }
    reservations->nextId = maxId + 1;
}

// Load reservations from a binary file
void loadReservations(ReservationList *reservations) {
    FILE *fp = fopen(BOOKING_FILE, "rb");

    if (!fp) {
        puts("NO RESERVATION FILE FOUND! INITIALIZING EMPTY LIST");
        initializeReservationList(reservations, 10);
        return;
    }

    if (fread(&reservations->total, sizeof(int), 1, fp) != 1) {
        perror("ERROR READING RESERVATION COUNT");
        logMessage(LOG_ERROR, "RESERVATION", "Error reading reservation count from disk.");
        initializeReservationList(reservations, 10);
        fclose(fp);
        return;
    }

    reservations->capacity = reservations->total > 0 ? reservations->total : 10;
    reservations->reservations = malloc(reservations->capacity * sizeof(Reservation));

    if (!reservations->reservations) {
        perror("ERROR ALLOCATING MEMORY FOR RESERVATIONS");
        logMessage(LOG_ERROR, "RESERVATION", "Memory allocation failed while loading reservations.");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    if (fread(reservations->reservations, sizeof(Reservation), reservations->total, fp) != (size_t)reservations->total) {
        puts("ERROR READING RESERVATION LIST! INITIALIZING EMPTY LIST");
        logMessage(LOG_ERROR, "RESERVATION", "Error reading reservation list from disk.");
        free(reservations->reservations);
        initializeReservationList(reservations, 10);
        fclose(fp);
        return;
    }

    recalculateNextReservationId(reservations);
    puts("RESERVATIONS LOADED SUCCESSFULLY!");
    logMessage(LOG_INFO, "RESERVATION", "Reservations loaded from disk.");
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

    int statusCount[5] = {0}; // Array to count reservations by status (index 0 unused)
    for (int i = 0; i < reservations->total; i++) {
        statusCount[reservations->reservations[i].status]++;
    }

    // Report results
    puts("----------RESERVATIONS BY STATUS----------");
    printf("PENDING: %d\n", statusCount[STATUS_PENDING]);
    printf("CONFIRMED: %d\n", statusCount[STATUS_CONFIRMED]);
    printf("FINALIZED: %d\n", statusCount[STATUS_FINALIZED]);
    printf("CANCELED: %d\n", statusCount[STATUS_CANCELED]);
}

// Lists reservations matching a date entered by the user (DD MM YYYY)
void reservationsByDate(ReservationList *reservations) {
    if (reservations->total == 0) {
        puts("NO RESERVATIONS AVAILABLE!");
        return;
    }

    int day = verify(1, 31, "DAY: ");
    int month = verify(1, 12, "MONTH: ");
    int year = verify(2000, 2100, "YEAR: ");

    printf("----------RESERVATIONS ON %02d/%02d/%04d----------\n", day, month, year);
    int shown = 0;
    for (int i = 0; i < reservations->total; i++) {
        Reservation *r = &reservations->reservations[i];
        if (r->date.day == day && r->date.month == month && r->date.year == year) {
            shown++;
            printf("Reservation ID: %d | Client ID: %d | Space ID: %d | Status: %d\n",
                   r->reservation_id, r->client_id, r->space_id, r->status);
        }
    }
    if (shown == 0) {
        puts("NO RESERVATIONS FOUND FOR THIS DATE.");
    }
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
