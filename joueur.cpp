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
    switch (color) {
    case 0:
        std::cout << "Couleur du joueur NOIRE" << std::endl;
        break;
    case 1:
        std::cout << "Couleur du joueur BLANCHE" << std::endl;
        break;
    }

    for(int i=0;i<8*2;i++){
        if (boite[i]==nullptr) std::cout << "attention nul" << std::endl;
        else std::cout << boite[i]->get_name() << " : " << boite[i]->get().get(0) << boite[i]->get().get(1) << std::endl;
    }
}

void Joueur::kill_piece(Piece* p){
    for(int i=0;i<8*2;i++){
        if (boite[i]==p){
            std::cout<<"travail terminé"<< std::endl;
            boite[i]=nullptr;}
    }
}

void Joueur::set_piece(Piece* p){
    for(int i=0;i<8*2;i++){
        if (boite[i]==nullptr){
            std::cout<<"travail terminé"<< std::endl;
            boite[i]=p;}
    }
}
void Joueur::set_petit_roque(bool value){petit_roque=value;}
void Joueur::set_grand_roque(bool value){grand_roque=value;}
int Joueur::get_color(){return color;}
Piece** Joueur::get_boite(){return boite;}
bool Joueur::get_check(){return check;}
bool Joueur::get_checkmate(){return checkmate;}
bool Joueur::get_petit_roque(){return petit_roque;}
bool Joueur::get_grand_roque(){return grand_roque;}
Plateau* Joueur::get_board(){return ptr_b;}
void Joueur::set_other_player(Joueur* J){J2=J;}

Piece* Joueur::get_my_king(){
    for(int i=0;i<8*2;i++){
        if(boite[i]!=nullptr && boite[i]->get_name()=="roi") return boite[i];
    }
    return nullptr;
}

bool Joueur::is_checkmate(){};

bool Joueur::bouge(Piece* p, Case c){
    if (p!=nullptr && p->get_color()==get_color()){
        Piece* eater = can_eat_me(get_my_king()->get());
        if ( eater != nullptr){
            std::cout<<"attention, le roi est en echec" << std::endl;
            if (p->get_name()=="roi"){
                ptr_b->set(nullptr, p->get());
                if (can_eat_me(c) == nullptr){
                    ptr_b->set(p, p->get());
                    return ptr_b->bouge(p,c); // si on peut echapper a lechec en se déplaçant on le fait
                }
                else {
                    ptr_b->set(p, p->get());
                    return false;
                }
            }
            else if (ptr_b->get(c)==nullptr){ // on couvre l'échec
                ptr_b->set(p,c);
                ptr_b->set(nullptr, p->get());
                if (can_eat_me(get_my_king()->get())==nullptr){
                    ptr_b->set(nullptr,c);
                    ptr_b->set(p, p->get());
                    return ptr_b->bouge(p,c);
                }
                else {
                    ptr_b->set(nullptr,c);
                    ptr_b->set(p, p->get());
                    return false;
                }
            }
            else { // on mange le checker
                if (eater->get() == c){
                    ptr_b->set(p,c);
                    ptr_b->set(nullptr, p->get());
                    if (can_eat_me(get_my_king()->get(), eater)==nullptr){
                        ptr_b->set(eater,c);
                        ptr_b->set(p, p->get());
                        return ptr_b->bouge(p,c);
                    }
                    else {
                        ptr_b->set(eater,c);
                        ptr_b->set(p, p->get());
                        return false;
                    }
                }

            }
        }
        else if (p->get_name()=="roi"){
            if (!can_eat_me(c)) return ptr_b->bouge(p,c);
            else return false;
        }
        else {
            std::cout << "toto" << std::endl;
            std::cout << can_eat_me(get_my_king()->get(), p) << std::endl;
            if (!can_eat_me(get_my_king()->get(), p)){
                return ptr_b->bouge(p,c);
            }
            else {
                return false;
            }
        }
    }
    else return false;
}

/*
bool Joueur::bouge(Piece* p, Case c){ // vérifie si la couleur de la pièce est identique à celle du joueur avant de bouger
    if (p!=nullptr && p->get_color()==get_color()){
        Piece* myking = get_my_king();
        int droit_bouge;
        check = can_eat_me(myking->get());
        if (check){
            std::cout << "attention echec !" << std::endl;
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
                else {
                    J2->kill_piece(ptr_b->get(c));
                    return ptr_b->bouge(p,c,droit_bouge);
                }
            }
            return false;
        }
        else if (p->get_name()=="roi"){ // je bouge le roi et on ne peut pas me manger
            std::cout << "je bouge le roi" << std::endl;

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
                std::cout << "attention on prend un pion" << std::endl;
                if (can_eat_me(myking->get(),ptr_b->get(c))) return false;
                else {
                    J2->kill_piece(ptr_b->get(c));
                    return ptr_b->bouge(p,c,droit_bouge);
                }
            case 3:
                if (ptr_b->get(c+Case(0,1)) != nullptr && ptr_b->get(c+Case(0,1))->get_name()=="pion")
                    J2->kill_piece(ptr_b->get(c+Case(0,1)));
                else J2->kill_piece(ptr_b->get(c+Case(0,-1)));
                return ptr_b->bouge(p,c);

            default:
                return ptr_b->bouge(p,c);
            }
            return ptr_b->bouge(p,c);
        }
    }
    return false;
}*/


Piece* Joueur::can_eat_me(Case c, Piece* ghosted){ // permet de retirer une pièce p au test => echecs à découvert
    // Ghosted est polymorphe xD
    /* il peut servir pour 'oublier' une pièce adverse qui ne peut donc plus nous manger
      OU bien il peut servir pour "oublier" une de nos pièces, pour tester si on ne "découvre pas un échec"
      en la bougeant, i.e. notre pièce est clouée.
    */
    Piece** ptr_boite = J2->get_boite();
    J2->affiche();
    for (int i=0;i<8*2;i++) {
        if (ptr_boite[i]!=nullptr &&ptr_boite[i]!=ghosted && ptr_b->permission_mange(ptr_boite[i],c, ghosted)){
            std::cout << "A piece of the following type can eat me : " << ptr_boite[i]->get_name() << " and is on the case("<<ptr_boite[i]->get().get(0)<<","<<ptr_boite[i]->get().get(1)<<")"<< std::endl;
            return ptr_boite[i];
        }
    }
    return nullptr;
}
