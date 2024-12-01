#ifndef SDL_UTILS_TEXTURE_H_
#define SDL_UTILS_TEXTURE_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
// Forward declarations
struct SDL_Surface;



// // later on in complex project can use this "enum" in stead of returning "int32_t" -> i.e. in createSurfaceFromFile() and any other methods
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
    static void freeSurface(SDL_Surface*& outSurface);
};

#endif // SDL_UTILS_TEXTURE_H_