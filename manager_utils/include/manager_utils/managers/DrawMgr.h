#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_MANAGERS_DRAWMGR_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_MANAGERS_DRAWMGR_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/managers/MgrBase.h"
#include "sdl_utils/Renderer.h"
#include "sdl_utils/MonitorWindow.h"
// Forward Declarations
struct DrawMgrCfg;
struct SDL_Texture;

class DrawMgr : public MgrBase{
public:

    // the automatic generation of the default Ctor is disabled, because we explicitly list below other Ctors...
    // ... therefore must explicitly set the default Ctor
    DrawMgr()=default; // NEVER forget to set explicitly the default Ctor in the base class (if any) as well 

    // forbid copy and move Ctors
    DrawMgr(const DrawMgr& other)=delete;
    DrawMgr(DrawMgr&& other)=delete; // && means R value reference

    // forbid copy and move assignment operators
    DrawMgr& operator=(const DrawMgr& other)=delete;
    DrawMgr& operator=(DrawMgr&& other)=delete; // && means R value reference

    // "init()" is not virtual because each Singleton will accept a configuration that is different     
    int32_t init(const DrawMgrCfg& cfg); // if std::any or void* (void* is for C language fans) are used "init()" can be set virtual - but we here do not want it to be virtual 
    virtual void deinit() final;
    virtual void process() final;

    void clearScreen();
    void finishFrame();
    void addDrawCmd(const DrawParams& drawParams) const;

    void setWidgetBlendMode(const DrawParams& drawParams, BlendMode blendMode);
    void setWidgetOpacity(const DrawParams& drawParams, int32_t opacity);

    int64_t getMaxFrameRate() const;

private:
    SDL_Texture* getTextureInternal(const DrawParams& drawParams) const;

    Renderer _renderer;
    MonitorWindow _window;

    // Hold maximum frame rate cap
    int64_t _maxFrames {0};
};

extern DrawMgr* gDrawMgr;

#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_MANAGERS_DRAWMGR_H_