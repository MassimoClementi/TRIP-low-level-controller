/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

#include "ThreadObject.h"

ThreadObject::ThreadObject(int updateInterval_ms){
    _updateInterval_ms = updateInterval_ms;
  
    // Configure internal thread
    periodicTimer.setIntervalMs(_updateInterval_ms);
    periodicTimer.setSingleShot(false);
    periodicTimer.sglTimeout.connect([this](){this->Update();});
    periodicTimer.start();    
}

ThreadObject::~ThreadObject()
{
}
