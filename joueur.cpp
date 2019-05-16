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

bool Joueur::is_checkmate(){};

bool Joueur::bouge(Piece* p, Case c){ // vérifie si la couleur de la pièce est identique à celle du joueur avant de bouger
    if (p!=nullptr && p->get_color()==get_color()){
        Piece* myking = get_my_king();
        int droit_bouge;
        check = can_eat_me(myking->get());
        if (check){
            droit_bouge=ptr_b->permission_bouge(p,c);
            Case old_case;
            switch (droit_bouge) {
            case 0: // on a pas le droit de bouger donc retourne faux
                return false;
            case 1: // on a le droit de bouger sur c sans prendre aucune pièce => on regarde si on peut bloquer l'echec
                old_case = p->get();
                p->bouge(c);
                ptr_b->set(p,c);
                if (can_eat_me(get_my_king()->get())){ // on est oblité de rappeler get_my_king pck on a peut être bougé le roi
                    p->bouge(old_case);
                    ptr_b->set(p,old_case);
                    return false;
                }
                else {
                    p->bouge(old_case);
                    ptr_b->set(p,old_case);
                    return ptr_b->bouge(p,c,droit_bouge);
                }
            case 2: // on a le droit de bouger sur c en prenant une pièce => on regarde si en ayant pris la pièce on est tjrs en echec
                if (can_eat_me(get_my_king()->get(),ptr_b->get(c))) return false;
                else return ptr_b->bouge(p,c,droit_bouge);
            }
            return false;
        }
        else if (p->get_name()=="roi"){ // je bouge le roi et on ne peut pas me manger
            if  (!can_eat_me(c)) return ptr_b->bouge(p,c);
            else return false;
        }
        else {
            droit_bouge=ptr_b->permission_bouge(p,c);
            Case old_case;
            switch (droit_bouge) {
            case 0:
                return false;
            case 1:
                old_case = p->get();
                p->bouge(c);
                ptr_b->set(p,c);
                if (can_eat_me(myking->get())){
                    p->bouge(old_case);
                    ptr_b->set(p,old_case);
                    return false;
                }
                else {
                    p->bouge(old_case);
                    ptr_b->set(p,old_case);
                    return ptr_b->bouge(p,c,droit_bouge);
                }
            case 2:
                if (can_eat_me(myking->get(),ptr_b->get(c))) return false;
                else return ptr_b->bouge(p,c,droit_bouge);
            }
            return ptr_b->bouge(p,c);
        }
    }
    return false;
}

bool Joueur::can_eat_me(Case c,Piece* p){
    Piece** ptr_boite = J2->get_boite();
    for (int i=0;i<8*2;i++) {
        if (ptr_boite[i]!=p && ptr_b->permission_mange(ptr_boite[i],c)) return true;
    }
    return false;
}

