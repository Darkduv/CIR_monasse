#pragma once
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
#include <map>
#include "piece.h"
#include "plateau.h"
using namespace Imagine;

class Case;
class Joueur;

const int SPACE=90;
const int FONT_SIZE = 30;
const int MARGIN = 50;
const AlphaColor LIGHTGREEN=AlphaColor(119, 149, 89, 255);
const AlphaColor IVORY=AlphaColor(238, 238, 211, 255);
const AlphaColor BOARD_COLORS[2] = {LIGHTGREEN, IVORY};

void load_all_pieces();
void coord(const Case c, int&x, int&y);

void display_grid_empty();
void display_piece(const Piece* p, const Case c);
void display_byte(const Piece* p, const Case c, const bool xor_mode=false, const double fact=1.);
void clr_case(const Case c);
void go_to(const Case c1, const Case c2, Piece* p);
bool click_move(Joueur& J,Case& c_start, Case& c_end);
void display_board(const Plateau& p);
int which_promotion(const Case c);
const std::string for_promotion[4] = {"cavalier", "dame", "fou", "tour"};
void display_promotion(Case c, int col);
