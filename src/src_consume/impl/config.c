#include <pthread.h>
#include <stdlib.h>

#include "rosalia/noise.h"

#include "rosalia/config.h"

#ifdef __cplusplus
extern "C" {
#endif

//////////////
// cfg locking

typedef struct cfg_lock_t {
    pthread_rwlock_t rwl;
} cfg_lock;

void* cfg_lock_create()
{
    cfg_lock* locki = (cfg_lock*)malloc(sizeof(cfg_lock));
    pthread_rwlock_init(&locki->rwl, NULL);
    return locki;
}

void cfg_lock_destroy(void* lock)
{
    cfg_lock* locki = (cfg_lock*)lock;
    pthread_rwlock_destroy(&locki->rwl);
    free(locki);
}

void cfg_rlock(void* lock)
{
    cfg_lock* locki = (cfg_lock*)lock;
    pthread_rwlock_rdlock(&locki->rwl);
}

void cfg_runlock(void* lock)
{
    cfg_lock* locki = (cfg_lock*)lock;
    pthread_rwlock_unlock(&locki->rwl);
}

void cfg_wlock(void* lock)
{
    cfg_lock* locki = (cfg_lock*)lock;
    pthread_rwlock_wrlock(&locki->rwl);
}

void cfg_wunlock(void* lock)
{
    cfg_lock* locki = (cfg_lock*)lock;
    pthread_rwlock_unlock(&locki->rwl);
}

#ifdef __cplusplus
}
#endif
