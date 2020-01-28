#include "board.h"
#include "piece.h"
#include <iostream>

Board::Board(){
    can_be_en_passant_captured = nullptr;
    work_board = new Piece*[8 * 8];
    P1=nullptr;
    P2=nullptr;
    // We create the new white pieces and put it on the board
    set(new Tower(Case('A', 1), 1), Case('A', 1));
    set(new Knight(Case('B', 1), 1), Case('B', 1));
    set(new Bishop(Case('C', 1), 1), Case('C', 1));
    set(new Queen(Case('D', 1), 1), Case('D', 1));
    set(new King(Case('E', 1), 1), Case('E', 1));
    set(new Bishop(Case('F', 1), 1), Case('F', 1));
    set(new Knight(Case('G', 1), 1), Case('G', 1));
    set(new Tower(Case('H', 1), 1), Case('H', 1));

    // idem for black
    set(new Tower(Case('A', 8), 0), Case('A', 8));
    set(new Knight(Case('B', 8), 0), Case('B', 8));
    set(new Bishop(Case('C', 8), 0), Case('C', 8));
    set(new Queen(Case('D', 8), 0), Case('D', 8));
    set(new King(Case('E', 8), 0), Case('E', 8));
    set(new Bishop(Case('F', 8), 0), Case('F', 8));
    set(new Knight(Case('G', 8), 0), Case('G', 8));
    set(new Tower(Case('H', 8), 0), Case('H', 8));

    for(int j=2;j<6;j++){
        for (int i=0;i<8;i++) {
            set(nullptr,Case(i,j));
        }
    }
    for (int j=0;j<8;j++){
        set(new Pawn(Case(j, 1), 1), Case(j, 1)); // creates white pawns
    }
    for (int j=0;j<8;j++){
        set(new Pawn(Case(j, 6), 0), Case(j, 6)); // creates black pawns
    }
}

Board::~Board(){
    for(int i=0;i<8*8;i++){
        delete work_board[i];
    }
    delete [] work_board;
}

void Board::set_player(Player* Jo1, Player* Jo2){
    P1=Jo1;
    P2=Jo2;
}

Piece* Board::operator[](Case c) const{
    return work_board[c.get(0) * 8 + c.get(1)];
}

Piece* Board::get(Case c) const{
    return work_board[c.get(0) * 8 + c.get(1)];
}
Piece* Board::get(int i, int j) const{
    return work_board[i * 8 + j];
}

void Board::set(Piece* p, Case c){
    work_board[c.get(0) * 8 + c.get(1)]=p;
}

bool Board::move(Piece* p, Case c, int i){
    Player* P_moving = nullptr;
    Player* P_waiting = nullptr;
    switch (p->get_color()) { // P1 has color 1
    case 0:
        P_moving = P2;
        P_waiting = P1;
        break;
    case 1:
        P_moving = P1;
        P_waiting = P2;
        break;
    }
    bool kingCastling = P_moving->get_king_castling();
    bool queenCastling = P_moving->get_queen_castling();
    if (i==-1) i = permission_move(p, c);
    if (i==0){
        return false;
    }
    else if (i==2 || i==5){ // "normal" capture
        delete get(c);
        P_waiting->kill_piece(get(c));
        clr_case(c);
    }
    else if (i==3){ // "en passant" capture
        set(nullptr, can_be_en_passant_captured->get());
        delete can_be_en_passant_captured;
        P_waiting->kill_piece(can_be_en_passant_captured);
        Case est_pris_en_passant(c.get(0), p->get().get(1));
        clr_case(est_pris_en_passant);
    }
    else if (i==7 || i==8){ // king or queen side castling, we "pre-move" the tower
        int row = c.get(1)+1;
        Case case_tower, new_case_tower;
        if (i==7) {
            case_tower.set('H', row);
            new_case_tower.set('F', row);
        }
        else {
            case_tower.set('A', row);
            new_case_tower.set('D', row);
        }
        Piece* tower = get(case_tower);
        go_to(case_tower, new_case_tower, tower);
        set(tower, new_case_tower);
        set(nullptr, case_tower);
        tower->move(new_case_tower);
    }
    if (i==6){
        can_be_en_passant_captured = p;
    }
    else{
        can_be_en_passant_captured = nullptr;
    }

    // management of castling to remove the right to do another one.
    if (p->get_name()=="king" && (kingCastling || queenCastling)){
        P_moving->set_queen_castling(false);
        P_moving->set_king_castling(false);
    }
    else if (p->get_name()=="tower"  && p->get().get(1) == 0+7*((p->get_color()+1)%2)){
        if (p->get().get(0) == 0 && queenCastling){
            P_moving->set_queen_castling(false);
        }
        else if (p->get().get(0) == 7 && kingCastling){
            P_moving->set_king_castling(false);
        }
    }
    // end of castling management

    if (i==4 || i==5){
        display_promotion(c, p->get_color());
        int promoted = which_promotion(c);
        int col_player = p->get_color();
        Case position_p = p->get();
        P_moving->kill_piece(p);
        delete p; // we remove the pawn, freeing the memory
        if (i==5) {
            P_waiting->kill_piece(get(c)); // the captured piece is removed from the box
        }
        switch(promoted){ // recreate the piece
            case 0:
                p = new Knight(position_p, col_player);
                break;
            case 1:
                p = new Queen(position_p, col_player);
                break;
            case 2:
                p = new Bishop(position_p, col_player);
                break;
            case 3:
                p = new Tower(position_p, col_player);
                break;
            default: // Todo : raise an error
                break;
        }
        P_moving->set_piece(p); // the piece is added to the box
    }
    go_to(p->get(),c,p);
    set(p,c);
    set(nullptr,p->get());
    p->move(c);
    return true;

}

/* Permissions move :
 *  - 0 = No
 *  - 1 = Yes, free case, classic move
 *  - 2 = Yes, take a piece
 *  - 3 = Yes, en passant capture
 *  - 4 = Yes, pawn promotion
 *  - 5 = Yes, capture and promotion
 *  - 6 = Yes, "en passant" capture of the piece will be possible next move
 *  - 7 = Kingside castling
 *  - 8 = Queenside castling */


// TODO : to complete and correct (check management is wrong)
int Board::permission_move(Piece* p, Case c){ // we check moving permissions by knowing the work_board,
    // the string let us know which piece moves.
    Player* P_moving = nullptr;
    Player* P_waiting = nullptr;
    switch (p->get_color()) { // P1's color is 1
    case 0:
        P_moving = P2;
        P_waiting = P1;
        break;
    case 1:
        P_moving = P1;
        P_waiting = P2;
        break;
    }
    bool king_castling = P_moving->get_king_castling();
    bool queen_castling = P_moving->get_queen_castling();
    if (p == nullptr) return 0; // Void can not be moved
    if (c == p->get()) return 0; // move is not possible on the same spot
    int must_take_or_not = 1;
    if (get(c) != nullptr){
        if (get(c)->get_color() == p->get_color()) return 0; // do not capture a piece of your own color
        must_take_or_not = 2;
    }
    if (!p->permission_move(c) && p->get_name() != "king") return 0;

    if (p->get_name()=="knight") {
        return must_take_or_not;
    }

    // testing if there is any piece on the move, blocking it

    Move dc = d_move(p->get(), c);
    Case c_test = p->get();
    int dx = c.get(0) - c_test.get(0);
    int dy = c.get(1) - c_test.get(1);
    int dl = std::max(std::abs(dx), std::abs(dy));
    for (int i=1;i < dl;i++){
        c_test = c_test+dc;
        if (get(c_test) != nullptr) return 0; // Occupied case on the move
    }

    /* At this time, if the arrival case is occupied, it is by a piece of the opposite color (and player)
     * So technically the piece can do the move, there is no occupied case on the move.
     * There is nothing more to check, except for pawn that are a source of problems,
     * like for the en passant capture or even the capture
     * and except for king, whose castling is a little more complicated */

    if (p->get_name() == "king"){
        if (std::abs(dx)==2 && (dy != 0 || (!king_castling && !queen_castling))) return false;
        if (dx == 2 && king_castling){
            if (get(p->get()+dc) != nullptr || P_moving->can_eat_me(p->get()) || P_moving->can_eat_me(p->get() + dc)) return 0;
            else return 7;
        }
        else if (dx == -2 && queen_castling){
            if (get(p->get()+dc) != nullptr || get(p->get()+dc+dc) != nullptr
                || P_moving->can_eat_me(p->get() + dc) || P_moving->can_eat_me(p->get() + dc + dc)
                || P_moving->can_eat_me(p->get())) return 0; // path should be free
            else return 8;
        }
        else if (p->permission_move(c)) return must_take_or_not;
        else return 0; // does also when "(std::abs(dx)==2)"
    }
    else if (p->get_name() == "pawn"){
        // TODO : to complete (?)
        if (std::abs(dy) == 2) return 6; // it will be possible to take it with "en passant" capture
        if (dx==0 && must_take_or_not == 2) return 0; // pawn can not capture straight.
        if (std::abs(dx)==1 && must_take_or_not == 1){ // is there a pawn that could be "en passant" captured ?
            if (can_be_en_passant_captured != nullptr){
                if (can_be_en_passant_captured->get_color() == p->get_color()) return 0;
                Case will_be_captured = can_be_en_passant_captured->get();
                if (will_be_captured.get(1) == p->get().get(1) && will_be_captured.get(0) == c.get(0)) return 3;
                else return 0;
            }
            else return 0;
        }

        if (c.get(1) == 7 or c.get(1) == 0){
            return must_take_or_not+3;  // 4 ou 5
        }
        return must_take_or_not;
    }
    return must_take_or_not;
}



bool Board::permission_capture(Piece *p, Case c, Piece* ghosted){
    if (c == p->get()) return false;
    if (p->get_name()=="knight") {
        return p->permission_move(c); // if it's a knight, just check if case is accessible
    }
    else if (p->get_name()=="pawn"){
        Case cp = p->get();
        int dy = 1; // a white (col=1) pawn can only go "up"
        if (p->get_color()==0)
            dy = -1; // a black (col=0) pawn can only go "down"
        return std::abs(cp.get(0)-c.get(0)) == 1 && cp.get(1)+dy == c.get(1);
    }
    else {
        Move dc = d_move(p->get(), c);
        Case c_test = p->get();
        int dx = c.get(0) - c_test.get(0);
        int dy = c.get(1) - c_test.get(1);
        int dl = std::max(std::abs(dx), std::abs(dy));
        for (int i=1;i < dl;i++){
            c_test = c_test+dc;
            if (get(c_test) != nullptr && get(c_test) != ghosted) return false; // Occupied case on the move
        }
        return p->permission_move(c); // we checked that all the cases were free
    }
}

void Board::display() const{
    std::cout << "____________________________" << std::endl;
    for(int j=7;j>=0;j--){
        for(int i=0;i<8;i++){
            if(get(Case(i,j))!=nullptr)
                std::cout << get(Case(i,j))->get_name() << " ";
            else
                std::cout << "empty ";
        }
        std::cout<<std::endl;
    }
    std::cout <<"_____________________________" << std::endl;
}
