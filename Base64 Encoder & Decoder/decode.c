/**
 * @file decode.c
 * @author Daniel Avisse (djavisse)
 * This is the deconde component and is the main component of the decode program.
 * This component creates a filebuffer that will contain all the bytes that will
 * be outputted to a new binary file. This component will first read an encoded file
 * and get all the chars and add them into a state24 to get the bytes from those chars.
 * Once all the chars have been added to the filebuffer then the new file will be created.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "filebuffer.h"
#include "state24.h"

/** Value used when working with command line arguments */
#define ARG_VALUE_THREE 3

/** Value used when working with command line arguments */
#define ARG_VALUE_TWO 2

/**
 * This is the main function of decode. This function will decode the encoded inputfile
 * by getting all the chars in the file and adding them into a state24. Once the state24
 * is full, the bytes will be retrieved from the state24 and then added to a filebuffer.
 * The filebuffer will then be used to output all the bytes to a new binary file.
 * @return int EXIT_SUCCESS
 */
int main ( int argc, char *argv[] ) 
{
    //If the incorrect amount of arguments is used then exit the program with usage message
    if ( argc != ARG_VALUE_THREE ) {
        fprintf( stderr, "usage: decode <input-file> <output-file>\n" );
        exit( EXIT_FAILURE );
    }
    //Make a new fileBuffer
    FileBuffer *decodeFileBuffer = makeFileBuffer();
    //Create a new State24 and initialize it
    State24 *newState24 = ( State24 * )malloc( sizeof( State24 ) );
    initState( newState24 );
    //Open the inputfile
    FILE *inStream = fopen( argv[1], "r" );
    //If the inputfile can't be open then exit with error message
    if ( !inStream ) {
        perror( argv[1] );
        exit( EXIT_FAILURE );
    }
    //Go to the end of the file
    fseek( inStream, 0, SEEK_END );
    //Get the length of the file
    int fileLength = ftell( inStream );
    //Go back to the beginning of the file
    rewind( inStream );
    //Create a charBuffer to hold all the chars
    char charBuffer[fileLength];
    //Create a miniByteBuffer to hold the bytes that will then be added to the filebuffer
    byte miniByteBuffer[MAX_NUMBER_OF_BYTES];
    int miniByteBufferCount = 0;
    char ch;
    //Flag that checks when an equal sign is found
    bool equalFlag = false;
    //Start adding all the chars into the charBuffer
    for( int i = 0; i < fileLength; i++ ) {
        ch = fgetc( inStream );
        //If the char is valid and doesn't come after a equal sign then add it to the buffer
        if ( validChar( ch ) && !equalFlag ) {
            charBuffer[i] = ch;
        }
        //Add the char to the buffer and set the equal fag to true
        else if ( ch == '=' ) {
            charBuffer[i] = ch;
            equalFlag = true;
        }
        //If the char is a newline then add that as well
        else if ( ch == '\n' ) {
            charBuffer[i] = ch;
        }
        //The char wasn't valid. Exit the program and print error message
        else {
            fprintf( stderr, "Invalid input file\n" );
            freeFileBuffer( decodeFileBuffer );
            free( newState24 );
            fclose(inStream);
            exit( EXIT_FAILURE );
        }
    }
    //Iterate through all the chars
    for ( int i = 0; i < fileLength; i++ ) {
        //Add the chars into the State24
        addChar( newState24, charBuffer[i] );

        //If the State24 has reached max bit count, then get bytes from it
        if ( newState24->bitCount == MAX_NUMBER_OF_BITS ) {
            miniByteBufferCount = getBytes( newState24, miniByteBuffer );
            for ( int j = 0; j < miniByteBufferCount; j++ ) {
                appendFileBuffer( decodeFileBuffer, miniByteBuffer[j] );
            }
        }

        //Get bytes from State24 if the bit count is less than max and we have reached the end
        else if ( newState24->bitCount < MAX_NUMBER_OF_BITS && i + 1 == fileLength ) {
            miniByteBufferCount = getBytes( newState24, miniByteBuffer );
            for ( int j = 0; j < miniByteBufferCount; j++ ) {
                appendFileBuffer( decodeFileBuffer, miniByteBuffer[j] );
            }
        }
    }
    //Save the filebuffer to a new file
    saveFileBuffer( decodeFileBuffer, argv[ARG_VALUE_TWO] );
    //Free everything
    freeFileBuffer(decodeFileBuffer);
    free(newState24);
    //Close the inputstream
    fclose(inStream);
    //Exit successfully
    EXIT_SUCCESS;
}
