// Corresponding header
#include "game/animations/WinnerAnimator.h"
// C system headers
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cmath>
// C++ system headers
#include <iostream>
#include <algorithm>
// Third-party headers
// Own headers
#include "utils/drawing/Color.h"
#include "game/defines/ChessDefines.h"
#include "game/proxies/PieceHandlerProxy.h"
#include "game/utils/BoardUtils.h"


int32_t WinnerAnimator::init(PieceHandlerProxy* pieceHandlerProxy, const std::function<void()>& showStartScreenCb,int32_t nextAnimTimerId,
                                int32_t endAnimTimerId, int32_t starRsrcId, int32_t fireworksRsrcId, int32_t medalRsrcId, int32_t fontId, int32_t windowWidth, int32_t windowHeight){
    if(nullptr==pieceHandlerProxy){
        std::cerr<<"Error, nullptr received for PieceHandlerProxy"<<std::endl;
        return EXIT_FAILURE;
    }

    if(INVALID_RSRC_ID==starRsrcId){
        std::cerr<<"Error, invalid resource Id provided"<<std::endl;
        return EXIT_FAILURE;
    }

    if(INVALID_RSRC_ID==fireworksRsrcId){
        std::cerr<<"Error, invalid resource Id provided"<<std::endl;
        return EXIT_FAILURE;
    }

    if(INVALID_RSRC_ID==medalRsrcId){
        std::cerr<<"Error, invalid resource Id provided"<<std::endl;
        return EXIT_FAILURE;
    }

    if(INVALID_RSRC_ID==nextAnimTimerId){
        std::cerr<<"Error, invalid timer Id provided"<<std::endl;
        return EXIT_FAILURE;
    }

    if(INVALID_RSRC_ID==endAnimTimerId){
        std::cerr<<"Error, invalid timerId provided"<<std::endl;
        return EXIT_FAILURE;
    }

    _showStartScreenCb=showStartScreenCb;
    _pieceHandlerProxy=pieceHandlerProxy;
    _starRsrcId=starRsrcId;
    _fireworksRsrcId=fireworksRsrcId;
    _medalRsrcId=medalRsrcId;
    _nextAnimTimerId=nextAnimTimerId;
    _endAnimTimerId=endAnimTimerId;

    _yRange.resize((windowHeight-100)/NumGenerator::_DELTA);
    std::generate(_yRange.begin(),_yRange.end(),NumGenerator());
    _yRangeIter=_yRange.begin();
    _onWinText.create("0",fontId,Colors::WHITE,Point(880,*_yRangeIter));
    // _onWinText.create("0",fontId,Colors::WHITE,Point(880,500)); // use if WinnerAnimator::rotateText() is enabled
    _onWinText.hide();
    _windowFrame=Rectangle(0,0,windowWidth,windowHeight);
    return EXIT_SUCCESS;
}

void WinnerAnimator::draw() const{
    _onWinText.draw();

    for(const Image& image:_medals){
        image.draw();
    }

    std::list<std::pair<CurveTypes,std::unique_ptr<Image>>>::const_iterator iterStar=_stars.begin();
    for(;iterStar!=_stars.end();++iterStar){
        iterStar->second->draw();
        // (*iter).second->draw();
    }
    std::list<std::pair<std::pair<bool,Point>,std::unique_ptr<Image>>>::const_iterator iterFireworks=_fireworks.begin();
    for(;iterFireworks!=_fireworks.end();++iterFireworks){
        iterFireworks->second->draw();
    }
}

void WinnerAnimator::activate(int32_t playerId){
    if(_isActive){
        std::cerr<<"Error, trying to active WinnerAnimator which is already active!"<<std::endl;
        return;
    }
    Defines::WHITE_PLAYER_ID == playerId ? _onWinText.setText("White pieces win the game!") : _onWinText.setText("Black pieces win the game!");
    TimerClient::startTimer(75,_nextAnimTimerId,TimerType::PULSE);
    TimerClient::startTimer(60000,_endAnimTimerId,TimerType::ONESHOT);
    _isActive=(TimerClient::isActiveTimerId(_nextAnimTimerId) && TimerClient::isActiveTimerId(_endAnimTimerId));
    _onWinText.show();
    WinnerAnimator::createMedals();

    // // use below parameters if WinnerAnimator::rotateText() is enabled
    // _textWidth=_onWinText.getWidth();
    // _textHeight=_onWinText.getHeight();
    // _textCounter=30;
    // _onWinText.setHeight(_textHeight/50);
    // _onWinText.setWidth(_textWidth/50);
    // _onWinText.setRotationCenter({_onWinText.getWidth()/2,_onWinText.getHeight()/2});
}

void WinnerAnimator::deactivate(){
    if(!_isActive){
        return;
    }
    
    TimerClient::stopTimer(_nextAnimTimerId);
    _isActive=TimerClient::isActiveTimerId(_nextAnimTimerId);
    _stars.clear();
    _medals.clear();
    _fireworks.clear();
    _fireworksPosCounter.clear();
    _onWinText.setText("0");
    _onWinText.hide();
}

void WinnerAnimator::onTimeout(const int32_t timerId){
    if(timerId==_nextAnimTimerId){
        WinnerAnimator::rotate();
        WinnerAnimator::moveText();
        // WinnerAnimator::rotateText();

        WinnerAnimator::createStars();
        WinnerAnimator::updateStars();

        WinnerAnimator::createFireworks();
        WinnerAnimator::updateFireworks();
        return;
    }

    if(timerId==_endAnimTimerId){
        _showStartScreenCb();
        return;
    }
    std::cerr<<"Error, WinnerAnimator::onTimeout() received unsupported timerId: "<<timerId<<std::endl;
}

bool WinnerAnimator::isActive() const{
    return _isActive;
}

void WinnerAnimator::restart(){
    _currRotAngle=0.0;
    _deltaRotAngle=1.0;
    _isClockwiseRotation=true;
    _moveUp=false;
    _yRangeIter=_yRange.begin();
    _onWinText.setPosition(880,*_yRangeIter);
}

void WinnerAnimator::rotate(){

    if(10.0==_currRotAngle){
        _deltaRotAngle=-1.0;
    }

    if(-10.0==_currRotAngle){
        _deltaRotAngle=1.0;
    }

    _currRotAngle+=_deltaRotAngle;
    
    _pieceHandlerProxy->rotateWinnerPieces(_deltaRotAngle);

    // obtain the current rotation angle of an arbitrary medal (i.e the first one)...
    // the remaining chess pieces feature the same current rotation angle
    const double currAngle=_medals.front().getRotationAngle();
    const size_t medalsSize=_medals.size();
    for(size_t i=0;i<medalsSize;++i){
        _medals[i].setRotationAngle(currAngle+_deltaRotAngle);
    }

}

void WinnerAnimator::moveText(){

    if(!_moveUp){
        ++_yRangeIter;
    }

    if(_yRange.begin()==_yRangeIter){
        _moveUp=false;
        ++_yRangeIter;
    }      

    if(_yRange.end()==_yRangeIter){
        _moveUp=true;
        --_yRangeIter;
    }

    if(_moveUp){
        --_yRangeIter;
    }

    _onWinText.setPosition(Point(880,*_yRangeIter));
}

// void WinnerAnimator::rotateText(){
//     const int32_t currWidth=_onWinText.getWidth();
//     const int32_t currHeight=_onWinText.getHeight();
//     const double currRotAngle=_onWinText.getRotationAngle();

//     if(currWidth<_textWidth){
//         _textWidth!=_textHeight ? 
//         (_textWidth>_textHeight ? _onWinText.setWidth(currWidth+static_cast<int32_t>(ceil(5.0*static_cast<double>(_textWidth/_textHeight)))) : 
//                                   _onWinText.setWidth(currWidth+5)) : (_onWinText.setWidth(currWidth+5), _onWinText.setHeight(currHeight+5)) ;
//     }

//     if(currHeight<_textHeight){
//         _textWidth!=_textHeight ? 
//         (_textWidth>_textHeight ? _onWinText.setHeight(currHeight+5) : 
//                                   _onWinText.setHeight(currHeight+static_cast<int32_t>(ceil(5.0*static_cast<double>(_textHeight/_textWidth))))) : 
//                                   (_onWinText.setWidth(currWidth+5), _onWinText.setHeight(currHeight+5)) ;
//     }
 
//     if(currWidth<_textWidth || currHeight<_textHeight){
//         _onWinText.setRotationCenter({_onWinText.getWidth()/2,_onWinText.getHeight()/2});
//         360.0<=currRotAngle ? _onWinText.setRotationAngle(currRotAngle-360.0+10.0) : _onWinText.setRotationAngle(currRotAngle+10.0);
//         return;
//     }

//     if(360.0!=currRotAngle){
//         _onWinText.setRotationAngle(currRotAngle+10.0);
//         return;
//     }

//     if(--_textCounter){
//         return;
//     }

//     _textCounter=30;

//     _onWinText.setHeight(_textHeight/50);
//     _onWinText.setWidth(_textWidth/50);
//     _onWinText.setRotationCenter({_onWinText.getWidth()/2,_onWinText.getHeight()/2});
//     _onWinText.setRotationAngle(0.0);

// }

void WinnerAnimator::createStars(){
    static uint32_t executionsStars=0;
    ++executionsStars;
    srand(static_cast<uint32_t>(executionsStars*time(nullptr)));

    const int32_t numNew=1+rand()%2;

    for(int32_t i=0;i<numNew;++i){
        const size_t perChance=rand()%101;
        if(90>perChance){
            continue;
        }
        const int32_t type=rand()%static_cast<int32_t>(CurveTypes::COUNT);
        const CurveTypes curveType=static_cast<CurveTypes>(type);

        std::unique_ptr<Image> unqPtrStar=std::make_unique<Image>();
        unqPtrStar->create(_starRsrcId,WinnerAnimator::calcStarPos(curveType));
        unqPtrStar->setWidth(unqPtrStar->getWidth()/6);
        unqPtrStar->setHeight(unqPtrStar->getHeight()/6);
        unqPtrStar->setRotationCenter(Point(unqPtrStar->getWidth()/2,unqPtrStar->getHeight()/2));
        _stars.push_back({curveType,std::move(unqPtrStar)});
    }
}

void WinnerAnimator::updateStars(){
    std::list<std::pair<CurveTypes,std::unique_ptr<Image>>>::iterator iter=_stars.begin();

    for(;iter!=_stars.end();++iter){
        std::unique_ptr<Image>& unqPtrRef=iter->second;
        const Point absPos=unqPtrRef->getPosition();
        if(!_windowFrame.isPointInside(absPos)){
            iter=_stars.erase(iter);
            continue;
        }
        unqPtrRef->setPosition(WinnerAnimator::calcStarPos(iter->first,absPos.x));

        0==static_cast<uint8_t>(iter->first)%2 ? unqPtrRef->rotateRight(10) : unqPtrRef->rotateLeft(10);
    }

}

Point WinnerAnimator::calcStarPos(const CurveTypes type, const int32_t absX){
    switch(type){
    case CurveTypes::LEFT_TO_RIGHT_DIAGONAL:
    {
        constexpr long double A= (double)0;
        constexpr long double B=(double)(229)/962500;
        constexpr long double C=(double)(3669)/19250;
        constexpr long double D=300.0;

        if(-1==absX){
            constexpr double xPos=0.0;
            return Point(static_cast<int32_t>(xPos),static_cast<int32_t>(A*pow(xPos,3)+B*pow(xPos,2)+C*xPos+D));
        }
        const double xPos=static_cast<double>(absX)+50.0;
        return Point(static_cast<int32_t>(xPos),static_cast<int32_t>(A*pow(xPos,3)+B*pow(xPos,2)+C*xPos+D));
    }

    case CurveTypes::RIGHT_TO_LEFT_DIAGONAL:
    {
        constexpr long double A=(double)0;
        constexpr long double B=(double)(-1517)/2433750;
        constexpr long double C=(double)(-3128)/243375;
        constexpr long double D=990.0;

        if(-1==absX){
            constexpr double xPos=1250.0;
            return Point(static_cast<int32_t>(xPos),static_cast<int32_t>(A*pow(xPos,3)+B*pow(xPos,2)+C*xPos+D));
        }
        const double xPos=static_cast<double>(absX)-50.0;
        return Point(static_cast<int32_t>(xPos),static_cast<int32_t>(A*pow(xPos,3)+B*pow(xPos,2)+C*xPos+D));        
    }

    case CurveTypes::TOP_TO_BOTTOM:
    {
        constexpr long double A=(double)0;
        constexpr long double B=(double)(457)/277550;
        constexpr long double C=(double)(-53617)/27755;
        constexpr long double D=770.0;

        if(-1==absX){
            constexpr double xPos=0.0;
            return Point(static_cast<int32_t>(xPos),static_cast<int32_t>(A*pow(xPos,3)+B*pow(xPos,2)+C*xPos+D));
        }
        const double xPos=static_cast<double>(absX)+50.0;
        return Point(static_cast<int32_t>(xPos),static_cast<int32_t>(A*pow(xPos,3)+B*pow(xPos,2)+C*xPos+D));
    }  
    default:
        std::cerr<<"Error, received unknown CurveType"<<std::endl;
        break;       
    }
    return Point::UNDEFINED;
}

void WinnerAnimator::createFireworks(){
    const int32_t numNew=1+rand()%2;

    for(int32_t i=0;i<numNew;++i){
        const size_t perChance=rand()%101;
        if(90>perChance){
            continue;
        }

        std::unique_ptr<Image> unqPtr=std::make_unique<Image>();
        const uint32_t locator=rand()%4;
        switch(locator)
        {
        case 0:
            unqPtr->create(_fireworksRsrcId,Point(0,(200+rand()%(_windowFrame.h-199))));
            break;
        case 1:
            unqPtr->create(_fireworksRsrcId,Point(rand()%(_windowFrame.w-199),_windowFrame.h));
            break;
        case 2:
            unqPtr->create(_fireworksRsrcId,Point(200+rand()%(_windowFrame.w-199),_windowFrame.h));
            break;
        case 3:
            unqPtr->create(_fireworksRsrcId,Point(_windowFrame.w,(200+rand()%(_windowFrame.h-199))));
            break;            
        default:
            std::cerr<<"Error, unknown firework position to be created."<<std::endl;
        }

        const uint32_t randNum=25+rand()%11;
        _fireworksPosCounter.emplace_back(randNum,randNum);
    
        if(1<locator){
            unqPtr->setFlipType(WidgetFlip::HORIZONTAL);
            _fireworks.emplace_back(std::pair<bool,Point>{true,unqPtr->getPosition()},std::move(unqPtr));
        } else {
            _fireworks.emplace_back(std::pair<bool,Point>{false,unqPtr->getPosition()},std::move(unqPtr));  
        }
    }

}

void WinnerAnimator::updateFireworks(){
    std::list<std::pair<std::pair<bool,Point>,std::unique_ptr<Image>>>::iterator iterFireworks=_fireworks.begin();
    std::list<std::pair<uint32_t,uint32_t>>::iterator iterFireworksMoveCount=_fireworksPosCounter.begin();
    
    while(_fireworks.end()!=iterFireworks){
        std::unique_ptr<Image>& fireworkRef=iterFireworks->second;

        if(fireworkRef->getMaxFrame()-1==fireworkRef->getFrame()){
            iterFireworks=_fireworks.erase(iterFireworks);
            iterFireworksMoveCount=_fireworksPosCounter.erase(iterFireworksMoveCount);
            continue;
        }

        if(0<(iterFireworksMoveCount->first)){
            if(iterFireworksMoveCount->second!=iterFireworksMoveCount->first){
                iterFireworks->first.first ? (iterFireworks->first.second.x-=3,iterFireworks->first.second.y-=9):
                                             (iterFireworks->first.second.x+=3,iterFireworks->first.second.y-=9); 
            }
            WinnerAnimator::calcFireworkPos(iterFireworks->first.second,fireworkRef);

            --(iterFireworksMoveCount->first);
            ++iterFireworks; ++iterFireworksMoveCount;
            continue;
        }

        fireworkRef->setNextFrame();

        WinnerAnimator::calcFireworkPos(iterFireworks->first.second,fireworkRef);

        ++iterFireworks; ++iterFireworksMoveCount;
    }
}

void WinnerAnimator::calcFireworkPos(const Point& pos, std::unique_ptr<Image>& fireworkRef){
    const int32_t width=fireworkRef->getWidth()/2;
    const int32_t height=fireworkRef->getHeight()/2;
    fireworkRef->setPosition({pos.x-width,pos.y-height});
}

void WinnerAnimator::createMedals(){
    const ChessPiece::PlayerPieces& pieces=_pieceHandlerProxy->getWinnerPieces();
    const size_t piecesCount=pieces.size();
    _medals.reserve(piecesCount);
    for (uint32_t i=0;i<piecesCount;++i){
        _medals.emplace_back();
        Image& currMedal=_medals[i];
        const Point piecePos=BoardUtils::getAbsPos(pieces[i]->getBoardPos());
        currMedal.create(_medalRsrcId,piecePos);
        currMedal.setHeight(currMedal.getHeight()/20);
        currMedal.setWidth(currMedal.getWidth()/20);
        currMedal.setRotationCenter(Point(currMedal.getWidth()/2,currMedal.getHeight()/2));
        const Point centeredPos=BoardUtils::getCenterPos(currMedal.getWidth(),currMedal.getHeight());
        currMedal.setPosition(Point(piecePos.x+centeredPos.x,piecePos.y+centeredPos.y));
    }
}