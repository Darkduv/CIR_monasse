#pragma once

class Piece;
class Plateau;
class Case;

class Joueur {
    int color;
    Piece** boite;
    Plateau* ptr_b;
    bool check;
    bool checkmate;
public:
    Joueur (Plateau& p, int col);
    void affiche() const;
    void kill_piece(Piece* p);
    bool bouge(Piece* p,Case c);
    Plateau* get_board();
    int get_color();
    bool get_checkmate();
    bool get_check();
    ~Joueur();
};
