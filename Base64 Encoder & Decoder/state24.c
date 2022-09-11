/**
 * @file state24.c
 * @author Daniel Avisse (djavisse)
 * This is the State24 component. This component handels different functionalities 
 * with the State24 struct. When using a State24 the user can either add bytes or 
 * chars inside the State24 which can then be used later. A State24 can hold at most
 * 3 bytes or 24 bits. The user can then retrieve the bytes stored in the State24 by 
 * calling getBytes or they can use those bytes and turn them into chars using getChars.
 * Once one of these has been called the State24 will reset.
 */

#include "state24.h"

void initState ( State24 *state )
{
    //Set both byte and bit count to zero
    state->byteCount = 0;
    state->bitCount = 0;
    //Set all the bytes inside the data array to zero
    for ( int i = 0; i < MAX_NUMBER_OF_BYTES; i++ ) {
        state->data[i] = 0x0;
    }
}

bool validChar ( char ch )
{
    //Returns true if any of these chars are used
    if( ( ch >= 'A' && ch <= 'Z' ) || ( ch >= 'a' && ch <= 'z' ) || 
    ( ch >= '/' && ch <= '9') || ch == '+' ) {
        return true;
    }
    //Return false if not valid
    return false;
}

void addByte ( State24 *state, byte b)
{
    //Add a byte inside the data array and increment both counts
    if (state->byteCount < MAX_NUMBER_OF_BYTES) {
        state->data[state->byteCount++] = b;
        state->bitCount += SIZE_OF_BYTE;
    }
}

void addChar ( State24 *state, char ch)
{
    //Have a table containing all the different chars avaiable
    char base64Table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                        'w', 'x', 'y', 'z', '0', '1', '2', '3',
                        '4', '5', '6', '7', '8', '9', '+', '/'};

    //Create a bitValue that will be used to store in the data array
    unsigned char bitValue = 0x0;
    //Check if the ch being added is valid
    if ( validChar( ch ) ) {

        //Iterate through until the ch matches with one of the chars in the table
        for ( int i = 0; i < BASE64; i++ ) {
            if ( ch == base64Table[i] ) {
                //Break out of loop when they match
                break;
            }
        //Increment the bitValue if the chars don't match
        bitValue++;
        }
        //Shift to the left by two to get the 6 bits we need
        bitValue = bitValue << SHIFT_VALUE_TWO;

        //Check if there are no bits in the data array
        if ( state->bitCount == 0 ) {
            //Add the 6 bytes directly into the data array
            state->data[state->byteCount] = bitValue; 
            //Increase the bit count
            state->bitCount = NUMBER_OF_BITS_IN_CHAR;
        }

        //Check if there are 6 bits in the data array
        else if ( state->bitCount == NUMBER_OF_BITS_IN_CHAR ) {
            //Add the first two bits at the end of the first byte
            state->data[state->byteCount] = state->data[state->byteCount] | 
            (bitValue >> SHIFT_VALUE_SIX);
            //Increment byte count
            state->byteCount++;
            //Add the last 4 into the second byte
            state->data[state->byteCount] = bitValue << SHIFT_VALUE_TWO;
            //Increase bit count
            state->bitCount += NUMBER_OF_BITS_IN_CHAR;
        }

        //Check if there are 12 bits in the data array
        else if ( state->bitCount == TWELEVE_BITES ) {
            //Add the first 4 bits at the end of the second byte
            state->data[state->byteCount] = state->data[state->byteCount] | 
            ( bitValue >> SHIFT_VALUE_FOUR );
            //Increment byte count
            state->byteCount++;
            //Add the last two into the third byte
            state->data[state->byteCount] = bitValue << SHIFT_VALUE_FOUR;
            //Increase bit count
            state->bitCount += NUMBER_OF_BITS_IN_CHAR;
        }

        //Check if there are 18 bits in the data array
        else if ( state->bitCount == EIGHTEEN_BITES ) {      
            //Add 6 bits in the thrid byte to fill the array 
            state->data[state->byteCount] = state->data[state->byteCount] | 
            bitValue >> SHIFT_VALUE_TWO;
            //Increment byte count
            state->byteCount++;
            //Increase bit count
            state->bitCount = MAX_NUMBER_OF_BITS;
        }
    }
}

int getBytes ( State24 *state, byte buffer[] )
{
    //For each byte in the data array, add it into the buffer
    for ( int i = 0; i < state->byteCount; i++ ) {
        buffer[i] = state->data[i];
    }
    //Get the total number of bytes
    int numberOfBytes = state->byteCount;
    //Reinitialize the State24
    initState( state );
    //Return bytes
    return numberOfBytes;
}

int getChars ( State24 *state, char buffer[] )
{
    //Have a table containing all the different chars avaiable
    char base64Table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                        'w', 'x', 'y', 'z', '0', '1', '2', '3',
                        '4', '5', '6', '7', '8', '9', '+', '/'};

    //Get number of characters that will be returned
    int numberOfChars = state->byteCount + 1;
    //Loop enough times to get the value of each character
    for ( int i = 0; i < numberOfChars; i++ ) {
        //Create a bitField that will hold the bits that are equal to the char
        unsigned char bitFieldFromByte;

        //Get first 6 bits
        if ( i % MAX_NUMBER_OF_CHARS == 0 ) {
            unsigned char mask = MASK_FOR_FIRST_6_BITS;
            bitFieldFromByte = ( state->data[i] & mask ) >> SHIFT_VALUE_TWO;
        }

        //Get bits 6-12
        else if ( i % MAX_NUMBER_OF_CHARS == 1 ) {
            unsigned mask1 = MASK_ONE_FOR_BITS_6_TO_12;
            unsigned mask2 = MASK_TWO_FOR_BITS_6_TO_12;
            bitFieldFromByte = ( state->data[i -1] & mask1 ) << SHIFT_VALUE_FOUR | 
            ( state->data[i] & mask2 ) >> SHIFT_VALUE_FOUR;            
        }

        //Get bits 12-18
        else if ( i % MAX_NUMBER_OF_CHARS == TWO_BYTES ) {
            unsigned mask1 = MASK_ONE_FOR_BITS_12_TO_18;
            unsigned mask2 = MASK_TWO_FOR_BITS_12_T0_18;
            bitFieldFromByte = ( state->data[i - 1] & mask1 ) << SHIFT_VALUE_TWO | 
            ( state->data[i] & mask2 ) >> SHIFT_VALUE_SIX;
        }

        //Get bits 18-24
        else if ( i % MAX_NUMBER_OF_CHARS == MAX_NUMBER_OF_BYTES ) {
            unsigned mask = MASK_FOR_LAST_6_BITS;
            bitFieldFromByte = ( state->data[i -1] & mask );
        }

        //Create a value that will be used to find the bitValue of a char
        unsigned char findChar = 0x0;
        //Iterate through all the chars in the table until a value is found
        for( int j = 0; j < BASE64; j++ ) {
            //If the values match then break out of the loop and add the char to the buffer
            if ( findChar == bitFieldFromByte ) {
                buffer[i] = base64Table[j];
                break;
            }
            //Increment findChar value if they don't match
            findChar++;
        }
    }
    //Reinitialize the State24
    initState( state );
    //Return numberOfChars
    return numberOfChars;
}
