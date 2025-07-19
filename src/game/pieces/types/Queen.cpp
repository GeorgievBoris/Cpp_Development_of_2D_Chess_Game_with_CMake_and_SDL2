// Corresponding header
#include "game/pieces/types/Queen.h"
// C system headers
// C++ system headers
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"

std::vector<MoveDirection> Queen::getBoardMoves() const {
    constexpr int32_t allowedDirs=8;
    constexpr int32_t allowedMovesPerDir=7;
    std::vector<MoveDirection> boardMoves(allowedDirs);

    constexpr std::array<Defines::Directions,allowedDirs> queenDirs {
        Defines::UP,
        Defines::UP_LEFT,
        Defines::LEFT,
        Defines::DOWN_LEFT,
        Defines::DOWN,
        Defines::DOWN_RIGHT,
        Defines::RIGHT,
        Defines::UP_RIGHT
    };

    BoardPos currPos;

    for(int32_t i=0;i<allowedDirs;++i){
        currPos=_boardPos;
        currPos=BoardUtils::getAdjacentPos(queenDirs[i],currPos);
        boardMoves[i].reserve(allowedMovesPerDir);
        while(BoardUtils::isInsideBoard(currPos)){
            boardMoves[i].push_back(currPos);
            currPos=BoardUtils::getAdjacentPos(queenDirs[i],currPos);
        }
    }
    return boardMoves;
}

std::vector<TileData> Queen::getMoveTiles(const std::array<ChessPiece::PlayerPieces, Defines::PLAYERS_COUNT>& activePlayers) const {

    if(_isTaken){
        return std::vector<TileData>();
    }
        
    constexpr int32_t maxQueenMoves=28;
    std::vector<TileData> moveTiles;
    moveTiles.reserve(maxQueenMoves); // not entirely accurate - try to obtain the actual number of possible moves !!!

    const std::vector<MoveDirection> boardMoves=getBoardMoves();

    const int32_t currActivePlayerId=getPlayerId();
    const int32_t opponentId=BoardUtils::getOpponentId(currActivePlayerId);

    TileType tileType;
    for(const MoveDirection& moveDir:boardMoves){
        if(moveDir.empty()){
            continue;
        }
        for(const BoardPos& pos:moveDir){
            tileType=BoardUtils::getTileType(pos,activePlayers[currActivePlayerId],activePlayers[opponentId]);
            moveTiles.emplace_back(pos,tileType);
            if(TileType::MOVE!=tileType){
                break;
            }
        }
    }
    return moveTiles;
}

void Queen::setIsTaken(bool isTaken){
    _isTaken=isTaken;
}

bool Queen::getIsTaken() const {
    return _isTaken;
}