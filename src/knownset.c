/**
 * @file knownset.c
 * Provides several functions to commit and knowns sets.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 */

#include <stdio.h>
#include <stdlib.h>

#include <knownset_repr.h>
#include <tools.h>
#include <errors.h>
#include <value_repr.h>

#define INIT_ELEM(e)				\
    (e)->state = PICC_UNKNOWN;			\
    (e)->value = NULL

PICC_KnownSet *PICC_create_known_set(int init_max_size, PICC_Error* e){
    
    PICC_KnownElement *elts = malloc(sizeof(PICC_KnownElement) * init_max_size);
    for(int i = 0; i< init_max_size; i++){
	INIT_ELEM(&elts[i]);
    }
    
    PICC_KnownSet *s = malloc(sizeof(*s));
    s->max_size = init_max_size;
    s->current_size = 0;
    s->content = elts;
    return s;
}

PICC_KnownSet *PICC_create_empty_known_set(){
    return PICC_create_known_set(SET_INIT_MAXSIZE, NULL);
}

void PICC_free_known_set(PICC_KnownSet *s)
{
    free(s->content);
    free(s);
}

bool PICC_known_set_add(PICC_KnownSet *s, PICC_KnownValue *val)
{
    if(val->index_in_knownset > -1)// already in set
    { 
	if(s->content[val->index_in_knownset].value != val)//double check
	{
	    //incohrent state !!
	    return false;
	}
	else //val in knownset and double check ok
	{
	    return true;
	}
    }
    
    if(s->current_size == s->max_size){
	//realloc
	int new_max_size = s->current_size + SET_INIT_MAXSIZE;
	PICC_KnownElement *new_elts = realloc(s->content, 
					      sizeof(PICC_KnownElement) * new_max_size);
	if(new_elts == NULL){
	    //crash !
	    return false;
	}
	s->max_size = new_max_size;

	for(int i = s->current_size; i < s->max_size; i++){
	    INIT_ELEM(&new_elts[i]);
	} 
	s->content = new_elts;
		
    }
    
    s->content[s->current_size].value = val;
    val->index_in_knownset = s->current_size;
    s->current_size ++;
    return true;
}

int PICC_known_size(PICC_KnownSet *s){
    return s->current_size;
}

#define PICC_KNOWNSET_FOREACH_ELEM(s, e)			\
    for(int i = 0;						\
	i < (s)->current_size && (e = &((s)->content[i]));	\
	i++)						


PICC_KnownSet *PICC_known_know(PICC_KnownSet *ks){
    PICC_KnownSet* s2 = PICC_create_empty_known_set();
    
    PICC_KnownElement *e;
    PICC_KNOWNSET_FOREACH_ELEM(ks, e){
	if(e->state == PICC_KNOWN)
	    PICC_known_set_add(s2, e->value);
    }

    return s2;
}

PICC_KnownSet *PICC_known_forget(PICC_KnownSet *ks){
    PICC_KnownSet* s2 = PICC_create_empty_known_set();
    
    PICC_KnownElement *e;
    PICC_KNOWNSET_FOREACH_ELEM(ks, e){
	if(e->state == PICC_FORGET)
	    PICC_known_set_add(s2, e->value);
    }

    return s2;
}

void PICC_known_forget_to_unknown(PICC_KnownSet *ks, PICC_KnownValue *val){
    ks->content[val->index_in_knownset].state = PICC_FORGET;
}

void PICC_known_forget_all(PICC_KnownSet *ks)
{
    PICC_KnownElement *e;
    PICC_KNOWNSET_FOREACH_ELEM(ks, e){
	e->state = PICC_FORGET;
    }
}

bool PICC_known_register(PICC_KnownSet *ks, PICC_KnownValue *val){
    PICC_KnownElement *elem;
    if(val->index_in_knownset > -1)// already in set
    { 
	elem = ks->content  + val->index_in_knownset;
	if(elem->value != val)//double check
	{
	    //incohrent state !! 
	    // MUST CRASH !!!
	    return false;
	}
	else //elem in knownset and double check ok
	{
	    if(elem->state == PICC_KNOWN) 
		return false;
	    if(elem->state == PICC_FORGET){
		elem->state = PICC_KNOWN;
		return false;
	    }
	}
    }
   
    PICC_known_set_add(ks, val);
    elem = ks->content  + val->index_in_knownset;
    elem->state = PICC_KNOWN;
    return true;
    
}

void PICC_known_set_inv(PICC_KnownSet *set){
    ASSERT(set->current_size<= set->max_size)
}
