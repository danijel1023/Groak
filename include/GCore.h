#pragma once


#ifdef GROAK_BUILD
#include "../Vendor/glad/include/glad/glad.h"
#include "../Vendor/glfw3/include/GLFW/glfw3.h"
#include "../Vendor/stb/include/stb_image.h"

#include "../Vendor/stb/include/stb_image_write.h"
#endif

//TODO: Abstract ft
#include "ft2build.h"
#include FT_FREETYPE_H

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


namespace Groak {
    GTexture Load_Texture(const GString& Path, bool Flip = false);
    GTexture Load_Texture_From_Memory(const unsigned char* Mem_Data, unsigned int Size, bool Flip = false);
    void Delete_Texture(GTexture& Texture);
}
