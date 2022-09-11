/**
 * @file state24.h
 * @author Daniel Avisse (djavisse)
 * This is the header file for the state24.c component. In this file it contains all the
 * constants and protypes used in state24.c
 */

#ifndef _STATE64_H_
#define _STATE64_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Include filebuffer to get the byte type.
#include "filebuffer.h"

/** Total number of bytes the State24 can store */
#define MAX_NUMBER_OF_BYTES 3

/** Number of valid chars we can use in are encoding */
#define BASE64 64

/** Value used to shift bits by 2 */
#define SHIFT_VALUE_TWO 2

/** Value used to shift bits by 4 */
#define SHIFT_VALUE_FOUR 4

/** Value used to shift bits by 6 */
#define SHIFT_VALUE_SIX 6

/** Total number of bites the State24 can store */
#define MAX_NUMBER_OF_BITS 24

/** Number of bits a char makes up in Base64 */
#define NUMBER_OF_BITS_IN_CHAR 6

/** Max number of chars can can be returned in getChars */
#define MAX_NUMBER_OF_CHARS 4

/** Condtional used for when there are 12 bits inside the State24 */
#define TWELEVE_BITES 12

/** Condtional used for when there are 18 bits inside the State24 */
#define EIGHTEEN_BITES 18

/** Condtional used for when there are 2 bytes inside the State24 */
#define TWO_BYTES 2

/** The value of the mask used to get the first 6 bits in getChars */
#define MASK_FOR_FIRST_6_BITS 0xFC

/** The value of the first mask used to get the bits 6-12 in getChars */
#define MASK_ONE_FOR_BITS_6_TO_12 0x3

/** The value of the second mask used to get the bits 6-12 in getChars */
#define MASK_TWO_FOR_BITS_6_TO_12 0xF0

/** The value of the first mask used to get the bits 12-18 in getChars */
#define MASK_ONE_FOR_BITS_12_TO_18 0xF

/** The value of the second mask used to get the bits 12-18 in getChars */
#define MASK_TWO_FOR_BITS_12_T0_18 0xC0

/** The value of the mask used to get the last 6 bits in getChars */
#define MASK_FOR_LAST_6_BITS 0x3F

/**
 * This is the State24 struct. In this struct there is a data array
 * that is able to hold up to three bytes. These bytes can be used for
 * encoding and decoding of files. To keep track of how many bytes and
 * bits inside the State24, there are also sum count values has fields 
 * in this struct.
 */
typedef struct {
  // Defined by you.
  // ...
  byte data[MAX_NUMBER_OF_BYTES];
  int byteCount;
  int bitCount;
} State24;

/**
 * This function is used to initialize the state. This will set all the fields 
 * inside the State24 to zero.
 * @param state The state24 that will be initialized
 */
void initState ( State24 *state );

/**
 * This function takes a char and makes sure that is a valid char in the Base64
 * encoding. If not then this fucntion will return false.
 * @param ch The char that will be checked to see if it's valid. 
 * @return true If the char is a valid char
 * @return false If the char is not valid
 */
bool validChar ( char ch );

/**
 * This function adds a byte to the State24. The max amount of bytes State24 
 * can have is 3 so no bytes will be added once 3 is reached.
 * @param state The state24 that will have bytes added to it
 * @param b The byte being added into the state24
 */
void addByte ( State24 *state, byte b );

/**
 * This function takes a valid char and converts that into 6 bits that
 * will then be added into the State24. A State24 can hold at most 24 bits
 * so chars won't be added once this is reached
 * @param state The state24 that will have chars added to it
 * @param ch The char being added to the state24
 */
void addChar ( State24 *state, char ch );

/**
 * This function gets all the bytes from the State24 and puts them
 * inside a buffer. At most 3 bytes can be returned into the buffer.
 * Once the bytes have been added to the buffer the state will then be reinitialized. 
 * @param state The state24 that will add bytes to a buffer
 * @param buffer The buffer that will get bytes from the state24
 * @return int Total number of bytes added into the buffer
 */
int getBytes ( State24 *state, byte buffer[] );

/**
 * This function will convert all the bits inside the State24 into chars that 
 * will be added inside a buffer. At most 4 chars can be returned into the buffer.
 * Once the bits have been added to the buffer the state will then be reinitialized.
 * @param state The state24 that will add chars into the buffer
 * @param buffer The buffer that will get chars from the state24
 * @return int The total number of chars added into the buffer
 */
int getChars ( State24 *state, char buffer[] );

#endif
