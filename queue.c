#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
struct list_head *queue_head = malloc(sizeof(struct list_head));
    if (!queue_head)
        return NULL;
    INIT_LIST_HEAD(queue_head);
    return queue_head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l) 
{
    if(!l)
        return;
    element_t *q_entry , *enable;
    list_for_each_entry_safe(q_entry, enable, l, list)
    {
        q_release_element(q_entry);
    }
    free(l);
}


/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)//testing
{
    if(!head)
        return false;
    element_t *insert_node = malloc(sizeof(element_t));
    if(!insert_node)
        return false;
    size_t strsize = strlen(s) + 1;
    insert_node->value = malloc(strsize * sizeof(char));
    if(!insert_node->value){
        free(insert_node);
        return false;
    }
    memcpy(insert_node->value, s, strsize);
    list_add(&insert_node->list, head);
    return true;
}


/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)//**testing
{
    if(!head)
        return false;
    element_t *insert_node = malloc(sizeof(element_t));
    if(!insert_node)
        return false;
    size_t strsize = strlen(s) + 1;
    insert_node->value = malloc(strsize * sizeof(char));
    if(!insert_node->value){
        free(insert_node);
        return false;
    }
    memcpy(insert_node->value, s, strsize);
    list_add_tail(&insert_node->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if(!head || list_empty(head))
        return NULL;
    element_t *rmtarget = list_first_entry(head, element_t, list);
    list_del(&rmtarget->list);
    if(sp){
        size_t len = strlen(rmtarget->value) + 1;
        len = (bufsize - 1) > len ? len : (bufsize - 1);
        strncpy(sp, rmtarget->value, len);
        sp[len] = '\0';
    }
    return rmtarget;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if(!head || list_empty(head))
        return NULL;
    element_t *rmtarget = list_last_entry(head, element_t, list);
    list_del(&rmtarget->list);
    if(sp){
        size_t len = strlen(rmtarget->value) + 1;
        len = (bufsize - 1) > len ? len : (bufsize - 1);
        strncpy(sp, rmtarget->value, len);
        sp[len] = '\0';
    }
    return rmtarget;
}


/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if(!head)
        return 0;
    int count = 0;
    struct list_head *node;
    list_for_each (node, head)
        count++;
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if(!head || list_empty(head))
        return false;
    struct list_head *s = head->next, *f = head->next;
    while (f != head && f != head->prev){
        s = s->next;
        f = f->next->next;
    }
    element_t *rmelement = list_entry(s, element_t, list);
    list_del(&rmelement->list);
    q_release_element(rmelement);
    
    return true;
        
}
/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{        if (!head || list_empty(head))
            return false;
        element_t *imm_node, *next_node;
        bool dup = false;
        list_for_each_entry_safe(imm_node, next_node, head, list){
                if(imm_node->list.next != head && strcmp(imm_node->value, next_node->value) == 0){
                    list_del(&imm_node->list);
                    q_release_element(imm_node);
                    dup = true;
                }
                else if(dup){
                    list_del(&imm_node->list);
                    q_release_element(imm_node);
                    dup = false;
                }
        }
        return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (head && !list_empty(head)){
    struct list_head *target = head->next;
    while (target != head && target->next != head){
        struct list_head *temp = target;
        list_move(target, temp->next);
        target = target->next;
        }
    }
    return;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *n, *s, *t;
    list_for_each_safe (n, s, head) {
        t = n->next;
        n->next = n->prev;
        n->prev = t;
    }
    t = head->next;
    head->next = head->prev;
    head->prev = t;
}
    

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if(!head || list_empty(head))
        return;
    struct list_head *n, *s, iter, *fake_head = head;
    int counter = 0;
    INIT_LIST_HEAD(&iter);
    list_for_each_safe(n, s, head){
        counter++;
        if(counter == k){
            list_cut_position(&iter, fake_head, n);
            q_reverse(&iter);
            list_splice_init(&iter, fake_head);
            counter = 0;
            fake_head = s->prev;
        }
    }
}
/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head){
    if(!head || list_empty(head) || list_is_singular(head))
        return;
    struct list_head *mid;
    {
        struct list_head *l, *r;
        l = head->prev;
        r = head->next;
        while (r != l && r->next != l)
        {
            r = r->next;
            l = l->prev;
        }
        mid = r;
    }
    LIST_HEAD(r);
    LIST_HEAD(l);

    list_cut_position(&r, head, mid);
    list_splice_init(head, &l);

    q_sort(&r);
    q_sort(&l);

    while (!list_empty(&r) && !list_empty(&l))
    {
        if (strcmp(list_first_entry(&r, element_t, list)->value,
        list_first_entry(&l, element_t, list)->value)<= 0)
        {
            list_move_tail(r.next, head);
        }
        else{
            list_move_tail(l.next, head);
        }
        
    }
    list_splice_tail(&r, head);
    list_splice_tail(&l, head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    struct list_head *c = head->prev, *n = head->prev->prev;
    while(n != head){
        if(strcmp(list_entry(n, element_t, list)->value,list_entry(c, element_t, list)->value) < 0){
            list_del(n);
        }
        else{
            c = n;
        }
        n = n->prev;
    }
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head))
        return 0;
    int count = 0;
    struct list_head *first_head, temp_head;
    first_head = list_entry(head->next, queue_contex_t, chain)->q;
    INIT_LIST_HEAD(&temp_head);
    for (;;) {
        element_t *min_num = NULL;
        queue_contex_t *entry, *min_content;
        list_for_each_entry (entry, head, chain) {
            if (entry->q == NULL || list_empty(entry->q))
                continue;
            element_t *element = list_first_entry(entry->q, element_t, list);
            if (!min_num || strcmp(element->value, min_num->value) < 0) {
                min_num = element;
                min_content = entry;
            }
        }
        if (!min_num)
            break;
        list_move_tail(&min_num->list, &temp_head);
        count++;
        if (min_content->q != first_head && list_empty(min_content->q))
            min_content->q = NULL;
    }
    list_splice(&temp_head, first_head);
    return count;
}
