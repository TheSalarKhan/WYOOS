#ifndef __TYPES_H
#define __TYPES_H
/**
	The purpose of this header file is to redefine primitive types to a more readable and standard version.
All the source code in this OS should use types defined here, and not use the primitive types on their own.
The reason behind doing so is that the data types in our code are redable, and that programmers can be sure
of the nature of the data type and the number of bits in it, this is crucial when dealing with hardware communication.
Another reason for doing this is that, if we want to compile our OS on a compiler that has different sizes for maybe
a 'long' then we'll redefine stuff here and then compile.
*/

typedef char int8_t;
typedef unsigned char uint8_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef int int32_t;
typedef unsigned int uint32_t;

typedef long long int int64_t;
typedef unsigned long long int uint64_t;

#endif
