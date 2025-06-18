#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

#define WIDTH 20
#define HEIGHT 10

#define PLAYER_CHAR "🟦"
#define OBSTACLE_CHAR "🟥"

int playerX = WIDTH / 5;
int obstacleY = 0;
int obstacleX;
int score = 0;

struct termios orig_termios;

void disable_input_buffering() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios new = orig_termios;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
}

void reset_input_buffering() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

int kbhit() {
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    return select(STDIN_FILENO+1, &set, NULL, NULL, &timeout) > 0;
}

void draw() {
    system("clear");
    printf("Score: %d\n", score);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == playerX && y == HEIGHT - 1) {
                printf("%s", PLAYER_CHAR);
            } else if (x == obstacleX && y == obstacleY) {
                printf("%s", OBSTACLE_CHAR);
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int main() {
    disable_input_buffering();
    atexit(reset_input_buffering);
    draw();
    return 0;
}
