#include "common_thread.h"

Thread::Thread(){}

void Thread::join(){
        thread.join();
}

void Thread::start(){
        thread = std::thread(&Thread::run, this, &this->end);
}

short int Thread::termino_thread(){
        return this -> end;
}
