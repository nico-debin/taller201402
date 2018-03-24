#ifndef _QueueLock_H_
#define _QueueLock_H_

#include "Lock.h"
#include <queue>

template <typename T>
class QueueLock {

private:
  std::queue<T> queue;
  Lock lock;

public:
  
	QueueLock(){}
  
  int size(){
	this->lock.start();
	int size = this->queue.size();
	this->lock.stop();
	return size;
  }

  void push(T const & value){
	this->lock.start();
    this->queue.push(value);
	this->lock.stop();
  }

  T pop(){
	this->lock.start();
    T v = this->queue.front();
    this->queue.pop();
	this->lock.stop();
    return v;
  }
  
  void clear(){
	this->lock.start();
	while (!this->queue.empty()){
        this->queue.pop();
    }
	this->lock.stop();
  }

};

#endif