#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_MANAGERS_RSRCMGR_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_MANAGERS_RSRCMGR_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/managers/MgrBase.h"
#include "sdl_utils/containers/ImageContainer.h"
#include "sdl_utils/containers/TextContainer.h"
// Forward Declarations
struct RsrcMgrCfg;

class RsrcMgr : public MgrBase, public ImageContainer, public TextContainer {
public:

    RsrcMgr()=default; // NEVER forget to set explicitly the default Ctor in the base class (if any) as well

    // forbid the copy and move Ctors
    RsrcMgr(const RsrcMgr& other)=delete;
    RsrcMgr(RsrcMgr&& other)=delete; // && means R value reference

    // forbid the copy and move assignment operators
    RsrcMgr& operator=(const RsrcMgr& other)=delete;
    RsrcMgr& operator=(RsrcMgr&& other)=delete; // && means R value reference

    int32_t init(const RsrcMgrCfg& cfg);

    virtual void deinit() final;
    virtual void process() final;
};

extern RsrcMgr* gRsrcMgr;


#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_MANAGERS_RSRCMGR_H_