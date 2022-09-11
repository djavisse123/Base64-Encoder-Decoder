/**
 * @file filebuffer.c
 * @author Daniel Avisse (djavisse)
 * 
 */
#include "filebuffer.h"

FileBuffer *makeFileBuffer () 
{
    //Allocate space for a new fileBuffer
    FileBuffer *buffer = ( FileBuffer * )malloc( sizeof( FileBuffer ) );
    //Set the inital capacity and set count to zero
    buffer->capacity = INITIAL_CAPACITY;
    buffer->count = 0;
    //Allocate space for the data array using the capacity
    buffer->data = ( byte * )malloc( buffer->capacity * sizeof(byte ) );
    //Return the new buffer
    return buffer;
}

void freeFileBuffer ( FileBuffer * buffer )
{
    //Free the data array in fileBuffer
    free( buffer->data );
    //Free the fileBuffer itself
    free( buffer );
}

void appendFileBuffer ( FileBuffer * buffer, byte val )
{
    //Add the byte to the data array in fileBuffer
    buffer->data[buffer->count++] = val;
    //Resize the buffer if necessary
    if ( buffer->count >= buffer->capacity ) {
        buffer->capacity *= DOUBLE_SIZE;
        buffer->data = ( byte * ) realloc( buffer->data, buffer->capacity * sizeof( byte ) );
    }
}

FileBuffer *loadFileBuffer ( char const * filename) 
{
    //Open the file in binary read mode
    FILE *stream = fopen( filename, "rb" );
    //If the file doesn't exist then exit the program and report error message
    if ( !stream ) {
        perror( filename );
        exit( EXIT_FAILURE );
    }
    //Create a new fileBuffer
    FileBuffer *buffer = makeFileBuffer();
    //Go to the end of the file
    fseek( stream, 0, SEEK_END );
    //Get the length of the file
    int fileSize = ftell( stream );
    //Go back to the beginning of the file
    rewind( stream );
    //Create a byte array of the size of the length of the file
    byte data[fileSize];
    //Read all the bytes into the data array
    fread( data, sizeof( byte ), fileSize, stream ); 
    //Add all the bytes from the data array into the filebuffer  
    for ( int i = 0; i < fileSize; i++ ) {
        appendFileBuffer( buffer, data[i] );
    } 
    //Return the buffer
    fclose( stream );
    return buffer;
}

void saveFileBuffer ( FileBuffer * buffer, char const *filename )
{
    //Open the filebuffer in binary write mode
    FILE *stream = fopen( filename, "wb" );
    //If the file can't be opened or made then exit the program and report error message
    if ( !stream ) {
        perror( filename );
        exit( EXIT_FAILURE );
    }
    //Write all the data from the buffer onto the file
    fwrite( buffer->data, sizeof( byte ), buffer->count, stream );
    fclose( stream );
}
