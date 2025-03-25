// Corresponding header
#include "game/pieces/types/Pawn.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"
#include "game/proxies/GameProxy.h"

Pawn::Pawn(GameProxy* gameProxy) :_gameProxy(gameProxy) {
    // TO DO !!!
    // IMPORTANT - listen to Lecture Game 3/4 at time 51:00 and write it down in the book !!!

    if(nullptr==gameProxy){
        std::cerr<<"Error, nullptr provided for gameProxy"<<std::endl;
    }
}

void Pawn::setBoardPos(const BoardPos& boardPos) {

    ChessPiece::setBoardPos(boardPos);

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

std::vector<TileData> Pawn::getMoveTiles(const std::array<ChessPiece::PlayerPieces,
                                                Defines::PLAYERS_COUNT>& activePieces) const{
    int32_t playerId=getPlayerId();
    if(Defines::WHITE_PLAYER_ID==playerId){
        return getWhiteMoveTiles(activePieces);
    }
    return getBlackMoveTiles(activePieces);
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
    // if(BoardUtils::isInsideBoard(futurePos)){ // Zhivko has omitted the IF statement
    boardMoves[Defines::UP].emplace_back(futurePos);
    // } // Zhivko has omitted the IF statement

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
    // if(BoardUtils::isInsideBoard(futurePos)) // Zhivko has omitted this IF statement
    boardMoves[Defines::DOWN].emplace_back(futurePos);
    // } // Zhivko has omitted this IF statement

    if(Defines::BLACK_PLAYER_START_PAWN_ROW==_boardPos.row){
        futurePos=BoardUtils::getAdjacentPos(Defines::DOWN,futurePos);
        boardMoves[Defines::DOWN].emplace_back(futurePos);
    }

    return boardMoves;
}

std::vector<TileData> Pawn::getWhiteMoveTiles(const std::array<ChessPiece::PlayerPieces,
                            Defines::PLAYERS_COUNT>& activePieces) const {

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
            moveTiles.emplace_back(pos,tileType);
        }
    }
    constexpr auto diagonalMovesCnt=2;
    constexpr std::array<Defines::Directions,diagonalMovesCnt> diagonalMoves {Defines::UP_LEFT, Defines::UP_RIGHT};

    for(const Defines::Directions move:diagonalMoves){
        it=boardMoves.find(move);
        if(boardMoves.end()!=it){
            for(const BoardPos& pos:it->second){
                const TileType tileType=BoardUtils::getTileType(pos, activePieces[_playerId],activePieces[opponentId]);
        
                if(TileType::MOVE!=tileType){
                    moveTiles.emplace_back(pos,tileType);
                } else if(Pawn::isEnPassantValid(pos,activePieces[opponentId])){
                    // this "else if()" part is added by me for the EnPassant check
                    moveTiles.emplace_back(pos,tileType);
                    const BoardPos enemyPawnPos=BoardUtils::getAdjacentPos(Defines::DOWN,pos);
                    moveTiles.emplace_back(enemyPawnPos,TileType::TAKE);
                }
            }
        }
    }

    return moveTiles;
}

std::vector<TileData> Pawn::getBlackMoveTiles(const std::array<ChessPiece::PlayerPieces,
                            Defines::PLAYERS_COUNT>& activePiece) const{
    std::unordered_map<Defines::Directions,MoveDirection> boardMoves=getBlackBoardMoves();
    std::vector<TileData> moveTiles;
    moveTiles.reserve(boardMoves.size());
    const int32_t opponentId=BoardUtils::getOpponentId(_playerId);

    std::unordered_map<Defines::Directions,MoveDirection>::const_iterator it=boardMoves.find(Defines::DOWN);
    if(boardMoves.end()!=it){
        for(const BoardPos& pos:it->second){
            const TileType tileType=BoardUtils::getTileType(pos,activePiece[_playerId],activePiece[opponentId]);
            // if first possible move is TAKE or GUARD - second move will be impossible
            if(TileType::MOVE!=tileType){
                break;
            }
            moveTiles.emplace_back(pos,tileType);
        }
    }

    constexpr auto diagonalMovesCnt=2;
    constexpr std::array<Defines::Directions,diagonalMovesCnt> diagonalMoves {Defines::DOWN_LEFT, Defines::DOWN_RIGHT};

    for(const Defines::Directions move:diagonalMoves){
        it=boardMoves.find(move);
        if(boardMoves.end()!=it){
            for(const BoardPos& pos:it->second){
                const TileType tileType=BoardUtils::getTileType(pos,activePiece[_playerId],activePiece[opponentId]);
                if(TileType::MOVE!=tileType){
                    moveTiles.emplace_back(pos,tileType);
                } else if(Pawn::isEnPassantValid(pos,activePiece[opponentId])){
                    // this "else if()" part is added by me for the EnPassant check
                    moveTiles.emplace_back(pos,tileType);
                    const BoardPos enemyPawnPos=BoardUtils::getAdjacentPos(Defines::UP,pos);
                    moveTiles.emplace_back(enemyPawnPos,TileType::TAKE);
                }                
            }
        }
    }

    return moveTiles;
}

bool Pawn::isEnPassantValid(const BoardPos& boardPos, const ChessPiece::PlayerPieces& enemyPieces) const{ // Pawn::isEnPassantValid() method is NOT added by Zhivko
    
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

    for(const std::unique_ptr<ChessPiece>& piece:enemyPieces){
        if(PieceType::PAWN!=piece->getPieceType()){
            continue;
        }     

        if(!piece->isPieceTheLastMovedPiece()){
            continue;
        }

        if(pos!=piece->getBoardPos()){
            continue;
        }

        if(!piece->isPieceFirstMoveNow()){
            continue;
        }

        return true;
    }
    return false;
}