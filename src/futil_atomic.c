
#include "futil_atomic.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define LOCK_PREFIX "lock;"

static inline unsigned long __cmpxchg(volatile void *ptr, unsigned long old, unsigned long new, int size)
{
    unsigned long prev;

    switch (size) {
    case 1:
        asm volatile(LOCK_PREFIX "cmpxchgb %b1,%2"
                 : "=a"(prev)
                 : "q"(new), "m"(*(unsigned char *)(ptr)), "0"(old)
                 : "memory");
        return prev;
    case 2:
        asm volatile(LOCK_PREFIX "cmpxchgw %w1,%2"
                 : "=a"(prev)
                 : "r"(new), "m"(*(unsigned short *)(ptr)), "0"(old)
                 : "memory");
        return prev;
    case 4:
        asm volatile(LOCK_PREFIX "cmpxchgl %k1,%2"
                 : "=a"(prev)
                 : "r"(new), "m"(*(unsigned int *)(ptr)), "0"(old)
                 : "memory");
        return prev;
    case 8:
        asm volatile(LOCK_PREFIX "cmpxchgq %1,%2"
                 : "=a"(prev)
                 : "r"(new), "m"(*(unsigned long *)(ptr)), "0"(old)
                 : "memory");
        return prev;
    }

    return old;
}

BOOL FUTIL_AtomicAdd(UINT32 * pAddr, UINT32 value)
{
    UINT32 oldvalue;
    UINT32 ret;

    do {
        oldvalue = *pAddr;
        ret = (UINT32)__cmpxchg(pAddr, oldvalue, oldvalue + value, sizeof(UINT32));
    } while ( ret != oldvalue );

    return B_TRUE;
}

BOOL FUTIL_AtomicSub(UINT32 * pAddr, UINT32 value)
{
    UINT32 oldvalue;
    UINT32 ret;

    do {
        oldvalue = *pAddr;
        ret = (UINT32)__cmpxchg(pAddr, oldvalue, oldvalue - value, sizeof(UINT32));
    } while ( ret != oldvalue );

    return B_TRUE;
}

BOOL FUTIL_AtomicInc(UINT32 * pAddr)
{
    UINT32 oldvalue;
    UINT32 ret;

    do {
        oldvalue = *pAddr;
        ret = (UINT32)__cmpxchg(pAddr, oldvalue, oldvalue + 1, sizeof(UINT32));
    } while ( ret != oldvalue );

    return B_TRUE;
}

BOOL FUTIL_AtomicDec(UINT32 * pAddr)
{
    UINT32 oldvalue;
    UINT32 ret;

    do {
        oldvalue = *pAddr;
        ret = (UINT32)__cmpxchg(pAddr, oldvalue, oldvalue - 1, sizeof(UINT32));
    } while ( ret != oldvalue );

    return B_TRUE;
}

BOOL FUTIL_AtomicCas(UINT32 * pAddr , UINT32 valueOld , UINT32 valueNew)
{
    UINT32 ret = (UINT32)__cmpxchg(pAddr, valueOld, valueNew, sizeof(UINT32));
    return ( ret == valueOld ) ? B_TRUE : B_FALSE;
}




BOOL FUTIL_Atomic64Add(UINT64 * pAddr, UINT64 value)
{
    UINT64 oldvalue;
    UINT64 ret;

    do {
        oldvalue = *pAddr;
        ret = __cmpxchg(pAddr, oldvalue, oldvalue + value, sizeof(UINT64));
    } while ( ret != oldvalue );

    return B_TRUE;
}

BOOL FUTIL_Atomic64Sub(UINT64 * pAddr, UINT64 value)
{
    UINT64 oldvalue;
    UINT64 ret;

    do {
        oldvalue = *pAddr;
        ret = __cmpxchg(pAddr, oldvalue, oldvalue - value, sizeof(UINT64));
    } while ( ret != oldvalue );

    return B_TRUE;
}

BOOL FUTIL_Atomic64Inc(UINT64 * pAddr)
{
    UINT64 oldvalue;
    UINT64 ret;

    do {
        oldvalue = *pAddr;
        ret = __cmpxchg(pAddr, oldvalue, oldvalue - 1, sizeof(UINT64));
    } while ( ret != oldvalue );

    return B_TRUE;
}

BOOL FUTIL_Atomic64Dec(UINT64 * pAddr)
{
    UINT64 oldvalue;
    UINT64 ret;

    do {
        oldvalue = *pAddr;
        ret = __cmpxchg(pAddr, oldvalue, oldvalue - 1, sizeof(UINT64));
    } while ( ret != oldvalue );

    return B_TRUE;
}


BOOL FUTIL_Atomic64Cas(UINT64 * pAddr, UINT64 valueold, UINT64 valuenew)
{
    UINT64 ret = __cmpxchg(pAddr, valueold, valuenew, sizeof(UINT64));
    return ( ret == valueold ) ? B_TRUE : B_FALSE;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


