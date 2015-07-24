#ifndef MODTYPES_H_
#define MODTYPES_H_

typedef unsigned char BYTE;
typedef unsigned short int USHORT;
typedef signed short int SSHORT;
typedef unsigned long int ULONG;
typedef signed long int SLONG;

union flint
{
    float f;
    VstInt32 i;
};

#endif