#include "IETThread.h"



void IETThread::start() {
    thread = std::thread(&IETThread::run, this);
    if (detachThread) {
        thread.detach();
    }
}


void IETThread::join() {
    if (thread.joinable()) {
        thread.join();
    }
}