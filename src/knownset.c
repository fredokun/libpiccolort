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

/**
 * Add a commit to a set
 *
 * @param s knownset
 * @param elem the value to add to the knownset s
 */
bool PICC_known_set_add(PICC_KnownSet *s, GEN_VALUE elem)
{
    int type = s->type;
    char c;
    switch(type)
    {
        case TREE:
        PICC_KnownsetTree ss = (PICC_KnownsetTree s);
        int res;

        while(ss->elem != NULL)
        {
            res = PICC_compare_value(ss->elem, elem);
            if(res >= 0)
            {
                ss->left = malloc(sizeof(PICC_KnownsetTree));
                ss = ss->left;
            }
            if(res < 0)
            {
                ss->right = malloc(sizeof(PICC_KnownsetTree));
                ss = ss->right;
            }
        }
        ss->val = elem;
        break;

        case LIST:
        PICC_KnownsetList ss = (PICC_KnownsetList) s;
        ss->size++;
        ss->liste[size] = elem;
        break;
    }
}

/**
 * Compare 2 commits
 *
 * @param s Commit
 * @param s2 Commit
 * @return res true if s is equal to s2
 */
bool PICC_compare_knownset(PICC_Knownset *s, PICC_Knownset s2)
{
    bool res = false;

    if(s->type != s2->type)
        return res;

    int type = s->type;

    switch(type)
    {
        case TREE:
        PICC_KnownsetTree ss = (PICC_KnownsetTree) s;
        PICC_KnownsetTree ss2 = (PICC_KnownsetTree) s2;

        while(ss->elem != NULL)
        {
            res = PICC_compare_value(ss->elem, elem);
            if(res >= 0)
            {
                ss->left = malloc(sizeof(PICC_KnownsetTree));
                ss = ss->left;
            }
            if(res < 0)
            {
                ss->right = malloc(sizeof(PICC_KnownsetTree));
                ss = ss->right;
            }
        }

        break;

        case LIST:
        PICC_KnownsetList ss = (PICC_KnownsetList) s;
        PICC_KnownsetList ss2 = (PICC_KnownsetList) s2;

        res &= ss->size != ss2->size

        int i;
        for(i=0 ; i<s->size ; i++)
            res &= (PICC_equals(ss[i], ss2[i]));

        break;
    }

    return res;
}

bool PICC_equals(GEN_VALUE v, GEN_VALUE v2)
{
    bool res = false;

    if(v->kind == v2->kind)
        return res;

    switch(v->kind)
    {
        case PICC_INT_VAL:
        res = v == v2;
        break;

        case PICC_FLOAT_VAL:
        res = v == v2;
        break;

        case PICC_STRING_VAL:
        res = (strcmp(v, v2)?false:true);
        break;

        case PICC_BOOL_VAL:
        res = v == v2;
        break;

        case PICC_CHANNEL_VAL:
        res = PICC_compare_channel(v, v2);
        break;
    }
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
/*     PICC_KnownsList* current = s1->element.knowns; */
/*     PICC_KnownsList* current2; */

/*     for(;;) */
/*     { */
/*         if(current == NULL) */
/*             break; */

/*         current2 = s2->element.knowns; */

/*         for(;;) */
/*         { */
0/*             if(current2 == NULL) */
/*                 break; */

/*             if(PICC_cmp_knowns(current->val, current2->val, NULL)) */
/*                 PICC_set_add_knowns(result, PICC_clone_knowns(current->val)); */

/*             current2 = current2->next; */
/*         } */

/*         current = current->next; */
/*     }; */

/*     return result; */
/* } */

/* void PICC_set_iter_knowns(PICC_Set* s, void (*func)(PICC_Knowns* arg)) */
/* { */
/*     PICC_KnownsList* it = s->element.knowns; */
/*     do */
/*     { */
/*         func(it->val); */
/*         it = it->next; */
/*     }while(it != NULL); */
/* } */

PICC_KnownSetIterator *PICC_create_known_set_iterator(PICC_KnownSet *s)
{
    if( s->set_type == LIST ){
	return (PICC_KnownSetIterator) PICC_create_known_set_list_iterator((KnownSetList) s);
    }else{
	return (PICC_KnownSetIterator) PICC_create_known_set_tree_iterator((KnownSetTree) s);
    }

}

PICC_KnownSetIterator *PICC_delete_known_set_iterator(PICC_KnownSetIterator *it)
{
    if( it->set->set_type == LIST ){
	return (PICC_KnownSetIterator) PICC_delete_known_set_list_iterator((KnownSetListIterator) s);
    }else{
	return (PICC_KnownSetIterator) PICC_delete_known_set_tree_iterator((KnownSetTreeIterator) s);
    }
}

GEN_VALUE PICC_known_set_next(PICC_KnownSetIterator *it)
{
    if( it->set->set_type == LIST ){
	return PICC_known_set_list_iterator_next((PICC_KnownSetListIterator) it);
    }else{
	return PICC_known_set_tree_iterator_next((PICC_KnownSetTreeIterator) it);
    }
}

bool PICC_known_set_has_next(PICC_KnownSetIterator *it)
{
    if( it->set->set_type == LIST ){
	return PICC_known_set_list_iterator_has_next((PICC_KnownSetListIterator) it);
    }else{
	return PICC_known_set_tree_iterator_has_next((PICC_KnownSetTreeIterator) it);
    }
}

// Tree structure //////////////////////////////////////////////////////////////

PICC_KnownSetTreeIterator *PICC_create_known_set_tree_iterator(PICC_KnownSetTree *s)
{
    PICC_KnownSetTreeIterator it = malloc(sizeof(PICC_KnownSetTreeIterator));
    it->set=s;

    PICC_Tree n = s->tree;
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
                    return PICC_known_set_tree_iterator_next(it->current->father, true);
                }
                else
                {
                    return it->current->right;
                }
            }
            else
            {
                return it->current->left;
            }
        }
        else
        {
            if(it->current->right == NULL)
            {
                return PICC_known_set_tree_iterator_next(it->current->father, true);
            }
            else
            {
                return it->current->right;
            }
        }
    }
}

bool PICC_known_set_tree_iterator_has_next(PICC_KnownSetListIterator *it)
{
    return it->next != NULL;
}


// List structure //////////////////////////////////////////////////////////////

PICC_KnownSetListIterator *PICC_create_known_set_list_iterator(KnownSetList *s)
{
    PICC_KnownSetListIterator it = malloc(sizeof(PICC_KnownSetListIterator));
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
