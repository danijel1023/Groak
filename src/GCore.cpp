#include <iostream>
#include "GCore.h"
#include "GLog_Manager.h"


namespace Groak {
    GTexture Load_Texture(const GString& Path, bool Flip) {
        GTexture Texture;
        stbi_set_flip_vertically_on_load(Flip);
        Texture.Pixels = stbi_load(Path.c_str(), &Texture.Size.X, &Texture.Size.Y, &Texture.Channels, 0);
        
        if (!Texture.Pixels) {
            GError() << "Could not load texture: " << Path;
        }

        return Texture;
    }

    GTexture Load_Texture_From_Memory(const unsigned char* Mem_Data, unsigned int Size, bool Flip) {
        GTexture Texture;
        stbi_set_flip_vertically_on_load(Flip);
        Texture.Pixels = stbi_load_from_memory(Mem_Data, Size, &Texture.Size.X, &Texture.Size.Y, &Texture.Channels, 0);

        if (!Texture.Pixels) {
            GError() << "Could not load texture from memory";
        }

        return Texture;
    }

    //Only deletes cpu-side texture. If the texture has been stored,
    //the GPU-side texture will get deleted on window destruction
    void Delete_Texture(GTexture& Texture) {
        if (Texture.Pixels) stbi_image_free(Texture.Pixels);
        Texture.Pixels = nullptr;
    }
}
