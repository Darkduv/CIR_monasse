#include "piece.h"
#include "board.h"
#include "cmath"

// name field.

const std::string Piece::name = "piece";
const std::string Dame::name = "dame";
const std::string Roi::name = "roi";
const std::string Tour::name = "tour";
const std::string Cavalier::name = "cavalier";
const std::string Fou::name = "fou";
const std::string Pion::name = "pion";

std::string Piece::get_name() const {return name;}
std::string Roi::get_name() const {return name;}
std::string Dame::get_name() const {return name;}
std::string Fou::get_name() const {return name;}
std::string Cavalier::get_name() const {return name;}
std::string Tour::get_name() const {return name;}
std::string Pion::get_name() const {return name;}



// Management of subclass builders

Roi::Roi(Case c, int couleur) : Piece(c,couleur){}
Dame::Dame(Case c, int couleur) : Piece(c,couleur){}
Cavalier::Cavalier(Case c, int couleur) : Piece(c,couleur){}
Fou::Fou(Case c, int couleur) : Piece(c,couleur){}
Tour::Tour(Case c, int couleur) : Piece(c,couleur){}
Pion::Pion(Case c, int couleur) : Piece(c,couleur){}

/******************************
 * Constructors of the pieces *
 ******************************/

Piece::Piece()=default;
Piece::Piece(Case case_depart, int col){
    couleur = col;
    c = case_depart;
}

/****************************************************
 ******************     Move     ********************
 ***************************************************/

void Piece::move(Case case_arrivee){
    c=case_arrivee;
}

void Pion::move(Case case_arrivee){
    c=case_arrivee;
}

void Roi::move(Case case_arrivee){
    c=case_arrivee;
}

void Tour::move(Case case_arrivee){
    c=case_arrivee;
}

/****************************************************
 ***************** permission_move ******************
 ***************************************************/

// Management of possible movements

bool Roi::permission_bouge(Case case_arrive) const {
    return case_arrive.distance(c) <= 1;
}

bool Dame::permission_bouge(Case case_arrive) const { // The queen has the movements of a bishop or a tower
    return (case_arrive.get(0) == c.get(0) || case_arrive.get(1) == c.get(1)) || (abs(case_arrive.get(0) - c.get(0)) == abs(case_arrive.get(1) - c.get(1)));
}

bool Cavalier::permission_bouge(Case case_arrive) const {
    return abs(case_arrive.get(0) - c.get(0)) * abs(case_arrive.get(1) - c.get(1)) == 2;
}

bool Fou::permission_bouge(Case case_arrive) const {
    return abs(case_arrive.get(0) - c.get(0)) == abs(case_arrive.get(1) - c.get(1));
}

bool Tour::permission_bouge(Case case_arrive) const {
    return case_arrive.get(0) == c.get(0) || case_arrive.get(1) == c.get(1);
}

bool Pion::permission_bouge(Case case_arrive) const { // black=0 on top, white=1 on bottom
    if (std::abs(case_arrive.get(0)-c.get(0))>1) return false;
    if (couleur == 1){
        if  (case_arrive.get(1)-c.get(1)==1){
            return true;
        }
        else return case_arrive.get(1) - c.get(1) == 2 && case_arrive.get(0) == c.get(0) && c.get(1) == 1;
    }
    else if (couleur == 0){
            if  (case_arrive.get(1)-c.get(1)==-1){
                return true;
            }
            else return case_arrive.get(1) - c.get(1) == -2 && case_arrive.get(0) == c.get(0) && c.get(1) == 6;
    }
    else return false; // the function should never reach this point (in theory)
}
