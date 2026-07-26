/* 
 * File:   input.c
 * Author: Pedro Cunha e Henrique Lopes
 * Date: 20-12-2024
 *
 * @brief In this file we will develop the data entry validation functions.
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include <time.h>

void cleanBuffer() {
    char clean;
    while ((clean = getchar()) != '\n' && clean != EOF);
}

int verify(int minVal, int maxVal, char *mensage) {
    int val;
    printf(mensage);
    while (scanf("%d", &val) != 1 || val < minVal || val > maxVal) {
        puts(INVALID_VALUE);
        cleanBuffer();
        printf(mensage);
    }
    return val;
}


void readStrg(char* strg, unsigned int sz, char *mensage) {
    printf(mensage);
    if (fgets(strg, sz, stdin) != NULL) {
        unsigned int len = strlen(strg);
        if (len > 0 && strg[len - 1] == '\n') {
            strg[len - 1] = '\0';
        } else {
            cleanBuffer();  
        }
    }
}

int registDay (){
        time_t day = time(NULL);
        int correct_day = localtime(&day)->tm_mday;
        return correct_day;
     }
    
     // função para obter o mes que é registado
     int registMonth (){
         time_t month = time(NULL);
        int correct_month = localtime(&month)->tm_mon + 1;
        return correct_month;
     }
     // função para obter o ano que é registado
      int registYear (){
         time_t year = time(NULL);
        int correct_year = localtime(&year)->tm_year + 1900;
        return correct_year;
     }
 


