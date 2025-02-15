// Corresponding header
#include "game/pieces/PieceHandlerPopulator.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/pieces/types/UnfinishedPiece.h"
#include "game/pieces/types/Rook.h"
#include "game/pieces/types/Pawn.h"

namespace {
constexpr auto STARTING_PIECE_COUNT=16;
};

static std::unique_ptr<ChessPiece> createPiece(PieceType pieceType){
    // this is called a factory method -> it follows the factory design pattern
    switch(pieceType){
    case PieceType::ROOK:
        return std::make_unique<Rook>();
    case PieceType::PAWN:
        return std::make_unique<Pawn>();
    case PieceType::BISHOP:
    case PieceType::KING:
    case PieceType::KNIGHT:
    case PieceType::QUEEN:
        return std::make_unique<UnfinishedPiece>();
    default:
        std::cerr<<"Error, received unknown PieceType: "<<static_cast<int32_t>(pieceType)<<std::endl;
        break;
    }
    return nullptr;
}

static int32_t populateWhitePieces(int32_t rsrcId, int32_t unfinishedPieceFontId, ChessPiece::PlayerPieces& whites){
    whites.reserve(STARTING_PIECE_COUNT); // this calls the automatic default ctor (which we have NOT disabled) of "ChessPiece" - so no problem !

    ChessPieceCfg pieceCfg;
    pieceCfg.playerId=Defines::WHITE_PLAYER_ID;
    pieceCfg.rsrcId=rsrcId;
    pieceCfg.boardPos.row=Defines::WHITE_PLAYER_START_PAWN_ROW;
    pieceCfg.pieceType=PieceType::PAWN;
    pieceCfg.unfinishedPieceFontId=unfinishedPieceFontId;
    for(int32_t i=0;i<Defines::PAWNS_COUNT;++i){
        pieceCfg.boardPos.col=i;
        whites.push_back(createPiece(pieceCfg.pieceType));
        if(EXIT_SUCCESS!=whites[i]->init(pieceCfg)){
            std::cerr<<"whites["<<i<<"].init() failed"<<std::endl;
            return EXIT_FAILURE;
        }
    }

    constexpr int32_t nonPawnCount=Defines::PAWNS_COUNT;
    constexpr PieceType nonPawnTypes[nonPawnCount]={PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN, 
                                                    PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK};

    ++pieceCfg.boardPos.row;

    for(int32_t i=nonPawnCount;i<STARTING_PIECE_COUNT;++i){
        pieceCfg.boardPos.col=i-nonPawnCount;
        pieceCfg.pieceType=nonPawnTypes[i-nonPawnCount];
        whites.push_back(createPiece(pieceCfg.pieceType));
        if(EXIT_SUCCESS!=whites[i]->init(pieceCfg)){
            std::cerr<<"whites["<<i<<"].init()"<<std::endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

static int32_t populateBlackPieces(int32_t rsrcId, int32_t unfinishedPieceFontId, ChessPiece::PlayerPieces& blacks){
    blacks.reserve(STARTING_PIECE_COUNT);

    ChessPieceCfg pieceCfg;
    pieceCfg.playerId=Defines::BLACK_PLAYER_ID;
    pieceCfg.rsrcId=rsrcId;
    pieceCfg.boardPos.row=Defines::BLACK_PLAYER_START_PAWN_ROW;
    pieceCfg.pieceType=PieceType::PAWN;
    pieceCfg.unfinishedPieceFontId=unfinishedPieceFontId;

    for(int32_t i=0;i<Defines::PAWNS_COUNT;++i){
        pieceCfg.boardPos.col=i;
        blacks.push_back(createPiece(pieceCfg.pieceType));
        if(EXIT_SUCCESS!=blacks[i]->init(pieceCfg)){
            std::cerr<<"blacks["<<i<<"].init() failed"<<std::endl;
            return EXIT_FAILURE;
        }
    }
    
    constexpr int32_t nonPawnCount=Defines::PAWNS_COUNT;
    constexpr PieceType nonPawnTypes[nonPawnCount]={
        PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN,
        PieceType::KING, PieceType::BISHOP, PieceType::KNIGHT, PieceType::ROOK
    };

    --pieceCfg.boardPos.row;

    for(int32_t i=nonPawnCount;i<STARTING_PIECE_COUNT;++i){
        pieceCfg.boardPos.col=i-nonPawnCount;
        pieceCfg.pieceType=nonPawnTypes[i-nonPawnCount];
        blacks.push_back(createPiece(pieceCfg.pieceType));
        if(EXIT_SUCCESS!=blacks[i]->init(pieceCfg)){
            std::cerr<<"blacks["<<i<<"].init() failed"<<std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int32_t PieceHandlerPopulator::populatePieceHandler(int32_t whitePiecesRsrcId, int32_t blackPiecesRsrcId,
                                        int32_t unfinishedPieceFontId,
                                        std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& outPieces){

    ChessPiece::PlayerPieces& whites=outPieces[Defines::WHITE_PLAYER_ID];
    ChessPiece::PlayerPieces& blacks=outPieces[Defines::BLACK_PLAYER_ID];

    if(EXIT_SUCCESS!=populateWhitePieces(whitePiecesRsrcId, unfinishedPieceFontId, whites)){
        std::cerr<<"Error, PieceHandler::populateWhitePieces() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=populateBlackPieces(blackPiecesRsrcId, unfinishedPieceFontId, blacks)){
        std::cerr<<"Error, PieceHandler::populateBlackPieces() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}