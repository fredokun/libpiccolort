/**
 * @file tools.h
 * Various utility functions.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 *
 */

#ifndef TOOLS_H
#define TOOLS_H

#define PICC_ALLOC(var, type, error) \
    type *var = malloc(sizeof(type)); \
    if (var == NULL) { \
        NEW_ERROR(error, ERR_OUT_OF_MEMORY); \
    } else

#define PICC_ALLOC_CRASH(var, type) \
    type *var = malloc(sizeof(type)); \
    if (var == NULL) { \
        CRASH_NEW_ERROR(ERR_OUT_OF_MEMORY); \
    } else

#define PICC_MALLOC(var, type, error) \
    var = malloc(sizeof(type)); \
    if (var == NULL) { \
        NEW_ERROR(error, ERR_OUT_OF_MEMORY); \
    } else

#define PICC_ALLOC_N_CRASH(var, type, size) \
    type *var = malloc(sizeof(type) * (size)); \
    if (size == 0) { \
        var = NULL; \
    } \
    if (var == NULL && size != 0) { \
        CRASH_NEW_ERROR(ERR_OUT_OF_MEMORY); \
    } else

#define PICC_FREE_MUTEX(m) \
    pthread_mutex_destroy(&m);

#define PICC_FREE_COMMIT(c) \
    free(c->clock); \
    free(c->clockval); \
    free(c->channel); \
    if(c->type == PICC_IN_COMMIT) \
        free(c->content.in); \
    else \
        free(c->content.out);

#define PICC_FREE_PITHREAD(p) \
    free(p->enabled); \
    PICC_FREE_SET(p->knowns); \
    int iCount; for(iCount = 0 ; iCount<p->env_length ; iCount++) \
        PICC_FREE_VALUE(p->(*env)); \
    free(env); \
    PICC_FREE_COMMIT(p->commit); \
    PICC_FREE_SET(p->commits); \
    free(p->proc); \
    PICC_FREE_VALUE(p->val); \
    free(p->clock); \
    PICC_FREE_MUTEX(lock);

#define PICC_FREE_VALUE(v) \
    PICC_FREE_MUTEX(v->lock); \
    if(v->kind == PICC_STRING) free(v->content.as_string); \
    if(v->kind == PICC_CHANNEL_VAL) PICC_FREE_CHANNEL(v->content.as_channel);

#define PICC_FREE_CHANNEL(chan) \
    PICC_FREE_MUTEX(chan->lock); \
    PICC_FREE_SET(chan->incommits); \
    PICC_FREE_SET(chan->outcommits);

#define PICC_FREE_SET(s) \
    PICC_set_destroy(s);


#define CONTRACT_PRE
#define CONTRACT_PRE_INV
#define CONTRACT_POST
#define CONTRACT_POST_INV

extern void debug(const char* s);

#endif
