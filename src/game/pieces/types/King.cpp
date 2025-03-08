// Corresponding header
#include "game/pieces/types/King.h"
// C system headers
// C++ system headers
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"


std::vector<MoveDirection> King::getBoardMoves() const {
    // Note: Probably can use std::unordered_map instead of std::vector ?
    constexpr int32_t kingMaxDirs=8;
    std::vector<MoveDirection> boardMoves(kingMaxDirs);

    constexpr std::array<Defines::Directions,kingMaxDirs> kingDirs {
        Defines::UP,
        Defines::UP_RIGHT,
        Defines::RIGHT,
        Defines::DOWN_RIGHT,
        Defines::DOWN,
        Defines::DOWN_LEFT,
        Defines::LEFT,
        Defines::UP_LEFT
    };

    BoardPos futurePos;
    for(int32_t i=0;i<kingMaxDirs;++i){
        futurePos=_boardPos;
        futurePos=BoardUtils::getAdjacentPos(kingDirs[i],futurePos);
        if(BoardUtils::isInsideBoard(futurePos)){
            boardMoves[i].push_back(futurePos);
        }
    }
    return boardMoves;
}

std::vector<TileData> King::getMoveTiles(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers) const {
    const std::vector<MoveDirection> boardMoves=getBoardMoves();
    const size_t maxKingMoves=boardMoves.size();
    std::vector<TileData> moveTiles;
    moveTiles.reserve(maxKingMoves);

    const int32_t activePlayerId=getPlayerId();
    const int32_t opponentId=BoardUtils::getOpponentId(activePlayerId);

    TileType tileType;
    for(const MoveDirection& moveDir:boardMoves){
        if(moveDir.empty()){
            continue;
        }

        const BoardPos& pos=moveDir.front();
        tileType=BoardUtils::getTileType(pos,activePlayers[activePlayerId],activePlayers[opponentId]);
        moveTiles.emplace_back(pos,tileType);
    }
    return moveTiles;
}

