#ifndef SDL_UTILS_TEXTURE_H_
#define SDL_UTILS_TEXTURE_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "utils/drawing/DrawParams.h"
// Forward declarations
struct SDL_Surface;
struct SDL_Texture;
struct SDL_Renderer;
typedef struct _TTF_Font TTF_Font;
class Color;

// // later on in complex projects can use this "enum" instead of returning "int32_t" -> i.e. in createSurfaceFromFile() and any other methods
// enum class ErrorCodes{
//     SUCCESS,
//     FAILURE,
//     OUT_OF_MEMORY
// };


// use this class to load CPU / GPU primitives which are used/needed to load images
class Texture{
public:
    Texture()=delete;
    ~Texture();
    static int32_t createSurfaceFromFile(const char* filePath, SDL_Surface*& outSurface); // try with "const char*&"
    static int32_t createTextureFromFile(const char* filePath, SDL_Texture*& outTexture);
    static int32_t createTextureFromSurface(SDL_Surface*& inOutSurface, SDL_Texture*& outTexture);

    static int32_t createTextureFromText(const char* text, const Color& color, TTF_Font* font,
                                            SDL_Texture*& outTexture,
                                            int32_t& outTextWidth, int32_t& outTextHeight);

    static int32_t createEmptyTexture(int32_t width, int32_t height, SDL_Texture*& outTexture);
    static void freeSurface(SDL_Surface*& outSurface);
    static void freeTexture(SDL_Texture*& outTexture);
    static void setRenderer(SDL_Renderer* renderer); // a helper method

    static int32_t setBlendModeTexture(SDL_Texture* texture, BlendMode blendMode); // blendMode is the type of algorithm used !!!
    static int32_t setAlphaTexture(SDL_Texture* texture, int32_t alpha);

    static int32_t clearCurrentRendererTarget(const Color& color);
    // actually the Frame Buffer Object is nothing more but and "SDL_Texture" that is created in a specific way
    static int32_t setRendererTarget(SDL_Texture* target);
    static int32_t resetRendererTarget();
};

#endif // SDL_UTILS_TEXTURE_H_