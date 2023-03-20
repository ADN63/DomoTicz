#include "DomoTiczCore.h"
#ifndef _DomoTicz_HPP_
#define _DomoTicz_HPP_

inline eeprom_data_t DomoTicz::make_factory_eeprom_data()
{
    eeprom_data_t factory;
    memcpy(&factory.nodeName, NODE_NAME, sizeof(factory.nodeName));
    memcpy(&factory.nodeId, NODE_ID, sizeof(factory.nodeId));

    // sprintf(factory.nodeName, PSTR("%s"), NODE_NAME);
    // sprintf(factory.nodeId, PSTR("%s"), NODE_ID);

    return factory;
}

inline void DomoTicz::init_eeprom()
{
    eeprom_data_t tdata;
    EEPROM.get(0, tdata);
    static bool dataCorupt = false;
    if ((strlen(tdata.nodeId) == 0) || (strlen(tdata.nodeId) > MAX_LENGTH_EEPROM))
    {
        dataCorupt = true;
    }
    else if ((strlen(tdata.nodeName) == 0) || (strlen(tdata.nodeName) > MAX_LENGTH_EEPROM))
    {
        dataCorupt = true;
    }

    if (dataCorupt)
    {
        tdata = make_factory_eeprom_data();
        EEPROM.put(0, tdata);
        EEPROM.get(0, tdata);
    }
    else
    {
        EEPROM.get(0, tdata);
    }
    memcpy(&_Saved_Ref, &tdata, sizeof(_Saved_Ref));
}
inline void DomoTicz::begin()
{
    init_eeprom();
    NodeControls::begin();
}
/**
 * @brief
 * If NodeControls is Use
 * --  show in ParseCmd() for more details
 */
inline void DomoTicz::loop()
{
    NodeControls::loop();
}

inline void DomoTicz::ReceivCmdChangeName()
{

    if (strlen(_VALUE) > 0 && strlen(_VALUE) < MAX_LENGTH_EEPROM)
    {
        TRACE_F("%s %s %s %s\n", String(F("Change current Name :")).c_str(), _Saved_Ref.nodeName, String(F("To")).c_str(), _VALUE);

        memcpy(&_Saved_Ref.nodeName, _VALUE, sizeof(_Saved_Ref.nodeName));
        TRACE_F("news name :%s\n", _Saved_Ref.nodeName);
        // sprintf(_Saved_Ref.nodeName, PSTR("%s"), _VALUE);
        EEPROM.put(0, _Saved_Ref);
        // EEPROM.get(0, _Saved_Ref);
    }
    else if (strlen(_VALUE) == 0)
    {

        TRACE_F("%s\n", _Saved_Ref.nodeName);
    }
    else if (strlen(_VALUE) > MAX_LENGTH_EEPROM)
    {
        TRACE_F("%s : %s\n", _VALUE, String(F("Is not Valid for EEPROM")).c_str());
    }
}

inline void DomoTicz::ReceivCmdChangeId()
{
    if (strlen(_VALUE) > 0 && strlen(_VALUE) < MAX_LENGTH_EEPROM)
    {
        int _id = (int)atoi(_VALUE);
        int _currentId = (int)atoi(_Saved_Ref.nodeId);

        if (_id == 0)
        {
            TRACE_F("%s\n", String(F("Error Node 0 is Reserved")).c_str());
            return;
        }
        else if (_id == _currentId)
        {
            TRACE_F("%s %d\n", String(F("Id  is already")).c_str(), _id);
            return;
        }
        else if (_id >= 255)
        {
            TRACE_F("%s\n", String(F("Max id 254")).c_str());
            return;
        }

        TRACE_F("%s %s %s %s\n", String(F("Change current Node Id :")).c_str(), _Saved_Ref.nodeId, String(F("To")).c_str(), _VALUE);
        // sprintf(_Saved_Ref.nodeName, PSTR("%s"), _VALUE);
        memcpy(&_Saved_Ref.nodeId, _VALUE, sizeof(_Saved_Ref.nodeId));
        TRACE_F("news id :%s\n", _Saved_Ref.nodeId);
        EEPROM.put(0, _Saved_Ref);
    }
    else if (strlen(_VALUE) == 0)
    {

        TRACE_F("%s\n", _Saved_Ref.nodeId);
    }
    else if (strlen(_VALUE) > MAX_LENGTH_EEPROM)
    {
        TRACE_F("%s : %s\n", _VALUE, String(F("Is not Valid for EEPROM")).c_str());
    }
}
static uint8_t _counter_reset = 0;
static unsigned long reset_interval = 2500;
static unsigned long _previous_reset = 0;
inline void DomoTicz::ReceivCmdReset()
{
    if (millis() - _previous_reset >= reset_interval)
    {
        _previous_reset = millis();
        _counter_reset = 0;
    }
    _counter_reset++;
    if (_counter_reset == 1)
    {
        TRACE_F("%s\n", String(F("One More Time")).c_str());
    }
    else if (_counter_reset >= 2)
    {
        eeprom_data_t tdata = make_factory_eeprom_data();
        EEPROM.put(0, tdata);
        EEPROM.get(0, _Saved_Ref);
        _counter_reset = 0;
        TRACE_F("%s\n", String(F("Reset Done !")).c_str());
    }
}

inline void DomoTicz::ReceivCmdRestart()
{
    TRACE_F("%s\n", String(F("Board while be Restart")).c_str());
    wdt_enable(WDTO_15MS);
    while (true)
    {
        /* code */
    }
    
}

DomoTicz::DomoTicz(/* args */)
{
}

DomoTicz::~DomoTicz()
{
}

#endif