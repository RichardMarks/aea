//
// Created by Richard Marks on 12/10/23.
//

#ifndef AEA_PLATFORM_TYPES_H
#define AEA_PLATFORM_TYPES_H

// signed
typedef signed char AEA_s8;
typedef signed short AEA_s16;
typedef signed int AEA_s32;
typedef signed long long AEA_s64;

// unsigned
typedef unsigned char AEA_u8;
typedef unsigned short AEA_u16;
typedef unsigned int AEA_u32;
typedef unsigned long long AEA_u64;

// floating point
typedef float AEA_f32;
typedef double AEA_f64;

// string
typedef char* AEA_str;
typedef const char* AEA_cstr;

// util
typedef void AEA_void;
typedef void* AEA_any;
typedef unsigned long AEA_size;

#endif //AEA_PLATFORM_TYPES_H
