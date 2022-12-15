#include <string.h>
#include "pattern_matching.h"
#include "slist.c"

pm_int_t size_of_pm = 0;
int pm_state_init(pm_state_t* new_pm_state){///initializing states.
    if(new_pm_state==NULL)return -1;
    new_pm_state->depth = 0;
    new_pm_state->id = 0;
    dbllist_t* new_output_dbllist = (dbllist_t*) malloc(sizeof (dbllist_t));
    dbllist_init(new_output_dbllist);
    dbllist_t* new_transitions_dbllist = (dbllist_t*) malloc(sizeof (dbllist_t));
    dbllist_init(new_transitions_dbllist);
    new_pm_state->output = new_output_dbllist;
    new_pm_state->_transitions = new_transitions_dbllist;
    return 0;
}
int pm_init(pm_t* new_pm){///initialize pm's
    if(new_pm==NULL)return -1;
    if(new_pm->zerostate== NULL){
        new_pm->newstate = size_of_pm;
        pm_state_t* new_pm_state =(pm_state_t*)malloc(sizeof (pm_state_t));
        new_pm_state->id = 0;
        new_pm_state->depth = 0;
        new_pm_state->output = NULL;
        new_pm_state->fail = NULL;
        new_pm_state->_transitions =NULL;
        pm_state_init(new_pm_state);
        new_pm->zerostate = new_pm_state;

    }
    return 0;
}
int pm_addstring(pm_t * pm_to_add_to,unsigned char* string_to_be_added, pm_int_t n){///adds strings , if the value of the char i in the string already exists we don't make another one.
    if(string_to_be_added == NULL|| pm_to_add_to == NULL||n<=0)return -1;
    pm_state_t* cur_state = pm_to_add_to->zerostate;
    for (int i = 0; i < n; i++) {
        if(string_to_be_added[i]=='\0')break;
        pm_state_t* new_state = pm_goto_get(cur_state,string_to_be_added[i]);///if the state with this edge already exists we countinue to it.
        if(new_state!=NULL){
            cur_state = new_state;
        }
        else {///if the state dosen't exits we make a new edge and then we set it as edge in the transitions of the last state.
            new_state = (pm_state_t*) malloc(sizeof (pm_state_t));
            pm_state_init(new_state);
            pm_goto_set(cur_state,string_to_be_added[i],new_state);
            cur_state = new_state;

        }
    }
    dbllist_append(cur_state->output,string_to_be_added);///at the last state of every word we set the output to the string given. (to know how is an accepting state)
    return 0;
}
pm_state_t* pm_goto_get(pm_state_t *state,unsigned char symbol) {///gets a state if there is an edge from the state given with the symbol given else returnes NULL.
    if(state->_transitions == NULL)return NULL;
    for (dbllist_node_t *k = dbllist_head(state->_transitions); k != NULL; k = dbllist_next(k)) {
        if(dbllist_data(k) == NULL)return NULL;
        pm_labeled_edge_t* new_labeled_data = (pm_labeled_edge_t *) (dbllist_data(k));
        if (new_labeled_data->label == symbol) {
            return new_labeled_data->state;
        }
    }
    return NULL;
}
int pm_goto_set(pm_state_t *from_state,unsigned char symbol,pm_state_t *to_state){///sets a new state (makes a new edge and then sets it as the next transition).
    if(from_state==NULL||to_state==NULL)return -1;
    pm_labeled_edge_t* new_edge = (pm_labeled_edge_t*)malloc(sizeof (pm_labeled_edge_t));
    new_edge->state = to_state;
    new_edge->label = symbol;
    new_edge->state->id = ++size_of_pm;
    new_edge->state->depth = from_state->depth + 1;
    printf("Allocating state %d\n%d->%c->%d \n",to_state->id,from_state->id,symbol,to_state->id);
    if(new_edge->state->depth<=1){
        new_edge->state->fail = from_state;
    }
    dbllist_append(from_state->_transitions,(void*)new_edge);
    return 0;
}
int pm_makeFSM(pm_t* cur_pm){
    ///a function that sets the fail part of every state in a spacific pm.
    if(cur_pm==NULL)return -1;
    dbllist_t* queue = (dbllist_t*)malloc(sizeof(dbllist_t));
    dbllist_init(queue);
    pm_state_t* new_state = cur_pm->zerostate;
    pm_state_t* fail_state = NULL;
    pm_state_t* child_state = NULL;
    dbllist_append(queue,new_state);///adds the zerostate to the queue.
    for (dbllist_node_t* j = dbllist_head(new_state->_transitions) ; j!=NULL; j= dbllist_next(j)){
    ///running on the transitions of the zero state alone becuase we set them in the go to set.
        dbllist_append(queue,((pm_labeled_edge_t*)dbllist_data(j))->state);
    }
    ///removes the head of the queue (zero state)
    dbllist_remove(queue,dbllist_head(queue),1);
    dbllist_size(queue) = dbllist_size(queue)-1;
    while(dbllist_size(queue)>0){///main loop until there is no objects in the queue.
        new_state = ((pm_state_t*)dbllist_data(dbllist_head(queue)));///every run sets the new state as the data of the head of the queue.
        dbllist_remove(queue,dbllist_head(queue),1);///removes the head of the queue.
        dbllist_size(queue)--;
            for (dbllist_node_t* j = dbllist_head(new_state->_transitions ) ; j != NULL; j = dbllist_next(j)){
            ///running on the children of the new_state and setting the fail state of every child to be the longest suffix that is a prifix of one of the added words.
                if(dbllist_data(j)!=NULL){
                    child_state = ((pm_labeled_edge_t*)dbllist_data(j))->state;
                    fail_state = new_state->fail;

                    while(pm_goto_get(fail_state,((pm_labeled_edge_t*)dbllist_data(j))->label)==NULL ){
                        if(fail_state->id != 0){
                            fail_state = fail_state->fail;
                        }
                        else break;
                    }
                    if(pm_goto_get(fail_state,((pm_labeled_edge_t*)dbllist_data(j))->label)!=NULL){
                        child_state->fail = pm_goto_get(fail_state,((pm_labeled_edge_t*)dbllist_data(j))->label);
                        if(dbllist_data(dbllist_head(child_state->fail->output))!=NULL){
                            if(dbllist_head(child_state->output) == dbllist_tail(child_state->output)){
                                dbllist_next(dbllist_head(child_state->output)) = dbllist_head(child_state->fail->output);
                            }
                            dbllist_next(dbllist_tail(child_state->output)) = dbllist_head(child_state->fail->output);
                            dbllist_tail(child_state->output) = dbllist_head(child_state->fail->output);
//                            printf("%s -> %s\n",(char*)dbllist_data(dbllist_head(child_state->output)),(char*)dbllist_data(
//                                    dbllist_head(child_state->fail->output)));
                        }
                        printf("Setting fail(%d) = %d \n",child_state->id,child_state->fail->id);
                    }
                    else{
                        child_state->fail = fail_state;
                    }
                }
                if(dbllist_data(j)!= NULL){
                    if(dbllist_head(queue) == NULL){
                        dbllist_head(queue) = (dbllist_node_t*) malloc(sizeof (dbllist_node_t));
                        dbllist_data(dbllist_head(queue)) = NULL;
                        dbllist_next(dbllist_head(queue)) = NULL;
                        dbllist_prev(dbllist_head(queue)) = NULL;
                    }
                    dbllist_append(queue,((pm_labeled_edge_t*)dbllist_data(j))->state);
                }
            }
        }
    dbllist_destroy(queue,1);
    return 0;
}
dbllist_t* pm_fsm_search(pm_state_t *state,unsigned char *where_to_search,pm_int_t length){
    ///a function that searches for substrings inside a string from a specific state.
    pm_state_t* cur_state = state;
    dbllist_t* returned_pm_matches =NULL;
    if(state == NULL ||where_to_search == "\0"||where_to_search)return returned_pm_matches;
    returned_pm_matches = (dbllist_t*) malloc(sizeof(dbllist_t));
    dbllist_init(returned_pm_matches);
//    pm_state_init(cur_state);
//    cur_state = state;
    for (int i = 0; i < length; i++) {
        if(pm_goto_get(cur_state,where_to_search[i])!=NULL) {
            cur_state = pm_goto_get(cur_state, where_to_search[i]);
            if (dbllist_data(dbllist_head(cur_state->output)) != NULL) {
                for (dbllist_node_t *j = dbllist_head(cur_state->output); j != NULL; j = dbllist_next(j)) {///for every output inside the state this makes a new pm_match
                    pm_match_t *new_match = (pm_match_t *) malloc(sizeof(pm_match_t));
                    new_match->end_pos = i;
                    new_match->start_pos = i - strlen((char *) (dbllist_data(j)))+1;
                    new_match->pattern = (char *) (dbllist_data(j));
                    new_match->fstate = cur_state;
                    dbllist_append(returned_pm_matches, new_match);
                    free(new_match);
                }
            }
        }
        else{
            if(cur_state->fail != NULL){
                i--;
                cur_state = cur_state->fail;
            }
        }
    }
    return returned_pm_matches;
}


//// dbllist_t* output;  //list of patterns //mallock list// char*
////dbllist_t* _transitions
void state_destroy(pm_state_t * cur_state){///destroys every state (reqursive loop that checks every transition state and checks if it is free or NULL and then frees the state and the things inside.)
    while(cur_state->_transitions!=NULL){///until the transitions are NULL ( there is no more transitions)
        if(dbllist_size(cur_state->_transitions) == 0){
            if(dbllist_head(cur_state->_transitions)!=NULL){///checks if the head is not NULL but the object is still malloced
                free(dbllist_head(cur_state->_transitions));
            }
            free(cur_state->_transitions);
            free(dbllist_head(cur_state->output));
            free(cur_state->output);
            cur_state->_transitions = NULL;
        }
        else{
            pm_state_t * child_state = ((pm_labeled_edge_t*)dbllist_data(dbllist_head(cur_state->_transitions)))->state;
            state_destroy(child_state);
            free(dbllist_data(dbllist_head(cur_state->_transitions)));
            dbllist_remove(cur_state->_transitions,dbllist_head(cur_state->_transitions),1);
            dbllist_size(cur_state->_transitions)--;
        }
    }
    free(cur_state);
}
void pm_destroy(pm_t *cur_pm){
    state_destroy(cur_pm->zerostate);
    free(cur_pm);
};


