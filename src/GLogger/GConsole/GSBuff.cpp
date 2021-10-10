#include "GConsole/GConsole.h"
#include "GConsole/GSBuff.h"


GSBuff::GSBuff(GConsole* Console) : m_Console(Console) {}
int GSBuff::sync() {
    std::unique_lock<std::mutex> Lck(m_Console->m_Buffer_Mutex);

    m_Console->m_Buffer.push_back(str());
    str("");

    GEvent Event;
    Event.Type = GEType::Console;
    Event.Console_Message = GEConsole_Message::Sync;

    m_Console->Post_Event(Event);
    return 0;
}
