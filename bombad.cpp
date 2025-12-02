#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <thread>
#include <chrono>

using namespace std;

const int WIDTH  = 20;
const int HEIGHT = 20;

char board[HEIGHT][WIDTH];

int snakeX[400];
int snakeY[400];
int snakeLength;

int dx = 1, dy = 0;          // initial direction (right)
int appleX, appleY;

bool gameOver = false;

void spawnApple() {
    while (true) {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;

        bool onSnake = false;
        for (int i = 0; i < snakeLength; i++) {
            if (snakeX[i] == x && snakeY[i] == y) {
                onSnake = true;
                break;
            }
        }
        if (!onSnake) {
            appleX = x;
            appleY = y;
            return;
        }
    }
}

void init() {
    snakeLength = 3;

    int startX = WIDTH / 2;
    int startY = HEIGHT / 2;

    // place initial snake horizontally
    for (int i = 0; i < snakeLength; i++) {
        snakeX[i] = startX - i;
        snakeY[i] = startY;
    }

    spawnApple();
    gameOver = false;

    //srand(GetTickCount());
}

void drawBoard() {
    system("cls");

    // top border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << "\n";

    for (int y = 0; y < HEIGHT; y++) {
        cout << "#"; // left border

        for (int x = 0; x < WIDTH; x++) {
            char cell = ' ';

            // apple
            if (x == appleX && y == appleY) {
                cell = '*';
            }

            // snake
            for (int i = 0; i < snakeLength; i++) {
                if (snakeX[i] == x && snakeY[i] == y) {
                    cell = (i == 0 ? 'O' : 'o');
                }
            }

            cout << cell;
        }

        cout << "#"; // right border
        cout << "\n";
    }

    // bottom border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << "\n";

    cout << "Length: " << snakeLength << "\n";
}

void input() {
    if (_kbhit()) {
        char key = _getch();

        if (key == 'w' && dy != 1) { dy = -1; dx = 0; }
        if (key == 's' && dy != -1) { dy = 1; dx = 0; }
        if (key == 'a' && dx != 1) { dx = -1; dy = 0; }
        if (key == 'd' && dx != -1) { dx = 1; dy = 0; }
    }
}

void updateSnake() {
    // move body from tail -> head
    for (int i = snakeLength - 1; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }

    // move head
    snakeX[0] += dx;
    snakeY[0] += dy;

    // wall collision
    if (snakeX[0] < 0 || snakeX[0] >= WIDTH ||
        snakeY[0] < 0 || snakeY[0] >= HEIGHT) {
        gameOver = true;
        return;
    }

    // self collision
    for (int i = 1; i < snakeLength; i++) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
            gameOver = true;
            return;
        }
    }

    // apple collision
    if (snakeX[0] == appleX && snakeY[0] == appleY) {
        snakeLength++;
        spawnApple();
    }
}

int main() {
    init();

    while (!gameOver) {
        input();
        updateSnake();
        drawBoard();
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // game speed
    }

    cout << "Game Over!\n";
    return 0;
}
