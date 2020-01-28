#include "piece.h"

// name field.

const std::string Piece::name = "piece";
const std::string Queen::name = "queen";
const std::string King::name = "king";
const std::string Tower::name = "tower";
const std::string Knight::name = "knight";
const std::string Bishop::name = "bishop";
const std::string Pawn::name = "pawn";

std::string Piece::get_name() const {return name;}
std::string King::get_name() const {return name;}
std::string Queen::get_name() const {return name;}
std::string Bishop::get_name() const {return name;}
std::string Knight::get_name() const {return name;}
std::string Tower::get_name() const {return name;}
std::string Pawn::get_name() const {return name;}



// Management of subclass builders

King::King(Case c, int col) : Piece(c, col){}
Queen::Queen(Case c, int col) : Piece(c, col){}
Knight::Knight(Case c, int col) : Piece(c, col){}
Bishop::Bishop(Case c, int col) : Piece(c, col){}
Tower::Tower(Case c, int col) : Piece(c, col){}
Pawn::Pawn(Case c, int col) : Piece(c, col){}

/******************************
 * Constructors of the pieces *
 ******************************/

// Piece::Piece()=default;
Piece::Piece(Case case_depart, int col){
    color = col;
    c = case_depart;
}

/****************************************************
 ******************     Move     ********************
 ***************************************************/

void Piece::move(Case end_of_move_case){
    c=end_of_move_case;
}

void Pawn::move(Case end_of_move_case){
    c=end_of_move_case;
}

void King::move(Case end_of_move_case){
    c=end_of_move_case;
}

void Tower::move(Case end_of_move_case){
    c=end_of_move_case;
}

/****************************************************
 ***************** permission_move ******************
 ***************************************************/

// Management of possible movements

bool King::permission_move(Case arrival_case) const {
    return arrival_case.distance(c) <= 1;
}

bool Queen::permission_move(Case arrival_case) const { // The queen has the movements of a bishop or a tower
    return (arrival_case.get(0) == c.get(0) || arrival_case.get(1) == c.get(1)) || (abs(arrival_case.get(0) - c.get(0)) == abs(arrival_case.get(1) - c.get(1)));
}

bool Knight::permission_move(Case arrival_case) const {
    return abs(arrival_case.get(0) - c.get(0)) * abs(arrival_case.get(1) - c.get(1)) == 2;
}

bool Bishop::permission_move(Case arrival_case) const {
    return abs(arrival_case.get(0) - c.get(0)) == abs(arrival_case.get(1) - c.get(1));
}

bool Tower::permission_move(Case arrival_case) const {
    return arrival_case.get(0) == c.get(0) || arrival_case.get(1) == c.get(1);
}

bool Pawn::permission_move(Case arrival_case) const { // black=0 on top, white=1 on bottom
    if (std::abs(arrival_case.get(0) - c.get(0)) > 1) return false;
    if (color == 1){
        if  (arrival_case.get(1) - c.get(1) == 1){
            return true;
        }
        else return arrival_case.get(1) - c.get(1) == 2 && arrival_case.get(0) == c.get(0) && c.get(1) == 1;
    }
    else if (color == 0){
            if  (arrival_case.get(1) - c.get(1) == -1){
                return true;
            }
            else return arrival_case.get(1) - c.get(1) == -2 && arrival_case.get(0) == c.get(0) && c.get(1) == 6;
    }
    else return false; // the function should never reach this point (in theory)
}
