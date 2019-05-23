#pragma once

class Piece;
class Board;
class Case;

class Player {
    Player* J2;
    int color;
    Piece** boite;
    Board* ptr_b;
    bool check;
    bool checkmate;
    bool petit_roque, grand_roque;
public:
    Player (Board& p, int col);
    void affiche() const;
    void kill_piece(Piece* p);
    void set_piece(Piece* p);
    bool bouge(Piece* p,Case c);
    Board* get_board();
    int get_color() const;
    Piece** get_boite();
    Piece* get_my_king();
    bool get_checkmate();
    bool get_check();
    bool get_petit_roque();
    bool get_grand_roque();
    Piece* can_eat_me(Case c, Piece* ghosted=nullptr);
    bool is_checkmate();
    void set_other_player(Player* J2);
    void set_petit_roque(bool value);
    void set_grand_roque(bool value);
    ~Player();
};
