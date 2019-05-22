#include "graphics.h"
#include "case.h"
using namespace std;
#include <string.h>

/**************************
 * byte* pieces functions *
 *************************/
// to load pieces' images
static std::map<std::string, AlphaColor*> byte_pieces;

std::string get_path_name(const string name, const string col){
    return stringSrcPath("img/"+to_string(SPACE)+"/"+col+name+".png");
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
    const string cols[2] = {"b", "w"};
    const string names[6] = {"roi", "dame", "fou", "cavalier", "tour", "pion"};
    for (int i=0;i < 2;i++){
        for (int j=0;j<6;j++){
            initializer_map_piece(names[j], cols[i]);
        }
    }
}


/*******************
 * tools functions *
 ******************/

string code_name(const Piece* p){
    return string(1, "bw"[p->get_color()])+p->get_name();
}

void coord(const Case c, int&x, int&y){
    // give the coordinates of the pixel of the corner of the case
    x = (c.get(0))*SPACE+MARGIN;
    y = (7-c.get(1))*SPACE+MARGIN;
}

Color color_case(const Case c){
    // gives the color of the case
    return BOARD_COLORS[(c.get(0)+c.get(1))%2];
}

/*********************
 * graphic functions *
 ********************/

void display_grid_empty(){
    int W = (8)*(SPACE)+2*MARGIN;
    int H = (8)*SPACE+2*MARGIN;
    openWindow(W, H, "Chessboard");
    const string columns = "ABCDEFGH";
    const string rows = "87654321";
    const int dx = 2*FONT_SIZE/7;
    const int dy = FONT_SIZE/2;
    const int y1 = MARGIN/2;
    const int y2 = MARGIN/2+MARGIN+8*SPACE;
    for (unsigned int i=0; i < 8;i++){
        // rows and columns numerotation
        const int x = MARGIN + int(i)*SPACE+SPACE/2;
        drawString(x-dx, y1+dy, string(1, columns[i]), BLACK, FONT_SIZE);
        drawString(x-dx, y2+dy, string(1, columns[i]), BLACK, FONT_SIZE);
        drawString(y1-dx, x+dy, string(1, rows[i]), BLACK, FONT_SIZE);
        drawString(y2-dx, x+dy, string(1, rows[i]), BLACK, FONT_SIZE);

        // display cases
        for (int j= 0; j < 8;j++){
            clr_case(Case(int(i), j));
        }
    }
}

void display_piece(const Piece* p, const Case c){
    int x, y;
    coord(c, x, y);
    string name = code_name(p);
    putAlphaColorImage(x, y, byte_pieces[name],  SPACE, SPACE, false);
}

void clr_case(const Case c){
    int x, y;
    coord(c, x, y);
    fillRect(x, y, SPACE, SPACE, color_case(c));
}
void go_to(const Case c1, const Case c2, const Piece* p){
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

// Promotion choice.

void display_promotion(Case c, int col){
    int x, y;
    coord(c, x, y);
    clr_case(c);
    string s = string(1, "bw"[col]);
    putAlphaColorImage(x, y, byte_pieces[s+for_promotion[0]],  SPACE, SPACE, false, 0.5);
    putAlphaColorImage(x+SPACE/2, y, byte_pieces[s+for_promotion[1]],  SPACE, SPACE, false, 0.5);
    putAlphaColorImage(x, y+SPACE/2, byte_pieces[s+for_promotion[2]],  SPACE, SPACE, false, 0.5);
    putAlphaColorImage(x+SPACE/2, y+SPACE/2, byte_pieces[s+for_promotion[3]],  SPACE, SPACE, false, 0.5);
}

int which_promotion(Case c){
    int x, y;
    int i, j;
    do {
        getMouse(x, y);
        i = (x-MARGIN)/SPACE;
        j = 7-(y-MARGIN)/SPACE;
    }
    while (i!=c.get(0) || j != c.get(1));
    int ii, jj;
    ii = (x-MARGIN-i*SPACE)/(SPACE/2);
    jj = (7*SPACE - (y-MARGIN)-j*SPACE)/(SPACE/2);
    return ii+2*jj;
}

