#ifndef INCLUDE_GAME_PIECES_TYPES_CHESSPIECE_H_
#define INCLUDE_GAME_PIECES_TYPES_CHESSPIECE_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/drawing/Image.h"
#include "game/utils/BoardPos.h"
#include "game/defines/ChessDefines.h"
// Forward Declarations
class InputEvent;

struct ChessPieceCfg{
    BoardPos boardPos{}; // calling the default ctor of "boardPos"
    int32_t playerId{};
    int32_t rsrcId=INVALID_RSRC_ID;
    PieceType pieceType=PieceType::UNKNOWN;
};

class ChessPiece{
public:
    int32_t init(const ChessPieceCfg& cfg);
    void draw() const;
    bool containsEvent(const InputEvent& e) const;

    void setBoardPos(const BoardPos& boardPos);
    BoardPos getBoardPos() const;
    int32_t getPlayerId() const;
private:
    Image _pieceImg;
    BoardPos _boardPos;
    int32_t _playerId;
    PieceType _pieceType;
};


#endif // INCLUDE_GAME_PIECES_TYPES_CHESSPIECE_H_