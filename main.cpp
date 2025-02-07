/*
Spieler1 = 1
Spieler2 = 2
*/

#include <iostream>     
#include <vector> 
#include <string>
#include <sstream>   
#include <cmath>
#include <iomanip>          //benötigt für setw() -> um die Verrückunbg der 10er Splate zu verhindern

//Gameboard hat eine 10x10 matrix, board[start_X,start_Y]= -1 ist ein weißes Feld, 0 ist ein freies schwarzes, 1 ist mit einem Piece von Player1 und 2 ist mit einem Piece von Player 2 besetzt
//start_X ist vertikal, start_Y ist horizontal

class GameBoard {
    public:
    
        int board[10][10];                                      //10x10 Bret

        void SetUpBoard () {                                    //Board wird initialisiert/gebaut
            for (int i = 0; i < 10; ++i) {                      //durch jedes Element des äußeren Array iterieren
                for (int j = 0; j < 10; ++j) {                  //durch jedes innere Array iterieren
                    if (((i + j) % 2) == 1){                    //für jedes Schwarze Feld
                        if (i < 3){board[i][j] = 2;}            //markiere Feld mit 2
                        else if(i > 6){board[i][j] = 1;}
                        else {board[i][j] = 0;}                 //leere Felder = 0
                    }
                    else {
                        board[i][j] = -1;                       //weiße/unspielbare Felder mit -1 markieren
                    }
                }
            }
        }

        void printBoard () {                                    //Board ausgeben
            std::cout << "   A B C D E F G H I J" << std::endl;
            for (int i = 0; i < 10; ++i) {                      //außen
                std::cout << std::setw(2) << 10 - i << " ";     //setw(2) setzt die width von jedem eintrag auf 2, dadurch wird verhindert dass die 10-er Reihe verrutscht weil die 10 2 stellen hat
                for (int j = 0; j < 10; ++j) {                  //innen
                    if (board[i][j] == -1){
                        std::cout << "x ";}
                    else if (board[i][j] == 0){
                        std::cout << 0 << " ";}
                    else {
                        std::cout << board[i][j] << " ";
                    }
                }
                    std::cout << std::endl;
            }
        }

        void killPiece (int PlayerID, int start_X, int start_Y, int final_X, int final_Y) {
            int middle_X = (start_X + final_X) / 2;
            int middle_Y = (start_Y + final_Y) / 2;
            std::cout << "Schlag\n";
            board[middle_X][middle_Y] = 0;
        }

        bool isValidInput (int start_X, int start_Y, int final_X, int final_Y){
            if ((start_X < 10 && start_Y < 10 && final_X < 10 && final_Y < 10) && 
                (start_X >= 0 && start_Y >= 0 && final_X >= 0 && final_Y >= 0) &&
                (board[start_X][start_Y] != -1)) {
                return true;
            }
            else {
                std::cout << "Ungültige Eingabe!" << std::endl;
                return false;
            }
        }

        bool isValidMove (int PlayerID, int start_X, int start_Y, int final_X, int final_Y) {
            if (!isValidInput(start_X, start_Y, final_X, final_Y)){
                return false;
            }
            if (board[start_X][start_Y] != PlayerID) {    
                std::cout << "Die Figur gehört nicht Spieler " << PlayerID << std::endl;
                return false;
            }
            else if (board[final_X][final_Y] != 0 || board[final_X][final_Y] == -1) {
                std::cout << "Das Zielfeld ist kein freies, schwarzes Feld!" << std::endl;
                return false;
            }
            else if (((PlayerID == 1) && ((start_X-final_X)==1) && (std::abs(start_Y-final_Y)==1)) ||
                    ((PlayerID == 2) && ((start_X-final_X)==-1) && (std::abs(start_Y-final_Y)==1))) {
                return true;
            }
            else if (((PlayerID == 1) && ((start_X-final_X)==2) && (std::abs(start_Y-final_Y)==2)) ||
                    ((PlayerID == 2) && ((start_X-final_X)==-2) && (std::abs(start_Y-final_Y)==2))) {
                int middle_X = (start_X + final_X) / 2;
                int middle_Y = (start_Y + final_Y) / 2;
                if ((board[middle_X][middle_Y] != 0) &&
                    (board[middle_X][middle_Y] != PlayerID)) {
                        killPiece(PlayerID, start_X, start_Y, final_X, final_Y);
                        return true;
                    }
            }
            return false;
        }

        void movePiece (int PlayerID, int start_X, int start_Y, int final_X, int final_Y) { 
            if (!isValidMove(PlayerID, start_X, start_Y, final_X, final_Y)){
                std::cout << "Ungültige Koordinaten" << std::endl;
                return;
            }
            board[start_X][start_Y] = 0;
            board[final_X][final_Y] = PlayerID;
        }
};

int main(){
    int Zug = 0;
    int PlayerToMove = 0;

    GameBoard game;
    game.SetUpBoard();

    while (true){

        Zug += 1;
        game.printBoard();

        if (Zug % 2 == 0){
            PlayerToMove = 2;
        }
        else {
            PlayerToMove = 1;
        }
        std::cout << "Spieler " << PlayerToMove << " ist am Zug" << std::endl;

        std::cin.clear();                           //bevor eine Benutzereingabe eingegeben wird, wird der Eingabepuffer geleert, um falsche Eingaben zu vermeiden
        std::string BenutzerEingabe;
        std::getline(std::cin, BenutzerEingabe);    //liest gesamte Zeil ein und speichert in benutzerEingabe

        char startChar, endChar;                    //Es gibt insgesamt 4 Werte, diese werden eingeteilt in:
        int startInt, endInt;                       //Start und End Charackter (sprich B aus B6) und Start und End int (sprich 6 aus B6)

        std::istringstream ss(BenutzerEingabe);     //Der String wird in einen Stream??(als käme er aus std::cin) umgewandelt
        ss >> startChar >> startInt >> endChar >> endInt; // Der Stream wird in die 4 Werte aufgeteilt

        std::cout << "Startpunkt: " << startChar << startInt << std::endl;
        std::cout << "Endpunkt: " << endChar << endInt << std::endl;

        int start_X = 10 - startInt;                      // 1 wird zu 9 etc, da arrays oben bei 0 starten, jedoch das oberste Feld 10 sein soll
        int start_Y = startChar - 'A';                     // A wird zu 0, J wird zu 9 etc
        int final_X = 10 - endInt;                       // funktioniert, da die Buchstaben zu ihren ASCII-Werten umgerechnet werden, dadurch rechnet man z.B bei 'J' - 'A' 74 - 65 = 9 
        int final_Y = endChar - 'A';  

        std::cout << "Valid Move? " << game.isValidMove(PlayerToMove, start_X, start_Y, final_X, final_Y) << std::endl;
        std::cout << "Werte als Zahlen: " << start_X << " " << start_Y << " : " << final_X  << " " << final_Y << std::endl;

        game.movePiece(PlayerToMove, start_X, start_Y, final_X, final_Y);

       
    }
}
