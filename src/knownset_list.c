
#include <knownset_repr.h>

PICC_KnownSetListIterator PICC_create_known_set_list_iterator(KnownSetList s)
{
    PICC_KnownSetListIterator it = malloc(sizeof(PICC_KnownSetListIterator));
    it->set=s;
    it->next=0;
    return it;
}

PICC_KnownSetListIterator PICC_delete_known_set_list_iterator(PICC_KnownSetListIterator it)
{
    free(it);
    return (it = NULL);
}

void* PICC_known_set_list_iterator_next(PICC_KnownSetListIterator it)
{
    void *ret = it->set->liste[it->next];
    it->next++;
    return ret;
}

bool PICC_known_set_list_iterator_has_next(PICC_KnownSetListIterator it)
{
    return it->next < it->set->size;
}
