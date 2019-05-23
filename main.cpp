#include <iostream>
using namespace std;
#include "case.h"
#include "player.h"
#include "piece.h"
#include "board.h"
#include "graphics.h"
#include "player.h"

int main()
{
    // we load the pieces images in memory
    load_all_pieces();
    // Display of the borad
    display_grid_empty();

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
    // il faut changer la fonction permission mange pour pouvoir l'appeler sur uen case vide pour simuler la présence de la piece sans la déplacer
    while(!J1.get_checkmate() && !J2.get_checkmate()){
        if (tour%2==0){

            cout << "white plays" << endl;
            click_move(J1,c1,c2);
            while (!(J1.bouge(b.get(c1), c2))){
                click_move(J1,c1,c2);
            }
            b.affiche();
        }
        else{

            cout << "black plays" << endl;
            click_move(J2,c1,c2);
            while (!(J2.bouge(b.get(c1), c2))){
                click_move(J2,c1,c2);
            }
            b.affiche();
        }
        ++tour;
    }
    click();
    return 0;
}
