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

PICC_KnownSet* PICC_create_empty_known_set()
{
    return PICC_create_known_set(PICC_MAX_LIST, NULL);
}

PICC_KnownSet* PICC_create_known_set(int size, PICC_Error* error)
{
    PICC_KnownSetList* s = malloc(sizeof(PICC_KnownSetList));
    s->size = 0;
    s->type = LIST;
    s->liste = malloc(size * sizeof(PICC_Knowns));

    return (PICC_KnownSet*) s;
}

void PICC_free_known_set(PICC_KnownSet *s) 
{
    if(s->type == TREE)
        PICC_free_known_set_tree((PICC_KnownSetTree *)s);
    else
        PICC_free_known_set_list((PICC_KnownSetList *)s);
}

void PICC_free_tree(PICC_Tree *tree)
{
    if (tree != NULL) {
        PICC_free_tree(tree->left);
        PICC_free_tree(tree->right);
        free(tree);
    }
}

void PICC_free_known_set_tree(PICC_KnownSetTree *s)
{
    PICC_free_tree(s->tree);
    free(s);
}
void PICC_free_known_set_list(PICC_KnownSetList *s)
{
    free(s->liste);
    free(s);
}

bool PICC_known_set_add_tree(PICC_KnownSet *s, GEN_VALUE *elem, PICC_KnownsState state)
{
    PICC_KnownSetTree* ss = (PICC_KnownSetTree*) s;
    int res;

    while(ss != NULL)
    {
        res = compare_values(ss->tree->known_val.val, elem);

        if(res == 0)
            return false;

        if(res > 0)
        {
            if(ss->tree->right != NULL)
                ss->tree = ss->tree->right;
            else
            {
                ss->tree->right = malloc(sizeof(PICC_KnownSetTree));
                ss->tree->right->known_val.val = elem;
                ss->tree->right->known_val.state = state;
            }
        }
        if(res < 0)
        {
            if(ss->tree->left != NULL)
                ss->tree = ss->tree->left;
            else
            {
                ss->tree->left = malloc(sizeof(PICC_KnownSetTree));
                ss->tree->left->known_val.val = elem;
                ss->tree->left->known_val.state = state;
            }
        }
    }

    return true;
}

bool PICC_known_set_add_list(PICC_KnownSet *s, GEN_VALUE *elem, PICC_KnownsState state)
{
    PICC_KnownSetList* ss = (PICC_KnownSetList*) s;
    ss->size++;

    // convert to tree
    if(ss->size > PICC_MAX_LIST)
    {
        int i;
        PICC_KnownSetTree* t = malloc(sizeof(PICC_KnownSetTree));
        for(i=0 ; i<PICC_MAX_LIST ; i++)
            PICC_known_set_add_tree((PICC_KnownSet *)t, ss->liste[i].val, state);
        PICC_known_set_add_tree((PICC_KnownSet *)t, elem, state);
        s = (PICC_KnownSet*) t;
    }
    else
    {
        ss->liste[ss->size - 1].val = elem;
        ss->liste[ss->size - 1].state = state;
    }

    return true;
}

/**
 * Add a commit to a set
 *
 * @param s knownset
 * @param elem the value to add to the knownset s
 */
 bool PICC_known_set_add(PICC_KnownSet *s, GEN_VALUE *elem)
{
    return PICC_known_set_add_with_state(s, elem, PICC_KNOWN);    
}
 
bool PICC_known_set_add_with_state(PICC_KnownSet *s, GEN_VALUE *elem, PICC_KnownsState state)
{
    if(s->type == TREE)
        return PICC_known_set_add_tree(s, elem, state);
    else
        return PICC_known_set_add_list(s, elem, state);
}

bool PICC_known_set_mem_tree(PICC_KnownSet *ss, GEN_VALUE *elem)
{
    PICC_KnownSetTree* s = (PICC_KnownSetTree*) ss;

    bool res = (compare_values(s->tree->known_val.val, elem) == 0);

    if(res)
        return res;
    else
    {
        if(s->tree->left != NULL)
        {
            res |= PICC_known_set_mem_tree((PICC_KnownSet*)s->tree->left, elem);
        }

        if(s->tree->right != NULL)
        {
            res |= PICC_known_set_mem_tree((PICC_KnownSet*)s->tree->right, elem);
        }

        return res;

    }
}

bool PICC_known_set_mem_list(PICC_KnownSet *ss, GEN_VALUE *elem)
{
    PICC_KnownSetList* s = (PICC_KnownSetList*) ss;
    int i;

    for(i=0 ; i<s->size ; i++)
    {
        if(compare_values(s->liste[i].val, elem))
            return true;
    }

    return false;
}


/**
 *  Checks if an element belongs to a knownset.
 *
 *  @param s knownset
 *  @param elem GEN_VALUE
 *  @return true if elem is in s else return false
 */
bool PICC_known_set_mem(PICC_KnownSet *s, GEN_VALUE *elem)
{
    if(s->type == TREE)
        return PICC_known_set_mem_tree(s, elem);
    else
        return PICC_known_set_mem_list(s, elem);
}

int PICC_known_set_size_tree(PICC_KnownSet *ss)
{
    PICC_KnownSetTree* s = (PICC_KnownSetTree*) ss;
    int size = 0;

    if(s->tree->left != NULL)
    {
        size += 1 + PICC_known_set_size_tree((PICC_KnownSet*)s->tree->left);
    }
    if(s->tree->right != NULL)
    {
        size += 1 + PICC_known_set_size_tree((PICC_KnownSet*)s->tree->right);
    }

    return size;
}

int PICC_known_set_size_list(PICC_KnownSet *ss)
{
    PICC_KnownSetList* s = (PICC_KnownSetList*) ss;
    int size = 0;

    while(size < s->size && s->liste[size].val != NULL)
        size++;

    return size;
}

int PICC_known_set_size(PICC_KnownSet *s)
{
    if(s->type == TREE)
        return PICC_known_set_size_tree(s);
    else
        return PICC_known_set_size_list(s);
}

bool PICC_known_set_compare_tree(PICC_KnownSet *s, PICC_KnownSet *s2)
{
    bool res = false;
    PICC_KnownSetTree* ss = (PICC_KnownSetTree*) s;
    PICC_KnownSetTree* ss2 = (PICC_KnownSetTree*) s2;

    res = (compare_values(ss->tree->known_val.val, ss2->tree->known_val.val) == 0);
    if(!res)
        return false;
    else
    {
        if(ss->tree->left != NULL)
            res &= PICC_known_set_compare_tree((PICC_KnownSet*)ss->tree->left, (PICC_KnownSet*)ss2->tree->left);
        if(ss->tree->right != NULL)
            res &= PICC_known_set_compare_tree((PICC_KnownSet*)ss->tree->right, (PICC_KnownSet*)ss2->tree->right);

        return res;
    }
}

bool PICC_known_set_compare_list(PICC_KnownSet *s, PICC_KnownSet *s2)
{
    bool res = false;
    int i;
    PICC_KnownSetList* ss = (PICC_KnownSetList*) s;
    PICC_KnownSetList* ss2 = (PICC_KnownSetList*) s2;

    res &= ss->size != ss2->size;

    for(i=0 ; i<ss->size ; i++)
        res &= ((compare_values(ss->liste[i].val, ss2->liste[i].val)) == 0);

    return res;
}

/**
 * Compare 2 commits
 *
 * @param s Commit
 * @param s2 Commit
 * @return res true if s is equal to s2
 */
bool PICC_known_set_compare(PICC_KnownSet *s, PICC_KnownSet *s2)
{
    if(s->type != s2->type)
        return false;

    if(s->type == TREE)
        return PICC_known_set_compare_tree(s, s2);
    else
        return PICC_known_set_compare_list(s, s2);
}

/* bool PICC_compare_tree(PICC_KnownSetTree *node, PICC_KnownSetTree *node2, (bool)(*f)(GEN_VALUE, GEN_VALUE)) */
/* { */
/*     if(!( */
/*         ((node->right == NULL && node2->right == NULL) || (node->right != NULL && node2->right != NULL)) */
/*         && ((node->left == NULL && node2->left == NULL) || (node->left != NULL && node2->left != NULL)) */
/*     )) */
/*         return false; */
/* //f(node->val, node2->val) */
/*     else */
/*     { */
/*         if(node->left != NULL) */
/*         { */
/*             f(node->val) & PICC_fold_tre; */
/*         } */
/*     } */
/* } */

/* PICC_Set* PICC_set_inter_knowns(PICC_Set* s1, PICC_Set* s2) */
/* { */
/*     ASSERT(s1->set_type == s2->set_type && s1->set_type == PICC_KNOWNS); */

/*     PICC_Set* result = PICC_set_make(PICC_KNOWNS); */
/*     GEN_VALUEList* current = s1->element.knowns; */
/*     GEN_VALUEList* current2; */

/*     for(;;) */
/*     { */
/*         if(current == NULL) */
/*             break; */

/*         current2 = s2->element.knowns; */

/*         for(;;) */
/*         { */
/*             if(current2 == NULL) */
/*                 break; */

/*             if(PICC_cmp_knowns(current->val, current2->val, NULL)) */
/*                 PICC_set_add_knowns(result, PICC_clone_knowns(current->val)); */

/*             current2 = current2->next; */
/*         } */

/*         current = current->next; */
/*     }; */

/*     return result; */
/* } */

/* void PICC_set_iter_knowns(PICC_Set* s, void (*func)(GEN_VALUE* arg)) */
/* { */
/*     GEN_VALUEList* it = s->element.knowns; */
/*     do */
/*     { */
/*         func(it->val); */
/*         it = it->next; */
/*     }while(it != NULL); */
/* } */

PICC_KnownSetIterator *PICC_create_known_set_iterator(PICC_KnownSet *s)
{
    if( s->type == LIST ){
	return (PICC_KnownSetIterator*) PICC_create_known_set_list_iterator((PICC_KnownSetList*) s);
    }else{
	return (PICC_KnownSetIterator*) PICC_create_known_set_tree_iterator((PICC_KnownSetTree*) s);
    }

}

PICC_KnownSetIterator *PICC_delete_known_set_iterator(PICC_KnownSetIterator *it)
{
    if( it->set->type == LIST ){
	return (PICC_KnownSetIterator*) PICC_delete_known_set_list_iterator((PICC_KnownSetListIterator*) it);
    }else{
	return (PICC_KnownSetIterator*) PICC_delete_known_set_tree_iterator((PICC_KnownSetTreeIterator*) it);
    }
}

GEN_VALUE *PICC_known_set_next(PICC_KnownSetIterator *it)
{
    if( it->set->type == LIST ){
	return PICC_known_set_list_iterator_next((PICC_KnownSetListIterator*) it);
    }else{
        return PICC_known_set_tree_iterator_next((PICC_KnownSetTreeIterator*) it, false);
    }
}

PICC_KnownsState PICC_known_set_iterator_state(PICC_KnownSetIterator *it)
{
    if( it->set->type == LIST ){
        return PICC_known_set_list_iterator_state((PICC_KnownSetListIterator*) it);
    }else{
        return PICC_known_set_tree_iterator_state((PICC_KnownSetTreeIterator*) it);
    }
}

void PICC_known_set_iterator_state_set(PICC_KnownSetIterator *it, PICC_KnownsState state)
{
    if( it->set->type == LIST ){
        PICC_known_set_list_iterator_state_set((PICC_KnownSetListIterator*) it, state);
    }else{
        PICC_known_set_tree_iterator_state_set((PICC_KnownSetTreeIterator*) it, state);
    }
}

bool PICC_known_set_has_next(PICC_KnownSetIterator *it)
{
    if( it->set->type == LIST ){
	return PICC_known_set_list_iterator_has_next((PICC_KnownSetListIterator*) it);
    }else{
	return PICC_known_set_tree_iterator_has_next((PICC_KnownSetTreeIterator*) it);
    }
}

// Tree structure //////////////////////////////////////////////////////////////

PICC_KnownSetTreeIterator *PICC_create_known_set_tree_iterator(PICC_KnownSetTree *s)
{
    PICC_KnownSetTreeIterator* it = malloc(sizeof(PICC_KnownSetTreeIterator));
    it->set=s;

    PICC_Tree* n = s->tree;
    while(n->left != NULL)
    n = n->left;

    it->next = n;
    it->current = NULL;
    return it;
}

PICC_KnownSetTreeIterator *PICC_delete_known_set_tree_iterator(PICC_KnownSetTreeIterator *it){
    free(it);
    return (it = NULL);

}


GEN_VALUE *PICC_known_set_tree_iterator_next(PICC_KnownSetTreeIterator *it, bool check)
{
    if(it->current == NULL)
    {
        return NULL;
    }
    else
    {
        if(!check)
        {
            if(it->current->left == NULL)
            {
                if(it->current->right == NULL)
                {
                    it->previous = it->current;
                    it->current = it->current->father;
                    return PICC_known_set_tree_iterator_next(it, true);
                }
                else
                {
                    it->previous = it->current;
                    it->current = it->current->right;
                }
            }
            else
            {
                it->previous = it->current;
                it->current = it->current->left;
            }
        }
        else
        {
            if(it->current->right == NULL)
            {
                it->previous = it->current;
                it->current = it->current->father;
                return PICC_known_set_tree_iterator_next(it, true);
            }
            else
            {
                if(compare_values(it->previous->known_val.val, it->current->right->known_val.val) == 0)
                {
                    if(it->current->father != NULL)
                    {
                        it->previous = it->current;
                        it->current = it->current->father;
                        return PICC_known_set_tree_iterator_next(it, true);
                    }
                    else
                        return NULL;
                }
                else
                {
                    it->previous = it->current;
                    it->current = it->current->right;
                }
            }
        }

        return it->current->known_val.val;
    }
}

PICC_KnownsState PICC_known_set_tree_iterator_state(PICC_KnownSetTreeIterator *it) 
{
    return it->current->known_val.state;
}

void PICC_known_set_tree_iterator_state_set(PICC_KnownSetTreeIterator *it, PICC_KnownsState state) 
{
    it->current->known_val.state = state;
}

bool PICC_known_set_tree_iterator_has_next(PICC_KnownSetTreeIterator *it)
{
    return it->next != NULL;
}


// List structure //////////////////////////////////////////////////////////////

PICC_KnownSetListIterator *PICC_create_known_set_list_iterator(PICC_KnownSetList *s)
{
    PICC_KnownSetListIterator* it = malloc(sizeof(PICC_KnownSetListIterator));
    it->set=s;
    it->next=0;
    return it;
}

PICC_KnownSetListIterator *PICC_delete_known_set_list_iterator(PICC_KnownSetListIterator *it)
{
    free(it);
    return (it = NULL);
}

GEN_VALUE *PICC_known_set_list_iterator_next(PICC_KnownSetListIterator *it)
{
    GEN_VALUE *ret = it->set->liste[it->next].val;
    it->next++;
    return ret;
}

PICC_KnownsState PICC_known_set_list_iterator_state(PICC_KnownSetListIterator *it) 
{
    return it->set->liste[it->next - 1].state;
}

void PICC_known_set_list_iterator_state_set(PICC_KnownSetListIterator *it, PICC_KnownsState state)
{
    it->set->liste[it->next - 1].state = state;
}

bool PICC_known_set_list_iterator_has_next(PICC_KnownSetListIterator *it)
{
    return it->next < it->set->size;
}

// Knowns

/**
 * Creates a new Knowns structure.
 *
 * @pre channel != NULL
 *
 * @post knows->state == PICC_UNKNOWN
 * @post knows->channel == channel
 *
 * @param error Error stack
 * @return Created knowns structure
 */
PICC_Knowns *PICC_create_knowns(GEN_VALUE *val, PICC_Error *error)
{
    #ifdef CONTRACT_PRE
        //pre
        ASSERT(val != NULL );
    #endif

    PICC_ALLOC(knowns, PICC_Knowns, error) {
        knowns->val = val;
        knowns->state = PICC_KNOWN;
    }

    #ifdef CONTRACT_POST_INV
        //inv
        PICC_Knowns_inv(knowns);
    #endif

    #ifdef CONTRACT_POST
        //post
        ASSERT(knowns->state == PICC_KNOWN );
        ASSERT(knowns->val == val );
    #endif
    return knowns;
}

// Spec KnownsSet
/**
 * search within a knownsSet the knowns with a specific state
 *
 * @pre ks != NULL
 *
 * @post each Knowns in result has the correct state
 * @post result set has the correct size
 *
 * @param state state wanted
 * @param ks knownsSet fetched
 */
PICC_KnownSet *PICC_knowns_set_search(PICC_KnownSet *ks, PICC_KnownsState state)
{

    #ifdef CONTRACT_PRE_INV
        //inv
        PICC_KnownSet_inv(ks);
    #endif

    #ifdef CONTRACT_PRE
        //pre
        ASSERT(ks != NULL);
    #endif

    int count=0;

    PICC_KNOWNSET_ONLY_STATE_FOREACH(GEN_VALUE, current_state, ks, it);
        if( current_state == state )
        {
            count ++;
        }
    END_KNOWNSET_FOREACH;

    PICC_KnownSet *result = PICC_create_known_set(count, NULL);

    PICC_KNOWNSET_STATE_FOREACH(GEN_VALUE, value, current_state, ks, it);
        if( current_state == state )
        {
            PICC_known_set_add_with_state(result, value, state);
        }
    END_KNOWNSET_FOREACH;

    #ifdef CONTRACT_POST_INV
        //inv
        PICC_KnownSet_inv(result);
        PICC_KnownSet_inv(ks);
    #endif

    #ifdef CONTRACT_POST
        //post
        ASSERT(result != NULL);
        ASSERT(PICC_known_set_size(result) == count);
    #endif

    return result;
}

/**
 * Returns a subset of all KNOWN-STATE in a knows set.
 *
 * @param ks Knowns set
 * @return Subset of all known channel in the given set
 */
PICC_KnownSet *PICC_knowns_set_knows(PICC_KnownSet *ks)
{
    return PICC_knowns_set_search(ks, PICC_KNOWN);
}

/**
 * Returns a subset of all FORGET-STATE in a knowns set.
 *
 * @param ks Knowns set
 * @return Subset of all forget state in the given set.
 */
PICC_KnownSet *PICC_knowns_set_forget(PICC_KnownSet *ks)
{
    return PICC_knowns_set_search(ks, PICC_FORGET);
}

/**
 * Switches an element of a KnowsSet from the FORGET state to the
 * UNKNOWN state.
 *
 * @pre ks != NULL
 * @pre ch != NULL

 * @param ks Knows set
 * @param ch Channel to switch state
 */
void PICC_knowns_set_forget_to_unknown_gen(PICC_KnownSet *ks, GEN_VALUE *val)
{
    #ifdef CONTRACT_PRE_INV
        //inv
        PICC_KnownSet_inv(ks);
    #endif

    #ifdef CONTRACT_PRE
        //pre
        ASSERT(ks != NULL);
        ASSERT(val != NULL);
    #endif

    PICC_KNOWNSET_FOREACH(GEN_VALUE, value, ks, it);
        if(compare_values(val, value) == 0){
            PICC_known_set_iterator_state_set(it, PICC_UNKNOWN);
        }
    END_KNOWNSET_FOREACH;

    #ifdef CONTRACT_POST_INV
        //inv
        PICC_KnownSet_inv(ks);
    #endif
}

/**
 * Switches all KNOWN state elements of a KnowsSet to FORGET state.
 *
 * @pre ks != NULL
 *
 * @post ks only have knowns values with PICC_FORGET state
 *
 * @param ks Knows set
 */
void PICC_knowns_set_forget_all(PICC_KnownSet *ks)
{
    #ifdef CONTRACT_PRE_INV
         //inv
         PICC_KnownSet_inv(ks);
    #endif

    #ifdef CONTRACT_PRE
         //pre
         ASSERT(ks != NULL);
    #endif

    PICC_KNOWNSET_ONLY_STATE_FOREACH(GEN_VALUE, state, ks, it);
        if (state == PICC_KNOWN) {
            PICC_known_set_iterator_state_set(it, PICC_FORGET);
        }
    END_KNOWNSET_FOREACH;

    #ifdef CONTRACT_POST_INV
        // inv
        PICC_KnownSet_inv(ks);
    #endif

    #ifdef CONTRACT_POST
        // post
        PICC_KNOWNSET_ONLY_STATE_FOREACH(GEN_VALUE, state, ks, it);
            ASSERT(state != PICC_KNOWN );
        END_KNOWNSET_FOREACH;
    #endif
}

/**
 * Adds a channel to a knowns set.
 *
 * Looks for a channel in a PICC_KnownsSet
 * - if the channel is in the PICC_KnownsSet in KNOWN-STATE, it returns false
 * - if the channel is in the PICC_KnownsSet in FORGET-STATE, it switches it to KNOWN then  returns false
 * - else it add the channel in the PICC_KnownsSet (KNOWS-STATE) then returns true
 *
 * @pre ks != NULL
 *
 * @param ks Knowns set
 * @param ch Channel to add
 * @return Whether the channel has been added
 */
bool PICC_knowns_register_gen(PICC_KnownSet *ks, GEN_VALUE *val)
{
    #ifdef CONTRACT_PRE_INV
        //inv
        PICC_KnownSet_inv(ks);
    #endif

    #ifdef CONTRACT_PRE
        //pre
        ASSERT(ks != NULL);
    #endif

    bool registered = true;
    GEN_VALUE* val0 = NULL;

    PICC_KNOWNSET_STATE_FOREACH(GEN_VALUE, value, state, ks, it);
        if(compare_values(val, value) == 0)
        {
            val0 = value;
            if(state == PICC_KNOWN || state == PICC_FORGET)
            {
                registered = false;
            }
            if(state == PICC_UNKNOWN || state == PICC_FORGET)
            {
                PICC_known_set_iterator_state_set(it, PICC_KNOWN);
            }
            break;
        }
    END_KNOWNSET_FOREACH;

    if (val0 == NULL)
    {
        registered = PICC_known_set_add(ks, val);
    }

    #ifdef CONTRACT_POST_INV
        // inv
        PICC_KnownSet_inv(ks);
    #endif

    return registered;
}

// Invariants //////////////////////////////////////////////////////////////////
/**
 * Checks knowns invariant.
 *
 * @inv knowns->channel != NULL
 */
void PICC_Knowns_inv(PICC_Knowns *knowns)
{
    ASSERT(knowns->val != NULL);
}

/**
 * Checks knownsSet invariant.
 *
 * @inv knownsSet->size > -1
 */
void PICC_KnownSet_inv(PICC_KnownSet *set)
{
    ASSERT(PICC_known_set_size(set) > -1);
}
