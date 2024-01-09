
#include "futil_queue.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define MAX_NUM       0xffff
#define MAX_SIZE      0xff

#define STATUS_NONE   1
#define STATUS_WRITE  2
#define STATUS_READ   3

typedef struct datastatus {
    UINT16 version;
    UINT16 status;
}DATA;

union UN_DATA {
    DATA   dat;
    UINT32 value;
};


typedef struct tagquenode {
    union UN_DATA flag;
    char data[0];
}QUE_DATA_S;


typedef struct posversion {
    UINT16 version;
    UINT16 vpos;
}POS_S;


union UN_POS {
    POS_S  pos;
    UINT32 value;
};


typedef struct tagquectrl {
    UINT32 num;
    UINT32 size;
    union UN_POS posR;
    union UN_POS posW;
    char * pdata;
}QUE_CTRL_S;


QUE_HANDLE FUTIL_QueCreate(UINT32 data_num, UINT32 data_size)
{
    UINT32 i,j;
    UINT32 node_size;
    UINT32 totalsize;

    QUE_CTRL_S * pQue;
    QUE_DATA_S * pNode;

    if( data_num > MAX_NUM || data_size > MAX_SIZE )
    {
        return NULL;
    }

    node_size = data_size + sizeof(QUE_DATA_S);
    node_size += ALIGN_NUM( node_size, sizeof(UINT64) );

    totalsize = data_num * node_size + sizeof(QUE_CTRL_S);

    pQue = (QUE_CTRL_S *)malloc(totalsize);
    if( NULL == pQue )
    {
        return NULL;
    }

    (void)memset((void *)pQue, 0, totalsize);

    pNode = (QUE_DATA_S *)(pQue + 1);

    pQue->num    = data_num;
    pQue->size   = node_size;
    pQue->posR.value = 0;
    pQue->posW.value = 0;

    pQue->pdata = (char *)pNode;

    for( i = 0 ; i < data_num ; i++ )
    {
        pNode->flag.dat.status  = STATUS_WRITE;
        pNode->flag.dat.version = 0;

        pNode = (QUE_DATA_S *)((char *)pNode + node_size);
    }

    return (QUE_HANDLE)pQue;
}


UINT32 FUTIL_QueRead(QUE_HANDLE handle, void * pdata)
{
    BOOL   ret;
    UINT32 cnt;

    union UN_POS oldValue;
    union UN_POS newValue;

    QUE_CTRL_S * pQue;
    QUE_DATA_S * pNode;

    pQue = (QUE_CTRL_S *)handle;
    if( NULL == pQue )
    {
        return RET_ERR;
    }

retry:

    oldValue = pQue->posR;

    if( oldValue.value == pQue->posW.value )
    {
        return RET_NODATA;
    }

    newValue.pos.version = oldValue.pos.version + 1;
    newValue.pos.vpos    = (oldValue.pos.vpos + 1) % pQue->num ;

    ret = FUTIL_AtomicCas(&(pQue->posR.value), oldValue.value , newValue.value );
    if( B_FALSE == ret )
    {
        goto retry;
    }

    cnt = 0;

    do {

        union UN_DATA olddat;
        union UN_DATA newdat;

        pNode = (QUE_DATA_S *)(pQue->pdata + ( newValue.pos.vpos * pQue->size ));

        olddat = pNode->flag;
        newdat.dat.version = olddat.dat.version + 1;

        if( olddat.dat.status != STATUS_READ )
        {
            cnt++;
            if( cnt > 3 )
            {
                newdat.dat.status = STATUS_NONE;
            }
            else
            {
                continue;
            }
        }
        else
        {
            newdat.dat.status = STATUS_WRITE;
        }

        ret = FUTIL_AtomicCas(&(pNode->flag.value), olddat.value , newdat.value );
        if( B_TRUE == ret )
        {
            if( olddat.dat.status == STATUS_READ )
            {
                memcpy(pdata, pNode->data, pQue->size);

                return RET_OK;
            }
            else
            {
                goto retry;
            }
        }
    }while(1);

    return RET_OK;
}

UINT32 FUTIL_QueWrite(QUE_HANDLE handle, void * pdata)
{
    BOOL   ret;
    UINT32 cnt;

    union UN_POS oldValue;
    union UN_POS newValue;

    QUE_CTRL_S * pQue;
    QUE_DATA_S * pNode;

    pQue = (QUE_CTRL_S *)handle;
    if( NULL == pQue )
    {
        return RET_ERR;
    }

retry:

    oldValue = pQue->posW;

    if( oldValue.pos.version == pQue->posR.pos.version + pQue->num )
    {
        return RET_FULL;
    }

    newValue.pos.version = (oldValue.pos.version + 1);
    newValue.pos.vpos    = (oldValue.pos.vpos + 1) % pQue->num;

    ret = FUTIL_AtomicCas(&(pQue->posW.value), oldValue.value , newValue.value );
    if( B_FALSE == ret )
    {
        goto retry;
    }

    cnt = 0;

    do {

        union UN_DATA olddat;
        union UN_DATA newdat;

        pNode = (QUE_DATA_S *)(pQue->pdata + ( newValue.pos.vpos * pQue->size ));

        olddat = pNode->flag;
        newdat.dat.version = olddat.dat.version + 1;

        if( olddat.dat.status != STATUS_WRITE )
        {
            cnt++;
            if( cnt > 3 )
            {
                newdat.dat.status = STATUS_WRITE;
            }
            else
            {
                continue;
            }
        }
        else
        {
            memcpy(pNode->data, pdata, pQue->size);
            newdat.dat.status = STATUS_READ;
        }

        ret = FUTIL_AtomicCas(&(pNode->flag.value), olddat.value , newdat.value );
        if( B_TRUE == ret )
        {
            if( olddat.dat.status != STATUS_WRITE )
            {
                goto retry;
            }
            else
            {
                return RET_OK;
            }
        }
    } while(1);

    return RET_OK;
}

UINT32 FUTIL_QueDelete(QUE_HANDLE handle)
{
    QUE_CTRL_S * pQue;

    pQue = (QUE_CTRL_S *)handle;
    if( NULL == pQue )
    {
        return RET_ERR;
    }

    (void)memset((void *)pQue, 0, sizeof(QUE_CTRL_S));

    (void)free((void *)pQue);

    return RET_OK;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */



