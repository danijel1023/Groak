#include "GConsole/GConsole.h"
#include "GConsole/GSBuff.h"

GSBuff::GSBuff(GConsole* Console) : m_Console(Console) {}
int GSBuff::sync() {
    while (m_Console->m_Read);
    m_Console->m_Read = true;

    m_Console->m_Buffer.push_back(str());
    str("");

    m_Console->m_Read = false;
    return 0;
}
