#include <iostream>   
#include <array>  
#include <vector> 
#include <string>
#include <sstream>          //benötigt, um die Eingabe in Einzelteile zu formatieren
#include <cmath>
#include <iomanip>          //benötigt für setw() -> um die Verrückunbg der 10er Splate zu verhindern


/*
*   @class Gameboard
*/
class GameBoard {
    public:
        
        std::array<std::array<int, 10>, 10> board;
        std::vector<std::array<std::array<int, 10>, 10>> boardSave; 
        int piecesSlayn = 0;

        /*
        *   @brief setzt das Board auf den Anfangszustand
        *   @pre board-array muss definiert sein
        */
        void SetUpBoard () {                                    
            for (int i = 0; i < 10; ++i) {                     
                for (int j = 0; j < 10; ++j) {
                    if (((i + j) % 2) == 1){
                        if (i < 3){board[i][j] = 2;}
                        else if(i > 6){board[i][j] = 1;}
                        else {board[i][j] = 0;}
                    }
                    else {board[i][j] = -1; }                                       //weiße/unspielbare Felder werden mit -1 markiert
                }
            }
            saveBoard();
        }

        /*
        *   @brief Gibt den aktuellen Stand des Spiels aus
        *   @pre alle Felder des Boards müssen einen Wert haben
        */
        void printBoard () {
            std::cout << "\n" << "    A  B  C  D  E  F  G  H  I  J" << std::endl;
            for (int i = 0; i < 10; ++i) {
                std::cout << std::setw(2) << 10 - i << " ";                         //setw verhindert die Verrückung durch 2-stellige Zahlen und das Unicode Symbol ⬜ und macht das Board übersichtlicher
                for (int j = 0; j < 10; ++j) {          
                    if (board[i][j] == -1){
                        std::cout << std::setw(2) << "⬜ ";}                       //anstatt von -1 wird ⬜ für unspielbare Felder ausgegeben
                    else if (board[i][j] == 0){
                        std::cout << std::setw(2) << 0 << " ";}
                    else {
                        std::cout << std::setw(2) << board[i][j] << " ";}
                }
                std::cout << std::endl;
            }
            std::cout << "Es wurden schon insgesamt " << piecesSlayn << " Figuren geschlagen" << std::endl;
        }

        /*
        *   @brief Überprüft ob die Eingabe gültig ist:
        *          erster und zweiter Buchstabe im Bereich A-J und erste, zweite Zahl 1-10
        *          eingegebene Felder dürfen keine weißen Felder sein
        *   @param start_X Koordinate X von dem zu bewegenden Spielstein
        *   @param start_Y Koordinate Y von dem zu bewegenden Spielstein
        *   @param final_X Koordinate X von dem Zielfeld
        *   @param final_Y Koordinate Y von dem Zielfeld
        *   @pre Die Eingabe muss von dem Inputsream in 4 integers aufgeteilt und umgerechnet sein
        *   @returns true, falls die Eingabe regelgerecht ist und false sonst
        */
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

        /*
        *   @brief Überprüft ob der Zug Regelgerecht ist
        *          legal: auf den Startkoordinaten liegt ein Stein des Spielers und die Endkoordinaten sind ein freies, 
        *          schwarzes Feld. Der Zug muss eine Länge von 1 oder 2 in die richtige Richtung haben
        *   @param PlayerId Die Id des Spielers, der gerade am Zug ist
        *   @param start_X Koordinate X von dem zu bewegenden Spielstein
        *   @param start_Y Koordinate Y von dem zu bewegenden Spielstein
        *   @param final_X Koordinate X von dem Zielfeld
        *   @param final_Y Koordinate Y von dem Zielfeld
        *   @pre Die Eingabe muss von dem Inputsream in 4 integers aufgeteilt und umgerechnet sein
        *   @returns true, falls der Zug legal ist
        *           (auf den Startkoordinaten ein Stein des Spielers liegt und die Endkoordinaten ein freies, schwarzes Feld sind 
        *           und Der Zug diagonaler Zug oder Schlag in die richtige Richtung ist.
        *           false, falls der Zug nicht legal ist.
        */
        bool isValidMove (int PlayerId, int start_X, int start_Y, int final_X, int final_Y) {
            if (!isValidInput(start_X, start_Y, final_X, final_Y)){
                return false;
            }
            else if (board[start_X][start_Y] != PlayerId) {
                std::cout << "Das Feld gehört nicht Spieler " << PlayerId << std::endl;
                return false;
            }
            else if (board[final_X][final_Y] != 0 || board[final_X][final_Y] == -1) {
                std::cout << "Das Zielfeld ist kein freies, schwarzes Feld!\n";
                return false;
            }
            else if (((PlayerId == 1) && ((start_X-final_X)==1) && (std::abs(start_Y-final_Y)==1)) ||
                    ((PlayerId == 2) && ((start_X-final_X)==-1) && (std::abs(start_Y-final_Y)==1))) {
                return true;
            }
            else if (((PlayerId == 1) && ((start_X-final_X)==2) && (std::abs(start_Y-final_Y)==2)) ||
                    ((PlayerId == 2) && ((start_X-final_X)==-2) && (std::abs(start_Y-final_Y)==2))) {
                int middle_X = (start_X + final_X) / 2;
                int middle_Y = (start_Y + final_Y) / 2;
                if ((board[middle_X][middle_Y] != 0) &&
                    (board[middle_X][middle_Y] != PlayerId)) {
                        killPiece(PlayerId, start_X, start_Y, final_X, final_Y);
                        return true;
                    }
            }
            std::cout << "Das ist ein ungültiger Zug, versuche es nochmal! \n";
            return false;
        }

        /*
        *   @brief "Bewegt" einen Spielstein von den Start- zu den Endkoordinaten
        *   @param PlayerId Die Id des Spielers, der gerade am Zug ist
        *   @param start_X Koordinate X von dem zu bewegenden Spielstein
        *   @param start_Y Koordinate Y von dem zu bewegenden Spielstein
        *   @param final_X Koordinate X von dem Zielfeld
        *   @param final_Y Koordinate Y von dem Zielfeld
        */
        void movePiece (int PlayerId, int start_X, int start_Y, int final_X, int final_Y) { 
            board[start_X][start_Y] = 0;
            board[final_X][final_Y] = PlayerId;
        }

        /*
        *   @brief Entfernt den geschlagenen Stein und addiert 1 zu piecesSlayn
        *   @param PlayerId Die Id des Spielers, der gerade am Zug ist
        *   @param start_X Koordinate X von dem zu bewegenden Spielstein
        *   @param start_Y Koordinate Y von dem zu bewegenden Spielstein
        *   @param final_X Koordinate X von dem Zielfeld
        *   @param final_Y Koordinate Y von dem Zielfeld
        */
        void killPiece (int PlayerId, int start_X, int start_Y, int final_X, int final_Y) {
            int middle_X = (start_X + final_X) / 2;
            int middle_Y = (start_Y + final_Y) / 2;
            piecesSlayn += 1;
            board[middle_X][middle_Y] = 0;
        }

        /*
        *   @brief spreichert den aktuellen Stand des Boards in einem Vektor
        */
        void saveBoard() {
            boardSave.push_back(board);
        }

        /*
        *   @brief stellt des vorherigen Stand des Boards wieder da
        */
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