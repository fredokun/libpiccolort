/**
 * @file knownset.c
 * Provides several functions to commit and knowns sets.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 */

#include <stdio.h>
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
    s->size = size;
    s->type = LIST;
    s->liste = malloc(size);

    return (PICC_KnownSet*) s;
}

bool PICC_known_set_add_tree(PICC_KnownSet *s, GEN_VALUE elem)
{
    PICC_KnownSetTree* ss = (PICC_KnownSetTree*) s;
    int res;

    while(ss != NULL)
    {
       // res = compare_values(ss->tree->val, elem);

        if(res == 0)
            return false;

        if(res > 0)
        {
            if(ss->tree->right != NULL)
                ss->tree = ss->tree->right;
            else
            {
                ss->tree->right = malloc(sizeof(PICC_KnownSetTree));
                //ss->tree->right->val = elem;
            }
        }
        if(res < 0)
        {
            if(ss->tree->left != NULL)
                ss->tree = ss->tree->left;
            else
            {
                ss->tree->left = malloc(sizeof(PICC_KnownSetTree));
                //ss->tree->left->val = elem;
            }
        }
    }

    return true;
}

bool PICC_known_set_add_list(PICC_KnownSet *s, GEN_VALUE elem)
{
    PICC_KnownSetList* ss = (PICC_KnownSetList*) s;
    ss->size++;

    // convert to tree
    if(ss->size > PICC_MAX_LIST)
    {
        int i;
        PICC_KnownSetTree* t = malloc(sizeof(PICC_KnownSetTree));
        for(i=0 ; i<PICC_MAX_LIST ; i++)
            PICC_known_set_add_tree(t, ss->liste[i]);
        PICC_known_set_add_tree(t, elem);
        s = (PICC_KnownSet*) t;
    }
    else
        ss->liste[ss->size] = elem;

    return true;
}

/**
 * Add a commit to a set
 *
 * @param s knownset
 * @param elem the value to add to the knownset s
 */
bool PICC_known_set_add(PICC_KnownSet *s, GEN_VALUE elem)
{
    if(s->type == TREE)
        return PICC_known_set_add_tree(s, elem);
    else
        return PICC_known_set_add_list(s, elem);
}

bool PICC_known_set_mem_tree(PICC_KnownSet *ss, GEN_VALUE elem)
{
    PICC_KnownSetTree* s = (PICC_KnownSetTree*) ss;

    bool res = compare_values(s->tree->val, elem);

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

bool PICC_known_set_mem_list(PICC_KnownSet *ss, GEN_VALUE elem)
{
    PICC_KnownSetList* s = (PICC_KnownSetList*) ss;
    int i;

    for(i=0 ; i<s->size ; i++)
    {
        if(compare_values(s->liste[i], elem))
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
bool PICC_known_set_mem(PICC_KnownSet *s, GEN_VALUE elem)
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

    while(s->liste[size] != NULL)
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

    res = compare_values(ss->tree->val, ss2->tree->val);
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
        res &= (compare_values(ss->liste[i], ss2->liste[i]));

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

GEN_VALUE PICC_known_set_next(PICC_KnownSetIterator *it)
{
    if( it->set->type == LIST ){
	return PICC_known_set_list_iterator_next((PICC_KnownSetListIterator*) it);
    }else{
        return PICC_known_set_tree_iterator_next((PICC_KnownSetTreeIterator*) it, false);
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


GEN_VALUE PICC_known_set_tree_iterator_next(PICC_KnownSetTreeIterator *it, bool check)
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
                if(compare_values(it->previous->val, it->current->right->val))
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

        return it->current;
    }
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

GEN_VALUE PICC_known_set_list_iterator_next(PICC_KnownSetListIterator *it)
{
    GEN_VALUE ret = it->set->liste[it->next];
    it->next++;
    return ret;
}

bool PICC_known_set_list_iterator_has_next(PICC_KnownSetListIterator *it)
{
    return it->next < it->set->size;
}


// Invariants //////////////////////////////////////////////////////////////////
void PICC_KnownSet_inv(PICC_KnownSet *ks)
{

}
