// Corresponding header
#include "game/pieces/types/Knight.h"
// C system headers
// C++ system headers
#include <unordered_map>
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"

std::vector<MoveDirection> Knight::getBoardMoves() const {
    constexpr int32_t allowedDirs=4;
    constexpr int32_t pairSize=2;
    std::vector<MoveDirection> boardMoves(allowedDirs);

    std::array<std::pair<Defines::Directions,std::array<Defines::Directions,pairSize>>,allowedDirs> knightDirs {
        std::make_pair(Defines::UP, std::array<Defines::Directions,pairSize> {Defines::LEFT, Defines::RIGHT}),
        std::make_pair(Defines::RIGHT, std::array<Defines::Directions,pairSize>{Defines::UP, Defines::DOWN}),
        std::make_pair(Defines::DOWN, std::array<Defines::Directions,pairSize>{Defines::LEFT, Defines::RIGHT}),
        std::make_pair(Defines::LEFT, std::array<Defines::Directions,pairSize>{Defines::DOWN, Defines::UP})
    };

    BoardPos currPos;
    bool isInsideBoard=true;
    
    for(int32_t i=0;i<allowedDirs;++i){
        currPos=_boardPos;
        isInsideBoard=true;
        
        for(int32_t j=0;j<pairSize;++j){
            currPos=BoardUtils::getAdjacentPos(knightDirs[i].first,currPos);
            if(!BoardUtils::isInsideBoard(currPos)){
                isInsideBoard=false;
                break;
            }
        }

        if(!isInsideBoard){
            continue;
        }

        const std::array<BoardPos,pairSize> arrayCurrPos {currPos, currPos};
        boardMoves[i].reserve(pairSize);
        
        for(int32_t j=0;j<pairSize;++j){
            currPos=BoardUtils::getAdjacentPos(knightDirs[i].second[j],arrayCurrPos[j]);
            if(BoardUtils::isInsideBoard(currPos)){
                boardMoves[i].push_back(currPos);
            }             
        }         
    }

    return boardMoves;
}

std::vector<TileData> Knight::getMoveTiles(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers) const {
    constexpr int32_t maxKnightMoves=8;

    std::vector<TileData> moveTiles;
    moveTiles.reserve(maxKnightMoves);

    const std::vector<MoveDirection> boardMoves=getBoardMoves();

    int32_t activePlayerId=getPlayerId();
    int32_t opponentId=BoardUtils::getOpponentId(activePlayerId);

    TileType tileType;
    for(const MoveDirection& movDir:boardMoves){
        if(movDir.empty()){
            continue;
        }
        for(const BoardPos& pos:movDir){
            tileType=BoardUtils::getTileType(pos,activePlayers[activePlayerId],activePlayers[opponentId]);
            moveTiles.emplace_back(pos,tileType);
        }
    }
    return moveTiles;
}