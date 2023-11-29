/*
 * Author: Massimo Clementi <massimo_clementi@icloud.com>
 * Date:   2023-10-14
 */ 

#include "DataExchangeAbstract.h"

DataExchangeAbstract::DataExchangeAbstract(int updateInterval_ms) : ThreadObject(updateInterval_ms)
{
}

DataExchangeAbstract::~DataExchangeAbstract()
{
}
