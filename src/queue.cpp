#include "queue.h"
#include <stdlib.h>

struct queueElement {
    void* content;
    struct queueElement* nextElement;
};

static struct queueElement* queueHead = NULL;
static struct queueElement* queueTail = NULL;
static int queueSize = 0;

void addToQueue(void* elementPtr) {
    struct queueElement* newElement = (queueElement*)(malloc(sizeof(struct queueElement)));
    newElement->content = elementPtr;
    newElement->nextElement = NULL;
    if(queueTail != NULL)
        queueTail->nextElement = newElement;
    queueTail = newElement;
    if(queueHead == NULL)
        queueHead = newElement;
    queueSize++;
}

void* getHead() {
    if(queueHead!=NULL)
        return queueHead->content;
    return NULL;
}
void removeHead() {
    if(queueHead != NULL) {
        struct queueElement* nextElement = queueHead->nextElement;
        free(queueHead->content);
        free(queueHead);
        // if we remove the last item of the queue
        if(nextElement == NULL)
            queueTail = NULL;
        queueHead = nextElement;
        queueSize--;
    }
}

void clearQueue() {
    while(queueHead != NULL)
        removeHead();
}

int getQueueSize() { return queueSize; }
