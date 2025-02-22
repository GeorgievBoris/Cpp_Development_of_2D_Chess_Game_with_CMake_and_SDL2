#ifndef INCLUDE_GAME_PIECES_TYPES_CHESSPIECE_H_
#define INCLUDE_GAME_PIECES_TYPES_CHESSPIECE_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <array>
#include <memory>
#include <vector>
// Third-party headers
// Own headers
#include "manager_utils/drawing/Image.h"
#include "game/utils/BoardPos.h"
#include "game/defines/ChessDefines.h"
#include "game/defines/ChessStructs.h"
// Forward Declarations
class InputEvent;
class Fbo;

struct ChessPieceCfg{
    BoardPos boardPos{}; // calling the default ctor of "boardPos"
    int32_t playerId{};
    int32_t rsrcId=INVALID_RSRC_ID;
    PieceType pieceType=PieceType::UNKNOWN;
    int32_t unfinishedPieceFontId; // not entirely correct to add it here, but we do so, in order to save time 
};

class ChessPiece{ // NOTE: ChessPiece is a composition !!!
public:
    virtual ~ChessPiece()=default;
    // since we will very often access the chess figures, this access must happen fast...
    // ... therefore use random_access_iterator -> std::vector...Furthermore,...
    // ... we use std::vector<> when the number/count of the elements inside it is dynamic (i.e. the count reduces/increases)    
    using PlayerPieces=std::vector<std::unique_ptr<ChessPiece>>;

    // Note: write this down...
    // Zhivko: "smart pointers are used mostly where you do not need to care/think about ..."
    // "... the lifetime of the objects"

    virtual int32_t init(const ChessPieceCfg& cfg);
    virtual void drawOnFbo(Fbo& fbo) const;
    virtual void draw() const;
    virtual void setBoardPos(const BoardPos& boardPos);
    virtual std::vector<TileData> getMoveTiles(const std::array<PlayerPieces, Defines::PLAYERS_COUNT>& activePlayers) const=0; // a pure-virtual method
    
    bool containsEvent(const InputEvent& e) const;
    BoardPos getBoardPos() const;
    int32_t getPlayerId() const;
    void setWidgetFlip(WidgetFlip flipType);
protected:
    Image _pieceImg;
    BoardPos _boardPos;
    int32_t _playerId;
    PieceType _pieceType;
};


#endif // INCLUDE_GAME_PIECES_TYPES_CHESSPIECE_H_