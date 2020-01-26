#include <iostream>
using namespace std;
#include "case.h"
#include "player.h"
// #include "piece.h"
#include "board.h"
#include "graphics.h"

int main()
{
    Window w = openWindow(400, 300, "Chess by Nexus&cie");
    drawString(30, 40, "Welcome on this GUI of Chess, this well-known game!", BLACK,12, 0, false, true);
    drawString(10, 80, "-> To play, click on the piece you want to move", BLACK);
    drawString(10, 100, "and then where you want to move it.", BLACK);
    drawString(10, 120, "-> You can quit with a right click at any time", BLACK);
    drawString(20, 200, "Please click on this window to proceed to the GUI.", BLACK,12, 0, false, true);
    drawString(120, 270, "Coded by Maximin Duvillard and Nicolas Birac", BLACK, 10, 0, true);
    drawString(200, 285, "© - 2019", BLACK, 10, 0, true);

    click();
    closeWindow(w);

    // we load the pieces images in memory
    load_all_pieces();
    // Display of the board
    Window window_game = display_grid_empty();
    // TODO: What's that doing here ?

    // initiations
    Board b;
    Player J1(b,1);
    Player J2(b,0);
    J1.set_other_player(&J2);
    J2.set_other_player(&J1);
    b.set_player(&J1,&J2);
    display_board(b);
    //
    Case c1, c2;
    int tour=0;

    bool dont_quit = true;
    // il faut changer la fonction permission mange pour pouvoir l'appeler sur uen case vide pour simuler la présence de la piece sans la déplacer
    while(!J1.get_checkmate() && !J2.get_checkmate() && dont_quit){
        if (tour%2==0){

            cout << "white to move" << endl;
            dont_quit = click_move(J1,c1,c2);
            while (!(J1.bouge(b.get(c1), c2)) && dont_quit){
                dont_quit = click_move(J1,c1,c2);
            }
        }
        else{

            cout << "black to move" << endl;
            dont_quit = click_move(J2,c1,c2);
            while (!(J2.bouge(b.get(c1), c2)) && dont_quit){
                dont_quit = click_move(J2,c1,c2);
            }
        }
        ++tour;        
    }


    Window w2 = openWindow(400, 200, "Chess by Nexus&cie");
    setActiveWindow(w2);
    drawString(30, 60, "We hope you enjoyed playing with this GUI", BLACK);
    drawString(30, 90, "See you later!", BLACK);
    drawString(20, 140, "Please click on this window to proceed to the GUI.", BLACK,12, 0, false, true);
    drawString(120, 180, "Coded by Maximin Duvillard and Nicolas Birac", BLACK, 10, 0, true);
    drawString(200, 195, "© - 2019", BLACK, 10, 0, true);

    endGraphics();

    cout << "Quit successfully" << endl;

    return 0;
}
