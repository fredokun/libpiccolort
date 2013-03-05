/**
 * @file knownset_tree.c
 * Tree structure for known set.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 * @author Loïc GIRAULT
 */

#include <knownset_repr.h>

PICC_KnownSetTreeIterator PICC_create_known_set_tree_iterator(KnownSetTree s)
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

PICC_KnownSetTreeIterator PICC_delete_known_set_tree_iterator(PICC_KnownSetTreeIterator it){
    free(it);
    return (it = NULL);

}

void* PICC_known_set_tree_iterator_next(PICC_KnownSetTreeIterator it)
{

    void *ret = it->set->liste[it->next];
    it->next++;
    return ret;
}


void* PICC_known_set_tree_iterator_next(PICC_KnownSetTreeIterator it)
{
    //TODO !!!
    return ret;
}

bool PICC_known_set_tree_iterator_has_next(PICC_KnownSetListIterator it)
{
    return it->next != NULL;
}
