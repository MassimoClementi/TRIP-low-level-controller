/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

#ifndef __THREAD_OBJECT_H__
#define __THREAD_OBJECT_H__

#include <MicroQt.h>
using namespace MicroQt;

/*
 * Class that embeds attributes and methods to instante
 * an internal timed cyclic thread
  */
class ThreadObject
{
private:
protected:
    int _updateInterval_ms;
    const Timer periodicTimer;
    virtual void Update() = 0;
public:
    ThreadObject(int updateInterval_ms);
    ~ThreadObject();
};

#endif
