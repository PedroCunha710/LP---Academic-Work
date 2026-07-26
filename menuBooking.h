/*
 * File:   menuBooking.h
 * Author: Henrique Lopes
 *
 * Created on 20 de dezembro de 2024, 13:54
 *
 * @brief In this header file we will declare the functions of the reservations to use in other pages.
 * This file contains the functions of the reservations to use in other pages.
 */


#ifndef MENUBOOKING_H
#define MENUBOOKING_H

#include "menuEquipment.h"
#include "menuSpace.h"
#include "menuClient.h"
#include "input.h"

// Defines
/** @def MAX_RESERVATIONS
 * Maximum reservations available.
 */
#define MAX_RESERVATIONS 100

/** @def MIN_RESERVATIONS
 * Minimun reservations available.
 */
#define MIN_RESERVATIONS 1

/** @def MAX_EQUIPMENT_PER_RESERVATION
 * Maximum number of different equipment types that can be attached to a single reservation.
 */
#define MAX_EQUIPMENT_PER_RESERVATION 5

/** @def MSG_RESERVATION_ID
 * Message to enter reservation ID.
 */
#define MSG_RESERVATION_ID "Enter the reservation ID: "

/** @def STATUS_PENDING
 * Pending book status.
 */
#define STATUS_PENDING 1

/** @def STATUS_CONFIRMED
 * Confirmed book status.
 */
#define STATUS_CONFIRMED 2

/** @def STATUS_FINALIZED
 * Finalized book status (the event already took place).
 */
#define STATUS_FINALIZED 3

/** @def STATUS_CANCELED
 * Canceled book status.
 */
#define STATUS_CANCELED 4

/** @def MSG_RESERVATION_STATUS
 * Message to enter reservation status.
 */
#define MSG_RESERVATION_STATUS "Enter the reservation status (1.Pending, 2.Confirmed, 3.Finalized, 4.Canceled): "



// Structures
typedef struct {
    int day, month, year, hour, minute;
} DateTime;

/**
 * @brief One line item of equipment (type + quantity) attached to a reservation.
 */
typedef struct {
    int equipment_id;
    int quantity;
} ReservationEquipmentItem;

typedef struct {
    int reservation_id;
    int client_id;
    int space_id;
    ReservationEquipmentItem equipmentItems[MAX_EQUIPMENT_PER_RESERVATION]; // Equipment reserved: type (id) + quantity
    int equipmentItemCount;
    DateTime date;
    int duration_hours; // Duration of the reservation in hours
    int status;         // Status of the reservation (pending, confirmed, finalized, canceled)
    int participants;   // Number of participants
} Reservation;


typedef struct {
    Reservation *reservations; // Dynamic list of reservations
    int total;                 // Total number of reservations
    int capacity;              // Current capacity of the list
    int nextId;                // Next sequential, unique id to assign. Never reused, even after deletions.
} ReservationList;

// CRUD Functions

/**
 * @brief This function create the reservations.
 * @param ReservationList,ClientList,geralSpaces,equipmentManager are used to send the adress of the mains variables "reservations","clients",spaces","equipments
 * to the following functions.
 * @return This function does not return any value. Just create the reservations.
 */
void createReservation(ReservationList *reservations, ClientList *clients, geralSpaces *spaces, equipmentManager *equipments);

/**
 * @brief This function initializes the reservations.
 * @param ReservationList are used to send the adress of the mains variables "reservations" to the following functions.
 * initialCapacity to define the initialCapacity
 * @return This function does not return any value. Just inicialize the reservations.
 */
void initializeReservationList(ReservationList *reservations, int initialCapacity);

/**
 * @brief This function free the reservations.
 * @param ReservationList are used to send the adress of the mains variables "reservations" to the following functions.
 * @return This function does not return any value. Just free the reservations.
 */
void freeReservationList(ReservationList *reservations);

/**
 * @brief This function list the reservations.
 * @param ReservationList are used to send the adress of the mains variables "reservations" to the following functions.
 * @return This function does not return any value. Just list the reservations.
 */
void listReservations(ReservationList *reservations);

/**
 * @brief This function search the reservations.
 * @param ReservationList are used to send the adress of the mains variables "reservations" to the following functions.
 * @param reservation_id is used to find the reservation.
 * @return This function return i if reservation is found or -1 if reservation is not found.
 */
int searchReservation(ReservationList *reservations, int reservation_id);

/**
 * @brief This function update the reservations. If the new status is Canceled, the associated
 * space and equipment are released through cancelReservation().
 * @param ReservationList,geralSpaces,equipmentManager are used to send the adress of the mains variables "reservations","spaces","equipments" to the following functions.
 * @return This function does not return any value. Just update the reservations.
 */
void updateReservation(ReservationList *reservations, geralSpaces *spaces, equipmentManager *equipments);

/**
 * @brief This function delete the reservations.
 * @param ReservationList,geralSpaces,equipmentManager are used to send the adress of the mains variables "reservations","spaces","equipments" to the following functions.
 * @return This function does not return any value. Just delete the reservations.
 */
void deleteReservation(ReservationList *reservations,geralSpaces *spaces,equipmentManager *equipments);

// Reports

/**
 * @brief This function show the total reservations.
 * @param ReservationList are used to send the adress of the mains variables "reservations" to the following functions.
 * @return This function does not return any value. Just show the total reservations.
 */
void totalReservations(ReservationList *reservations);

/**
 * @brief This function show the reservations matching a given date entered by the user.
 * @param ReservationList are used to send the adress of the mains variables "reservations" to the following functions.
 * @return This function does not return any value. Just show the reservations for the chosen date.
 */
void reservationsByDate(ReservationList *reservations);

/**
 * @brief This function show the most participants reservations.
 * @param ReservationList are used to send the adress of the mains variables "reservations" to the following functions.
 * @return This function does not return any value. Just show the most participants reservations.
 */
void mostParticipantsReservation(ReservationList *reservations);

/**
 * @brief This function show the least participants reservations.
 * @param ReservationList are used to send the adress of the mains variables "reservations" to the following functions.
 * @return This function does not return any value. Just show the least participants reservations.
 */
void leastParticipantsReservation(ReservationList *reservations);


//Functions
/**
 * @brief This function initializes the reservations.
 * @param ReservationList are used to send the adress of the mains variables "reservations" to the following functions.
 * @return This function does not return any value. Just inicialize the reservations.
 */
void turnOnBooking(ReservationList *reservations);

/**
 * @brief This function saves the record in reservations, for example Create, List, Update and Remove.
 * @param ReservationList are used to send the adress of the mains variables "reservations" to the following functions.
 * @return This function does not return any value. Its only save the reservations.
 */
void saveReservations(ReservationList *reservations);

/**
 * @brief This function load the record in reservations, for example Create, List, Update and Remove.
 * @param ReservationList are used to send the adress of the mains variables "reservations" to the following functions.
 * @return This function does not return any value. Its only load the reservations.
 */
void loadReservations(ReservationList *reservations);

//secondary functions
/**
 * @brief This function shows the reservations grouped/counted by status.
 * @param ReservationList are used to send the adress of the mains variables "reservations" to the following functions.
 * @return This function does not return any value. Just show the reservations by status.
 */
void reservationsByStatus(ReservationList *reservations);

/**
 * @brief This function show the most Active Client.
 * @param ClientList, ReservationList are used to send the adress of the mains variables "list",reservations" to the following functions.
 * @return This function does not return any value. Just show the most Active Client.
 */
void mostActiveClient(ClientList *list, ReservationList *reservations);

/**
 * @brief This function find the Space.
 * @param geralSpaces are used to send the adress of the mains variables "spaces" to the following functions.
 * @param space_id is used to find the space for the id.
 * @return This function return i if the space has found or -1 if space has not found.
 */
int findSpace(geralSpaces *spaces, int space_id);

/**
 * @brief This function show the least Reservations Client.
 * @param ClientList, ReservationList are used to send the adress of the mains variables "list",reservations" to the following functions.
 * @return This function does not return any value. Just show the least Reservations Client.
 */
void leastReservationsClient(ClientList *clients,ReservationList *reservations);

/**
 * @brief This function show the most space reserved.
 * @param geralSpaces, ReservationList are used to send the adress of the mains variables "spaces",reservations" to the following functions.
 * @return This function does not return any value. Just show the most space reserved.
 */
void mostSpaceReservated(geralSpaces *spaces,ReservationList *reservations);

/**
 * @brief This function show the least space reserved.
 * @param geralSpaces, ReservationList are used to send the adress of the mains variables "spaces",reservations" to the following functions.
 * @return This function does not return any value. Just show the least space reserved.
 */
void leastSpaceReservated(geralSpaces *spaces,ReservationList *reservations);

/**
 * @brief This function shows, for each space, the percentage of all reservations made for it.
 * @param geralSpaces, ReservationList are used to send the adress of the mains variables "spaces",reservations" to the following functions.
 * @return This function does not return any value. Just show the occupancy rate of each space.
 */
void spaceOccupancyRate(geralSpaces *spaces, ReservationList *reservations);

/**
 * @brief This function shows, for each client, the total reservations made and the spaces used.
 * @param ClientList, ReservationList, geralSpaces are used to send the adress of the mains variables "clients","reservations","spaces" to the following functions.
 * @return This function does not return any value. Just show the reservation details per client.
 */
void clientReservationDetails(ClientList *clients, ReservationList *reservations, geralSpaces *spaces);

/**
 * @brief This function cancel the reservation.
 * @param ReservationList,geralSpaces,equipmentManager are used to send the adress of the mains variables "reservations","spaces","equipments to the following functions.
 * @param reservation_id is used to find the reservation to cancel
 * @return This function does not return any value. Just cancel the reservation for the id.
 */
void cancelReservation(ReservationList *reservations, geralSpaces *spaces,equipmentManager *equipments, int reservation_id);

/**
 * @brief This function deletes a space. If the space has reservations pointing to it, it is marked
 * Inactive instead of being physically removed, to preserve referential integrity.
 * @param geralSpaces, ReservationList are used to send the adress of the mains variables "spaces","reservations" to the following functions.
 * @return This function does not return any value.
 */
void deleteSpace(geralSpaces *spaces, ReservationList *reservations);

/**
 * @brief This function deletes a client. If the client has reservations pointing to it, it is marked
 * Inactive instead of being physically removed, to preserve referential integrity.
 * @param ClientList, ReservationList are used to send the adress of the mains variables "list","reservations" to the following functions.
 * @return This function does not return any value.
 */
void removeClient(ClientList *list, ReservationList *reservations);

/**
 * @brief This function deletes an equipment. If the equipment has reservations pointing to it, it is
 * marked Inactive instead of being physically removed, to preserve referential integrity.
 * @param equipmentManager, ReservationList are used to send the adress of the mains variables "equipments","reservations" to the following functions.
 * @return This function does not return any value.
 */
void deleteEquipment(equipmentManager *equipments, ReservationList *reservations);

/**
 * @brief Checks whether any reservation references the given space.
 * @param ReservationList are used to send the adress of the mains variables "reservations" to the following functions.
 * @param space_id the space id to check.
 * @return 1 if at least one reservation references the space, 0 otherwise.
 */
int spaceHasReservations(ReservationList *reservations, int space_id);

/**
 * @brief Checks whether any reservation references the given client.
 * @param ReservationList are used to send the adress of the mains variables "reservations" to the following functions.
 * @param client_id the client id to check.
 * @return 1 if at least one reservation references the client, 0 otherwise.
 */
int clientHasReservations(ReservationList *reservations, int client_id);

/**
 * @brief Checks whether any reservation references the given equipment.
 * @param ReservationList are used to send the adress of the mains variables "reservations" to the following functions.
 * @param equipment_id the equipment id to check.
 * @return 1 if at least one reservation references the equipment, 0 otherwise.
 */
int equipmentHasReservations(ReservationList *reservations, int equipment_id);

#endif/* MENUBOOKING_H */
