#pragma once

#ifndef NDEBUG
#define Gcout(x) std::cout << x
#else
#define Gcout(x)
#endif // NDEBUG


#include "../Vendor/glad/include/glad/glad.h"
#include "../Vendor/glfw3/include/GLFW/glfw3.h"
#include "../Vendor/stb/include/stb_image.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#ifdef _WIN64
#  define GROAK_FONT_FILE "C:/Windows/Fonts/consola.ttf"
#else
#  define GROAK_FONT_FILE "/home/danijel/Desktop/consola.ttf"
//#  error supply font file for specific platform
#endif

#include "GEvent.h"
#include "GQueue/GQueue.h"
#include "GQueue/GTQueue.h"
#include "GString.h"

using GCallback = int(*)(void* _This, const GEvent& Event);
using GDispatcher = int(*)(void* _This, const GEvent& Event);
using GEQueue = GQueue<GEvent>;     //Groak Event Queue
using GTEQueue = GTQueue<GEvent>;   //Groak Thread safe Event Queue

//Use to call functions - prevents mistakes
#define GCall(_This, Func, Event) (*_This->Func)(_This, Event)
#define GGenCall(_This, Func, ...) (*_This->Func)(__VA_ARGS__)

#define GApp() GApplication::m_Instance
