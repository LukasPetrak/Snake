// snake.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <iomanip> 
#include <stdlib.h>
#include <time.h>
#include <string>


using namespace std;
enum objects { EMPTY, WALL, SNAKE_HEAD, SNAKE_TAIL, FRUIT };
objects** Map;
const int m_x = 50;
const int m_y = 25;

HANDLE cHandle = GetStdHandle(STD_OUTPUT_HANDLE);

vector<int>score = { 10, 30, 50 };
vector<string>player = { "C", "B", "A" };
string name;
int collision = 0;
int ChangeScore = 0;

#define black 0
#define dark_blue 1
#define dark_green 2
#define dark_cyan 3
#define dark_red 4
#define dark_magenta 5
#define dark_yellow 6
#define grey 7
#define dark_grey 8
#define blue 9
#define green 10
#define cyan 11
#define red 12
#define magenta 13
#define yellow 14
#define white 15



void colored_cout(int background_color, int foreground_color, string text)
{
    SetConsoleTextAttribute(cHandle, background_color * 16 + foreground_color);//nová barva
    cout << text;
    SetConsoleTextAttribute(cHandle, grey);//nastavení původní (základní) barvy
}

void colored_cout(int background_color, int foreground_color, int number)
{
    SetConsoleTextAttribute(cHandle, background_color * 16 + foreground_color);//nová barva
    cout << number;
    SetConsoleTextAttribute(cHandle, grey);//původní barva
}




class GameObject
{
public: virtual void update() = 0;
};

class Snake : public GameObject
{
public:
    void update();

private:
    // Startovní pozice hada.
    int _x = 10, _y = 3;
    int _x_old = 0, _y_old = 0;
    int _direction = 2; // Směr pohybu. 
    int _tsize = 0;
    int min = 0;
    int row = 0;
    int col = 0;
    int tillHighScore = score[2];

    //PRIVATE STRUCT
    struct Tail
    {
        int x;
        int y;
    };
    
    vector<Tail>tails; //ARRAY OR TAILS

    //HELPER METHODS
    void _KeyBoard();
    void _MoveSnake();
    void _AddTail();
    bool _Ate();
    void _MoveTail();
    void _Collision();
};

void Snake::_KeyBoard()
{
    /*
        1                up(W)72
    3       4     left(A)75    right(D)77
        2               down(S)80
    */

    if (_kbhit()) //Vráti nenulovou hodnotu, pokud je klávesa zmáčknuta
    {
        char input = _getch(); //Vrátí hodnotu znaku
        input = toupper(input);    
        if (input == 'W') _direction = 1;
        else if (input == 'S') _direction = 2;
        else if (input == 'A') _direction = 3;
        else if (input == 'D') _direction = 4;

    }
}

void Snake::_MoveSnake()
{
    //Pohyb hada
    _x_old = _x;
    _y_old = _y;

    switch (_direction){
        case 1: { _y--; break;}
        case 2: { _y++; break;}
        case 3: { _x--; break;}
        case 4: { _x++; break;}
    }

    //Umístění hada
    Map[_y_old][_x_old] = EMPTY;
    Map[_y][_x] = SNAKE_HEAD;
}

void Snake::_AddTail()
{
    if (_tsize == 0)
    {
        for (int i = 0; i < 2; i++)
        {
            //Přídá 2 konce k hadovi
            tails.push_back(Tail());
            tails[i].x = 10 - i;
            tails[i].y = 10;

            //Přídání ocasu do mapy
            Map[tails[i].y][tails[i].x] = SNAKE_TAIL;
            _tsize++;
        }
    }

    if (_Ate())
    {
        //Přídání ocasu
        tails.push_back(Tail());

        // Orientace a zvětšení ocasu o 1
        switch (_direction)
        {
            case 1:
            {
                tails[_tsize].x = tails[_tsize - 1].x;
                tails[_tsize].y = tails[_tsize - 1].y - 1;
                break;
            }
            case 2:
            {
                tails[_tsize].x = tails[_tsize - 1].x;
                tails[_tsize].y = tails[_tsize - 1].y + 1;
                break;
            }
            case 3:
            {
                tails[_tsize].x = tails[_tsize - 1].x + 1;
                tails[_tsize].y = tails[_tsize - 1].y;
                break;
            }
            case 4:
            {
                tails[_tsize].x = tails[_tsize - 1].x - 1;
                tails[_tsize].y = tails[_tsize - 1].y;
                break;
            }
        }
        ChangeScore += 10;
        _tsize++;
    }
}

bool Snake::_Ate()
{
    // Snězení ovoce
    if (_direction == 1 && Map[_y - 1][_x] == FRUIT)
    {
        return true;
    }
    else if (_direction == 2 && Map[_y + 1][_x] == FRUIT)
    {
        return true;
    }
    else if (_direction == 3 && Map[_y][_x - 1] == FRUIT)
    {
        return true;
    }
    else if (_direction == 4 && Map[_y][_x + 1] == FRUIT)
    {
        return true;
    }

    return false;
}

void Snake::_MoveTail()
{
    //Zadní ocas se smaže
    Map[tails[_tsize - 1].y][tails[_tsize - 1].x] = EMPTY;

    //Posunutí ocasu(Zvětšení)

    for (int i = _tsize - 1; i != 0; i--)
    {
        tails[i].x = tails[i - 1].x;
        tails[i].y = tails[i - 1].y;
    }
    //Přídá ocas na pozici hlavy
    tails[0].x = _x_old;
    tails[0].y = _y_old;

    //Nastavení polohy orientace ocasu
    Map[_y_old][_x_old] = SNAKE_TAIL;
}

void Snake::_Collision()
{
    //Kolize se zdí
    if ((_x == 1 || _x == 49) or (_y == -1 || _y == 24) or 
        (_direction == 1 && Map[_y - 1][_x] == WALL || _direction == 2 && Map[_y + 1][_x] == WALL))
    {
        collision = 1;
    }

    //Kolize s hadem(ocas)
    if ((_direction == 1 && Map[_y - 1][_x] == SNAKE_TAIL) or 
        (_direction == 2 && Map[_y + 1][_x] == SNAKE_TAIL) or
        (_direction == 3 && Map[_y][_x - 1] == SNAKE_TAIL) or
        (_direction == 4 && Map[_y][_x + 1] == SNAKE_TAIL))
    {
        collision = 2;
    }
 }

//Aktualizace všech věcí
void Snake::update()
{
    _KeyBoard();
    _AddTail();
    _Collision();
    _MoveSnake();
    _MoveTail();
}



class Fruit : public GameObject
{
public:

    void update();

private:
    int _x;
    int _y;

    //HELPER METHODS
    void _spawnFruit();
    bool _check_for_fruit();
};

void Fruit::_spawnFruit()
{
    int x, y;
    x = ((double)rand() / RAND_MAX) * (28 - 3) + 3;
    y = ((double)rand() / RAND_MAX) * (18 - 3) + 3;

    Map[y][x] = FRUIT;
}

bool Fruit::_check_for_fruit()
{
    for (int i = 1; i < m_y - 1; i++)
    {
        for (int j = 1; j < m_x - 1; j++)
        {
            if (Map[i][j] == FRUIT)
            {
                return true;
            }
        }
    }
    return false;
}

void Fruit::update()
{
    if (!_check_for_fruit())
    {
        _spawnFruit();
    }
}

void Scene(vector<GameObject*>& updates)
{
    if (updates.size() == 0)
    {
        updates.push_back(new Snake());
        updates.push_back(new Fruit());
    }

    for (vector<GameObject*>::iterator itr = updates.begin(), end = updates.end(); itr != end; itr++)
    {
        (*itr)->update();
    }
}

void draw()
{
    //REFRESH SCREEN  
    COORD cur = { 0,0 };
    SetConsoleCursorPosition(cHandle, cur);

       

    //Vytvoření herního pole
    for (int i = 0; i < m_y; i++){
        for (int j = 0; j < m_x; j++){
            if (Map[i][j] == WALL) cout << "*";
            else if (Map[i][j] == EMPTY) cout << " ";
            else if (Map[i][j] == SNAKE_HEAD) colored_cout(black, yellow, "0");
            else if (Map[i][j] == SNAKE_TAIL) colored_cout(black, green , "o");
            else if (Map[i][j] == FRUIT) colored_cout(black, cyan, "*");
        }
        cout << endl;
    }
}

string Read(string Path) {
    string data;
    ifstream infile;// open a file in read mode.
    infile.open(Path);
    infile >> data;
    infile.close();// close the opened file.
    return data;
}

bool Write(string Path, string data) {
    ofstream outfile;// open a file in write mode.
    outfile.open(Path);
    outfile << data;// write inputted data into the file.
    outfile.close(); // close the opened file.
    if (Read(Path).compare(data) == 0)  return 1;
    else return 0;
}

bool FifoExists(LPCTSTR szPath, bool Fifo)
{
    DWORD dwAttrib = GetFileAttributes(szPath);
    if (Fifo) {
        if (dwAttrib == FILE_ATTRIBUTE_ARCHIVE) return 1;
        else return 0;
    }
    else {
        if (dwAttrib == FILE_ATTRIBUTE_DIRECTORY) return 1;
        else return 0;
    }
}

string retchoose = "Y";

void getName(){

    do {
        std::system("cls"); // cls or clear
        std::cout << "Enter your name [max 8 char]: ";
        // std::cin.getline(name, MAX_NAME_LEN);
        getline(std::cin, name);
        name = name.substr(0, 8);
        std::cout << "Your name is: " << name << "\n";
        std::cout << "It's ok? [Y/N]";
        getline(std::cin, retchoose);
        if (retchoose[0] >= 97 && retchoose[0] <= 122)   retchoose[0] -= 32; // Změna na velké písmeno.
    } while (retchoose[0] != 'Y');
}

void getHiScore() {
    
    string path;
    char* pValue ;
    size_t len;
    errno_t err = _dupenv_s(&pValue, &len, "APPDATA");
    path = pValue + string("\\Snake");
    bool direx = FifoExists(path.c_str(), 0); // Kontrola adresáře:
    if (direx) {
        string path_file = path + "\\savefile.txt";
        bool filex = FifoExists((path_file).c_str(), 1);// Kontrola souboru.
        if (filex) {
            string s_score = Read(path_file);// Načte hodnoty do score.
            char poz;
            int ply = 0;
            bool pl_sc = true;

            string tmp_score = "";
            for (int i = 0; i < player.size(); i++) {
                player[i] = "";
            }
            for (int i = 0; i < s_score.size(); i++) {
                poz = s_score[i];
                if (poz == ':') {
                    pl_sc = false;
                }
                else if (poz == ';') {
                    score[ply] = stoi(tmp_score);
                    tmp_score = "";
                    ply++;
                    pl_sc = true;
                }
                else {
                    if (pl_sc) {
                        player[ply] += poz;
                    }
                    else {
                        tmp_score += poz;
                    }
                }
            }
        }
    }

}

bool saveHiScore() {
    string path;
    char* pValue;
    size_t len;
    errno_t err = _dupenv_s(&pValue, &len, "APPDATA");
    path = pValue + string("\\Snake");
    // Zápis do souboru.
    if (CreateDirectory(path.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
    {
        path += "\\savefile.txt";

        string wdata;
        for (int i = 0; i < player.size(); i++) {
            wdata += player[i] + ":";
            wdata += to_string(score[i]) + ";";
        }
        return Write(path, wdata);
    }
    else {
        std::cout << "Failed to create directory." << endl;
        return false;
    }

}

void setScoreBoard(int TotalScoreA, int NumberOfLives, int CurrScore) {

    int with_res = 10;
    int with_label = 30;
    
    for (int row = 1; row < 19; row++) {
        COORD cur = { 60, row };
        SetConsoleCursorPosition(cHandle, cur);

        switch (row) {
        case 1:
            cout << setw(with_label);
            colored_cout(black, red, "You are now playing as:");
            cout << setw(with_res);
            colored_cout(black, yellow, name);
            break;
        case 2:

            break;
        case 4:
            cout << setw(with_label);
            colored_cout(black, red, "Total score:");
            cout << setw(with_res);
            colored_cout(black, yellow, TotalScoreA);
            break;
        case 5:
            cout << setw(with_label);
            colored_cout(black, red, "To 1st place left:");
            cout << setw(with_res);
            colored_cout(black, yellow, score[2] - TotalScoreA);
            break;
        case 8:
            cout << setw(with_label + with_res);
            colored_cout(black, green, "    Best score ever      ");
            break;
        case 9:
            cout << setw(with_label + with_res);
            colored_cout(black, cyan, "===========================");
            break;
        case 10:
            cout << setw(with_label - 10);
            colored_cout(black, green, "1st place:");
            cout << setw(with_label - 20);
            colored_cout(black, yellow, player[2]);
            cout << setw(with_res);
            colored_cout(black, red, score[2]);
            break;
        case 11:
            cout << setw(with_label - 10);
            colored_cout(black, green, "2st place:");
            cout << setw(with_label - 20);
            colored_cout(black, yellow, player[1]);
            cout << setw(with_res);
            colored_cout(black, blue, score[1]);
            break;
        case 12:
            cout << setw(with_label - 10);
            colored_cout(black, green, "3st place:");
            cout << setw(with_label - 20);
            colored_cout(black, yellow, player[0]);
            cout << setw(with_res);
            colored_cout(black, magenta, score[0]);
            break;
        case 15:
            cout << setw(with_label + with_res);
            colored_cout(black, green, "     Current Session      ");
            break;
        case 16:
            cout << setw(with_label + with_res);
            colored_cout(black, cyan, "===========================");
            break;
        case 17:
            cout << setw(with_label);
            colored_cout(black, green, "Number of lives remaining:");
            cout << setw(with_res);
            colored_cout(black, blue, NumberOfLives);
            break;
        case 18:
            cout << setw(with_label);
            colored_cout(black, green, "Current score:");
            cout << setw(with_res);
            colored_cout(black, green, CurrScore);
            break;
        }
    }
}

void changeScoreBoard(int TotalScoreB){
    if (TotalScoreB > score[0] && TotalScoreB <= score[1]) {
        player[0] = name;
        score[0] = TotalScoreB;
    }
    if (TotalScoreB > score[1] && TotalScoreB <= score[2]) {
        player[1] = name;
        score[1] = TotalScoreB;
    }
    if (TotalScoreB > score[2]) {
        player[2] = name;
        score[2] = TotalScoreB;
    }
}

int main(){

    int TotalScore = 0;
    int CurrScore = 0;
    int NumberOfLives = 3;

    getName(); // Požadavek na jméno hráče.
    srand(time(0));// Vstupní hodnota pro rnd.
    getHiScore(); // Načte Hiscore ze souboru.
    // Skrytí kursoru.
    CONSOLE_CURSOR_INFO cinf;
    GetConsoleCursorInfo(cHandle, &cinf);
    cinf.bVisible = false;
    SetConsoleCursorInfo(cHandle, &cinf);

    do {
        collision = 0;
        std::system("cls"); // cls or clear
        
        Map = new objects * [m_y];
        for (int i = 0; i < m_y; i++) {
            Map[i] = new objects[m_x];
        }

        for (int i = 0; i < m_y; i++) {
            for (int j = 0; j < m_x; j++) {
                if (i == 0 || i == m_y - 1 || j == 0 || j == m_x - 1) {
                    Map[i][j] = WALL;
                }
                else {
                    Map[i][j] = EMPTY;
                }
            }
        }
        
        vector<GameObject*>updates; //Update seznamu objektů třídy object
        
        int tmpChangeScore = -10;
        while (!collision)  //Hlavní smyčka
        {
            draw();
            Scene(updates);
            if(ChangeScore != tmpChangeScore){
                tmpChangeScore = ChangeScore;
                TotalScore += ChangeScore;
                CurrScore += ChangeScore;
                ChangeScore = 0;
                setScoreBoard(TotalScore, NumberOfLives, CurrScore);
            }
            Sleep(100);
        }
        NumberOfLives--; // odečte život.
        setScoreBoard(TotalScore, NumberOfLives, CurrScore);

        // pokud již žádný není nabídne konec nebo novou hru.
        if (!NumberOfLives){
            changeScoreBoard(TotalScore);
            setScoreBoard(TotalScore, NumberOfLives, CurrScore);
            
            COORD cur_two = { 80, 25 };
            SetConsoleCursorPosition(cHandle, cur_two);
            colored_cout(black, red, "YOU SUCK, GET BETTER!");
            cout << endl;
            std::cout << setw(20) << "Znovu ? [Y/N]";
            getline(std::cin, retchoose);
            if (retchoose[0] >= 97 && retchoose[0] <= 122)   retchoose[0] -= 32; // Změna na velké písmeno.
            if (retchoose[0] != 'N') {
                NumberOfLives = 3;
                TotalScore = 0;
                CurrScore = 0;
            }
        }
        else {
            CurrScore = 0;
        }
        
    } while (retchoose[0] != 'N');

    
    
    COORD cur_three = { 80, 27 };
    SetConsoleCursorPosition(cHandle, cur_three);
    if (!saveHiScore()) std::cout << setw(20) << "Score not saved !" <<endl<<endl; // Zápis score do souboru.
    colored_cout(dark_green, yellow, "This is the end");

    std::cin.get();
    return 0;
    
}