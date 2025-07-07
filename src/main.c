#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

#define WIDTH 20
#define HEIGHT 10
#define MAX_OBSTACLES 5

#define PLAYER_CHAR "🐍"
#define OBSTACLE_CHAR "🪨"

// Estrutura para representar um obstáculo
typedef struct {
    int x;
    int y;
    int active;  // 0 = inativo, 1 = ativo
    int spawn_delay;  // delay para spawn do próximo obstáculo
} Obstacle;

int playerX = WIDTH / 5;
int score = 0;
Obstacle obstacles[MAX_OBSTACLES];
int obstacle_spawn_timer = 0;

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

void init_obstacles()
{
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        obstacles[i].x = rand() % WIDTH;
        obstacles[i].y = -1;  // Começa fora da tela
        obstacles[i].active = 0;
        obstacles[i].spawn_delay = i * 15 + 10;  // Delay diferentes para cada obstáculo
    }
}

void spawn_obstacle()
{
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        if (!obstacles[i].active && obstacle_spawn_timer >= obstacles[i].spawn_delay)
        {
            obstacles[i].x = rand() % WIDTH;
            obstacles[i].y = 0;
            obstacles[i].active = 1;
            obstacles[i].spawn_delay = obstacle_spawn_timer + 20 + rand() % 30;  // Próximo spawn em 20-50 frames
            break;
        }
    }
}

void update_obstacles()
{
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        if (obstacles[i].active)
        {
            obstacles[i].y++;
            if (obstacles[i].y >= HEIGHT)
            {
                obstacles[i].active = 0;
                obstacles[i].y = -1;
                score++;
            }
        }
    }
}

int check_collision()
{
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        if (obstacles[i].active && obstacles[i].y == HEIGHT - 1 && obstacles[i].x == playerX)
        {
            return 1;  // Colisão detectada
        }
    }
    return 0;  // Sem colisão
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
            else
            {
                int obstacle_here = 0;
                for (int i = 0; i < MAX_OBSTACLES; i++)
                {
                    if (obstacles[i].active && x == obstacles[i].x && y == obstacles[i].y)
                    {
                        obstacle_here = 1;
                        break;
                    }
                }
                if (obstacle_here)
                {
                    printf("%s", OBSTACLE_CHAR);
                }
                else
                {
                    printf(" ");
                }
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
    score = 0;
    obstacle_spawn_timer = 0;
    init_obstacles();
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
            if (check_collision())
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

            obstacle_spawn_timer++;
            spawn_obstacle();
            update_obstacles();

            draw();
            usleep(100000);
        }
    }

    return 0;
}
