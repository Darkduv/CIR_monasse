#include "joueur.h"
#include "plateau.h"
#include "piece.h"
#include <iostream>

Joueur::Joueur(Plateau& p, int col){
    J2=this;
    petit_roque=true;
    grand_roque=true;
    check=false;
    checkmate=false;
    ptr_b=&p;
    color = col;
    boite = new Piece*[8*2];
    int k=0;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            Piece* piece_parcourue = p[Case(i,j)];
            if (piece_parcourue!=nullptr && piece_parcourue->get_color()==col){
                boite[k]=piece_parcourue;
                ++k;
            }
        }
    }

}
Joueur::~Joueur(){
    delete [] boite;
}

void Joueur::affiche() const{
    std::cout << "DEBUT AFFICHE JOUEUR" << std::endl;
    for(int i=0;i<8*2;i++) std::cout << boite[i] << std::endl;
    std::cout <<"FIN AFFICHE JOUEUR" << std::endl;
}

void Joueur::kill_piece(Piece* p){
    for(int i=0;i<8*2;i++){
        if (boite[i]==p) boite[i]=nullptr;
    }
}

int Joueur::get_color(){return color;}
Piece** Joueur::get_boite(){return boite;}
bool Joueur::get_check(){return check;}
bool Joueur::get_checkmate(){return checkmate;}
Plateau* Joueur::get_board(){return ptr_b;}
void Joueur::set_other_player(Joueur* J){J=J2;}
Piece* Joueur::get_my_king(){
    for(int i=0;i<8*2;i++){
        if(boite[i]->get_name()=="roi") return boite[i];
    }
    return nullptr;
}

bool Joueur::bouge(Piece* p, Case c){ // vérifie si la couleur de la pièce est identique à celle du joueur avant de bouger
    if (p!=nullptr && p->get_color()==get_color()){
        // pour pouvoir tester quand même parceque là c'est pénible
        return ptr_b->bouge(p,c);
        if (p->get_name()!="roi" && !can_eat_me(c)){
        return ptr_b->bouge(p,c);
        }
    }
    return false;
}

bool Joueur::can_eat_me(Case c){
    Piece** ptr_boite = J2->get_boite();
    for (int i=0;i<8*2;i++) {
        if (ptr_b->permission_mange(ptr_boite[i],c)) return true;
    }
    return false;
}

