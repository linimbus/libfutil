

#include "futil_typedef.h"
#include "futil_atomic.h"



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#ifndef __FUTIL_QUEUE_H__
#define __FUTIL_QUEUE_H__

typedef void * QUE_HANDLE;


extern QUE_HANDLE FUTIL_QueCreate(UINT32 data_num, UINT32 data_size);

extern UINT32 FUTIL_QueRead(QUE_HANDLE handle, void * pdata);

extern UINT32 FUTIL_QueWrite(QUE_HANDLE handle, void * pdata);

extern UINT32 FUTIL_QueDelete(QUE_HANDLE handle);



#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */



