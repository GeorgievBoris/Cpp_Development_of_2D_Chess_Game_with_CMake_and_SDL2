// Corresponding header
#include "game/pieces/types/Pawn.h"
// C system headers
#include <cstdlib>
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"
#include "game/proxies/GameProxy.h"
#include "game/proxies/PieceHandlerProxy.h"

Pawn::Pawn(GameProxy* gameProxy, PieceHandlerProxy* pieceHandlerProxy) :_gameProxy(gameProxy), _pieceHandlerProxy(pieceHandlerProxy) {
    // TO DO !!!
    // IMPORTANT - listen to Lecture Game 3/4 at time 51:00 and write it down in the book !!!

    if(nullptr==gameProxy){
        std::cerr<<"Error, nullptr provided for gameProxy"<<std::endl;
    }

    if(nullptr==pieceHandlerProxy){
        std::cerr<<"Error, nullptr provided for pieceHandlerProxy"<<std::endl;
    }
}

std::vector<TileData> Pawn::getMoveTiles(const std::array<ChessPiece::PlayerPieces,
                                                Defines::PLAYERS_COUNT>& activePieces) const{
    if(_isTaken){
        return std::vector<TileData>();
    }

    int32_t playerId=getPlayerId();
    const bool isAnotherPieceGetMoveTilesCalled=ChessPiece::isGetMoveTilesCalled(activePieces);
    BoardPos kingBoardPos;
    if(!isAnotherPieceGetMoveTilesCalled){
        kingBoardPos=BoardUtils::getKingBoardPos(activePieces[_playerId]);
        _isFncGetMoveTilesCalled=true;
    }      
    if(Defines::WHITE_PLAYER_ID==playerId){
        return getWhiteMoveTiles(activePieces,kingBoardPos,isAnotherPieceGetMoveTilesCalled);
    }
    return getBlackMoveTiles(activePieces,kingBoardPos,isAnotherPieceGetMoveTilesCalled);
}

void Pawn::checkMoveForEnPassant(const BoardPos& boardPos) { // Pawn::checkMoveForEnPassant() is NOT added by Zhivko
    const int32_t boardPosDiff=_boardPos.row-boardPos.row;
    if(2!=abs(boardPosDiff)) {
        return;
    }
    _isPawnTargetedForEnPassant=true;
}

bool Pawn::isPawnTargetedForEnPassant() const { // Pawn::isPawnTargetedForEnPassant() is NOT added by Zhivko
    return _isPawnTargetedForEnPassant;
}

std::unordered_map<Defines::Directions,MoveDirection> Pawn::getWhiteBoardMoves() const {
    constexpr auto allowedDirs=3;
    std::unordered_map<Defines::Directions,MoveDirection> boardMoves(allowedDirs); // remember this when using a map!!!

    BoardPos futurePos=BoardUtils::getAdjacentPos(Defines::UP_LEFT,_boardPos);
    if(BoardUtils::isInsideBoard(futurePos)){
        boardMoves[Defines::UP_LEFT].emplace_back(futurePos);
    }

    futurePos=BoardUtils::getAdjacentPos(Defines::UP_RIGHT,_boardPos);
    if(BoardUtils::isInsideBoard(futurePos)){
        boardMoves[Defines::UP_RIGHT].emplace_back(futurePos);
    }

    futurePos=BoardUtils::getAdjacentPos(Defines::UP,_boardPos);
    if(BoardUtils::isInsideBoard(futurePos)){ // Zhivko has omitted this IF statement
        boardMoves[Defines::UP].emplace_back(futurePos);
    } // Zhivko has omitted this IF statement

    // do the double PAWN move
    if(Defines::WHITE_PLAYER_START_PAWN_ROW==_boardPos.row){
        futurePos=BoardUtils::getAdjacentPos(Defines::UP,futurePos);
        boardMoves[Defines::UP].emplace_back(futurePos);
    }
    return boardMoves;
}

std::unordered_map<Defines::Directions,MoveDirection> Pawn::getBlackBoardMoves() const {
    constexpr auto allowedDirs=3;
    std::unordered_map<Defines::Directions,MoveDirection> boardMoves(allowedDirs);

    BoardPos futurePos=BoardUtils::getAdjacentPos(Defines::DOWN_LEFT,_boardPos);
    if(BoardUtils::isInsideBoard(futurePos)){
        boardMoves[Defines::DOWN_LEFT].emplace_back(futurePos);
    }

    futurePos=BoardUtils::getAdjacentPos(Defines::DOWN_RIGHT,_boardPos);
    if(BoardUtils::isInsideBoard(futurePos)){
        boardMoves[Defines::DOWN_RIGHT].emplace_back(futurePos);
    }

    futurePos=BoardUtils::getAdjacentPos(Defines::DOWN,_boardPos);
    if(BoardUtils::isInsideBoard(futurePos)){ // Zhivko has omitted this IF statement
        boardMoves[Defines::DOWN].emplace_back(futurePos);
    } // Zhivko has omitted this IF statement

    if(Defines::BLACK_PLAYER_START_PAWN_ROW==_boardPos.row){
        futurePos=BoardUtils::getAdjacentPos(Defines::DOWN,futurePos);
        boardMoves[Defines::DOWN].emplace_back(futurePos);
    }
    return boardMoves;
}

std::vector<TileData> Pawn::getWhiteMoveTiles(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePieces, 
                                              const BoardPos& kingBoardPos, const bool isAnotherPieceGetMoveTilesCalled) const {

    const std::unordered_map<Defines::Directions,MoveDirection> boardMoves=getWhiteBoardMoves();
    std::vector<TileData> moveTiles;
    moveTiles.reserve(boardMoves.size());
    const int32_t opponentId=BoardUtils::getOpponentId(_playerId);

    std::unordered_map<Defines::Directions,MoveDirection>::const_iterator it=boardMoves.find(Defines::UP);
    if(boardMoves.end()!=it){
        const MoveDirection& moveDir=it->second;
        for(const BoardPos& pos:moveDir){
            const TileType tileType=BoardUtils::getTileType(pos,activePieces[_playerId],activePieces[opponentId]);
            // if first possible move is TAKE or GUARD - second move will be impossible
            if(TileType::MOVE!=tileType){
                break;
            }

            if(isAnotherPieceGetMoveTilesCalled){
                moveTiles.emplace_back(pos,tileType);
                continue;
            }
            
            if(ChessPiece::isMoveTileValid(pos,kingBoardPos,activePieces)){
                moveTiles.emplace_back(pos,tileType);
            }                
        }
    }

    constexpr auto diagonalMovesCnt=2;
    constexpr std::array<Defines::Directions,diagonalMovesCnt> diagonalMoves {Defines::UP_LEFT, Defines::UP_RIGHT};

    for(const Defines::Directions move:diagonalMoves){
        it=boardMoves.find(move);
        if(boardMoves.end()==it){
            continue;
        }
        const MoveDirection& moveDir=it->second;
        for(const BoardPos& pos:moveDir){
            const TileType tileType=BoardUtils::getTileType(pos, activePieces[_playerId],activePieces[opponentId]);

            if(TileType::GUARD==tileType){
                moveTiles.emplace_back(pos,tileType);
                continue;
            }

            if(isAnotherPieceGetMoveTilesCalled){
                if(TileType::TAKE==tileType){
                    moveTiles.emplace_back(pos,tileType);
                }
                continue;
            }        

            if(TileType::TAKE==tileType){
                if(ChessPiece::isTakeTileValid(pos,kingBoardPos,activePieces)){
                    moveTiles.emplace_back(pos,tileType);
                }
                continue;      
            }

            if(TileType::MOVE==tileType){
                int32_t enemyPawnIndx=INVALID_RSRC_ID;
                if(Pawn::isEnPassantPossible(pos,activePieces[opponentId],enemyPawnIndx)){
                    if(Pawn::areEnPassantTilesValid(pos,kingBoardPos,activePieces,enemyPawnIndx)){
                        moveTiles.emplace_back(pos,tileType);
                        const BoardPos enemyPawnPos=BoardUtils::getAdjacentPos(Defines::DOWN,pos);
                        moveTiles.emplace_back(enemyPawnPos,TileType::TAKE);                        
                    }
                }
            }
        }
    }
    _isFncGetMoveTilesCalled=false;
    return moveTiles;
}

std::vector<TileData> Pawn::getBlackMoveTiles(const std::array<ChessPiece::PlayerPieces, Defines::PLAYERS_COUNT>& activePieces, 
                                              const BoardPos& kingBoardPos,const bool isAnotherPieceGetMoveTilesCalled) const{
    std::unordered_map<Defines::Directions,MoveDirection> boardMoves=getBlackBoardMoves();
    std::vector<TileData> moveTiles;
    moveTiles.reserve(boardMoves.size());
    const int32_t opponentId=BoardUtils::getOpponentId(_playerId);

    std::unordered_map<Defines::Directions,MoveDirection>::const_iterator it=boardMoves.find(Defines::DOWN);
    if(boardMoves.end()!=it){
        const MoveDirection& moveDir=it->second;
        for(const BoardPos& pos:moveDir){
            const TileType tileType=BoardUtils::getTileType(pos,activePieces[_playerId],activePieces[opponentId]);
            // if first possible move is TAKE or GUARD - second move will be impossible
            if(TileType::MOVE!=tileType){
                break;
            }

            if(isAnotherPieceGetMoveTilesCalled){
                moveTiles.emplace_back(pos,tileType);
                continue;
            }
            
            if(ChessPiece::isMoveTileValid(pos,kingBoardPos,activePieces)){
                moveTiles.emplace_back(pos,tileType);
            }
        }
    }

    constexpr auto diagonalMovesCnt=2;
    constexpr std::array<Defines::Directions,diagonalMovesCnt> diagonalMoves {Defines::DOWN_LEFT, Defines::DOWN_RIGHT};

    for(const Defines::Directions move:diagonalMoves){
        it=boardMoves.find(move);
        if(boardMoves.end()==it){
            continue;
        }
        const MoveDirection& moveDir=it->second;
        for(const BoardPos& pos:moveDir){
            const TileType tileType=BoardUtils::getTileType(pos,activePieces[_playerId],activePieces[opponentId]);

            if(TileType::GUARD==tileType){
                moveTiles.emplace_back(pos,tileType);
                continue;
            }

            if(isAnotherPieceGetMoveTilesCalled){
                if(TileType::TAKE==tileType){
                    moveTiles.emplace_back(pos,tileType);
                }
                continue;
            }            

            if(TileType::TAKE==tileType){
                if(ChessPiece::isTakeTileValid(pos,kingBoardPos,activePieces)){
                    moveTiles.emplace_back(pos,tileType);
                }
                continue;      
            }

            if(TileType::MOVE==tileType){
                int32_t enemyPawnIndx=INVALID_RSRC_ID;
                if(Pawn::isEnPassantPossible(pos,activePieces[opponentId],enemyPawnIndx)){
                    if(Pawn::areEnPassantTilesValid(pos,kingBoardPos,activePieces,enemyPawnIndx)){
                        moveTiles.emplace_back(pos,tileType);
                        const BoardPos enemyPawnPos=BoardUtils::getAdjacentPos(Defines::UP,pos);
                        moveTiles.emplace_back(enemyPawnPos,TileType::TAKE);                        
                    }
                }
            }               
        }
    }
    _isFncGetMoveTilesCalled=false;
    return moveTiles;
}

bool Pawn::isEnPassantPossible(const BoardPos& boardPos, const ChessPiece::PlayerPieces& enemyPieces, int32_t& enemyPawnIndx) const{ // Pawn::isEnPassantPossible() method is NOT added by Zhivko
    
    if(_isTaken){
        return false;
    }

    const int32_t currPlayerId = BoardUtils::getOpponentId(enemyPieces.front()->getPlayerId());

    const BoardPos pos = Defines::WHITE_PLAYER_ID==currPlayerId ? 
                            BoardUtils::getAdjacentPos(Defines::DOWN, boardPos) : BoardUtils::getAdjacentPos(Defines::UP, boardPos);

    if(Defines::WHITE_PLAYER_ID==currPlayerId){
        if(Defines::WHITE_PLAYER_EN_PASSANT_ROW!=pos.row){
            return false;
        }
    } else if(Defines::BLACK_PLAYER_ID==currPlayerId){
        if(Defines::BLACK_PLAYER_EN_PASSANT_ROW!=pos.row){
            return false;
        }        
    }

    int32_t indx=-1;
    for(const std::unique_ptr<ChessPiece>& piece:enemyPieces){
        ++indx;
        if(PieceType::PAWN!=piece->getPieceType()){
            continue;
        }

        const ChessPiece* const chessPiecePtr=piece.get();
        const Pawn* const pawnPtr=static_cast<const Pawn*>(chessPiecePtr);

        if(!pawnPtr->isPawnTargetedForEnPassant()){
            continue;
        }

        if(pos!=piece->getBoardPos()){
            continue;
        }
        enemyPawnIndx=indx;
        return true;
    }
    return false;
}

bool Pawn::areEnPassantTilesValid(const BoardPos& boardPos, const BoardPos& kingBoardPos,
                                  const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers,
                                  const int32_t enemyPawnIndx) const {
    const int32_t opponentId=BoardUtils::getOpponentId(_playerId);
    const BoardPos initialBoardPos=activePlayers[opponentId][enemyPawnIndx]->getBoardPos();

    const std::pair<int32_t,int32_t> pawnPair{opponentId,enemyPawnIndx};
    const BoardPos temporaryBoardPos(Point::UNDEFINED.x,Point::UNDEFINED.y);
    _pieceHandlerProxy->changePawnPosIfEnPassant(pawnPair,temporaryBoardPos);

    if(!ChessPiece::isMoveTileValid(boardPos,kingBoardPos,activePlayers)){
        _pieceHandlerProxy->changePawnPosIfEnPassant(pawnPair,initialBoardPos);
        return false;
    }
    _pieceHandlerProxy->changePawnPosIfEnPassant(pawnPair,initialBoardPos);
    return true;
}

void Pawn::checkForPawnPromotion(){ // Pawn::checkForPawnPromotion() is NOT added by Zhivko
    if(Defines::WHITE_PLAYER_ID==_playerId) {
        if(Defines::WHITE_PLAYER_START_END_ROW==_boardPos.row) {
            _gameProxy->onPawnPromotion();
        }
    } else {
        if(Defines::BLACK_PLAYER_START_END_ROW==_boardPos.row) {
            _gameProxy->onPawnPromotion();
        }
    }
}

void Pawn::setIsPawnTargetedForEnPassant(bool isPawnTargetedForEnPassant){ // Pawn::setIsPawnTargetedForEnPassant() is NOT added by Zhivko
    _isPawnTargetedForEnPassant=isPawnTargetedForEnPassant;
}