#pragma once
#include "piece.h"
#include "case.h"
#include "graphics.h"
#include "player.h"


class Board {
    Piece** work_board; // The work_board is set private :
    // if a piece has to check whether if it can move or not, we ask to the board with a method
    Piece* can_be_en_passant_captured;
    Player* P1;
    Player* P2;
public:
    Board();
    ~Board();
    void display() const;
    bool move(Piece* p, Case c, int i = -1); // move a piece
    int permission_move(Piece* p, Case c);
    bool permission_capture(Piece* p, Case c, Piece* ghosted=nullptr);
    Piece* get(Case c) const; // work_board accessor
    Piece* get(int i, int j) const;
    Piece* operator[](Case c) const;
    void set(Piece* p, Case c);
    void set_player(Player* Jo1, Player* Jo2);
};
