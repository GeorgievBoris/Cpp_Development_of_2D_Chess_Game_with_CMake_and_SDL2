// Corresponding header
#include "game/pieces/PieceHandler.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "sdl_utils/InputEvent.h"
#include "game/utils/BoardUtils.h"
#include "game/proxies/GameBoardProxy.h"

namespace {
constexpr auto STARTING_PIECE_COUNT=16;
constexpr auto PAWNS_COUNT=8;
constexpr auto WHITE_PLAYER_START_PAWN_ROW=6;
constexpr auto BLACK_PLAYER_START_PAWN_ROW=1;
};

int32_t PieceHandler::init(GameBoardProxy* gameBoardProxy, int32_t whitePiecesRsrcId, int32_t blackPiecesRsrcId){

    if(nullptr==gameBoardProxy){
        std::cerr<<"Error, nullptr provided for gameBoardProxy"<<std::endl;
        return EXIT_FAILURE;
    }

    _gameBoardProxy=gameBoardProxy;

    if(EXIT_SUCCESS!=populateWhitePieces(whitePiecesRsrcId)){
        std::cerr<<"Error, PieceHandler::populateWhitePieces() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=populateBlackPieces(blackPiecesRsrcId)){
        std::cerr<<"Error, PieceHandler::populateBlackPieces() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void PieceHandler::draw() const{
    for(const PlayerPieces& playerPieces:_pieces){
        for(const ChessPiece& piece:playerPieces){
            piece.draw();
        }
    }
}

void PieceHandler::handleEvent(const InputEvent& e){
    // NOTE: write this in the book .... this is called тернарен operator 
    _isPieceGrabbed ? handlePieceGrabbedEvent(e) : handlePieceUngrabbedEvent(e);
}

void PieceHandler::handlePieceGrabbedEvent(const InputEvent& e){
    if(TouchEvent::TOUCH_RELEASE!=e.type){
        return;
    }

    if(!BoardUtils::isInsideBoard(e.pos)){
        return;
    }

    _isPieceGrabbed=false;

    const BoardPos boardPos=BoardUtils::getBoardPos(e.pos);
    _pieces[_selectedPiecePlayerId][_selectedPieceId].setBoardPos(boardPos);

    _gameBoardProxy->onPieceUngrabbed();
}

void PieceHandler::handlePieceUngrabbedEvent(const InputEvent& e){
    if(TouchEvent::TOUCH_RELEASE!=e.type){
        return;
    }

    int32_t currPlayerId=Defines::WHITE_PLAYER_ID;
    for(const PlayerPieces& playerPieces:_pieces){
        int32_t relativePieceId=0;
        for(const ChessPiece& piece:playerPieces){
            if(piece.containsEvent(e)){
                _selectedPieceId=relativePieceId;
                _selectedPiecePlayerId=currPlayerId;
                _isPieceGrabbed=true;
                _gameBoardProxy->onPieceGrabbed(BoardUtils::getBoardPos(e.pos));
                return;
            }
            ++relativePieceId;
        }
        ++currPlayerId;
    }
}

int32_t PieceHandler::populateWhitePieces(int32_t rsrcId){
    PlayerPieces& whites=_pieces[Defines::WHITE_PLAYER_ID];
    whites.resize(STARTING_PIECE_COUNT); // this calls the automatic default ctor (which we have NOT disabled) of "ChessPiece" - so no problem !

    ChessPieceCfg pieceCfg;
    pieceCfg.playerId=Defines::WHITE_PLAYER_ID;
    pieceCfg.rsrcId=rsrcId;
    pieceCfg.boardPos.row=WHITE_PLAYER_START_PAWN_ROW;
    pieceCfg.pieceType=PieceType::PAWN;
    for(int32_t i=0;i<PAWNS_COUNT;++i){
        pieceCfg.boardPos.col=i;
        if(EXIT_SUCCESS!=whites[i].init(pieceCfg)){
            std::cerr<<"whites["<<i<<"].init() failed"<<std::endl;
            return EXIT_FAILURE;
        }
    }

    constexpr int32_t nonPawnCount=PAWNS_COUNT;
    constexpr PieceType nonPawnTypes[nonPawnCount]={PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN, 
                                                    PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK};

    ++pieceCfg.boardPos.row;

    for(int32_t i=nonPawnCount;i<STARTING_PIECE_COUNT;++i){
        pieceCfg.boardPos.col=i-nonPawnCount;
        pieceCfg.pieceType=nonPawnTypes[i-nonPawnCount];
        if(EXIT_SUCCESS!=whites[i].init(pieceCfg)){
            std::cerr<<"whites["<<i<<"].init()"<<std::endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int32_t PieceHandler::populateBlackPieces(int32_t rsrcId){
    PlayerPieces& blacks=_pieces[Defines::BLACK_PLAYER_ID];
    blacks.resize(STARTING_PIECE_COUNT);

    ChessPieceCfg pieceCfg;
    pieceCfg.playerId=Defines::BLACK_PLAYER_ID;
    pieceCfg.rsrcId=rsrcId;
    pieceCfg.boardPos.row=BLACK_PLAYER_START_PAWN_ROW;
    pieceCfg.pieceType=PieceType::PAWN;

    for(int32_t i=0;i<PAWNS_COUNT;++i){
        pieceCfg.boardPos.col=i;
        if(EXIT_SUCCESS!=blacks[i].init(pieceCfg)){
            std::cerr<<"blacks["<<i<<"].init() failed"<<std::endl;
            return EXIT_FAILURE;
        }
    }
    
    constexpr int32_t nonPawnCount=PAWNS_COUNT;
    constexpr PieceType nonPawnTypes[nonPawnCount]={
        PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN,
        PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK
    };

    --pieceCfg.boardPos.row;

    for(int32_t i=nonPawnCount;i<STARTING_PIECE_COUNT;++i){
        pieceCfg.boardPos.col=i-nonPawnCount;
        pieceCfg.pieceType=nonPawnTypes[i-nonPawnCount];
        if(EXIT_SUCCESS!=blacks[i].init(pieceCfg)){
            std::cerr<<"blacks["<<i<<"].init() failed"<<std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}