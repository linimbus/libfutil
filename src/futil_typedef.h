

#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#ifndef __FUTIL_TYPEDEF_H__
#define __FUTIL_TYPEDEF_H__


/* 基本符号定义 */

typedef int8_t          INT8;
typedef uint8_t        UINT8;
typedef int16_t        INT16;
typedef uint16_t      UINT16;
typedef int32_t        INT32;
typedef uint32_t      UINT32;
typedef int64_t        INT64;
typedef uint64_t      UINT64;

typedef unsigned long UINTPTR;

typedef size_t         SIZE_T;


typedef char           CHAR;
typedef unsigned char UCHAR;

typedef void           VOID;

typedef float         FLOAT;
typedef double       DOUBLE;

typedef UINT32        BOOL;


#define B_FALSE    0
#define B_TRUE     1

#define RET_OK     0
#define RET_NODATA 1
#define RET_FULL   2



#define RET_ERR   -1



#define ALIGN_NUM(val,num) (((val) + (num) - 1) & ~((num) - 1))


#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */



