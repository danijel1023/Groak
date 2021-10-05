#pragma once
#include "GCore.h"

class GCursor {
public:
    const char* Data = nullptr;
    GSize Size;
    GPos Hot_Spot;

private:
    GLFWcursor* m_Ptr = nullptr;
    friend class GApplication;
};
