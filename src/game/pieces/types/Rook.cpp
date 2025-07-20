// Corresponding header
#include "game/pieces/types/Rook.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"
#include "game/pieces/types/King.h" // NOT added by Zhivko

extern bool isCastleEnquiryMadeOnce;

int32_t Rook::init(const ChessPieceCfg& cfg){ // Rook::init() method is NOT added by Zhivko
    if(EXIT_SUCCESS!=ChessPiece::init(cfg)){
        std::cerr<<"ChessPiece::init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    _initialColumnPosition=_boardPos.col;
    return EXIT_SUCCESS;
}

std::vector<MoveDirection> Rook::getBoardMoves() const{
    constexpr auto allowedDirs=4;
    constexpr std::array<Defines::Directions,allowedDirs> rookDirs {Defines::UP, 
                                                Defines::RIGHT, Defines::DOWN, Defines::LEFT};
    constexpr auto maxRookMoves=7;
    std::vector<MoveDirection> boardMoves(allowedDirs);

    BoardPos futurePos;
    for(int32_t dirIdx=0;dirIdx<allowedDirs;++dirIdx){
        boardMoves[dirIdx].reserve(maxRookMoves);
        futurePos=_boardPos;
        futurePos=BoardUtils::getAdjacentPos(rookDirs[dirIdx],futurePos);
        while(BoardUtils::isInsideBoard(BoardUtils::getAbsPos(futurePos))){
            boardMoves[dirIdx].push_back(futurePos);
            futurePos=BoardUtils::getAdjacentPos(rookDirs[dirIdx],futurePos);
        }
    }
    return boardMoves;
}

bool Rook::isCastlePossible(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers,
                            const BoardPos& kingBoardPos) const { // Rook::checkForCastle() is NOT added by Zhivko

    if(_isTaken){
        return false;
    }

    const int32_t activePlayerId=ChessPiece::getPlayerId();
    const int32_t opponentPlayerId=BoardUtils::getOpponentId(activePlayerId);

    const int32_t activePlayerNumOfPieces=static_cast<int32_t>(activePlayers[activePlayerId].size());

    const BoardPos rookBoardPos=ChessPiece::getBoardPos();

    int32_t columnMin=rookBoardPos.col;
    int32_t columnMax=kingBoardPos.col; 

    if(columnMin>columnMax){
        const int32_t columnMinCopy=columnMin;
        columnMin=columnMax;
        columnMax=columnMinCopy;
    }

    for(int32_t j=0;j<activePlayerNumOfPieces;++j){
        const BoardPos& currPiecePos=activePlayers[activePlayerId][j]->getBoardPos();

        if(kingBoardPos.row!=currPiecePos.row){
            continue;
        }

        if(currPiecePos.col<=columnMin || currPiecePos.col>=columnMax){
            continue;
        }
    
        return false;
    }

    const int32_t opponentPlayerNumOfPieces=static_cast<int32_t>(activePlayers[opponentPlayerId].size());

    for(int32_t j=0;j<opponentPlayerNumOfPieces;++j){

        const BoardPos opponentPiecePos=activePlayers[opponentPlayerId][j]->getBoardPos();

        if(kingBoardPos.row!=opponentPiecePos.row){
            continue;
        }

        if(opponentPiecePos.col<columnMin || opponentPiecePos.col>columnMax){
            // not possible to have an opponent chess piece located at the position of your king
            continue;
        }
    
        return false;
    }

    for(int32_t j=0;j<opponentPlayerNumOfPieces;++j){
        const std::vector<TileData> enemyPieceMoveTiles =activePlayers[opponentPlayerId][j]->getMoveTiles(activePlayers);
        const int32_t sizeTileData=static_cast<int32_t>(enemyPieceMoveTiles.size());
        for(int32_t k=0;k<sizeTileData;++k){
            if(TileType::GUARD==enemyPieceMoveTiles[k].tileType){
                continue;
            }
            const BoardPos& tileDataPos=enemyPieceMoveTiles[k].boardPos;

            if(tileDataPos.row!=kingBoardPos.row){
                continue;
            }

            if(tileDataPos.col==rookBoardPos.col){
                continue;
            }            

            if(tileDataPos.col<columnMin || tileDataPos.col>columnMax){
                continue;
            }

            if(tileDataPos.col<(columnMax-2) && (tileDataPos.col!=kingBoardPos.col)){
                continue;
            }

            return false;
        }
    }
    return true;    
}

std::vector<TileData> Rook::getMoveTiles(const std::array<ChessPiece::PlayerPieces,
                                            Defines::PLAYERS_COUNT>& activePlayers) const{
    if(_isTaken){
        return std::vector<TileData>();
    }

    const std::vector<MoveDirection> boardMoves=getBoardMoves();

    std::vector<TileData> moveTiles;
    const auto opponentId=BoardUtils::getOpponentId(_playerId);

    TileData tileData;

    for(const MoveDirection& moveDir:boardMoves){
        if(moveDir.empty()){
            continue;
        }
        for(const BoardPos& boardPos:moveDir){
            const TileType tileType=BoardUtils::getTileType(boardPos,activePlayers[_playerId],activePlayers[opponentId]);
            tileData.tileType=tileType;
            tileData.boardPos=boardPos;
            moveTiles.push_back(tileData);
            if(TileType::MOVE!=tileType){
                break;
            }
        }
    }

    if(_isMoved) {
        _isCastlingPossible=false;
        return moveTiles;
    }

    if(isCastleEnquiryMadeOnce) {
        return moveTiles;
    }    

    _isCastlingPossible=false;
    isCastleEnquiryMadeOnce=true;

    for(const std::unique_ptr<ChessPiece>& piece:activePlayers[_playerId]){
        if(PieceType::KING!=piece->getPieceType()){
            continue;
        }

        const ChessPiece* const chessPiecePtr=piece.get();
        const King* const kingPtr=static_cast<const King*>(chessPiecePtr);

        if(kingPtr->isMoved() || kingPtr->isInCheck()){
            break;
        }
        _isCastlingPossible=Rook::isCastlePossible(activePlayers,piece->getBoardPos());
        break;
    }
    isCastleEnquiryMadeOnce=false;
    return moveTiles;
}

bool Rook::getIsCastlingPossible() const{ // Rook::getIsCastlingPossible() is NOT added by Zhivko
    return _isCastlingPossible;
}

bool Rook::isMoved() const { // Rook::isMoved() is NOT added by Zhivko
    return _isMoved;
}

void Rook::setIsTaken(bool isTaken){ // Rook::setIsTaken() is NOT added by Zhivko
    _isTaken=isTaken;
}

bool Rook::getIsTaken() const { // Rook::getIsTaken() is NOT added by Zhivko
    return _isTaken;
}

void Rook::setWhenFirstMoveIsMade() { // Rook::setWhenFirstMoveIsMade() is NOT added by Zhivko
    if(_isMoved){
        return;
    }
    Defines::WHITE_PLAYER_ID==_playerId ?
    _isMoved=_boardPos!=BoardPos(Defines::WHITE_PLAYER_START_PAWN_ROW+1,_initialColumnPosition) :
    _isMoved=_boardPos!=BoardPos(Defines::BLACK_PLAYER_START_PAWN_ROW-1,_initialColumnPosition);
}