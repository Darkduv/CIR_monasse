#pragma once

class Piece;
class Plateau;
class Case;

class Joueur {
    Joueur* J2;
    int color;
    Piece** boite;
    Plateau* ptr_b;
    bool check;
    bool checkmate;
    bool petit_roque, grand_roque;
public:
    Joueur (Plateau& p, int col);
    void affiche() const;
    void kill_piece(Piece* p);
    void set_piece(Piece* p);
    bool bouge(Piece* p,Case c);
    Plateau* get_board();
    int get_color();
    Piece** get_boite();
    Piece* get_my_king();
    bool get_checkmate();
    bool get_check();
    bool get_petit_roque();
    bool get_grand_roque();
    Piece* can_eat_me(Case c, Piece* ghosted=nullptr);
    bool is_checkmate();
    void set_other_player(Joueur* J2);
    void set_petit_roque(bool value);
    void set_grand_roque(bool value);
    ~Joueur();
};
