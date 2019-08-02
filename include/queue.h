/* this file provides an easy to use queue structure
 * Note : there can only be one queue */

#ifndef QUEUE_H
#define QUEUE_H

/* add an element to the queue (to the tail) */
void addToQueue(void* elementPtr);
/* get the first element of the queue (or NULL if no element is in the queue) */
void* getHead();
/* remove the first element and put the second at the head (if any)
 * WARNING : it also disallocate the element */
void removeHead();
/* remove all the elements in the queue and disallocate the elements */
void clearQueue();
/* returns the number of elements in the queue */
int getQueueSize();

#endif
