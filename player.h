#pragma once

class Piece;
class Board;
class Case;

class Player {
    Player* J2;
    int color;
    Piece** box;
    Board* ptr_b;
    bool check;
    bool checkmate;
    bool king_castling, queen_castling;
public:
    Player (Board& p, int col);
    void display() const;
    void kill_piece(Piece* p);
    void set_piece(Piece* p);
    bool move(Piece* p, Case c);
    Board* get_board();
    int get_color() const;
    Piece** get_box();
    Piece* get_my_king();
    bool get_checkmate();
    bool get_check();
    bool get_king_castling();
    bool get_queen_castling();
    Piece* can_eat_me(Case c, Piece* ghosted=nullptr);
    static bool is_checkmate();
    void set_other_player(Player* J2);
    void set_king_castling(bool value);
    void set_queen_castling(bool value);
    ~Player();
};
