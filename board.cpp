#include "board.h"
#include "piece.h"
#include <iostream>

Board::Board(){
    peut_etre_pris_en_passant = nullptr;
    plateau = new Piece*[8*8];
    J1=nullptr;
    J2=nullptr;
    // On place les blancs
    set(new Tour(Case('A',1),1),Case('A',1));
    set(new Cavalier(Case('B',1),1),Case('B',1));
    set(new Fou(Case('C',1),1),Case('C',1));
    set(new Dame(Case('D',1),1),Case('D',1));
    set(new Roi(Case('E',1),1),Case('E',1));
    set(new Fou(Case('F',1),1),Case('F',1));
    set(new Cavalier(Case('G',1),1),Case('G',1));
    set(new Tour(Case('H',1),1),Case('H',1));

    // On place les noirs
    set(new Tour(Case('A',8),0),Case('A',8));
    set(new Cavalier(Case('B',8),0),Case('B',8));
    set(new Fou(Case('C',8),0),Case('C',8));
    set(new Dame(Case('D',8),0),Case('D',8));
    set(new Roi(Case('E',8),0),Case('E',8));
    set(new Fou(Case('F',8),0),Case('F',8));
    set(new Cavalier(Case('G',8),0),Case('G',8));
    set(new Tour(Case('H',8),0),Case('H',8));

    for(int j=2;j<6;j++){
        for (int i=0;i<8;i++) {
            set(nullptr,Case(i,j));
        }
    }
    for (int j=0;j<8;j++){
        set(new Pion(Case(j,1),1),Case(j,1));// on créé les pions blanc
    }
    for (int j=0;j<8;j++){
        set(new Pion(Case(j,6),0),Case(j,6));// on créé les pions noirs
    }
}

Board::~Board(){
    for(int i=0;i<8*8;i++){
        delete plateau[i];
    }
    delete [] plateau;
}

void Board::set_player(Player* Jo1, Player* Jo2){
    J1=Jo1;
    J2=Jo2;
}

Piece* Board::operator[](Case c) const{
    return plateau[c.get(0)*8+c.get(1)];
}

Piece* Board::get(Case c) const{
    return plateau[c.get(0)*8+c.get(1)];
}
Piece* Board::get(int i, int j) const{
    return plateau[i*8+j];
}

void Board::set(Piece* p, Case c){
    plateau[c.get(0)*8+c.get(1)]=p;
}

bool Board::bouge(Piece* p, Case c, int i){
    Player* J_moving = nullptr;
    Player* J_waiting = nullptr;
    switch (p->get_color()) { //J1 est de couleur 1
    case 0:
        J_moving = J2;
        J_waiting = J1;
        break;
    case 1:
        J_moving = J1;
        J_waiting = J2;
        break;
    }
    bool petit_roque = J_moving->get_petit_roque();
    bool grand_roque = J_moving->get_grand_roque();
    if (i==-1) i = permission_bouge(p,c);
    std::cout << i << std::endl;
    if (i==0){
        return false;
    }
    else if (i==2 || i==5){ // prise "normale"
        delete get(c);
        J_waiting->kill_piece(get(c));
        clr_case(c);
    }
    else if (i==3){ // prise "en passant"
        set(nullptr, peut_etre_pris_en_passant->get());
        delete peut_etre_pris_en_passant;
        J_waiting->kill_piece(peut_etre_pris_en_passant);
        Case est_pris_en_passant(c.get(0), p->get().get(1));
        clr_case(est_pris_en_passant);
    }
    else if (i==7 || i==8){ // petit ou grand roque, on "pré-bouge" la tour
        int row = c.get(1)+1;
        Case case_tour, new_case_tour;
        std::cout << "hey" << std::endl;
        if (i==7) {
            case_tour.set('H', row);
            new_case_tour.set('F', row);
        }
        else {
            case_tour.set('A', row);
            new_case_tour.set('D', row);
        }
        Piece* tour = get(case_tour);
        go_to(case_tour, new_case_tour, tour);
        set(tour, new_case_tour);
        set(nullptr, case_tour);
        tour->bouge(new_case_tour);
    }
    if (i==6){
        peut_etre_pris_en_passant = p;
    }
    else{
        peut_etre_pris_en_passant = nullptr;
    }
    // gestion du roque pour supprimer le droit d'en faire un.
    if (p->get_name()=="roi" && (petit_roque || grand_roque)){
        J_moving->set_grand_roque(false);
        J_moving->set_petit_roque(false);
    }
    else if (p->get_name()=="tour"  && p->get().get(1) == 0+7*((p->get_color()+1)%2)){
        if (p->get().get(0) == 0 && grand_roque){
            J_moving->set_grand_roque(false);
        }
        else if (p->get().get(0) == 7 && petit_roque){
            J_moving->set_petit_roque(false);
        }
    }
    // fin gestion du roque
    if (i==4 || i==5){
        display_promotion(c, p->get_color());
        int promoted = which_promotion(c);
        int col_joueur = p->get_color();
        Case position_p = p->get();
        J_moving->kill_piece(p);
        delete p; // on supprime le pion en libérant la mémoire
        if (i==5) {
            J_waiting->kill_piece(get(c)); // on supprime de la boite la pièce prise
        }
        switch(promoted){ // on recréer la pièce
        case 0:
            p = new Cavalier(position_p, col_joueur);
            break;
        case 1:
            p = new Dame(position_p, col_joueur);
            break;
        case 2:
            p = new Fou(position_p, col_joueur);
            break;
        case 3:
            p = new Tour(position_p, col_joueur);
            break;
        }
        J_moving->set_piece(p); // on ajoute à la boite a pièce
    }
    go_to(p->get(),c,p);
    set(p,c);
    set(nullptr,p->get());
    p->bouge(c);
    return true;

}

/* Permissions bouge :
 *  - 0 = Non
 *  - 1 = Oui, case Libre, coup classique
 *  - 2 = Oui, prise de piece
 *  - 3 = Oui, fait une prise en passant
 *  - 4 = Oui, promotion d'un pion.
 *  - 5 = Oui, promotion en prenant
 *  - 6 = Oui, pourra être pris en passant
 *  - 7 = Petit Roque
 *  - 8 = Grand Roque */


// TODO : à compléter (echec)
int Board::permission_bouge(Piece* p, Case c){ // on teste les permissions de bouger en connaissant le plateau, string pour indiquer quel piece bouge
    Player* J_moving = nullptr;
    Player* J_waiting = nullptr;
    switch (p->get_color()) { //J1 est de couleur 1
    case 0:
        J_moving = J2;
        J_waiting = J1;
        break;
    case 1:
        J_moving = J1;
        J_waiting = J2;
        break;
    }
    bool petit_roque = J_moving->get_petit_roque();
    bool grand_roque = J_moving->get_grand_roque();
    if (p == nullptr) return 0; //on ne peut pas bouger du vide
    if (c == p->get()) return 0; // on ne peut pas bouger sur la même case
    int must_take_or_not = 1;
    if (get(c) != nullptr){
        if (get(c)->get_color() == p->get_color()) return 0; // On en peut pas prendre une pièce de sa couleur
        must_take_or_not = 2;
    }
    if (!p->permission_bouge(c) && p->get_name() != "roi") return 0;

    if (p->get_name()=="cavalier") {
        return must_take_or_not;
    }

    // on teste si il y a des pièces sur le trajet
    Move dc = d_move(p->get(), c);
    Case c_test = p->get();
    int dx = c.get(0) - c_test.get(0);
    int dy = c.get(1) - c_test.get(1);
    int dl = std::max(std::abs(dx), std::abs(dy));
    for (int i=1;i < dl;i++){
        c_test = c_test+dc;
        if (get(c_test) != nullptr) return 0; // Case occupée sur le déplacement
    }
    /* à ce moment, si la case d'arrivée est occupée, c'est par une pièce de la couleur opposée
     * la pièce a le droit de faire ce déplacement
     * il n'y a pas de case occupée sur le trajet
     * Il ne reste plus rien à vérifier. (à part les pions qui sont source de problèmes
     * notamment pour la prise en passant ou la prise tout court. et le roi dont le roque est compliqué) */

    if (p->get_name() == "roi"){
        if (std::abs(dx)==2 && (dy != 0 || (!petit_roque && !grand_roque))) return false;
        if (dx == 2 && petit_roque){
            std::cout << "yolo" << std::endl;
            std::cout << get(p->get()+dc) << std::endl;
            if (get(p->get()+dc) != nullptr) return 0;
            else return 7;
        }
        else if (dx == -2 && grand_roque){
            if (get(p->get()+dc) != nullptr || get(p->get()+dc+dc) != nullptr) return 0; // le chemin doit être libre
            else return 8;
        }
        else if (std::abs(dx)==2) return 0;
        else if (p->permission_bouge(c)) return must_take_or_not;
        else return 0;
    }
    else if (p->get_name() == "pion"){
        // TODO : à compléter
        if (std::abs(dy) == 2) return 6; // pourra être pris en passant
        if (dx==0 && must_take_or_not == 2) return 0; // pion ne peut pas prendre tout droit
        if (std::abs(dx)==1 && must_take_or_not == 1){ // y a-t-il un pion qu'on peut prendre en passant ?
            if (peut_etre_pris_en_passant != nullptr){
                if (peut_etre_pris_en_passant->get_color() == p->get_color()) return 0;
                Case sera_pris = peut_etre_pris_en_passant->get();
                if (sera_pris.get(1) == p->get().get(1) && sera_pris.get(0) == c.get(0)) return 3;
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



bool Board::permission_mange(Piece *p, Case c, Piece* ghosted){
    if (c == p->get()) return false;
    if (p->get_name()=="cavalier") {
        return p->permission_bouge(c); // si je suis un cavalier, on cherche juste à voir si la case est accessible
    }
    else if (p->get_name()=="pion"){
        std::cout << "yyolo cooucou" << std::endl;
        Case cp = p->get();
        int dy = 1; // a white (col=1) pawn can only go up
        if (p->get_color()==0)
            dy = -1; // a black (col=0) pawn can only go down
        std::cout << dy << " " << cp.get(0) << " " << cp.get(1) << " "<< c.get(0) << " " << c.get(1) << std::endl;
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
            if (get(c_test) != nullptr && get(c_test) != ghosted) return false; // Case occupée sur le déplacement
        }
        return p->permission_bouge(c); // on a vérifié que toutes les cases sont libres,
    }
}

void Board::affiche() const{
    std::cout << "DEBUT AFFICHE PLATEAU" << std::endl;
    for(int j=7;j>=0;j--){
        for(int i=0;i<8;i++){
            if(get(Case(i,j))!=nullptr)
                std::cout << get(Case(i,j))->get_name() << " ";
            else
                std::cout << "vide ";
        }
        std::cout<<std::endl;
    }
    std::cout <<"FIN AFFICHE PLATEAU" << std::endl;
}
