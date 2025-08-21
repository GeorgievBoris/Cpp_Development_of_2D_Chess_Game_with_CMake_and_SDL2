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
};

class ChessPiece{ // NOTE: ChessPiece is a composition !!!
public:
    virtual ~ChessPiece()=default;
    
    // since we will very often access the chess figures, this access must happen fast...
    // ... therefore use random_access_iterator -> std::vector...Furthermore,...
    // ... we use std::vector<> when the number/count of the elements inside it is dynamic (i.e. the count reduces/increases)    
    
    using PlayerPieces=std::vector<std::unique_ptr<ChessPiece>>; // originally used by Zhivko
    // typedef std::vector<std::unique_ptr<ChessPiece>> PlayerPieces; // an alternative to "using" - NOT added by Zhivko

    // Note: write this down...
    // Zhivko: "smart pointers are used mostly where you do not need to care/think about ..."
    // "... the lifetime of the objects"

    virtual int32_t init(const ChessPieceCfg& cfg);
    virtual void drawOnFbo(Fbo& fbo) const;
    virtual void draw() const;
    virtual void setBoardPos(const BoardPos& boardPos);
    virtual std::vector<TileData> getMoveTiles(const std::array<PlayerPieces, Defines::PLAYERS_COUNT>& activePlayers) const=0; // a pure-virtual method
    virtual void setIsTaken(bool isTaken); // ChessPiece::setIsTaken() is NOT added by Zhivko
    virtual bool getIsTaken() const; // ChessPiece::getIsTaken() is NOT added by Zhivko 

    bool containsEvent(const InputEvent& e) const;
    BoardPos getBoardPos() const;
    int32_t getPlayerId() const;
    int32_t getRsrcId() const;
    void setWidgetFlip(WidgetFlip flipType);
    PieceType getPieceType()const; // ChessPiece::getPieceType() is NOT added by Zhivko
    void setRotationAngle(double angle); // ChessPiece::setRotationAngle() is NOT added by Zhivko
    double getRotationAngle() const; // ChessPiece::getRotationAngle() is NOT added by Zhivko

    void activateAlphaModulation(); // ChessPiece::activateAlphaModulation() is NOT added by Zhivko
    void setOpacity(int32_t opacity); // ChessPiece::setOpacity() is NOT added by Zhivko
    int32_t getOpacity() const; // ChessPiece::getOpacity() is NOT added by Zhivko
    void deactivateAlphaModulation(); // ChessPiece::deactivateAlphaModulation() is NOT added by Zhivko
protected:
    bool isGetMoveTilesCalled(const std::array<PlayerPieces, Defines::PLAYERS_COUNT>& activePlayers) const; // NOT added by Zhivko
    bool isMoveTileValid(const BoardPos& boardPos, const BoardPos& kingBoardPos, const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers) const;
    bool isTakeTileValid(const BoardPos& boardPos, const BoardPos& kingBoardPos, const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers) const;    
    Image _pieceImg;
    mutable BoardPos _boardPos; // "mutable" is NOT added by Zhivko
    int32_t _playerId;
    PieceType _pieceType;
    bool _isTaken=false; // NOT added by Zhivko
    mutable bool _isFncGetMoveTilesCalled=false; // NOT added by Zhivko
};


#endif // INCLUDE_GAME_PIECES_TYPES_CHESSPIECE_H_