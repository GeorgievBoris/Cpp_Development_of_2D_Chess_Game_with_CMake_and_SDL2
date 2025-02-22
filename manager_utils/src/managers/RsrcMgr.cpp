// Corresponding header
#include "manager_utils/managers/RsrcMgr.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "manager_utils/config/RsrcMgrCfg.h"

RsrcMgr* gRsrcMgr=nullptr;

int32_t RsrcMgr::init(const RsrcMgrCfg& cfg){

    if(EXIT_SUCCESS!=ImageContainer::init(cfg.imgContainerCfg)){
        std::cerr<<"ImageContainer::init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=TextContainer::init(cfg.textContainerCfg)){
        std::cerr<<"TextContainer::init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=FboContainer::init()){
        std::cerr<<"FboContainer::init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void RsrcMgr::deinit(){
    FboContainer::deinit();
    TextContainer::deinit();
    ImageContainer::deinit();
}

void RsrcMgr::process(){


}