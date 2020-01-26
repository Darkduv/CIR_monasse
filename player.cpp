#include "player.h"
#include "board.h"
#include "piece.h"
#include <iostream>

Player::Player(Board& p, int col){
    J2=this;
    petit_roque=true;
    grand_roque=true;
    check=false;
    checkmate=false;
    ptr_b=&p;
    color = col;
    box = new Piece*[8*2];
    int k=0;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            Piece* piece_parcourue = p[Case(i,j)];
            if (piece_parcourue!=nullptr && piece_parcourue->get_color()==col){
                box[k]=piece_parcourue;
                ++k;
            }
        }
    }

}
Player::~Player(){
    delete [] box;
}

void Player::prompt() const{
    std::string colstr[2] = {"NOIR", "BLANC"};
    std::cout << "Couleur du joueur "+colstr[color]  << std::endl;

    for(int i=0;i<8*2;i++){
        if (box[i]==nullptr) std::cout << "attention nul" << std::endl;
        else std::cout << box[i]->get_name() << " : " << box[i]->get().get(0) << box[i]->get().get(1) << std::endl;
    }
}

void Player::kill_piece(Piece* p){
    for(int i=0;i<8*2;i++){
        if (box[i]==p){
            box[i]=nullptr;}
    }
}

void Player::set_piece(Piece* p){
    for(int i=0;i<8*2;i++){
        if (box[i]==nullptr){
            std::cout<<"travail terminé"<< std::endl;
            box[i]=p;}
    }
}
void Player::set_petit_roque(bool value){petit_roque=value;}
void Player::set_grand_roque(bool value){grand_roque=value;}
int Player::get_color() const{return color;}
Piece** Player::get_boite(){return box;}
bool Player::get_check(){return check;}
bool Player::get_checkmate(){return checkmate;}
bool Player::get_petit_roque(){return petit_roque;}
bool Player::get_grand_roque(){return grand_roque;}
Board* Player::get_board(){return ptr_b;}
void Player::set_other_player(Player* J){J2=J;}

Piece* Player::get_my_king(){
    for(int i=0;i<8*2;i++){
        if(box[i]!=nullptr && box[i]->get_name()=="roi") return box[i];
    }
    return nullptr;
}

bool Player::is_checkmate(){ // not implemented yet
    return false;
};

bool Player::bouge(Piece* p, Case c){
    if (p!=nullptr && p->get_color()==get_color()){
        Piece* eater = can_eat_me(get_my_king()->get());
        if (eater != nullptr){
            if (p->get_name()=="roi"){
                ptr_b->set(nullptr, p->get());
                if (can_eat_me(c) == nullptr){
                    ptr_b->set(p, p->get());
                    return ptr_b->bouge(p,c); // if the check can be avoided by moving, we doing that
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
                else return false;

            }
        }
        else if (p->get_name()=="roi"){
            if (!can_eat_me(c)) return ptr_b->bouge(p,c);
            else return false;
        }
        else {
            if (!can_eat_me(get_my_king()->get(), p)){
                return ptr_b->bouge(p,c);
            }
            else {
                return false;
            }
        }
    }
    else
        return false;
}

Piece* Player::can_eat_me(Case c, Piece* ghosted){ // permet de retirer une pièce p au test => echecs à découvert
    // Ghosted est polymorphe xD
    /* il peut servir pour 'oublier' une pièce adverse qui ne peut donc plus nous manger
      OU bien il peut servir pour "oublier" une de nos pièces, pour tester si on ne "découvre pas un échec"
      en la bougeant, i.e. notre pièce est clouée.
    */
    Piece** ptr_boite = J2->get_boite();
    for (int i=0;i<8*2;i++) {
        if (ptr_boite[i]!=nullptr &&ptr_boite[i]!=ghosted && ptr_b->permission_mange(ptr_boite[i],c, ghosted)){
            std::cout << "A piece of the following type can eat me : " << ptr_boite[i]->get_name() << " and is on the case("<<ptr_boite[i]->get().get(0)<<","<<ptr_boite[i]->get().get(1)<<")"<< std::endl;
            return ptr_boite[i];
        }
    }
    return nullptr;
}
