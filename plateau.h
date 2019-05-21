#pragma once
#include "piece.h"
#include "case.h"
#include "graphics.h"
#include "joueur.h"


class Plateau {
    Piece** plateau; // On met le plateau en privé, si une pièce veut checker qu'elle peut bouger on demande le reseignement au tableau avec une méthode
    Piece* peut_etre_pris_en_passant;
    Joueur* J1;
    Joueur* J2;
public:
    Plateau();
    ~Plateau();
    void affiche() const;
    bool bouge(Piece* p, Case c,int i = -1); // bouge une piece
    int permission_bouge(Piece* p, Case c);
    bool permission_mange(Piece* p, Case c, Piece* ghosted=nullptr);
    Piece* get(Case c) const; // accesseur du plateau
    Piece* get(int i, int j) const;
    Piece* operator[](Case c) const;
    void set(Piece* p, Case c);
    void set_joueur(Joueur* Jo1, Joueur* Jo2);
};
