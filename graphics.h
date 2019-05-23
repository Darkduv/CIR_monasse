#pragma once
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
#include <map>
#include "piece.h"
#include "plateau.h"
#include "case.h"
#include "player.h"
using namespace Imagine;

// Constants for display, adjusted for a classic laptop screen (15')
const int SPACE = 90;
const int FONT_SIZE = 30;
const int MARGIN = 50;

// For colors
const AlphaColor LIGHTGREEN=AlphaColor(119, 149, 89, 255);
const AlphaColor IVORY=AlphaColor(238, 238, 211, 255);
const AlphaColor BOARD_COLORS[2] = {LIGHTGREEN, IVORY};

// Tools
void coord(const Case c, int&x, int&y);

// init graphics
void load_all_pieces();
void display_grid_empty();

// Tools box
void display_piece(const Piece* p, const Case c);
void clr_case(const Case c);
void clear_and_display(const Piece* p, const Case c);
void go_to(const Case c1, const Case c2, const Piece* p);
void display_board(const Plateau& p);

bool click_move(Player& J,Case& c_start, Case& c_end);

// Promotion
int which_promotion(const Case c);
const std::string for_promotion[4] = {"cavalier", "dame", "fou", "tour"};
void display_promotion(Case c, int col);
