/*
Spieler1 = 1
Spieler2 = 2
*/

#include <iostream>     
#include <vector> 
#include <string>
#include <sstream>   

class Piece{
    public:
        int position_x;
        int position_y;
        int player_id;

    Piece(int posx, int posy, int PlayerID) : position_x(posx), position_y(posy), player_id(PlayerID){}
};

//Gameboard hat eine 10x10 matrix, board[x,y]= -1 ist ein weißes Feld, 0 ist ein freies schwarzes, 1 ist mit einem Piece von Player1 und 2 ist mit einem Piece von Player 2 besetzt
//x ist vertikal, y ist horizontal

class GameBoard {
    public:
    
        int board[10][10];                                      //10x10 Bret
        std::vector<Piece> Piece_Vektor;                        //Vektor, der alle pieces abspeichert -> pieces.push_back()

        void SetUpBoard () {                                    //Board wird initialisiert/gebaut
            for (int i = 0; i < 10; ++i) {                      //durch jedes Element des äußeren Array iterieren
                for (int j = 0; j < 10; ++j) {                  //durch jedes innere Array iterieren
                    if (((i + j) % 2) == 1){                    //für jedes Schwarze Feld
                        if (i < 3){                             //falls spieler oben (2)
                            Piece_Vektor.push_back(Piece(i,j,2));      //erstelle ein Piece
                            board[i][j] = 2;}                   //markiere Feld mit 2
                        else if(i > 6){                         //falls Spieler unten(1)
                            Piece_Vektor.push_back(Piece(i,j,1));
                            board[i][j] = 1;
                            }
                        else {
                            board[i][j] = 0;}                   //leere Felder = 0
                    }
                    else {
                        board[i][j] = -1;                       //weiße/unspielbare Felder mit -1 markieren
        }}}}

        void printBoard () {                                    //Board ausgeben
            for (int i = 0; i < 10; ++i) {                      //außen
                for (int j = 0; j < 10; ++j) {                  //innen
                    if (board[i][j] == -1){
                        std::cout << "x ";}
                    else if (board[i][j] == 0){
                        std::cout << 0 << " ";}
                    else {
                        for (const auto& piece : Piece_Vektor) {    //checke alle Pieces in Piece Vektor
                        if (piece.position_x == i && piece.position_y == j) {   //ob das Piece auf Feld ij ist
                            std::cout << piece.player_id << " ";    //falls ja, gib ID aus
                    }}}}
                    std::cout << std::endl;
        }}

        bool isValidMove (int PlayerID, int x, int y, int x2, int y2) { //x,y sind die aktuellen Koordinaten, x2,y2 sind die Zielkoordinaten
            if (board[x][y] != PlayerID) {                      //gehört die Figur dem Spieler?
                return false;
            }
            else if (board[x2][y2] != 0) {                      //ist das Zielfeld frei?
                return false;
            }
            else {
                return true;
            }
        }

        void movePiece (int PlayerID, int x, int y, int x2, int y2) {   
            if (isValidMove(PlayerID, x, y, x2, y2) == true){
                for (auto& piece : Piece_Vektor) {    //checke alle Pieces in Piece Vektor
                        if (piece.position_x == x && piece.position_y == y && piece.player_id == PlayerID) {   //ob das Piece auf Feld xy ist
                            piece.position_x = x2;
                            piece.position_y = y2;
                            board[x][y] = 0;
                            board[x2][y2] = PlayerID;
                        }
        }}}
        

};



int main(){
    int Zug = 0;
    GameBoard game;
    game.SetUpBoard();
    while (true){
        Zug += 1;
        game.printBoard();

        std::cin.clear();                           //bevor eine Benutzereingabe eingegeben wird, wird der Eingabepuffer geleert, um falsche Eingaben zu vermeiden
        std::string BenutzerEingabe;
        std::getline(std::cin, BenutzerEingabe);    //liest gesamte Zeil ein und speichert in benutzerEingabe

        char startChar, endChar;                    //Es gibt insgesamt 4 Werte, diese werden eingeteilt in:
        int startInt, endInt;                       //Start und End Charackter (sprich B aus B6) und Start und End int (sprich 6 aus B6)

        std::istringstream ss(BenutzerEingabe);     //Der String wird in einen Stream??(als käme er aus std::cin) umgewandelt
        ss >> startChar >> startInt >> endChar >> endInt; // Der Stream wird in die 4 Werte aufgeteilt

        std::cout << "Startpunkt: " << startChar << startInt << std::endl;
        std::cout << "Endpunkt: " << endChar << endInt << std::endl;

        int x = 10 - startInt;                      // 1 wird zu 9 etc, da arrays oben bei 0 starten, jedoch das oberste Feld 10 sein soll
        int y = startChar - 'A';                     // A wird zu 0, J wird zu 9 etc
        int x2 = 10 - endInt;                       // funktioniert, da die Buchstaben zu ihren ASCII-Werten umgerechnet werden, dadurch rechnet man z.B bei 'J' - 'A' 74 - 65 = 9 
        int y2 = endChar - 'A';  

        std::cout << "Werte als Zahlen: " << x << " " << y << " : " << x2  << " " << y2 << std::endl;
    }
}
