#include <iostream>   
#include <array>  
#include <vector> 
#include <string>
#include <sstream>          //benötigt, um die Eingabe in Einzelteile zu formatieren
#include <cmath>
#include <iomanip>          //benötigt für setw()


/*
*   @class Die Klasse gameBoard beinhaltet das Brett und is dafür zuständing, Eingaben und Züge zu validieren, durchzuführen und anzuzeigen.
*   
*   Diese Klasse beinhaltet alle nötigen Funktionen für das gesamte Spiel. Dazu gehören: eine Funktion die das Board auf den Anfangszustand setzt, das Board ausgibt, 
*   Inputs validiert, Züge überprüft, Züge und Schläge ausführt und mitzählt wie viele Steine geschlagen wurden, Spielstände speichert und auch auf frühere Spielstände zurückgeht.
*
*   "Alle weiteren ublichen Regeln finden für diese Aufgabe keine Anwendung."
*   Mögliche Erweiterungen wären: Eine Menüführung, mitzählen von Punkten von den Spielern, eine Win-Condition, Auto-Moves von dem Computer und natürlich die Damen-Figur.
*
*   @author Nikita Saburdjajew
*   @date   09/02/2025
*/
class gameBoard {
    public:
        
        std::array<std::array<int, 10>, 10> board;
        std::vector<std::array<std::array<int, 10>, 10>> boardSave; 
        int piecesSlayn = 0;

        /*
        *   @brief setzt das Board auf den Anfangszustand
        *   @pre board-array muss deklariert sein
        *       Diese Funktion iteriert durch alle Arrays, also durch jedes Array welches element des äußeren Arrays ist und setzt regelgerecht einen integer Wert an jede Stelle
        *       Da nur jedes zweite Feld schwarz ist, wird mit modulo von 2 gearbeitet und die weißen Felder werden gleich 0 gesetzt
        *       Die schwarzen Felder der obersten drei Reihen werden gleich 2 gestezt, die unteren 3 werden gleich 1 gesetzt und der Rest wird gleich 0 gesetzt
        */
        void setUpBoard () {                                    
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
        *   @bug Das Unicode Symbol "⬜" (weißes Feld) wird auf manchen Systemen nicht richtif dargestellt
        *   @bug trotz der setw()-funktion, die die breite der Felder kontrollieren soll, passiert dies auf einigen Systemen nicht korrekt und jede 2te Spalte verrutscht unnatürlich
        *       Diese Funktion ist ähnlich aufgebaut wie setUpBoard, da sie auch durch alle arrays iteriert. Der Unterschied ist, das diese Funktion nur überprüft welche Werte die Felder haben und diese dann ausgibt
        *       Für bessere Übersicht hat diese Funktion auch eine Legende, die oben alle Charactere von A-J und an den Seiten alle Zahlen 1-10 anzeigt, damit die Spieler ihre Züge leichter eingeben können
        *       Mit der setw() funktion sollten alle Spalten untereinander gerade sein
        */
        void printBoard () {
            std::cout << std::setw(3) << " ";                                      //setw verhindert die Verrückung durch 2-stellige Zahlen und das Unicode Symbol ⬜ und macht das Board übersichtlicher
            for (char c = 'A'; c <= 'J'; ++c) {                     
                std::cout << std::setw(3) << c;                              
            }
            std::cout << std::endl;
            for (int i = 0; i < 10; ++i) {
                std::cout << std::setw(3) << 10 - i;
                for (int j = 0; j < 10; ++j) {          
                    if (board[i][j] == -1){
                        std::cout << std::setw(3) << " ⬜";}                       //anstatt von -1 wird ⬜ für unspielbare Felder ausgegeben
                    else if (board[i][j] == 0){
                        std::cout << std::setw(3) << 0;}
                    else {
                        std::cout << std::setw(3) << board[i][j];}
                }
                std::cout << std::endl;
            }
            std::cout << "\nEs wurden schon insgesamt " << piecesSlayn << " Figuren geschlagen" << std::endl;
        }

        /*
        *   @brief Überprüft ob die Eingabe gültig ist.
        *   @param start_X Koordinate X von dem zu bewegenden Spielstein
        *   @param start_Y Koordinate Y von dem zu bewegenden Spielstein
        *   @param final_X Koordinate X von dem Zielfeld
        *   @param final_Y Koordinate Y von dem Zielfeld
        *   @pre Die Eingabe muss von dem Inputsream in 4 integers aufgeteilt und umgerechnet sein
        *   @returns true, falls die Eingabe regelgerecht ist und false sonst
        *       Diese Funktion überprüft, ob die Eingabe regelgerecht ist, das heißt in diesem Fall:
        *       -erster und zweiter Buchstabe sind im Bereich A-J und erste, zweite Zahl ist im Bereich 1-10
        *       -eingegebene Felder dürfen keine weißen Felder sein
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
        *   @param playerId Die Id des Spielers, der gerade am Zug ist
        *   @param start_X Koordinate X von dem zu bewegenden Spielstein
        *   @param start_Y Koordinate Y von dem zu bewegenden Spielstein
        *   @param final_X Koordinate X von dem Zielfeld
        *   @param final_Y Koordinate Y von dem Zielfeld
        *   @pre Die Eingabe muss von dem Inputsream in 4 integers aufgeteilt und umgerechnet sein
        *   @returns true, falls der Zug legal ist
        *            false, falls der Zug nicht legal ist.
        *       Diese Funktion verbietet illegale Züge. Die Regeln sind wie folgt: 
        *       -auf den Startkoordinaten liegt ein Stein des Spielers 
        *       -die Endkoordinaten sind ein freies, schwarzes Feld
        *       -der Zug muss eine Länge von 1 oder 2 in die richtige Richtung (Player 1 nach oben, Player 2 nach unten) haben
        */
        bool isValidMove (int playerId, int start_X, int start_Y, int final_X, int final_Y) {
            if (!isValidInput(start_X, start_Y, final_X, final_Y)){
                return false;
            }
            else if (board[start_X][start_Y] != playerId) {
                std::cout << "Das Feld gehört nicht Spieler " << playerId << std::endl;
                return false;
            }
            else if (board[final_X][final_Y] != 0 || board[final_X][final_Y] == -1) {
                std::cout << "Das Zielfeld ist kein freies, schwarzes Feld!\n";
                return false;
            }
            else if (((playerId == 1) && ((start_X-final_X)==1) && (std::abs(start_Y-final_Y)==1)) ||
                    ((playerId == 2) && ((start_X-final_X)==-1) && (std::abs(start_Y-final_Y)==1))) {
                return true;
            }
            else if (((playerId == 1) && ((start_X-final_X)==2) && (std::abs(start_Y-final_Y)==2)) ||
                    ((playerId == 2) && ((start_X-final_X)==-2) && (std::abs(start_Y-final_Y)==2))) {
                int middle_X = (start_X + final_X) / 2;
                int middle_Y = (start_Y + final_Y) / 2;
                if ((board[middle_X][middle_Y] != 0) &&
                    (board[middle_X][middle_Y] != playerId)) {
                        killPiece(playerId, start_X, start_Y, final_X, final_Y);
                        return true;
                    }
            }
            std::cout << "Das ist ein ungültiger Zug, versuche es nochmal! \n";
            return false;
        }

        /*
        *   @brief "Bewegt" einen Spielstein von den Start- zu den Endkoordinaten
        *   @param playerId Die Id des Spielers, der gerade am Zug ist
        *   @param start_X Koordinate X von dem zu bewegenden Spielstein
        *   @param start_Y Koordinate Y von dem zu bewegenden Spielstein
        *   @param final_X Koordinate X von dem Zielfeld
        *   @param final_Y Koordinate Y von dem Zielfeld
        *       Diese Funktion sorgt dafür, dass bei regelgerechten Zügen ein Spielstein "bewegt" wird, also von der Startposition gelöscht und ind der Zielposition kreiert wird
        */
        void movePiece (int playerId, int start_X, int start_Y, int final_X, int final_Y) { 
            board[start_X][start_Y] = 0;
            board[final_X][final_Y] = playerId;
        }

        /*
        *   @brief Entfernt den geschlagenen Stein und addiert 1 zu piecesSlayn
        *   @param playerId Die Id des Spielers, der gerade am Zug ist
        *   @param start_X Koordinate X von dem zu bewegenden Spielstein
        *   @param start_Y Koordinate Y von dem zu bewegenden Spielstein
        *   @param final_X Koordinate X von dem Zielfeld
        *   @param final_Y Koordinate Y von dem Zielfeld
        *       Durch diese Funktion wird das Schlagen ermöglicht. Bei einem regelgerechten Schlag wird est diese Funktion und dann movePiece() aufgerufen
        *       Wenn aufgerufen, kalkuliert die Funktion den Mittelpunkt zwischen Anfangs- und EndKoordinaten eines Schlages und entfernt den generischen Stein, der auf dem Mittelpunkt liegen muss
        *       Danach wird die Variable piecesSlayn um 1 erhöht, damit der Computer mitzählen kann wie viele Spielsteine schon geschlagen wurden
        */
        void killPiece (int playerId, int start_X, int start_Y, int final_X, int final_Y) {
            int middle_X = (start_X + final_X) / 2;
            int middle_Y = (start_Y + final_Y) / 2;
            piecesSlayn += 1;
            board[middle_X][middle_Y] = 0;
        }

        /*
        *   @brief spreichert den aktuellen Stand des Boards in einem Vektor
        *       Der board-Array wird in dieser Funktion hinten an den boardSave-Vektor angehangen, um ein Speichern zu ermöglichen
        */
        void saveBoard() {
            boardSave.push_back(board);
        }

        /*
        *   @brief stellt des vorherigen Stand des Boards wieder da
        *       In dieser Funktion wird ewrst geprüft, ob der Spieler schon mindestens einen Zug getätigt hat und falls ja wird der Spielstand von dem vorherrigen Zug geladen.
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