// Corresponding header
#include "game/logScreen/LogScreen.h"
// C system headers
#include <cstdlib>
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "utils/drawing/Color.h"
#include "game/proxies/PieceHandlerProxy.h"
#include "game/proxies/GameProxy.h"
#include "game/utils/BoardUtils.h"
#include "manager_utils/drawing/Fbo.h"
#include "sdl_utils/InputEvent.h"

size_t LogScreen::_counter{};
size_t LogScreen::_counterMissedWhitesTurns{};
size_t LogScreen::_counterMissedBlacksTurns{};

int32_t LogScreen::init(PieceHandlerProxy* pieceHandlerProxy,GameProxy* gameProxy, const int32_t fontId, const int32_t rsrcId){
    if(INVALID_RSRC_ID==fontId){
        std::cerr<<"Error, received invalid fondId\n";
        return EXIT_FAILURE;
    }

    if(nullptr==pieceHandlerProxy){
        std::cerr<<"Error, received nullptr for pieceHandlerProxy\n";
        return EXIT_FAILURE;
    }

    if(nullptr==gameProxy){
        std::cerr<<"Error, received nullptr for gameProxy\n";
        return EXIT_FAILURE;
    }
    _pieceHandlerProxy=pieceHandlerProxy;
    _gameProxy=gameProxy;
    _fontId=fontId;

    // const Point bckgAbsPos(0,90);
    const Point titleAbsPos(10,10);
    _logScreenTitle.first.create("(Press 'L' to hide log screen)",_fontId,Colors::RED,{titleAbsPos.x,titleAbsPos.y-50});
    _logScreenTitle.second.create("Last 10 moved pieces:",_fontId,Colors::RED,titleAbsPos);
    _logScreenBckg.create(rsrcId,Point::ZERO);
    _separator.create(" ",fontId,Colors::RED,titleAbsPos);
    _logScreenNote.create("Press 'L' to show log screen",_fontId,Colors::RED,{1025,80});
    _logScreenNote.hide();

    return EXIT_SUCCESS;
}

void LogScreen::deinit(){
    
    if(_movedPieces.empty()){
        return;
    }
    _movedPieces.clear();
    _isLogScreenVisible=true;
    _gameProxy->showLogFbo(_isLogScreenVisible);
    _logScreenNote.hide();
    _counter=0;
}

void LogScreen::draw() const{
    _logScreenNote.draw();
}

void LogScreen::update(int32_t playerId){

    const std::pair<PieceType,std::pair<BoardPos,BoardPos>> pair=_pieceHandlerProxy->getTypeAndPosOfLastMovedPiece();
    const PieceType pieceType=pair.first;

    if(PieceType::UNKNOWN==pieceType){
        Defines::WHITE_PLAYER_ID==playerId ? ++_counterMissedWhitesTurns : ++_counterMissedBlacksTurns;
        return;
    }

    size_t logSize=_movedPieces.size();

    if(logSizeLimit==logSize){
        _movedPieces.pop_front();
    } 

    const BoardPos initBoardPos=BoardUtils::getInvertedBoardPos(pair.second.first,WidgetFlip::HORIZONTAL);
    const BoardPos newBoardPos=BoardUtils::getInvertedBoardPos(pair.second.second,WidgetFlip::HORIZONTAL);    
    
    _movedPieces.emplace_back();

    const char colStart=char(97+initBoardPos.col); const char rowStart=char(49+initBoardPos.row);
    const char colEnd=char(97+newBoardPos.col); const char rowEnd=char(49+newBoardPos.row);
    const char textNewPosition[]={colStart,rowStart,'-','>',colEnd,rowEnd,'\0'};
    
    std::string textNewPiece;
    switch (pieceType){
    case PieceType::PAWN:
        textNewPiece=". Pawn ";
        break;
    case PieceType::ROOK:
        textNewPiece=". Rook ";
        break;
    case PieceType::KNIGHT:
        textNewPiece=". Knight ";
        break; 
    case PieceType::BISHOP:
        textNewPiece=". Bishop ";
        break; 
    case PieceType::QUEEN:
        textNewPiece=". Queen ";
        break; 
    case PieceType::KING:
        textNewPiece=". King ";
        break;
    default:
        textNewPiece="Error!";
        break;
    }

    ++_counter;
    std::string finalText=std::to_string(_counter);

    const Point titleAbsPos=_logScreenTitle.second.getPosition();
    const Point textPieceAbsPos(titleAbsPos.x+20,titleAbsPos.y+60);
    finalText.append(textNewPiece);
    finalText.append(textNewPosition);

    
    if(0<_counterMissedBlacksTurns || 0<_counterMissedWhitesTurns){
        std::string textMissedTurns;
        std::string textBlacks=std::to_string(_counterMissedBlacksTurns);
        textBlacks.append("b )");
        textMissedTurns=std::to_string(_counterMissedWhitesTurns);
        textMissedTurns.append("w, ");
        textMissedTurns.append(textBlacks);
        textMissedTurns.insert(0," ( ");
        finalText.append(textMissedTurns);
        _counterMissedBlacksTurns=0;
        _counterMissedWhitesTurns=0;
    }

    Defines::WHITE_PLAYER_ID==playerId ? 
    _movedPieces.back().create(finalText,_fontId,Colors::WHITE,textPieceAbsPos) :
    _movedPieces.back().create(finalText,_fontId,Colors::BLACK,textPieceAbsPos) ;

    std::list<Text>::iterator iter=_movedPieces.begin();
    std::list<Text>::const_iterator iterStop=--_movedPieces.cend();
  

    for(;iter!=iterStop;++iter){
        const Point oldAbsPos=iter->getPosition();
        iter->setPosition(oldAbsPos.x,oldAbsPos.y+65);
    }    
}

void LogScreen::drawOnFbo(Fbo& fbo) {
    if(_movedPieces.empty()){
        return;
    }
    fbo.addWidget(_logScreenBckg);
    fbo.addWidget(_logScreenTitle.first);
    fbo.addWidget(_logScreenTitle.second);

    const Point titleAbsPos=_logScreenTitle.second.getPosition();
    _separator.setPosition(titleAbsPos.x,titleAbsPos.y+30);
    _separator.setText("-----------------");
    fbo.addWidget(_separator);

    std::list<Text>::const_iterator iter=_movedPieces.cbegin();
    std::list<Text>::const_iterator iterEnd=_movedPieces.cend();
    for(;iter!=iterEnd;++iter){
        fbo.addWidget(*iter);
        const Point textAbsPos=iter->getPosition();
        _separator.setPosition({textAbsPos.x,textAbsPos.y+35});
        _separator.setText("--------------");
        fbo.addWidget(_separator);
    }
}

void LogScreen::handleEvent(const InputEvent& e){
    if(TouchEvent::KEYBOARD_PRESS!=e.type){
        return;
    }
    
    if(Keyboard::KEY_L!=e.key){
        return;
    }

    _isLogScreenVisible=!_isLogScreenVisible;
    _isLogScreenVisible ? _logScreenNote.hide() : _logScreenNote.show();
    _gameProxy->showLogFbo(_isLogScreenVisible);
    return;
}