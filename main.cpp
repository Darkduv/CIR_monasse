#include <iostream>
using namespace std;
#include "case.h"
#include "joueur.h"
#include "piece.h"
#include "plateau.h"
#include "graphics.h"
#include "joueur.h"

int main()
{
    display_grid_empty();
    load_all_pieces();
    Plateau b;
    Joueur J1(b,1);
    Joueur J2(b,0);
    J1.set_other_player(&J2);
    J2.set_other_player(&J1);
    b.set_joueur(&J1,&J2);
    display_board(b);
    Case c1, c2;
    int tour=0;
    // il faut changer la fonciton permission mange pour pouvoir l'appeler sur uen case vide pour simuler la présence de la piece sans la déplacer
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
