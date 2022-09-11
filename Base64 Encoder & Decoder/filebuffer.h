/**
 * @file filebuffer.h
 * @author Daniel Avisse (djavisse)
 * This is the header file for the filebuffer.c component. In this file it contains all the
 * constants and protypes used in filebuffer.c
 */

#ifndef _FILEBUFFER_H_
#define _FILEBUFFER_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** A shorthand for talking about a byte. */
typedef unsigned char byte;

/** The inital capacity when creating filebuffers */
#define INITIAL_CAPACITY 5

/** Value used to double the capacity of the filebuffer */
#define DOUBLE_SIZE 2

/** How many bits make up a byte */
#define SIZE_OF_BYTE 8

/** Representation of a resizable array of bytes, with functions to
    make it easy to read and write the contents of this array to a
    file. */
typedef struct {
  /** Resizable array of bytes stored in this filebuffer. */
  byte *data;

  // More fields added by you.
  // ..
  int capacity;
  int count; 
} FileBuffer;

/**
 * This function creates a new fileBuffer. In this function it will allocate space for the
 * filebuffer and also initialize all the fields.
 * @return FileBuffer* The new filebuffer that was created
 */
FileBuffer *makeFileBuffer();

/**
 * This function frees all the memory that has be allocated by the filebuffer. 
 * @param buffer The filebuffer that will have it's memory freed
 */
void freeFileBuffer( FileBuffer * buffer );

/**
 * This function adds new bytes to the fileBuffer. This function makes sure to resize
 * the filebuffer data array if the capacity is too small to add more bytes
 * @param buffer The buffer that will get bytes added to
 * @param val The byte that will be added to the filebuffer
 */
void appendFileBuffer( FileBuffer * buffer, byte val );

/**
 * This function reads a bin and is able to load all the contents of the file into a new 
 * filebuffer. The filebuffer is then returned once all the contents of the file have
 * been read. 
 * @param filename The file that will be read
 * @return FileBuffer* The filebuffer returned after reading all the contents of the file
 */
FileBuffer *loadFileBuffer ( char const * filename);

/**
 * This function saves all the bytes of a filebuffer to a new bin file. 
 * @param buffer The filebuffer that will output all the bytes to a new file
 * @param filename The name of the file that will be created
 */
void saveFileBuffer ( FileBuffer * buffer, char const *filename);

#endif
