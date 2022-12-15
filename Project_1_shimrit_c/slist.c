#include "slist.h"
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

/** Initialize a double linked list
	\param list - the list to initialize */
void dbllist_init(dbllist_t* List){
    if(List == NULL)return;
    dbllist_head(List) = (dbllist_node_t*)malloc(sizeof(dbllist_node_t));
    dbllist_data(dbllist_head(List)) = NULL;
    dbllist_prev(dbllist_head(List)) = NULL;
    dbllist_next(dbllist_head(List)) = NULL;
    assert(dbllist_head(List)!=NULL);
    dbllist_tail(List) = dbllist_head(List);
    dbllist_size(List) = 0;
}

/** Destroy and de-allocate the memory hold by a list
	\param list - a pointer to an existing list
	\param dealloc flag that indicates whether stored data should also be de-allocated */
void dbllist_destroy(dbllist_t *List,dbllist_destroy_t dealloc){///destorys the list given inside if given dealloc ==DBLLIST_FREE_DATA it frees the data
    if(List == NULL)return;
    if (dealloc==DBLLIST_FREE_DATA)
    {
        if(dbllist_head(List)== NULL) {
            free(List);
            return;
        }
        for (dbllist_node_t* head= dbllist_next(dbllist_head(List));dbllist_data(head) != dbllist_data(dbllist_tail(List)); head = dbllist_next(head))
        {
            free(dbllist_prev(head));
        }
        free(dbllist_prev(dbllist_tail(List)));
        free(dbllist_tail(List));
        free(List);
    }
    else{
        free(List);
    }
};


/** Append data to list (add as last node of the list)
	\param list - a pointer to a list
	\param data - the data to place in the list
	\return 0 on success, or -1 on failure */
int dbllist_append(dbllist_t* List,void* data){
///adds to the List given (already initialized) a node with the data given as the data inside the node.
    if(List==NULL)return -1;
    if(dbllist_data(dbllist_head(List))==NULL){///adds to the first object that was initialized inside the list (in db_init) the data given
        dbllist_data(dbllist_head(List))= data;
        dbllist_next(dbllist_head(List)) = NULL;
        dbllist_tail(List) = dbllist_head(List);
    }
    else{
        dbllist_node_t* new_Node = (dbllist_node_t*)malloc(sizeof(dbllist_node_t));///creating a new node and setting its values with the data. and changing the tail to it.
        assert(new_Node!=NULL);
        dbllist_data(new_Node) = data;
        dbllist_next(new_Node) = NULL;
        dbllist_prev(new_Node) = dbllist_tail(List);
        if(dbllist_head(List) == dbllist_tail(List)){
            dbllist_next(dbllist_head(List)) = new_Node;
        }
        if(dbllist_tail(List)!=NULL){
            dbllist_next(dbllist_tail(List)) = new_Node;
            dbllist_tail(List) = dbllist_next(dbllist_tail(List));

        }

    }
    dbllist_size(List)++;
    return 0;
}

/** Prepend data to list (add as first node of the list)
	\param list - a pointer to list
	\param data - the data to place in the list
	\return 0 on success, or -1 on failure
*/
int dbllist_prepend(dbllist_t * List,void * data){
    if(List == NULL)return -1;
    if(dbllist_data(dbllist_head(List))==NULL){
        dbllist_data(dbllist_head(List))= data;
        dbllist_next(dbllist_head(List)) = NULL;
    }
    else{
        dbllist_node_t* new_Node = (dbllist_node_t*)malloc(sizeof(dbllist_node_t));///creating a new node and setting its values with the data. and changing the tail to it.
        dbllist_data(new_Node) = data;
        dbllist_prev(new_Node) = NULL;
        dbllist_next(new_Node) = dbllist_head(List);
        dbllist_prev(dbllist_head(List)) = new_Node;
        dbllist_head(List) = new_Node;
    }
    return 0;
}
/** \brief Remove the specific node from the list. 
	\param to a pointer to the list
	\param pointer to the node that should be removed.
	\param dealloc flag that indicates whether to de-allocated the data in the node 
	\return 0 on success, or -1 on failure
*/
int dbllist_remove(dbllist_t* List, dbllist_node_t* node_to_remove ,dbllist_destroy_t dealloc){
    ///removes the node given from the List if it is in the List given.
    if(dbllist_head(List)==NULL)return -1;
    for (dbllist_node_t* j=dbllist_head(List);j!=NULL;j = dbllist_next(j)) {
            if(j==node_to_remove){
                if(j == dbllist_head(List)){
                    if(j==dbllist_tail(List)){
                        if(dealloc == DBLLIST_FREE_DATA)
                            free(dbllist_head(List));
                        dbllist_head(List)=NULL;
                        dbllist_tail(List)=NULL;
                        break;

                    }
                    else{
                        dbllist_head(List) = dbllist_next(dbllist_head(List));
                        if(dealloc ==DBLLIST_FREE_DATA)
                            free(dbllist_prev(dbllist_head(List)));
                        dbllist_prev(dbllist_head(List)) = NULL;
                        break;

                    }
                }
                else if(j == dbllist_tail(List)){
                    dbllist_tail(List) = dbllist_prev(dbllist_tail(List));
                    if (dealloc == DBLLIST_FREE_DATA)
                        free(dbllist_next(dbllist_tail(List)));
                    dbllist_next(dbllist_tail(List)) = NULL;
                    break;

                }
                else{
                    dbllist_next(dbllist_prev(j)) = dbllist_next(j);
                    dbllist_prev(dbllist_next(j)) = dbllist_prev(j);
                    if(dealloc == DBLLIST_FREE_DATA)
                        free(j);
                    break;
                }
            }
        }
    return 0;
}