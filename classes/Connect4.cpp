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

    for (int y = 0; y < _gameOptions.rowY; y++) {
        
        for (int x = 0; x < _gameOptions.rowX; x++) {

            ChessSquare* at_square = _grid->getSquare(x, y);

            if (at_square->bit() == nullptr) {
                continue;
            }

            // std::cout << "found a piece" << std::endl;

            ChessSquare* squareToCheck = at_square;

            // check right

            for (int i = 0; i < 3; i++) {
                
                // safety checks

                if(squareToCheck->getColumn() + 1 >= _gameOptions.rowX) {
                    break;
                }

                // grabs a ref to the new chess square
                squareToCheck = _grid->getE(squareToCheck->getColumn(), squareToCheck->getRow());

                // checks if it has a player
                if(squareToCheck->bit() == nullptr) {
                    break;
                }
                
                // break if at any point the elements don't match up
                if (at_square->bit()->getOwner() != squareToCheck->bit()->getOwner()) {
                    break;
                }

                // Win check
                if(i == 2) {
                    return at_square->bit()->getOwner();
                }

            }

            // Checks below

            squareToCheck = at_square;

            for (int i = 0; i < 3; i++) {
                
                // safety checks

                if(squareToCheck->getRow() + 1 >= _gameOptions.rowY) {
                    break;
                }

                // grabs a ref to the new chess square
                squareToCheck = _grid->getS(squareToCheck->getColumn(), squareToCheck->getRow());

                // checks if it has a player
                if(squareToCheck->bit() == nullptr) {
                    break;
                }
                
                // break if at any point the elements don't match up
                if (at_square->bit()->getOwner() != squareToCheck->bit()->getOwner()) {
                    break;
                }

                // Win check
                if(i == 2) {
                    return at_square->bit()->getOwner();
                }

            }

            // Checks up right

            squareToCheck = at_square;

            for (int i = 0; i < 3; i++) {
                
                // safety checks

                if(squareToCheck->getColumn() + 1 >= _gameOptions.rowX || squareToCheck->getRow() - 1 < 0) {
                    break;
                }

                // grabs a ref to the new chess square
                squareToCheck = _grid->getFR(squareToCheck->getColumn(), squareToCheck->getRow());

                // checks if it has a player
                if(squareToCheck->bit() == nullptr) {
                    break;
                }
                
                // break if at any point the elements don't match up
                if (at_square->bit()->getOwner() != squareToCheck->bit()->getOwner()) {
                    break;
                }

                // Win check
                if(i == 2) {
                    return at_square->bit()->getOwner();
                }

            }

            // Checks down right

            squareToCheck = at_square;

            for (int i = 0; i < 3; i++) {
                
                // safety checks

                if(squareToCheck->getColumn() + 1 >= _gameOptions.rowX || squareToCheck->getRow() + 1 >= _gameOptions.rowY) {
                    break;
                }

                // grabs a ref to the new chess square
                squareToCheck = _grid->getBR(squareToCheck->getColumn(), squareToCheck->getRow());

                // checks if it has a player
                if(squareToCheck->bit() == nullptr) {
                    break;
                }
                
                // break if at any point the elements don't match up
                if (at_square->bit()->getOwner() != squareToCheck->bit()->getOwner()) {
                    break;
                }

                // Win check
                if(i == 2) {
                    return at_square->bit()->getOwner();
                }

            }

        }

    }

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