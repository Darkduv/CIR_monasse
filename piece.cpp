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



// Gestion des constructeurs des sous classes

Roi::Roi(Case c, int couleur) : Piece(c,couleur){}
Dame::Dame(Case c, int couleur) : Piece(c,couleur){}
Cavalier::Cavalier(Case c, int couleur) : Piece(c,couleur){}
Fou::Fou(Case c, int couleur) : Piece(c,couleur){}
Tour::Tour(Case c, int couleur) : Piece(c,couleur){}
Pion::Pion(Case c, int couleur) : Piece(c,couleur){}

/**************************
 * Constructeurs de Piece *
 *************************/

Piece::Piece(){}
Piece::Piece(Case case_depart, int col){
    couleur = col;
    c = case_depart;
}

/****************************************************
 ******************    bouge     ********************
 ***************************************************/

void Piece::bouge(Case case_arrivee){
    c=case_arrivee;
}

void Pion::bouge(Case case_arrivee){
    c=case_arrivee;
}

void Roi::bouge(Case case_arrivee){
    c=case_arrivee;
}

void Tour::bouge(Case case_arrivee){
    c=case_arrivee;
}

/****************************************************
 ***************** permission_bouge *****************
 ***************************************************/

// Gestion des déplacements possibles

bool Roi::permission_bouge(Case case_arrive) const {
    if (case_arrive.distance(c)<=1) return true;
    else return false;
}

bool Dame::permission_bouge(Case case_arrive) const { // déplacement du fou OU de la tour
    if ((case_arrive.get(0) == c.get(0) || case_arrive.get(1) == c.get(1)) || (abs(case_arrive.get(0)-c.get(0))== abs(case_arrive.get(1)-c.get(1)))) return true;
    else return false;
}

bool Cavalier::permission_bouge(Case case_arrive) const {
    if (abs(case_arrive.get(0)-c.get(0))*abs(case_arrive.get(1)-c.get(1))==2) return true;
    else return false;
}

bool Fou::permission_bouge(Case case_arrive) const {
    if (abs(case_arrive.get(0)-c.get(0)) == abs(case_arrive.get(1)-c.get(1))) return true;
    else return false;
}

bool Tour::permission_bouge(Case case_arrive) const {
    if (case_arrive.get(0) == c.get(0) || case_arrive.get(1) == c.get(1)) return true;
    else return false;
}

bool Pion::permission_bouge(Case case_arrive) const { // les noirs=0 en haut les blanc=1 en bas
    if (std::abs(case_arrive.get(0)-c.get(0))>1) return false;
    if (couleur==1){
        if  (case_arrive.get(1)-c.get(1)==1){
            return true;
        }
        else if (case_arrive.get(1)-c.get(1)==2 && case_arrive.get(0)==c.get(0)) {
            return c.get(1)==1; // pion pas encore avancé, on peut l'avancer de 2 case.
        }
        else return false;
    }
    else if (couleur==0){
            if  (case_arrive.get(1)-c.get(1)==-1){
                return true;
            }
            else if (case_arrive.get(1)-c.get(1)==-2 && case_arrive.get(0)==c.get(0)) {
                return c.get(1)==6; // pion pas encore avancé, on peut l'avancer de 2 case.
            }
            else return false;
    }
    else return false; // jamais atteint normalement
}
