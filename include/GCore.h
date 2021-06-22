#pragma once

#ifndef NDEBUG
#define Gcout(x) std::cout << x
#else
#define Gcout(x)
#endif // NDEBUG

#include "../Vendor/glew-2.1.0/include/GLEW/glew.h"
#include "../Vendor/glfw3/include/GLFW/glfw3.h"


#ifdef _WIN64
#include "ft2build.h"
#include FT_FREETYPE_H
#else
// I've put the whole include directory with library to force platform
// specific code seperation - in this case the binaries
// ; After code seperation, the freetype include dir should be seperated from
// os-specific binaries and this #ifdef must be removed
#error Add OS-specific FreeType binaries
#endif


#include "GEvent.h"
#include "GQueue/GQueue.h"
#include "GQueue/GTQueue.h"
#include "GString.h"

using GCallback = int(*)(void* _This, GEvent* Event);
using GDispatcher = int(*)(void* _This, GEvent* Event);
using GEQueue = GQueue<GEvent>;     //Groak Event Queue
using GTEQueue = GTQueue<GEvent>;   //Groak Thread safe Event Queue

//Use to call functions - prevents mistakes
#define GCall(_This, Func, Event) (*_This->Func)(_This, Event)
#define GGenCall(_This, Func, ...) (*_This->Func)(__VA_ARGS__)

#define GApp() GApplication::m_Instance
