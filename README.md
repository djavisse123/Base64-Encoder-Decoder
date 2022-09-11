# Overview
The Base64 Encoder & Decoder uses the Base64 binary to text encoding technique where a total of 64 characters are represented by 6 bits. Bits are read in a sequence of 24 bits which means that a total of 4 characters can be represented in each sequence of 24 bits. If there are less than 24 bits in this sequence then padding is used to fill in the leftovers with an equal sign being used to pad in the missing bits. In this project the user is able to encode a binary file to a readable text file or decode a text file into a binary file. The user also has the option to choose if they want no padding or line breaks when encoding a binary file.
# Usage

To compile and run this project you must have gcc. This program was written in C and needs gcc to compile. Follow these steps to run the implementation:

Follow these steps to run the implementation:

1. Run this command in Linux: `make`

### To Use the Encoder:

Default usage: `encode <input-file> <output-file>`

Use without padding: `encode [-p] <input-file> <output-file>`

Use without breaks: `encode [-b] <input-file> <output-file>`

Use without padding and breaks: `encode [-b] [-p] <input-file> <output-file>`

### To Use the Decoder:

Usage: `decode <input-file> <output-file>`

# Design Requirements

### Programming Language
The implementation of this project is required to be created in C.

### Components
The creation of this project requires the use of six components total, three for encoding and three for decoding. All information regarding how each components works along with a figure displaying the overall design of the project is listed below.

![image7](https://user-images.githubusercontent.com/112775148/189508058-feace1a3-237f-4058-b8e5-2cff57334379.png)

* The ***State24*** component is responsible for managing a sequence of 24 bits for encoding and decoding. This component can either convert a sequence of 24 bits into 4 base64 characters or check for valid characters and convert them into a sequence of 24 bits. **Note: The header file for the State24 component was provided.**
* The ***FileBuffer*** component is responsible for managing bytes that will be used for encoding and decoding. For encoding, the FileBuffer component will read all the bytes in a binary file which can then be processed by the State24 component. For decoding, the FileBuffer component will output all the converted bytes from the characters into a binary file. **Note: The header file for the FileBuffer component was provided.**
* The ***encode*** component is responsible for encoding a binary file to a readable text file. The encode component uses the State24 and FileBuffer components to gather the bytes from the binary file, convert the bytes to ASCII characters, and then print the result to a new output file.
* The ***decode*** component is responsible for decoding a text file into a binary file. The decode component uses the State24 and FileBuffer components to convert all ASCII characters from the input file into binary and then output all the bytes to a new binary file.

### Additional Information

This project is required to be completed individually. All diagrams presented in this README belong to the NCSU CSC Department.

# Implementation
For this project, the main focus was to use bit shifting to convert binary bits to ASCII characters and vise versa. We also used techniques from previous projects like dynamic memory allocation and creating structs. The first major thing I needed to do in this project was to create structs for filebuffer and state24. These structs would be used to hold the bytes used for encoding and decoding with filebuffer holding the entire file’s bytes and state24 hold 3 bytes at a time. Both of these use allocated memory with filebuffer being able to dynamically allocate more space if the amount of bytes in the file is more than the initial capacity. Once execution of the program is done then all memory allocated is freed. These programs have been tested in many different situations to ensure there are no memory leaks. For bit shifting the state24 component is where we had to use the bit shifting technique to convert binary bits to ASCII characters. When converting from binary to ASCII I created an algorithm in the getChars function where depending on the number of bytes in state24, it use a mask that would be able to get the bits needed after shifting. I would then use this bit field gathered from the bytes in state24 and compare it with the binary value of the different ASCII characters. Once there is a match the ASCII character is added to a char buffer where they can be printed by the encoder. The algorithm for the addChar function in state24 kind of works in reverse where I use the binary value of the ASCII character and then use bit shifting to fit in a 3 byte state 24. This can then be used the decoder to output a binary file.
