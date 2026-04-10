#include "RequestQueue.h"
#include <new> // for std::nothrow

RequestQueue::RequestQueue()
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    // start with a small default capacity
    resize(4);
}

RequestQueue::RequestQueue(int initialCapacity)
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    if (initialCapacity < 1) {
        initialCapacity = 4;
    }
    resize(initialCapacity);
}

RequestQueue::~RequestQueue() {
    delete[] data;
}

bool RequestQueue::isEmpty() const {
    return count == 0;
}

bool RequestQueue::isFull() const {
    return count == capacity;
}

int RequestQueue::size() const {
    return count;
}

int RequestQueue::nextIndex(int idx) const {
    if (capacity == 0) return 0;
    return (idx + 1) % capacity;
}

bool RequestQueue::enqueue(const Request& req) {
    //Implement enqueue function as explained in the PDF.
    //(void)req; // suppress unused warning until implemented
    if(size() == capacity){
        int doubleCapacity = 2 * capacity;
        resize(doubleCapacity);
    }

    if (isEmpty()) {
        front = 0;
        rear = 0; 
    }
    else {
        //modular aricthmetic
        rear = (rear + 1) % capacity; 
    }   

    data[rear] = req;
    count++;  

    return true;
}

bool RequestQueue::dequeue(Request& outReq) {
    //Implement dequeue function as explained in the PDF.
    //(void)outReq; // suppress unused warning until implemented
    if(isEmpty()){
        return false;        
    }
    
    if(rear == front){
        outReq = data[front];
        front = rear = 0;
        count--;
        return true;
    }

    outReq = data[front];
    front = (front +1) % capacity;
    count--;
    return true;
}

bool RequestQueue::peek(Request& outReq) const {
    //Implement peek function as explained in the PDF.
    if (isEmpty()) {
        return false;
    }
    // En öndeki elemanı (front) ver
    outReq = data[front];
    return true;
}

void RequestQueue::clear() {
    front = 0;
    rear = 0;
    count = 0;
}

bool RequestQueue::removeById(const std::string& id) {
    //Implement removeById function as explained in the PDF.
    //(void)id; // suppress unused warning until implemented
    
    if (isEmpty()) return false;

    //founding the element
    int foundIndex = -1;
    for(int i = 0; i < capacity ; i++){
        int modulerIndex = (front + i) % capacity;
        if(data[modulerIndex].getId() == id){
            foundIndex = modulerIndex;
            break;
        }
    }
    // exit if it is not found 
    if (foundIndex == -1){ return false; }


    // shifting one left 
    for (int j = foundIndex; j < count - 1; j++) {
        int currentPos = (front + j) % capacity;
        int nextPos = (front + j + 1) % capacity;
        
        data[currentPos] = data[nextPos]; // take the one behind to the front
    }
    // Tail Reduction
    rear = (rear - 1 + capacity) % capacity;
    count--;

    // If the queue is empty, reset it.
    if (count == 0) {
        front = 0;
        rear = -1; 
    }

    return true;
}

bool RequestQueue::resize(int newCapacity) {
    //Implement resize function as explained in the PDF.
    
    Request* temp = new Request[newCapacity];

    if(isEmpty()){
        delete[] data;
        data = temp;
        temp = nullptr;
    }
    else{
        int indexFront = front;
        for(int i = 0; i< size() ; i++){
            int modulerIndex = indexFront++ % capacity;
            temp[i] = data[modulerIndex];
        }
        delete[] data;
        data = temp;
        temp = nullptr;

        front = 0;
        rear = count - 1;
    }
    capacity = newCapacity;
    return true;
}
