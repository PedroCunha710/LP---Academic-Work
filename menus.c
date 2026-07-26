/* 
 * File:   menus.c
 * Author: Pedro Cunha e Henrique Lopes
 *
 * Created on 4 de janeiro de 2025, 23:10
 * 
 * @brief In this file we declare all the menus of the programs.
 * This file contains all the menus that were used in the program such as main menus and menus for reports.
 */

#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "menus.h"
#include "menuSpace.h"
#include "menuEquipment.h"
#include "menuClient.h"
#include "menuBooking.h"


//FUNCTION TO MENU SPACE
void menuSpace(geralSpaces *spaces,ReservationList *reservations){
    if (spaces==NULL || spaces->spaceList == NULL){
        puts("ERROR! SPACE LIST IS NOT INITIALIZED");
        return;
    }
    int option;
    do{
        puts("\n----------------SPACE MENU----------------\n");
        puts("[1] CREATE SPACES");
        puts("[2] LIST SPACES");
        puts("[3] UPDATE SPACES");
        puts("[4] DELETE SPACES");
        puts("[5] NEXT PAGE");
        puts("[6] RETURN TO MAIN MENU");
        
        option=verify(1,6,"OPTION:");
       
    
        switch(option){
           case 1:
                createSpace(spaces);
                break;
            case 2:
                listSpace(spaces);
                break;
            case 3:
                updateSpace(spaces);
                break;
            case 4:
                deleteSpace(spaces);
                break;
            case 5:
                menuSpace2(spaces,reservations);
                break;        
            case 6:
                saveSpace(spaces);
            puts("RETURN TO MAIN MENU...");
            return;
                
            default:
                puts("INVALID OPTION!! TRY AGAIN!");
            }
        
        }while(option!=6);
        
}

//FUNTION OF REPORT MENU
void menuSpace2(geralSpaces *spaces,ReservationList *reservations){
    if(spaces->counter==0){
        puts("NO SPACES AVALIABLE TO REPORTS!!");
        return;
    }
    
    int option;
    
    do{
        puts("\n----------------REPORT SPACE----------------\n");
        puts("[1] TOTAL AVALIABLE SPACES");
        puts("[2] SPACES BY TYPE");
        puts("[3] MOST SPACE RESERVED");
        puts("[4] LEAST SPACE RESERVED");
        puts("[5] PREVIOUS PAGE");
        puts("[6] RETURN TO MAIN MENU");
        
        option=verify(1,6,"OPTION->");
        
        switch(option){
            case 1:
            totalAvaliableSpaces(spaces);
            break;
            
        case 2:
            spacesByType(spaces);
            break;
            
        case 3:
            mostSpaceReservated(spaces,reservations);
            break;
            
        case 4:
            leastSpaceReservated(spaces,reservations);
            break;
            
        case 5:
            puts("RETURN TO SPACE MENU");
            return;
            
        case 6:
            puts("RETURN TO MAIN MENU...");
            return;
                
        default:
            puts("Invalid option! Try Again!");
               
        }
  
    }while(option!=6);
  
}

//FUNCTION TO MENU EQUIPMENT
void menuEquipment(equipmentManager *equipments){
    if (equipments==NULL || equipments->equipmentList == NULL){
        puts("ERROR! EQUIPMENT LIST IS NOT INITIALIZED!");
        return;
    }
    int option;
    do{
        puts("\n----------------EQUIPMENT MENU----------------\n");
        puts("[1] CREATE EQUIPMENT");
        puts("[2] LIST EQUIPMENT");
        puts("[3] UPDATE EQUIPMENT");
        puts("[4] DELETE EQUIPMENT");
        puts("[5] NEXT PAGE");
        puts("[6] RETURN TO MAIN MENU");
        
        option=verify(1,6,"OPTION:");
       
    
        switch(option){
           case 1:
                createEquipment(equipments);
                break;
            case 2:
                listEquipment(equipments);
                break;
            case 3:
                updateEquipment(equipments);
                break;
            case 4:
                deleteEquipment(equipments);
                break;
            case 5:
                menuEquipment2(equipments);
                break;        
            case 6:
                saveEquipment(equipments);
            puts("RETURN TO MAIN MENU...");
            return;
                
            default:
                puts("INVALID OPTION! TRY AGAIN!!");
            }
        
        }while(option!=6);
        

    
}

//REPORT FUNCTIONS MENU EQUIPMENT
void menuEquipment2(equipmentManager *equipments){
    if(equipments->counter==0){
        puts("NO EQUIPMENTS AVALIABLE TO REPORTS!!");
        return;
    }
    
    int option;
    
    do{
        puts("\n----------------REPORT SPACE----------------\n");
        puts("[1] MOST USED EQUIPMENT");
        puts("[2] LEAST USED EQUIPMENT");
        puts("[3] DETAILED EQUIPMENT USAGE");
        puts("[4] PREVIOUS PAGE");
        puts("[5] RETURN TO MAIN MENU");
        
        option=verify(1,5,"OPTION->");
        
        switch(option){
            case 1:
            mostUsedEquipment(equipments);
            break;
            
        case 2:
            leastUsedEquipment(equipments);
            break;
            
        case 3:
            detailedUsageReport(equipments);
            break;
            
        case 4:
            puts("RETURN TO EQUIPMENT MENU");
            return;
            
        case 5:
            puts("RETURN TO MAIN MENU...");
            return;
                
        default:
            puts("Invalid option! Try Again!");
               
        }
  
    }while(option!=6);
  
}

//FUNCTION TO MENU CLIENT
void menuClient(ClientList *clients,ReservationList *reservations) {
    if (clients == NULL || clients->clients == NULL) {
        puts("ERROR!: CLIENT LIST IS NOT INITIALIZED");
        return;
    }
    
    int option;
    do {
        puts("\n----------------CLIENT MENU----------------\n");
        puts("[1] CREATE CLIENT");
        puts("[2] LIST CLIENT");
        puts("[3] UPDATE CLIENT");
        puts("[4] DELETE CLIENT");
        puts("[5] NEXT PAGE");
        puts("[6] RETURN TO MAIN MENU");

        option = verify(1, 6, "OPTION:");
        
        switch (option) {
            case 1:
                createClient(clients);
                break;
            case 2:
                listClients(clients);
                break;
            case 3:
                updateClient(clients);
                break;
            case 4:
                removeClient(clients);
                break;
            case 5:
                menuClient2(clients,reservations);  // Chamando um menu de relatórios de clientes
                break;        
            case 6:
                saveClients(clients);
                puts("RETURN TO MAIN MENU...");
                return;
            default:
                puts("Invalid option! Try Again!");
        }
    } while (option != 6);
}

//REPORT FUNCTIONS MENU CLIENT
void menuClient2(ClientList *clients, ReservationList *reservations) {
    if (clients->total == 0) {
        puts("NO CLIENTS AVAILABLE FOR REPORTS!!");
        return;
    }

    int option;
    
    do {
        puts("\n----------------CLIENT REPORTS----------------\n");
        puts("[1] TOTAL CLIENTS");
        puts("[2] MOST RESERVATIONS");
        puts("[3] LEAST RESERVATIONS");
        puts("[4] PREVIOUS PAGE");
        puts("[5] RETURN TO MAIN MENU");

        option = verify(1, 5, "OPTION->");
        
        switch (option) {
            case 1:
                totalAvailableClients(clients);
                break;
            case 2:
                mostActiveClient(clients, reservations);
                break;
            case 3:
                leastReservationsClient(clients, reservations);
                break;
            case 4:
                puts("RETURN TO CLIENT MENU");
                return;
            case 5:
                puts("RETURN TO MAIN MENU...");
                return;
            default:
                puts("Invalid option! Try Again!");
        }
    } while (option != 5);
}

//FUNCTION TO MENU
void menuReservations(ReservationList *reservations,geralSpaces *spaces,ClientList *clients, equipmentManager *equipments, int reservation_id) { 

    if (reservations == NULL || reservations->reservations == NULL) {
        puts("ERROR!: RESERVATION LIST IS NOT INITIALIZED");
        return;
    }


    int option;
    do {
        puts("\n----------------RESERVATION MENU----------------\n");
        puts("[1] CREATE RESERVATION");
        puts("[2] SEARCH RESERVATION");
        puts("[3] UPDATE RESERVATION");
        puts("[4] DELETE RESERVATION");
        puts("[5] NEXT PAGE");
        puts("[6] RETURN TO MAIN MENU");

        option = verify(1, 6, "OPTION:");

        switch (option) {
            case 1:
                createReservation(reservations, clients, spaces,equipments);
                break;
            case 2:
                listReservations(reservations);
                break;
            case 3:
                updateReservation(reservations, reservation_id);
                break;
            case 4:
                deleteReservation(reservations,spaces,equipments,reservation_id);
                break;
            case 5:
                menuReservationReports(reservations);
                break;
            case 6:
                saveReservations(reservations);
                
                return;
            default:
                puts("Invalid option! Try Again!");
        }
    } while (1);
}

//REPORT FUNCTIONS MENU BOOKING
void menuReservationReports(ReservationList *reservations) {
    if (reservations->total == 0) {
        puts("NO RESERVATIONS AVAILABLE FOR REPORTS!");
        return;
    }

    int option;

    do {
        puts("\n----------------REPORT RESERVATIONS----------------\n");
        puts("[1] TOTAL RESERVATIONS");
        puts("[2] RESERVATIONS BY STATUS");
        puts("[3] RESERVATION WITH MOST PARTICIPANTS");
        puts("[4] RESERVATION WITH LEAST PARTICIPANTS");
        puts("[5] RETURN TO RESERVATION MENU");
        puts("[6] RETURN TO MAIN MENU");

        option = verify(1, 6, "OPTION->");

        switch (option) {
            case 1:
                totalReservations(reservations);
                break;

            case 2:
                reservationsByStatus(reservations);
                break;

            case 3:
                mostParticipantsReservation(reservations);
                break;

            case 4:
                leastParticipantsReservation(reservations);
                break;

            case 5:
                puts("RETURN TO RESERVATION MENU");
                return;

            case 6:
                puts("RETURN TO MAIN MENU...");
                return;

            default:
                puts("Invalid option! Try Again!");
        }
    } while (1);
}

//MAIN MENU
void mainMenu(){
    int reservation_id;
    //SPACES
    geralSpaces spaces;
    turnOnSpaces(&spaces);
    loadSpace(&spaces);
    
    //EQUIPMENT
    equipmentManager equipments;
    turnOnEquipment(&equipments);
    loadEquipment(&equipments);
    
    //CLIENT 
    ClientList clients;
    turnOnClient(&clients);
    loadClients(&clients);
    
    //BOOKING
    ReservationList list;
    turnOnBooking (&list);
    loadReservations(&list);
    
    
    int option;
                do{
        puts("\n----------------MAIN MENU----------------\n");
        puts("[1] SPACE");
        puts("[2] CLIENT");
        puts("[3] RESERVATIONS");
        puts("[4] EQUIPMENT");
        puts("[5] LEFT PROGRAM");
        option= verify(1,5,"Option->");       
        
        switch(option){
            case 1:
                menuSpace(&spaces,&list);
                break;
            case 2:
                menuClient(&clients,&list);
                break;
            case 3:
                menuReservations(&list,&spaces,&clients, &equipments,reservation_id);
                break;
            case 4:
                menuEquipment(&equipments);
                break;
            case 5:
                freeSpaces(&spaces);
                freeEquipment(&equipments);
                freeClientList(&clients);
                freeReservationList(&list);
                puts("Exit the Program...");
                return;
            default:
                puts("Invalid option! Try Again!");
        }
        
        }while(1);
        
      
        
}


