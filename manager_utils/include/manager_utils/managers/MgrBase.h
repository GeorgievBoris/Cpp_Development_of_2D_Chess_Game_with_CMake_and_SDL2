#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_MANAGERS_MGRBASE_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_MANAGERS_MGRBASE_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
// Forward Declarations

enum {
    DRAW_MGR_IDX,
    RSRC_MGR_IDX,
    MANAGERS_COUNT
};

class MgrBase{
public:

    // the automatic generation of the default Ctor is disabled, because we explicitly list below other Ctors...
    // ... therefore must explicitly set the default Ctor 
    MgrBase()=default;
    virtual ~MgrBase()=default; // the Dtor of the base class must be virtual - very important!

    // forbid the copy and move Ctors
    // forbid the copy and move assignment operators

    MgrBase(const MgrBase& other)=delete;
    MgrBase& operator=(const MgrBase& other)=delete;

    MgrBase(MgrBase&& other)=delete; // && means R value reference
    MgrBase& operator=(MgrBase&& other)=delete; // && means R value reference

    virtual void deinit()=0;
    virtual void process()=0;

private:

};


#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_MANAGERS_MGRBASE_H_