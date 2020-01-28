#include "player.h"
#include "board.h"
#include "piece.h"
#include <iostream>

Player::Player(Board& p, int col){
    J2=this;
    king_castling=true;
    queen_castling=true;
    check=false;
    checkmate=false;
    ptr_b=&p;
    color = col;
    box = new Piece*[8*2];
    int k=0;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            Piece* processed_piece = p[Case(i, j)];
            if (processed_piece != nullptr && processed_piece->get_color() == col){
                box[k]=processed_piece;
                ++k;
            }
        }
    }

}
Player::~Player(){
    delete [] box;
}

void Player::display() const{
    std::string col_str[2] = {"BLACK", "WHITE"};
    std::cout << "Player's Color " + col_str[color] << std::endl;

    for(int i=0;i<8*2;i++){
        if (box[i]==nullptr) std::cout << "Warning nullptr in box" << std::endl;
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
            std::cout<<"Work done"<< std::endl;
            box[i]=p;}
    }
}
void Player::set_king_castling(bool value){ king_castling=value;}
void Player::set_queen_castling(bool value){ queen_castling=value;}
int Player::get_color() const{return color;}
Piece** Player::get_box(){return box;}
bool Player::get_check(){return check;}
bool Player::get_checkmate(){return checkmate;}
bool Player::get_king_castling(){return king_castling;}
bool Player::get_queen_castling(){return queen_castling;}
Board* Player::get_board(){return ptr_b;}
void Player::set_other_player(Player* J){J2=J;}

Piece* Player::get_my_king(){
    for(int i=0;i<8*2;i++){
        if(box[i]!=nullptr && box[i]->get_name()=="king") return box[i];
    }
    return nullptr;
}

bool Player::is_checkmate(){ // not implemented yet
    return false;
};

bool Player::move(Piece* p, Case c){
    if (p!=nullptr && p->get_color()==get_color()){
        Piece* eater = can_eat_me(get_my_king()->get());
        if (eater != nullptr){
            if (p->get_name()=="king"){
                ptr_b->set(nullptr, p->get());
                if (can_eat_me(c) == nullptr){
                    ptr_b->set(p, p->get());
                    return ptr_b->move(p, c); // if the check can be avoided by moving, we do that
                }
                else {
                    ptr_b->set(p, p->get());
                    return false;
                }
            }
            else if (ptr_b->get(c)==nullptr){ // taking care of check
                ptr_b->set(p,c);
                ptr_b->set(nullptr, p->get());
                if (can_eat_me(get_my_king()->get())==nullptr){
                    ptr_b->set(nullptr,c);
                    ptr_b->set(p, p->get());
                    return ptr_b->move(p, c);
                }
                else {
                    ptr_b->set(nullptr,c);
                    ptr_b->set(p, p->get());
                    return false;
                }
            }
            else { // the checker is captured
                if (eater->get() == c){
                    ptr_b->set(p,c);
                    ptr_b->set(nullptr, p->get());
                    if (can_eat_me(get_my_king()->get(), eater)==nullptr){
                        ptr_b->set(eater,c);
                        ptr_b->set(p, p->get());
                        return ptr_b->move(p, c);
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
        else if (p->get_name()=="king"){
            if (!can_eat_me(c)) return ptr_b->move(p, c);
            else return false;
        }
        else {
            if (!can_eat_me(get_my_king()->get(), p)){
                return ptr_b->move(p, c);
            }
            else {
                return false;
            }
        }
    }
    else
        return false;
}

Piece* Player::can_eat_me(Case c, Piece* ghosted){ // allow a piece to be removed during the test
    // => we tried to deal with discovered check using this method
    // Ghosted is kind of polymorphic xD
    /* $ghosted can be used to "forget" a piece of the opposite player which can no more capture us
     * OR it can be used to "forget" one of our own pieces, to test if it doesnt "discover" a check by moving it,
     * which means the piece is pinned. */
    Piece** ptr_box = J2->get_box();
    for (int i=0;i<8*2;i++) {
        if (ptr_box[i] != nullptr && ptr_box[i] != ghosted && ptr_b->permission_capture(ptr_box[i], c, ghosted)){
            std::cout << "A piece of the following type can eat me : " << ptr_box[i]->get_name() << " and is on the case(" << ptr_box[i]->get().get(0) << "," << ptr_box[i]->get().get(1) << ")" << std::endl;
            return ptr_box[i];
        }
    }
    return nullptr;
}
