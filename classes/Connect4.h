#pragma once
#include "Game.h"

// NOTE: If Square class needs modifications to support colored squares for checkerboard pattern,
// add a method like setColor(ImVec4 color) to Square class

class Connect4 : public Game
{
public:
    Connect4();
    ~Connect4();

    // Required virtual methods from Game base class
    void        setUpBoard() override;
    Player*     checkForWinner() override;
    bool        checkForDraw() override;
    std::string initialStateString() override;
    std::string stateString() override;
    void        setStateString(const std::string &s) override;
    bool        actionForEmptyHolder(BitHolder &holder) override;
    bool        canBitMoveFrom(Bit &bit, BitHolder &src) override;
    bool        canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;
    void        stopGame() override;
    void        bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;

    // AI methods
    void        updateAI() override;
    bool        gameHasAI() override { return false; } // Set to true when AI is implemented
    Grid* getGrid() override { return _grid; }

private:
    // Constants for piece types
    static const int EMPTY = 0;
    static const int RED_PIECE = 1;
    static const int YELLOW_PIECE = 3;


    // Player constants
    static const int RED_PLAYER = 0;
    static const int YELLOW_PLAYER = 1;

    // Helper methods
    Bit*        createPiece(int pieceType);
    int         getPieceType(const Bit& bit) const;
    bool        isValidMove(int srcX, int srcY, int dstX, int dstY, Player* player) const;
    void        getBoardPosition(BitHolder &holder, int &x, int &y) const;
    bool        isValidSquare(int x, int y) const;

    // Board representation
    Grid*        _grid;

    // Game state
    
};