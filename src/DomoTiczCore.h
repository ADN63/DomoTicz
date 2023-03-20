#pragma once
#ifndef _DomoTicz_Core_H_
#define _DomoTicz_Core_H_
#include <DomoTiczCommon.h>
#include <EEPROM.h>

#define EEPROM_SIZE 1024
#if EEPROM_SIZE > (E2END + 1)
#error("Card not Support this EEPROM_SIZE")
#endif
P(p_node_name) = NAME;
#define NODE_NAME Fstring(p_node_name)
#define NODE_ID string(ID)


class DomoTicz
{
private:
    static eeprom_data_t make_factory_eeprom_data();
    static void init_eeprom();

public:
    static void begin();
    static void loop();
    static void ReceivCmdChangeName();
    static void ReceivCmdChangeId();
    static void ReceivCmdReset();
    static void ReceivCmdRestart();
    DomoTicz(/* args */);
    ~DomoTicz();
};

#include <NodeControls.hpp>

#include <DomoTicz.hpp>

#endif