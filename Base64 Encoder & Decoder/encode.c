/**
 * @file encode.c
 * @author Daniel Avisse (djavisse)
 * This is the encode component and is the main component for the encode program.
 * This component uses FileBuffer and State24 to read a binary file and use the
 * contents of the binary file to output it to a text file with characters.
 * The user also has the option to change the output of the encoded files by
 * adding optional commands, -b and -p. -b allows for no line breaks and -p
 * allows for no padding. 
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "filebuffer.h"
#include "state24.h"

/** Value used to limit the amount of chars that can appear in one line */
#define CHAR_LIMIT 77

/** The command used to not have line breaks in the encoded file */
#define BREAK_COMMAND "-b"

/** The command used to not have padding in the encoded file */
#define PADDING_COMMAND "-p"

/** Value used when working with command line arguments */
#define ARG_VALUE_FIVE 5

/** Value used when working with command line arguments */
#define ARG_VALUE_FOUR 4

/** Value used when working with command line arguments */
#define ARG_VALUE_THREE 3

/** Value used when working with command line arguments */
#define ARG_VALUE_TWO 2

/**
 * This function is a helper function used to help encode the input bin file into
 * a txt file with encoded letters. This function also has optional commands that allow
 * for the user to change the output of the output file if they desire.
 * @param inputfile The input bin file containing the bytes that will be encoded
 * @param outputfile The output txt file that will have output encoded letters
 * @param bFlag Flag that tells if the user wants to use the break command
 * @param pFlag Flag that tells if the user want to use the padding command
 */
static void encode ( char inputfile[], char outputfile[], bool bFlag, bool pFlag ) 
{
    //Create and load a new filebuffer with all the bytes
    FileBuffer *encodeFileBuffer = loadFileBuffer( inputfile );
    //Create a new State24 to use the bytes to convert to chars
    State24 *newState24 = ( State24 * )malloc(sizeof( State24 ) );
    initState( newState24 );
    int charBufferCapcity = INITIAL_CAPACITY;
    //Create a charBuffer that will contain all the chars 
    char *fullCharbuffer = ( char * )malloc( charBufferCapcity * sizeof( char ) );
    int charBufferCount = 0;
    //Create a miniBuffer that will temporarily hold the chars that will be added in the fullBuffer
    char miniCharbuffer[MAX_NUMBER_OF_CHARS];
    int miniCharBufferCount = 0;

    //Iterate through all the bytes in the filebuffer
    for( int i = 0; i < encodeFileBuffer->count; i++ ) {
        //Add the byte into the State24
        addByte( newState24, encodeFileBuffer->data[i] );

        //Check if State24 has the max amount of bytes it can contain
        if ( newState24->byteCount == MAX_NUMBER_OF_BYTES ) {
            //Fill the minicharBuffer with the chars from State24
            miniCharBufferCount = getChars( newState24, miniCharbuffer );
            //Put each char into the fullcharBuffer
            for ( int j = 0; j < miniCharBufferCount; j++ ) {
                //Resize the fullCharBuffer if necessary
                if ( charBufferCount >= charBufferCapcity ) {
                    charBufferCapcity *= DOUBLE_SIZE;
                    void *temp = ( char *) realloc( fullCharbuffer, 
                    charBufferCapcity * sizeof( char ) );
                    if (temp == NULL) {
                        free(fullCharbuffer);
                        free(encodeFileBuffer);
                        free(newState24);
                        exit(EXIT_FAILURE);
                    }
                    fullCharbuffer = temp;
                }
            fullCharbuffer[charBufferCount++] = miniCharbuffer[j];
            }
        }

        //Check if the bytes are less than 3 and we reached the end of the file
        else if ( newState24->byteCount < MAX_NUMBER_OF_BYTES && i + 1 == encodeFileBuffer->count ) {
            //Check if the bytes in State24 are equal to two
            if( newState24->byteCount == TWO_BYTES ) {
                //Fill the minicharBuffer with the chars from State24
                miniCharBufferCount = getChars( newState24, miniCharbuffer );
                //Put each char into the fullcharBuffer
                for( int j = 0; j < miniCharBufferCount + 1; j++ ) {
                    //Resize the fullCharBuffer if necessary
                    if ( charBufferCount >= charBufferCapcity ) {
                        charBufferCapcity *= DOUBLE_SIZE;
                        void *temp = ( char *) realloc( fullCharbuffer, 
                        charBufferCapcity * sizeof( char ) );
                        if (temp == NULL) {
                            free(fullCharbuffer);
                            free(encodeFileBuffer);
                            free(newState24);
                        }
                        fullCharbuffer = temp;
                    }
                    //Add one equal sign at the end of the input if the pFlag is false
                    if ( j == miniCharBufferCount && !pFlag ) {
                        fullCharbuffer[charBufferCount++] = '=';
                    }
                    //Add the chars into the fullCharBuffer
                    else if ( j < miniCharBufferCount ) {
                        fullCharbuffer[charBufferCount++] = miniCharbuffer[j];
                    }
                }
            }

            //Check if the bytes in State24 are equal to one
            if( newState24->byteCount == 1 ) {
                //Fill the minicharBuffer with the chars from State24
                miniCharBufferCount = getChars( newState24, miniCharbuffer );
                //Put each char into the fullcharBuffer
                for( int j = 0; j < miniCharBufferCount + TWO_BYTES; j++ ) {
                    //Resize the fullCharBuffer if necessary
                    if ( charBufferCount >= charBufferCapcity ) {
                        charBufferCapcity *= DOUBLE_SIZE;
                        void *temp = ( char *) realloc( fullCharbuffer, 
                        charBufferCapcity * sizeof( char ) );
                        if (temp == NULL) {
                            free(fullCharbuffer);
                            free(encodeFileBuffer);
                            free(newState24);
                        }
                        fullCharbuffer = temp;
                    }
                    //Add two equal signs at the end of the input if the pFlag is false
                    if ( j >= miniCharBufferCount && !pFlag ) {
                        fullCharbuffer[charBufferCount++] = '=';
                    }
                    //Add the chars into the fullCharBuffer
                    else if ( j < miniCharBufferCount ) {
                        fullCharbuffer[charBufferCount++] = miniCharbuffer[j];
                    }
                }
            }
        }
    }
    //Use this to count how many chars have be printed in a line
    int charCount = 0;
    //Open the outputfile
    FILE *outstream = fopen( outputfile, "w" );
    //Report failure message and exit program if the outputfile can't be opened
    if ( !outstream ) {
        perror( outputfile );
        exit( EXIT_FAILURE );
    }
    //If the bFlag is set to true then print the chars with line breaks
    if ( bFlag ) {
        for( int i = 0; i < charBufferCount; i++ ) {
        fprintf( outstream, "%c", fullCharbuffer[i] );
        }
    }
    //Print the chars with line breaks
    else {
        for( int i = 0; i < charBufferCount; i++ ) {
            charCount++;
            if ( charCount == CHAR_LIMIT ) {
                fprintf( outstream, "\n" );
                charCount = 1;
            }
            fprintf( outstream, "%c", fullCharbuffer[i] );
        }
    }
    //Print a newline at the end
    fprintf( outstream, "\n" );
    //Free everything
    freeFileBuffer( encodeFileBuffer );
    free( fullCharbuffer );
    free( newState24 );
    fclose(outstream);
}

/**
 * This is the main function of encode. This function will check to see if the 
 * user has inputted the correct arguments and will call the encode helper function
 * with the appropriate paramters. If the user inputs incorrect arguments then the 
 * program will exit and print out the usasge for the user.
 * @param argc Number of command-line arguments
 * @param argv Array containing the different command-line arguments
 * @return int EXIT_SUCCESS
 */
int main ( int argc, char *argv[] )
{
    //If the user inputted an incorrect amount of arguments then print usage message
    if ( argc == 1 || argc > ARG_VALUE_FIVE ) {
        fprintf( stderr, "usage: encode [-b] [-p] <input-file> <output-file>\n" );
        exit( EXIT_FAILURE );
    }
    /*If number of arguments is equal to 3 then call the encode function 
    using the input and output files that the user has inputted */
    if ( argc == ARG_VALUE_THREE ) {
        encode( argv[1], argv[ARG_VALUE_TWO], false, false );
    }
    //If the number of arguments is 4 then check if the user inptted a valid command
    else if ( argc == ARG_VALUE_FOUR ) {
        //If the command is equal to the break command then set bFlag to true
        if ( strcmp( BREAK_COMMAND, argv[1] ) == 0 ) {
            encode(argv[ARG_VALUE_TWO], argv[ARG_VALUE_THREE], true, false);
        }
        //If the command is equal to the padding command then set pFlag to true
        else if ( strcmp( PADDING_COMMAND, argv[1] ) == 0 ) {
            encode(argv[ARG_VALUE_TWO], argv[ARG_VALUE_THREE], false, true);
        }
        //Exit the program and print usage message since the command is invalid
        else {
            fprintf( stderr, "usage: encode [-b] [-p] <input-file> <output-file>\n" );
            exit( EXIT_FAILURE );
        }
    }
    //If the number of arguments is 5 then check that both commands are valid
    else if ( argc == ARG_VALUE_FIVE ) {
        //If both commands are valid then set both flags to true
        if ( ( strcmp( BREAK_COMMAND, argv[1] ) == 0 ) && 
        ( strcmp( PADDING_COMMAND, argv[ARG_VALUE_TWO] ) == 0 ) ) {
            encode( argv[ARG_VALUE_THREE], argv[ARG_VALUE_FOUR], true, true );
        }
        //If both commands are valid then set both flags to true
        else if ( ( strcmp( PADDING_COMMAND, argv[1] ) == 0 ) && 
        ( strcmp( BREAK_COMMAND, argv[ARG_VALUE_TWO] ) == 0 ) ) {
            encode( argv[ARG_VALUE_THREE], argv[ARG_VALUE_FOUR], true, true );
        }
        //If the user puts the same command twice then just run encode with the correct command
        else if ( ( strcmp( BREAK_COMMAND, argv[1] ) == 0 ) && 
        ( strcmp( BREAK_COMMAND, argv[ARG_VALUE_TWO] ) == 0 ) ) {
            encode( argv[ARG_VALUE_THREE], argv[ARG_VALUE_FOUR], true, false );
        }
        //If the user puts the same command twice then just run encode with the correct command
        else if ( ( strcmp( PADDING_COMMAND, argv[1] ) == 0 ) && 
        ( strcmp( PADDING_COMMAND, argv[ARG_VALUE_TWO] ) == 0 ) ) {
            encode( argv[ARG_VALUE_THREE], argv[ARG_VALUE_FOUR], false, true );
        }
        //Exit the program and print usage message since the commands are invalid
        else {
            fprintf( stderr, "usage: encode [-b] [-p] <input-file> <output-file>\n" );
            exit( EXIT_FAILURE );
        }
    }
    //Exit program successfully
    return EXIT_SUCCESS;
}
