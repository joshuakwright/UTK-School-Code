#include "mymalloc.h"
#include <stdio.h>
#include <unistd.h>

struct Flist {
    int size;
    struct Flist *next;
};

struct Flist *head = NULL;


/**
* @name my_malloc.
* @brief Uses sbrk to allocate space in memory by putting free memory on a linked list and then carving out memory to allocate from that list.
* @param[in] size The size in bytes to be allocated.
* @param[out] mptr A void * pointer to the beginning of the user allocated memory (8 bytes before are used for book keeping).
* @return A pointer to memory of the specified size in bytes.
*/
void* my_malloc(size_t size) {
    size = (size + 7 + 8) & -8;
    
    struct Flist *ptr = head;
    struct Flist *mptr;

    //allocates intitail memory
    if (head == NULL) {
        void *tptr;
        if (size < 8192) {
            tptr = sbrk(8192);
            ptr = (struct Flist *)tptr;
            ptr->size = 8192;
            int temp = ptr->size;
            mptr = ptr;
            mptr->size = size;
            ptr = (void *)ptr + size;
            ptr->size = temp - size;
            head = ptr;
        }
        else {
            tptr = sbrk(size);
            ptr = (struct Flist *)tptr;
            ptr->size = size;
            mptr = ptr;
            mptr->size = size;
        }
        
    }
    //allocates memory if only head exists
    else if (head->next == NULL && head->size < size) {
        void *tptr;
        if (size < 8192) {
            tptr = sbrk(8192);
            ptr = (struct Flist *)tptr;
            ptr->size = 8192;
            int temp = ptr->size;
            mptr = ptr;
            mptr->size = size;
            ptr = (void *)ptr + size;
            ptr->size = temp - size;
            head->next = ptr;
        }
        else {
            tptr = sbrk(size);
            ptr = (struct Flist *)tptr;
            ptr->size = size;
            mptr = ptr;
            mptr->size = size;
        }
    }
    //if head and head->next already have elements
    else {  
        //checks the first element in the list                                             
        if (ptr->size >= size) {
            if (ptr->size == size) {
                int temp = ptr->size;
                mptr = ptr;
                mptr->size = size;
                head = ptr->next;
            }
            else {
                int temp = ptr->size;
                struct Flist *tptr = ptr->next;
                mptr = ptr;
                mptr->size = size;
                ptr = (void *)ptr + size;
                ptr->size = temp - size;
                head = ptr;
                head->next = tptr;
            }
        }
        //if the first element doesn't fit, then goes through the rest of the list to find the correct place for the pointer
        else {
            struct Flist *prevptr = ptr;
            while (ptr->next != NULL) {
                ptr = ptr->next;
                if (ptr->size >= size) {
                    int temp = ptr->size;
                    struct Flist *tptr = ptr->next;
                    mptr = ptr;
                    mptr->size = size;
                    ptr = (void *)ptr + size;
                    ptr->size = temp - size;
                    ptr->next = tptr;
                    prevptr->next = ptr;
                    break;
                }
                //allocates more space if there isn't enough free memeory in the current free memory blocks
                else if(ptr->size < size && ptr->next == NULL) {
                    void *tptr;
                    struct Flist *tptr2;
                    if (size < 8192) {
                        tptr = sbrk(8192);
                        tptr2 = (struct Flist *)tptr;
                        tptr2->size = 8192;
                        int temp = tptr2->size;
                        mptr = tptr2;
                        mptr->size = size;
                        tptr2 = (void *)tptr2 + size;
                        tptr2->size = temp - size;
                        ptr->next = tptr2;
                    }
                    else {
                        tptr = sbrk(size);
                        mptr = (struct Flist *)tptr;
                        mptr->size = size;
                    }
                    break;
                }
                prevptr = prevptr->next;
            }
        }
    }
    
    return (void *)mptr + 8;
}

/**
* @name my_free.
* @brief Frees the memory pointed to by ptr and adds it back to the free list.
* @param[in] ptr The pointer to the area of memory to be freed.
*/
void my_free(void* ptr) {
    ptr -= 8;

    struct Flist *mptr = (struct Flist *)ptr;
    struct Flist *hptr = head;
    
    if(hptr == NULL) head = mptr;
    else if (hptr->next == NULL) {
        if (mptr < hptr) {
            mptr->next = hptr;
            head = mptr;
        }
        else head->next = mptr; 
    }
    else if (mptr < hptr) {
        mptr->next = hptr;
        head = mptr;
    }
    else {
        while (hptr->next != NULL) {
            if (hptr < mptr && hptr->next > mptr) {
                struct Flist *temp = hptr->next;
                hptr->next = mptr;
                mptr->next = temp;
                break;
            }
            hptr = hptr->next;
        }
        if (hptr->next == NULL) hptr->next = mptr; 
    }
}

/**
* @name free_list_begin.
* @brief Returns the head of the linked list holding all the free memory, or NULL if the list is empty.
* @return A void * pointer to the head of the list of free memory.
*/
void* free_list_begin() {
    return head;
}

/**
* @name free_list_next.
* @brief Returns the next block of free memory stored after the inputted node in the linked list of free memory.
* @param[in] node A pointer to a block of memory in the free memory list.
* @return A pointer to the block of free memory after the specified node or NULL if there is none.
*/
void* free_list_next(void* node) {
    struct Flist *fnode = (struct Flist *)node;
    return fnode->next;
}

/**
* @name coalesce_free_list.
* @brief Merges all of the adjacent blocks of memory in the free list.
*/
void coalesce_free_list() {
    struct Flist *hptr = head;
    while (hptr->next != NULL) {
        if ((void*)hptr + hptr->size == hptr->next) {
            hptr->size += hptr->next->size;
            hptr->next = hptr->next->next;
        }
        else {
            hptr = hptr->next;
        }
    }
}