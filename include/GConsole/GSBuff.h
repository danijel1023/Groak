#pragma once
#include <sstream>

class GConsole;
class GSBuff : public std::stringbuf {
public:
    GSBuff(GConsole* Console);
    int sync() override;

private:
    GConsole* m_Console;
};
