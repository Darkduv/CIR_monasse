#include "graphics.h"
#include "case.h"
using namespace std;
#include <string.h>

/**************************
 * byte* pieces functions *
 *************************/
// to load pieces' images
std::map<std::string, AlphaColor*> byte_pieces;

std::string get_path_name(const string name, const string col){
    string s = col+name;
    return stringSrcPath("img/"+to_string(SPACE)+"/"+s+".png");
}

AlphaColor* load_bitmap(const string name, const string col){
    AlphaColor* piece;
    const std::string path = get_path_name(name, col);
    int w, h;
    loadAlphaColorImage(path, piece, w, h);
    return piece;
}

void initializer_map_piece(const string name, const string col){
    byte_pieces[col+name] = load_bitmap(name, col);
}

void load_all_pieces(){
    string cols[2] = {"b", "w"};
    string names[6] = {"roi", "dame", "fou", "cavalier", "tour", "pion"};
    for (int i=0;i < 2;i++){
        for (int j=0;j<6;j++){
            initializer_map_piece(names[j], cols[i]);
        }
    }
}


/*******************
 * tools functions *
 ******************/

void coord(const Case c, int&x, int&y){
    x = (c.get(0))*SPACE+MARGIN;
    y = (7-c.get(1))*SPACE+MARGIN;
}

Color color_case(const Case c){
    return BOARD_COLORS[(c.get(0)+c.get(1))%2];
}

std::string get_path_image(const Piece* p){
    string col = string(1, "bw"[p->get_color()]); //b = black = 0, w = white = 1
    string s = col+p->get_name();
    return stringSrcPath("img/"+to_string(SPACE)+"/"+s+".png");
}

/*********************
 * graphic functions *
 ********************/

void display_grid_empty(){
    int W = (8)*(SPACE)+2*MARGIN;
    int H = (8)*SPACE+2*MARGIN;
    openWindow(W, H, "Chessboard");
    string columns = "ABCDEFGH";
    string rows = "87654321";
    for (int i=0; i < 8;i++){
        // rows and columns
        int x = MARGIN + i*SPACE+SPACE/2;
        int y1 = MARGIN/2;
        int y2 = MARGIN/2+MARGIN+8*SPACE;
        int dx = 2*FONT_SIZE/7;
        int dy = FONT_SIZE/2;
        drawString(x-dx, y1+dy, string(1, columns[i]), BLACK, FONT_SIZE);
        drawString(x-dx, y2+dy, string(1, columns[i]), BLACK, FONT_SIZE);
        drawString(y1-dx, x+dy, string(1, rows[i]), BLACK, FONT_SIZE);
        drawString(y2-dx, x+dy, string(1, rows[i]), BLACK, FONT_SIZE);

        // cases
        for (int j= 0; j < 8;j++){
            clr_case(Case(i, j));
        }
    }
}

void display_byte(const Piece* p, const Case c, const bool xor_mode, const double fact){
    int x, y;
    coord(c, x, y);
    string name = string(1, "bw"[p->get_color()])+p->get_name();
    putAlphaColorImage(x, y, byte_pieces[name],  SPACE, SPACE, xor_mode, fact);
}

void display_piece(const Piece* p, const Case c){
    int x, y;
    coord(c, x, y);
    string name = string(1, "bw"[p->get_color()])+p->get_name();
    putAlphaColorImage(x, y, byte_pieces[name],  SPACE, SPACE, false);
}

/*
void display_piece(Case c, Piece* p){
    Image<AlphaColor> im; // Image en niveaux de gris
    string file = get_path_image(p);
    if (! load(im, file)) {
        cerr << "Impossible d’ouvrir le fichier " << file << endl ;
        exit (1);
    }
    int x, y;
    coord(c, x, y);
    display(im, x, y);
}*/

void clr_case(const Case c){
    int x, y;
    coord(c, x, y);
    fillRect(x, y, SPACE, SPACE, color_case(c));
}
void go_to(const Case c1, const Case c2, Piece* p){
    clr_case(c1);
    clr_case(c2);
    display_piece(p, c2);
}
bool click_move(Joueur& J, Case& c_start, Case& c_end){
    int x, y;
    int count = 0;
    while (count < 2 && getMouse(x, y) != 3){  // right click = 3 = No more moves, I wanna stop
        count++;
        int i = (x-MARGIN)/SPACE;
        int j = 7-(y-MARGIN)/SPACE;
        if (count==1){
            c_start.set(i, j);
            Piece* p=J.get_board()->get(c_start);
            if (p==nullptr || p->get_color()!=J.get_color()){
                count=0;
            }
        }
        else if (count==2) {
            //cout << "Got ending Case" << endl;
            c_end.set(i, j);
        }
    }
    return count == 2;
}
void display_board(const Plateau& p){
    for (int j=0;j<8;j++){
        for (int i=0;i<8;i++){
            Piece* pp = p.get(i, j);
            if (pp != nullptr){
                display_piece(pp, Case(i, j));
            }
        }
    }
}
