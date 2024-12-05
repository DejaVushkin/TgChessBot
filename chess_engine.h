#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

#include <vector>
#include <string>
#include <map>
#include <unordered_map>

using namespace std;

class ChessEngine {
private:
    vector<vector<char>> board; 
    bool whiteTurn;                       
    map<char, int> fileToIndex;      
    unordered_map<char, int> pieceValues; 
   
    bool isWithinBounds(int row, int col);
    bool isOpponentPiece(int row, int col);
    bool isKingInCheck(bool forWhite);
    void PawnMoves(int row, int col, vector<string>& moves);
    void KnightMoves(int row, int col, vector<string>& moves);
    void BishopMoves(int row, int col, vector<string>& moves);
    void RookMoves(int row, int col, vector<string>& moves);
    void QueenMoves(int row, int col, vector<string>& moves);
    void KingMoves(int row, int col, vector<string>& moves);
   

   
    

    

public:
    ChessEngine();                            
    string getBoard();                   
    bool isValidMove(const string&);     
    string calculateBestMove();          
    bool isCheckmate();                       
    vector<string> Moves(); 
    
    void makeMove(const string& move); 
  
};

#endif
