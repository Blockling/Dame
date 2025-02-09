#include "header.hpp"

int main(){
    int Zug = 1;
    int PlayerToMove;
    gameBoard game;
    game.setUpBoard();
    std::cout <<"1 sind Figuren von Spieler 1, 2 sind Figuren von Spieler 2 und 0 sind freie Felder.\n" <<
                "Das Bewegen der Spielfiguren erfolgt duch eine Eingabe wie bei Schach, sprich C3 D4.\n" <<
                "Um einen früheren Spielstand zu laden, kann ein - eingegeben werden!\n\n";

    while (true){
        game.printBoard();

        if (Zug % 2 == 0){
            PlayerToMove = 2;
        }
        else {
            PlayerToMove = 1;
        }
        std::cout << "Dies ist der " << Zug << "te Zug" << std::endl;
        std::cout << "Spieler " << PlayerToMove << " ist am Zug\n";

        std::cin.clear();
        std::string benutzerEingabe;
        std::getline(std::cin, benutzerEingabe);

        if (benutzerEingabe == "-") {
            game.previousMove();
            Zug -= 1;
            continue;
        }

        char startChar, endChar;
        int startInt, endInt;

        //String wird in Stream umgewandelt und aufgeteilt
        std::istringstream ss(benutzerEingabe);
        ss >> startChar >> startInt >> endChar >> endInt;

        int start_X = 10 - startInt;
        int start_Y = startChar - 'A';
        int final_X = 10 - endInt;
        int final_Y = endChar - 'A';  

        if (!game.isValidMove(PlayerToMove, start_X, start_Y, final_X, final_Y)) {
            continue;
        }
        else {std::cout << "Gültiger Zug! Wird ausgeführt...\n\n";}
        game.movePiece(PlayerToMove, start_X, start_Y, final_X, final_Y);
        game.saveBoard();
        Zug += 1;
    }
}
