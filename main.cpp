#include "header.hpp"

int main(){
    int zug = 1;
    int playerToMove;
    gameBoard game;
    game.setUpBoard();
    std::cout <<"1 sind Figuren von Spieler 1, 2 sind Figuren von Spieler 2 und 0 sind freie Felder.\n" <<
                "Das Bewegen der Spielfiguren erfolgt duch eine Eingabe wie bei Schach, sprich C3 D4.\n" <<
                "Um einen früheren Spielstand zu laden, kann ein - eingegeben werden!\n\n";

    while (true){
        game.printBoard();

        if (zug % 2 == 0){
            playerToMove = 2;
        }
        else {
            playerToMove = 1;
        }
        std::cout << "Dies ist der " << zug << "te Zug" << std::endl;
        std::cout << "Spieler " << playerToMove << " ist am Zug\n";

        std::cin.clear();
        std::string userInput;
        std::getline(std::cin, userInput);

        if (userInput == "-") {
            game.previousMove();
            zug -= 1;
            continue;
        }

        char startChar, endChar;
        int startInt, endInt;

        //String wird in Stream umgewandelt und aufgeteilt
        std::istringstream ss(userInput);
        ss >> startChar >> startInt >> endChar >> endInt;

        int start_X = 10 - startInt;
        int start_Y = startChar - 'A';
        int final_X = 10 - endInt;
        int final_Y = endChar - 'A';  

        if (!game.isValidMove(playerToMove, start_X, start_Y, final_X, final_Y)) {
            continue;
        }
        else {std::cout << "Gültiger Zug! Wird ausgeführt...\n\n";}
        game.movePiece(playerToMove, start_X, start_Y, final_X, final_Y);
        game.saveBoard();
        zug += 1;
    }
}
