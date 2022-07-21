#include "token.h"

// bool Token::activateCapture() {
//     return captureAbility();
// }

// bool Token::isValidMove(int diceroll, int flexroll, int move) const {
//     return checkValid(diceroll, flexroll, move);
// }

// bool Token::checkValid(int diceroll, int flexroll, int move) const {
//     return diceroll == move;
// }

size_t Token::getPlayerId() const {
    return playerId;
}
size_t Token::getTokenId() const {
    return id;
}

Token::Token(size_t playerId, size_t id, size_t row, size_t col, size_t pathProgress):
    playerId{playerId}, id{id}, position{row, col}, pathProgress{pathProgress} {}

void Token::updatePosition(std::pair<size_t, size_t> newPos) {
    position = newPos;
}

std::pair<size_t, size_t> Token::getPosition() {
    return position;
}

Token::~Token() {}

// Token::Token(size_t playerId): playerId{playerId} {}
