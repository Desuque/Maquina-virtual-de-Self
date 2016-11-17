#ifndef __common_thread_h__
#define __common_thread_h__

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

class Thread {
        private:
                std::thread thread;
                int end = 0;
  
        public:
                Thread();
                void join();
                virtual void run(int* fin) = 0;
                void start();
                short int termino_thread();
                virtual ~Thread() {}
  
        private:
                Thread(const Thread&) = delete;
                Thread& operator=(const Thread&) = delete;
                Thread(Thread&& other){
                        this->thread = std::move(other.thread);
                }
                Thread& operator=(Thread&& other){
                        this->thread = std::move(other.thread);
                        return *this;
                }
};

#endif
