#include "Connect4.h"
#include <utility>

Connect4::Connect4() : Game() {
    _grid = new Grid(6, 7);
}

Connect4::~Connect4() {
    delete _grid;
}

void Connect4::setUpBoard() {
    setNumberOfPlayers(2);
    _gameOptions.rowX = 6;
    _gameOptions.rowY = 7;

    // Initialize all squares
    _grid->initializeSquares(80, "square.png");

    startGame();
}

Bit* Connect4::createPiece(int pieceType) {
    Bit* bit = new Bit();
    bool isRed = (pieceType == RED_PIECE);
    bit->LoadTextureFromFile(isRed ? "red.png" : "yellow.png");
    bit->setOwner(getPlayerAt(isRed ? RED_PLAYER : YELLOW_PLAYER));
    bit->setGameTag(pieceType);
    
    return bit;
}

bool Connect4::actionForEmptyHolder(BitHolder &holder) {
    
    ChessSquare* some_square = (ChessSquare*) &holder;
    
    BitHolder* top_holder = (BitHolder*)_grid->getSquare(some_square->getColumn(), 0);
    some_square = (ChessSquare*)top_holder;

    if (top_holder->bit()) {
        return false;
    }

    Bit *bit = createPiece(getCurrentPlayer()->playerNumber() == 0 ? HUMAN_PLAYER : AI_PLAYER);
    if (bit) {

        ImVec2 current = top_holder->getPosition();
        bit->setPosition(current);

        std::pair<int, int> coords = {some_square->getColumn(), some_square->getRow()};

        std::cout << "Dropped at height " << some_square->getRow() << std::endl;

        for (int y = coords.second; y < _gameOptions.rowY - 1; y++) {

            BitHolder* some_holder = (BitHolder*)_grid->getSquare(coords.first, coords.second + 1);

            if (some_holder->bit()) {
                break;
            }
            else {
                coords.second++;
            }

        }

        bit->moveTo(_grid->getSquare(coords.first, coords.second)->getPosition());
        _grid->getSquare(coords.first, coords.second)->setBit(bit);

        endTurn();
        return true;
    }
    
    return false;
}

bool Connect4::canBitMoveFrom(Bit &bit, BitHolder &src) {
    return false;
}

bool Connect4::canBitMoveFromTo(Bit& bit, BitHolder& src, BitHolder& dst) {
    return false;
}

void Connect4::bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) {
    
}

Player* Connect4::checkForWinner() {
    return nullptr;
}

bool Connect4::checkForDraw() {
    return false;
}

void Connect4::stopGame() {
    _grid->forEachSquare([](ChessSquare* square, int x, int y) {
        square->destroyBit();
    });
}

std::string Connect4::initialStateString() {
    return "-------------------------------------------------";
}

std::string Connect4::stateString() {
    return _grid->getStateString();
}

void Connect4::setStateString(const std::string &s) {
    if (s.length() != 32) return;

    _grid->setStateString(s);

    // Recreate pieces from state
    size_t index = 0;
    _grid->forEachEnabledSquare([&](ChessSquare* square, int x, int y) {
        if (index < s.length()) {
            int pieceType = s[index++] - '0';
            if (pieceType != 0) {
                Bit* piece = createPiece(pieceType);
                
            }
        }
    });
}

void Connect4::updateAI() {}