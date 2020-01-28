#pragma once
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
#include <map>

class Case;
class Board;
class Piece;
class Player;

using namespace Imagine;

// Constants for display, adjusted for a classic laptop screen (15')
const int SPACE = 90;
const int FONT_SIZE = 30;
const int MARGIN = 50;

// For colors
const AlphaColor LIGHT_GREEN=AlphaColor(119, 149, 89, 255);
const AlphaColor IVORY=AlphaColor(238, 238, 211, 255);
const AlphaColor BOARD_COLORS[2] = {LIGHT_GREEN, IVORY};

// Tools
void coord(const Case c, int&x, int&y);

// init graphics
void load_all_pieces();
Window display_grid_empty();

// Tools box
void display_piece(const Piece* p, const Case c);
void clr_case(const Case c);
void clear_and_display(const Piece* p, const Case c);
void go_to(const Case c1, const Case c2, const Piece* p);
void display_board(const Board& p);

bool click_move(Player& J,Case& c_start, Case& c_end);

// Promotion
int which_promotion(const Case c);
const std::string for_promotion[4] = {"knight", "queen", "bishop", "tower"};
void display_promotion(Case c, int col);
