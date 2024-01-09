
#include "futil_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern BOOL FUTIL_AtomicAdd(UINT32 * pAddr, UINT32 value);

extern BOOL FUTIL_AtomicSub(UINT32 * pAddr, UINT32 value);

extern BOOL FUTIL_AtomicInc(UINT32 * pAddr);

extern BOOL FUTIL_AtomicDec(UINT32 * pAddr);

extern BOOL FUTIL_AtomicCas(UINT32 * pAddr, UINT32 valueold, UINT32 valuenew);



extern BOOL FUTIL_Atomic64Add(UINT64 * pAddr, UINT64 value);

extern BOOL FUTIL_Atomic64Sub(UINT64 * pAddr, UINT64 value);

extern BOOL FUTIL_Atomic64Inc(UINT64 * pAddr);

extern BOOL FUTIL_Atomic64Dec(UINT64 * pAddr);

extern BOOL FUTIL_Atomic64Cas(UINT64 * pAddr, UINT64 valueold, UINT64 valuenew);




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


