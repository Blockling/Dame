#include <iostream>   
#include <array>  
#include <vector> 
#include <string>
#include <sstream>          //benötigt, um die Eingabe in Einzelteile zu formatieren
#include <cmath>
#include <iomanip>          //benötigt für setw() -> um die Verrückunbg der 10er Splate zu verhindern

class GameBoard {
    public:
        
        std::array<std::array<int, 10>, 10> board;
        std::vector<std::array<std::array<int, 10>, 10>> boardSave; 
        int piecesSlayn = 0;

        void SetUpBoard () {                                    
            for (int i = 0; i < 10; ++i) {                     
                for (int j = 0; j < 10; ++j) {
                    if (((i + j) % 2) == 1){
                        if (i < 3){board[i][j] = 2;}
                        else if(i > 6){board[i][j] = 1;}
                        else {board[i][j] = 0;}
                    }
                    else {board[i][j] = -1; }                                     //weiße/unspielbare Felder werden mit -1 markiert
                }
            }
            saveBoard();
        }

        void printBoard () {
            std::cout << "\n" << "    A  B  C  D  E  F  G  H  I  J" << std::endl;
            for (int i = 0; i < 10; ++i) {
                std::cout << std::setw(2) << 10 - i << " ";                     //setw verhindert die Verrückung durch 2-stellige Zahlen und das Unicode Symbol ⬛ und macht das Board übersichtlicher
                for (int j = 0; j < 10; ++j) {
                    if (board[i][j] == -1){
                        std::cout << std::setw(2) << "⬛ ";}
                    else if (board[i][j] == 0){
                        std::cout << std::setw(2) << 0 << " ";}
                    else {
                        std::cout << std::setw(2) << board[i][j] << " ";}
                }
                std::cout << std::endl;
            }
            std::cout << "Es wurden schon insgesamt " << piecesSlayn << " Figuren geschlagen" << std::endl;
        }

        bool isValidInput (int start_X, int start_Y, int final_X, int final_Y){
            if ((start_X < 10 && start_Y < 10 && final_X < 10 && final_Y < 10) && 
                (start_X >= 0 && start_Y >= 0 && final_X >= 0 && final_Y >= 0) &&
                (board[start_X][start_Y] != -1) && (board[final_X][final_Y] != -1)) {
                return true;
            }
            else {
                std::cout << "Ungültige Eingabe! Achte Darauf dieses Format zu verwenden:\nGroßbuchstabe,Integer Großbuchstabe,Integer. z.B: C3 D4\n";
                return false;
            }
        }

        bool isValidMove (int PlayerID, int start_X, int start_Y, int final_X, int final_Y) {
            if (!isValidInput(start_X, start_Y, final_X, final_Y)){
                return false;
            }
            else if (board[start_X][start_Y] != PlayerID) {
                std::cout << "Das Feld gehört nicht Spieler " << PlayerID << std::endl;
                return false;
            }
            else if (board[final_X][final_Y] != 0 || board[final_X][final_Y] == -1) {
                std::cout << "Das Zielfeld ist kein freies, schwarzes Feld!\n";
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
            std::cout << "Das ist ein ungültiger Zug, versuche es nochmal! \n";
            return false;
        }

        void movePiece (int PlayerID, int start_X, int start_Y, int final_X, int final_Y) { 
            board[start_X][start_Y] = 0;
            board[final_X][final_Y] = PlayerID;
        }

        void killPiece (int PlayerID, int start_X, int start_Y, int final_X, int final_Y) {
            int middle_X = (start_X + final_X) / 2;
            int middle_Y = (start_Y + final_Y) / 2;
            piecesSlayn += 1;
            board[middle_X][middle_Y] = 0;
        }

        void saveBoard() {
            boardSave.push_back(board);
        }

        void previousMove() {
            if (boardSave.size() > 1) {
                boardSave.pop_back();
                board = boardSave.back();
                std::cout << "Vorherriger Spielstand geladen!\n";
            }
            else {
                std::cout << "Es existiert kein früherer Spielstand!\n";
            }
        }
};

int main(){
    int Zug = 1;
    int PlayerToMove;
    GameBoard game;
    game.SetUpBoard();

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
        std::string BenutzerEingabe;
        std::getline(std::cin, BenutzerEingabe);

        if (BenutzerEingabe == "-") {
            game.previousMove();
            Zug -= 1;
            continue;
        }

        char startChar, endChar;
        int startInt, endInt;

        std::istringstream ss(BenutzerEingabe);                                     //String wird in Stream umgewandelt und aufgeteilt
        ss >> startChar >> startInt >> endChar >> endInt;

        int start_X = 10 - startInt;
        int start_Y = startChar - 'A';                                              //hier wird mit ASCII-Werten gerechnet
        int final_X = 10 - endInt;
        int final_Y = endChar - 'A';  

        if (!game.isValidMove(PlayerToMove, start_X, start_Y, final_X, final_Y)) {

            continue;
        }
        else {std::cout << "Gültiger Zug! Wird ausgeführt...\n";}
        game.movePiece(PlayerToMove, start_X, start_Y, final_X, final_Y);
        game.saveBoard();
        Zug += 1;
    }
}
