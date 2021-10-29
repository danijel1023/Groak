#pragma once
#include "GCore.h"
struct GLFWcursor;

class GCursor {
public:
    uint8_t* Data = nullptr;
    GSize Size;
    GPos Hot_Spot;

private:
    GLFWcursor* m_Ptr = nullptr;
    friend class GApplication;
};
