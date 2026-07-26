/* 
 * File:   input.h
 * Author: Pedro Cunha e Henrique Lopes
 *
 * Created on 20 de dezembro de 2024, 15:53
 * 
 * @brief In this file I will declare the intput functions.
 * This file contains the functions for later in input.c to develop them. These functions serve to save some work developing the other parts.
 * 
 */

#ifndef INPUT_H
#define INPUT_H

/**
 * @def INVALID_VALUE
 * Error message displayed when an invalid value is entered by the user.
 */
#define INVALID_VALUE "THE VALUE ENTERED IS INVALID!"

/**
 * @brief Clears the input buffer to remove any leftover characters.
 * 
 * This function is used to flush the input buffer, clearing any extraneous characters or newline characters 
 * left behind by previous user inputs. It ensures that the buffer is clean before accepting new input, 
 * preventing issues when reading input later.
 */
void cleanBuffer();

/**
 * @brief Verifies whether the input value is within a specified range.
 * 
 * This function prompts the user to enter a value and checks if it falls within a specified range 
 * defined by the `minVal` and `maxVal` parameters. If the user enters a value outside of this range, 
 * the function will display an error message and prompt the user to try again until a valid value is provided.
 * 
 * @param minVal The minimum acceptable value for the input.
 * @param maxVal The maximum acceptable value for the input.
 * @param mensage The message to display when asking for the input.
 * 
 * @return The valid integer input provided by the user, which is guaranteed to be within the specified range.
 */
int verify(int minVal, int maxVal, char *mensage);

/**
 * @brief Reads a string from the user with a maximum specified length.
 * 
 * This function prompts the user for input and stores the entered string in the provided `strg` buffer. 
 * The string is read up to the specified size `sz` to prevent buffer overflow. This function ensures that 
 * the entered string is properly terminated and that no characters beyond the buffer size are read.
 * 
 * @param strg The buffer where the input string will be stored.
 * @param sz The size of the string buffer, which limits how many characters can be read.
 * @param mensage The message displayed to the user prompting for the input.
 */
void readStrg(char* strg, unsigned int sz, char *mensage);

    /**
     * @brief Gets the current date from the system and returns the day of the month.
     * @return The day of the month.
     */
    int registDay ();
    
    /**
     * @brief Gets the current date from system and returns the month of the year.
     * @return The month of the year.
     */
    int registMonth();
    
    /**
     * @brief Gets the current date from system and returns the year.
     * @return The year.
     */
    int registYear();

#endif /* INPUT_H */
