/*
 * File:   menus.h
 * Author: Pedro Cunha e Henrique Lopes
 *
 * Created on 4 de janeiro de 2025, 23:09
 *
 * @brief In this header file we will declare the menus of all functions.
 * This file contains the all menus to use to run the functions.
 */

#ifndef MENUS_H
#define MENUS_H

#include "input.h"
#include "menuSpace.h"
#include "menuEquipment.h"
#include "menuClient.h"
#include "menuBooking.h"

//MENU SPACE
/**
 * @brief This function shows the options to the user choose in menu space.
 * @param geralSpaces and ReservationList is used to send the adress of the mains variables "spaces","reservations" to the following functions.
 * The last one is used to the next menu for the reports.
 * @return This function does not return any value.Its only show the options.
 */
void menuSpace(geralSpaces *spaces,ReservationList *reservations);

/**
 * @brief This function shows the options to the user choose in menu space(Menu Reports).
 * @param geralSpaces and ReservationList is used to send the adress of the mains variables "spaces","reservations" to the following functions.
 * @return This function does not return any value.Its only show the options.
 */
void menuSpace2(geralSpaces *spaces,ReservationList *reservations);

//MENU EQUIPMENT

/**
 * @brief This function shows the options to the user choose in menu equipment.
 * @param equipmentManager, ReservationList are used to send the adress of the mains variables "equipments","reservations" to the following functions.
 * The reservations list is required to check for relationships before a hard delete.
 * @return This function does not return any value.Its only show the options.
 */
void menuEquipment(equipmentManager *equipments, ReservationList *reservations);

/**
 * @brief This function shows the options to the user choose in menu equipment(Menu Reports).
 * @param equipmentManager is used to send the adress of the mains variables "equipments" to the following functions.
 * @return This function does not return any value.Its only show the options.
 */
void menuEquipment2(equipmentManager *equipments);

//MENU CLIENT
/**
 * @brief This function shows the options to the user choose in menu client.
 * @param ClientList, ReservationList, geralSpaces are used to send the adress of the mains variables "clients","reservations","spaces" to the following functions.
 * @return This function does not return any value.Its only show the options.
 */
void menuClient(ClientList *clients,ReservationList *reservations, geralSpaces *spaces);

/**
 * @brief This function shows the options to the user choose in menu client(Menu Reports).
 * @param ClientList, ReservationList, geralSpaces are used to send the adress of the mains variables "clients","reservations","spaces" to the following functions.
 * @return This function does not return any value.Its only show the options.
 */
void menuClient2(ClientList *clients, ReservationList *reservations, geralSpaces *spaces);

//MENU BOOKING

/**
 * @brief This function shows the options to the user choose in menu reservations.
 * @param ReservationList,geralSpaces,ClientList,equipmentManager are used to send the adress of the mains variables "reservations","spaces",clients","equipments
 * to the following functions.
 * @return This function does not return any value.Its only show the options.
 */
void menuReservations(ReservationList *reservations,geralSpaces *spaces,ClientList *clients, equipmentManager *equipments);

/**
 * @brief This function shows the options to the user choose in menu reservations(Menu Reports).
 * @param ReservationList,geralSpaces are used to send the adress of the mains variables "reservations","spaces"
 * to the following functions.
 * @return This function does not return any value.Its only show the options.
 */
void menuReservationReports(ReservationList *list, geralSpaces *spaces);

//MAIN MENU

/**
 * @brief This function shows the options to the user choose in main menu.
 * @param This menu shows only the menu of Space,Client,Reservation and Equipment
 * @return This function does not return any value.Its only show the options.
 */
void mainMenu();


#endif /* MENUS_H */
