#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
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

void disable_input_buffering()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios new = orig_termios;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
}

void reset_input_buffering()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

int kbhit()
{
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    return select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) > 0;
}

void draw()
{
    system("clear");
    printf("Score: %d\n", score);
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (x == playerX && y == HEIGHT - 1)
            {
                printf("%s", PLAYER_CHAR);
            }
            else if (x == obstacleX && y == obstacleY)
            {
                printf("%s", OBSTACLE_CHAR);
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void wait_for_enter()
{
    printf("\nPress ENTER to continue...");
    while (getchar() != '\n')
        ;
}

void reset_game()
{
    playerX = WIDTH / 5;
    obstacleY = 0;
    obstacleX = rand() % WIDTH;
    score = 0;
}

int main()
{
    srand(time(NULL));
    disable_input_buffering();
    atexit(reset_input_buffering);

    while (1)
    {
        reset_game();

        while (1)
        {
            if (obstacleY == HEIGHT - 1 && obstacleX == playerX)
            {
                draw();
                printf("\n💀 Game Over! Score final: %d\n", score);
                wait_for_enter();
                break;
            }

            if (kbhit())
            {
                char ch = getchar();
                if (ch == 'a' && playerX > 0)
                    playerX--;
                if (ch == 'd' && playerX < WIDTH - 1)
                    playerX++;
            }

            obstacleY++;
            if (obstacleY >= HEIGHT)
            {
                obstacleY = 0;
                obstacleX = rand() % WIDTH;
                score++;
            }

            draw();
            usleep(100000);
        }
    }

    return 0;
}
