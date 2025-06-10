#include <iostream>
#include <string>
#include <limits>
#include <random>
#include <conio.h>
#include <sstream>
#include <thread>
#include <windows.h>

using namespace std;

int RNG(int min, int max) {
    unsigned int time_seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    unsigned int thread_seed = static_cast<unsigned int>(hash<std::thread::id>{}(std::this_thread::get_id()));
    random_device rd;

    seed_seq seed{ time_seed, thread_seed, rd() };
    mt19937 gen(seed);
    uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

int stringToInt(const string& str) {
    istringstream iss(str);
    int result;
    if (!(iss >> result)) {

        throw invalid_argument("Invalid input: " + str);
    }
    return result;
}

int height[3] = { 10, 20 ,30 };
int width[3] = { 40, 40, 40 };

string option;
int numberOption;
string speedOption;
int numberSpeedOption;
int speed;

int x = width[numberOption] / 2;
int y = height[numberOption] / 2;

int xapple = RNG(2, width[numberOption] - 1);
int yapple = RNG(2, height[numberOption] - 1);

int playerScore = 0;

string playerName;

int snakeLen;

int XBody[3600], YBody[3600];

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction Dir = STOP;

char layout;
bool isGameOver = false;

void choseName() {
    cout << "WelcomeEFASDF to Snake Game! What is your name?" << endl;
    getline(cin, playerName);
    if (playerName == "")
    {
        playerName = "Ermac";
    }
    system("cls");
}

void choseMap() {
    int flag1 = 0;
    while (flag1 == 0) {
        cout << "Chose size pls (1 - small, 2 - medium, 3 - large): ";
        cin >> option;
        numberOption = stringToInt(option);
        if (cin.fail() || numberOption < 1 || numberOption > 3) {

            cout << "Invalid input! Please enter a number between 1 and 3." << endl;
            cin.clear();
            system("pause");
            system("cls");
            continue;
        }
        flag1 = 1;
    }
}

void choseSpeed() {
    int flag1 = 0;
    while (flag1 == 0) {
        cout << "Chose speed pls (1 - slow, 2 - normal, 3 - fast): ";
        cin >> speedOption;
        numberSpeedOption = stringToInt(option);
        if (cin.fail() || numberSpeedOption < 1 || numberSpeedOption > 3) {
            cout << "Invalid input! Please enter a number between 1 and 3." << endl;
            cin.clear();
            system("pause");
            system("cls");
            continue;
        }
        flag1 = 1;
    }
    if (numberSpeedOption == 1)
        speed = 150;
    else if (numberSpeedOption == 2)
        speed = 250;
    else if (numberSpeedOption == 3)
        speed = 400;
}

void maplayout() {
    system("cls");

    for (int i = 0; i < width[numberOption] + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height[numberOption]; i++) {
        for (int j = 0; j <= width[numberOption]; j++) {

            if (j == 0 || j == width[numberOption])
                cout << "#";

            else if (i == y && j == x)
                cout << "O";

            else if (i == yapple && j == xapple)
                cout << "$";

            else {
                bool partOfBody = false;
                for (int k = 0; k < snakeLen; k++) {
                    if (XBody[k] == j && YBody[k] == i) {
                        cout << "o";
                        partOfBody = true;
                    }
                }
                if (!partOfBody)
                    cout << " ";
            }
        }
        cout << endl;
    }


    for (int i = 0; i < width[numberOption]; i++)
        cout << "#";
    cout << endl;


    cout << playerName << "'s Score: " << playerScore << endl;
}

void getInput() {
    if (_kbhit()) {

        char input = _getch();

        switch (input) {
        case 'a':
            if (Dir != RIGHT)
                Dir = LEFT;
            break;
        case 'd':
            if (Dir != LEFT)
                Dir = RIGHT;
            break;
        case 'w':
            if (Dir != DOWN)
                Dir = UP;
            break;
        case 's':
            if (Dir != UP)
                Dir = DOWN;
            break;
        case 'x':
            isGameOver = true;
            break;
        }
    }
}

void updateGame() {

    int prevX = XBody[0];
    int prevY = YBody[0];

    XBody[0] = x;
    YBody[0] = y;

    for (int i = 1; i < snakeLen; i++) {
        int tempX = XBody[i];
        int tempY = YBody[i];
        XBody[i] = prevX;
        YBody[i] = prevY;
        prevX = tempX;
        prevY = tempY;
    }

    switch (Dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    }


    if (x >= width[numberOption] || x < 0 || y >= height[numberOption] || y < 0)
        isGameOver = true;


    for (int i = 0; i < snakeLen; i++) {
        if (XBody[i] == x && YBody[i] == y)
            isGameOver = true;
    }


    if (x == xapple && y == yapple) {
        playerScore += 10;
        xapple = RNG(2, width[numberOption] - 1);
        yapple = RNG(2, height[numberOption] - 1);
        snakeLen++;
    }
}

int main() {

    choseName();

    choseMap();
    system("cls");

    choseSpeed();
    system("cls");

    while (isGameOver == false) {
        maplayout();
        getInput();
        updateGame();

        Sleep(speed);
    }

    system("pause");

    return 0;
}