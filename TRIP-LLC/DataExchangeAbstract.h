/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

#ifndef __DATA_EXCHANGE_ABSTRACT_H__
#define __DATA_EXCHANGE_ABSTRACT_H__

#include <MicroQt.h>
using namespace MicroQt;

#include "ThreadObject.h"
#include "Data.h"

/*
 * Class that embeds attributes and methods to instante
 * a data exchange object with internal thread
  */
class DataExchangeAbstract : public ThreadObject
{
private:
protected:
    virtual void Update() = 0;
    Command _lastCommand;
public:
    DataExchangeAbstract(int updateInterval_ms);
    ~DataExchangeAbstract();
    virtual void SendMessage(const String message) = 0;

    Signal<const Command> ECommandReceived;
};

#endif
