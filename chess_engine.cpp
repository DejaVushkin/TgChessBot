#include "chess_engine.h"
#include <sstream>


using namespace std;


ChessEngine::ChessEngine() {
    board = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };
    whiteTurn = true;

    fileToIndex = {
        {'a', 0}, {'b', 1}, {'c', 2}, {'d', 3},
        {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7}
    };
}

bool ChessEngine::isWithinBounds(int row, int col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool ChessEngine::isOpponentPiece(int row, int col) {
    if (!isWithinBounds(row, col)) return false;
    char piece = board[row][col];
    return (whiteTurn && islower(piece)) || (!whiteTurn && isupper(piece));
}

bool ChessEngine::isKingInCheck(bool forWhite) {
    int kingRow = -1, kingCol = -1;
    char king = forWhite ? 'K' : 'k';

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board[row][col] == king) {
                kingRow = row;
                kingCol = col;
                break;
            }
        }
        if (kingRow != -1) break;
    }

    ChessEngine temp = *this;
    temp.whiteTurn = !forWhite;
    vector<string> opponentMoves = temp.Moves();

    for (const auto& move : opponentMoves) {
        int endRow = 8 - (move[3] - '0');
        int endCol = fileToIndex[move[2]];
        if (endRow == kingRow && endCol == kingCol) {
            return true;
        }
    }
    return false;
}

string ChessEngine::getBoard() {
    ostringstream oss;
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            char piece = board[rank][file];
            oss << piece << ' ';
        }
        oss << '\n';
    }
    return oss.str();
}

void ChessEngine::PawnMoves(int row, int col, vector<string>& moves) {
    int direction = whiteTurn ? -1 : 1;
    int startRow = whiteTurn ? 6 : 1;

    if (isWithinBounds(row + direction, col) && board[row + direction][col] == '.') {
        moves.push_back(string(1, 'a' + col) + to_string(8 - row) +
            string(1, 'a' + col) + to_string(8 - (row + direction)));
        if (row == startRow && board[row + 2 * direction][col] == '.') {
            moves.push_back(string(1, 'a' + col) + to_string(8 - row) +
                string(1, 'a' + col) + to_string(8 - (row + 2 * direction)));
        }
    }

    for (int dcol : {-1, 1}) {
        if (isWithinBounds(row + direction, col + dcol) && isOpponentPiece(row + direction, col + dcol)) {
            moves.push_back(string(1, 'a' + col) + to_string(8 - row) +
                string(1, 'a' + (col + dcol)) + to_string(8 - (row + direction)));
        }
    }
}

void ChessEngine::KnightMoves(int row, int col, vector<string>& moves) {
    int knightMoves[8][2] = { {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1} };

    for (auto& move : knightMoves) {
        int newRow = row + move[0];
        int newCol = col + move[1];
        if (isWithinBounds(newRow, newCol) &&
            (board[newRow][newCol] == '.' || isOpponentPiece(newRow, newCol))) {
            moves.push_back(string(1, 'a' + col) + to_string(8 - row) +
                string(1, 'a' + newCol) + to_string(8 - newRow));
        }
    }
}
void ChessEngine::RookMoves(int row, int col, vector<string>& moves) {
    int directions[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    for (auto& dir : directions) {
        for (int dist = 1; dist < 8; ++dist) {
            int newRow = row + dist * dir[0];
            int newCol = col + dist * dir[1];

            if (!isWithinBounds(newRow, newCol)) break;

            if (board[newRow][newCol] == '.') {
                moves.push_back(string(1, 'a' + col) + to_string(8 - row) +
                    string(1, 'a' + newCol) + to_string(8 - newRow));
            }
            else {
                if (isOpponentPiece(newRow, newCol)) {
                    moves.push_back(string(1, 'a' + col) + to_string(8 - row) +
                        string(1, 'a' + newCol) + to_string(8 - newRow));
                }
                break;
            }
        }
    }
}

void ChessEngine::BishopMoves(int row, int col, vector<string>& moves) {
    int directions[4][2] = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

    for (auto& dir : directions) {
        for (int dist = 1; dist < 8; ++dist) {
            int newRow = row + dist * dir[0];
            int newCol = col + dist * dir[1];

            if (!isWithinBounds(newRow, newCol)) break;

            if (board[newRow][newCol] == '.') {
                moves.push_back(string(1, 'a' + col) + to_string(8 - row) +
                    string(1, 'a' + newCol) + to_string(8 - newRow));
            }
            else {
                if (isOpponentPiece(newRow, newCol)) {
                    moves.push_back(string(1, 'a' + col) + to_string(8 - row) +
                        string(1, 'a' + newCol) + to_string(8 - newRow));
                }
                break;
            }
        }
    }
}

void ChessEngine::QueenMoves(int row, int col, vector<string>& moves) {
    RookMoves(row, col, moves);
    BishopMoves(row, col, moves);
}

void ChessEngine::KingMoves(int row, int col, vector<string>& moves) {
    int directions[8][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

    for (auto& dir : directions) {
        int newRow = row + dir[0];
        int newCol = col + dir[1];

        if (isWithinBounds(newRow, newCol) &&
            (board[newRow][newCol] == '.' || isOpponentPiece(newRow, newCol))) {
            moves.push_back(string(1, 'a' + col) + to_string(8 - row) +
                string(1, 'a' + newCol) + to_string(8 - newRow));
        }
    }
}

void ChessEngine::makeMove(const string& move) {
    int startCol = fileToIndex[move[0]];
    int startRow = 8 - (move[1] - '0');
    int endCol = fileToIndex[move[2]];
    int endRow = 8 - (move[3] - '0');

    board[endRow][endCol] = board[startRow][startCol];
    board[startRow][startCol] = '.';

    whiteTurn = !whiteTurn;
}

string ChessEngine::calculateBestMove() {
    auto moves = Moves();
    if (!moves.empty()) return moves[0];
    return "";
}

vector<string> ChessEngine::Moves() {
    vector<string> moves;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            char piece = board[row][col];
            if ((whiteTurn && isupper(piece)) || (!whiteTurn && islower(piece))) {
                switch (tolower(piece)) {
                case 'p': PawnMoves(row, col, moves); break;
                case 'n': KnightMoves(row, col, moves); break;
                case 'r': RookMoves(row, col, moves); break;
                case 'b': BishopMoves(row, col, moves); break;
                case 'q': QueenMoves(row, col, moves); break;
                case 'k': KingMoves(row, col, moves); break;
                }
            }
        }
    }
    return moves;
}

bool ChessEngine::isCheckmate() {
    vector<string> moves = Moves();
    for (const auto& move : moves) {
        ChessEngine temp = *this;
        temp.makeMove(move);
        if (!temp.isKingInCheck(whiteTurn)) {
            return false;
        }
    }
    return isKingInCheck(whiteTurn);
}
bool ChessEngine::isValidMove(const string& move) {
   
    if (move.length() != 4 || !isalpha(move[0]) || !isdigit(move[1]) ||
        !isalpha(move[2]) || !isdigit(move[3])) {
        return false;
    }

   
    int startCol = fileToIndex[move[0]];
    int startRow = 8 - (move[1] - '0');
    int endCol = fileToIndex[move[2]];
    int endRow = 8 - (move[3] - '0');

    
    if (!isWithinBounds(startRow, startCol) || !isWithinBounds(endRow, endCol)) {
        return false;
    }

    
    vector<string> moves = Moves();

   
    for (const auto& validMove : moves) {
        if (validMove == move) {
            return true;
        }
    }
    return false;
}
