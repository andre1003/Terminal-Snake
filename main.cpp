#include "iostream"
#include <list>
#include "stdio.h"
#include "termios.h"
#include "unistd.h"

using namespace std;

typedef struct POINT {
    int x, y;
}point;

// Remake of getch function (I don't know what I did here, but it works!)
int getc() {
    static struct termios oldt, newt;

    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr(STDIN_FILENO, &oldt);
    /*now the settings will be copied*/
    newt = oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON);

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    int c = getchar();

    /*restore the old settings*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

    return c;
}

// Function for printing the map, considering every aspect
void print_map(int matrix[10][10]) {
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            if(matrix[i][j] == 1) printf("\033[1;31m %c\033[0m", 35);      // Wall
            else if(matrix[i][j] == 0) cout << "  ";                       // Free block
            else if(matrix[i][j] == 2) printf("\033[1;33mO-\033[0m");      // Snake (head)
            else if(matrix[i][j] == 3) printf("\033[1;33mOO\033[0m");      // Snake (body)
            else if(matrix[i][j] == 4) printf("\033[1;31m +\033[0m");      // Ghost
            else if(matrix[i][j] == 5) printf(" *");                       // Score
        }
        cout << endl;
    }
}

// list<point> update(list<point> snake, point p) {
//     snake.push_front(p);
//     snake.pop_back();

//     return snake;
// }

list<point> movement(int matrix[10][10], list<point> snake, int *score) {
    int command;
    command = getc(); // Get keyboard input
    point p;

    int oldx = snake.back().x, oldy = snake.back().y;

    // Based on WASD letters
    if(command == 'w' || command == 'W') {
        p.x = snake.front().x;
        p.y = snake.front().y - 1;
    }
    else if(command == 'a' || command == 'A') { // Left
        p.x = snake.front().x - 1;
        p.y = snake.front().y;
    }
    else if(command == 's' || command == 'S') { // Down
        p.x = snake.front().x;
        p.y = snake.front().y + 1;
    }
    else if(command == 'd' || command == 'D') { // Right
        p.x = snake.front().x + 1;
        p.y = snake.front().y;
    }
    else if(command == 27) exit(0);             // (Esc) Exit game  

    int x = p.x, y = p.y;

    if(matrix[y][x] == 0) {     
        matrix[snake.back().y][snake.back().x] = 0;   
        snake.push_front(p);
        snake.pop_back();
        
    }
    else if(matrix[y][x] == 5) {
        matrix[snake.back().y][snake.back().x] = 0;
        snake.push_front(p);
        *score+=1;
    }
    else if(matrix[y][x] == 3) {
        system("clear");
        cout << "Game Over!" << endl;
        exit(0);
    }

    for(list<point>::iterator i = snake.begin(); i != snake.end(); i++) {
        point p = *i;
        if(i == snake.begin())
            matrix[p.y][p.x] = 2;
        else
            matrix[p.y][p.x] = 3;
    }

    return snake;
}


int main() {
    list<point> snake;

    point p;
    p.x = 5;
    p.y = 4;

    int matrix[10][10] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 2, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };

    int count = 0, score = 0;

    srand(time(NULL));

    system("clear");

    snake.push_back(p);

    while(true) {
        print_map(matrix);

        cout << "Score: " << score << endl;

        snake = movement(matrix, snake, &score);
        system("clear");

        if(count % 10 == 0) {
            matrix[1 + rand() % 8][1 + rand() % 8] = 5;
        }
        count++;
    }
    
    
}