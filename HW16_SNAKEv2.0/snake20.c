#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses/ncurses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>
#define MIN_Y  2
#define CONTROL_OPTIONS_NUM 2
enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME=KEY_F(10)};
enum {MAX_TAIL_SIZE=100, START_TAIL_SIZE=3, MAX_FOOD_SIZE=20, FOOD_EXPIRE_SECONDS=10,SEED_NUMBER=3};
#define SNAKES_COUNT 2
#define SNAKE_START_X(I) (((I) & 1) * COLS - (((I) & 1) * 2 - 1) * (START_TAIL_SIZE + 5))
#define SNAKE_START_Y(I) ((((I) / 2) + 1) * (LINES / (1 + (SNAKES_COUNT + 1) / 2)))

typedef struct control_buttons
{
    int down;
    int up;
    int left;
    int right;
} control_buttons;
control_buttons snake_controls[CONTROL_OPTIONS_NUM][SNAKES_COUNT] = {
    {
        {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT},
        {0, 0, 0, 0}
    },
    {
        {'s', 'w', 'a', 'd'},
        {'S', 'W', 'A', 'D'}
    }
};

/*
 Голова змейки содержит в себе
 x,y - координаты текущей позиции
 direction - направление движения
 tsize - размер хвоста
 *tail -  ссылка на хвост
 */
typedef struct snake_t
{
    int x;
    int y;
    int direction;
    size_t tsize;
    struct tail_t *tail;
    struct control_buttons* controls;

} snake_t;
/*
 Хвост это массив состоящий из координат x,y
 */
typedef struct tail_t
{
    int x;
    int y;
} tail_t;
/*
 Еда — это массив точек, состоящий из координат x,y, времени,
 когда данная точка была установлена, и поля, сигнализирующего,
 была ли данная точка съедена.
 */
struct food
{
    int x;
    int y;
    time_t put_time;
    char point;
    uint8_t enable;
} food[MAX_FOOD_SIZE];
void initFood(struct food f[], size_t size)
{
    struct food init = {0,0,0,0,0};
    for(size_t i=0; i<size; i++)
    {
        f[i] = init;
    }
}
/*
 Обновить/разместить текущее зерно на поле
 */
void putFoodSeed(struct food *fp)
{
    int max_x=0, max_y=0;
    char spoint[2] = {0};
    getmaxyx(stdscr, max_y, max_x);
    mvprintw(fp->y, fp->x, " ");
    fp->x = rand() % (max_x - 1);
    fp->y = rand() % (max_y - 2) + 1; //Не занимаем верхнюю строку
    fp->put_time = time(NULL);
    fp->point = '$';
    fp->enable = 1;
    spoint[0] = fp->point;
    mvprintw(fp->y, fp->x, "%s", spoint);
}
/*
 Разместить еду на поле
 */
void putFood(struct food f[], size_t number_seeds)
{
    for(size_t i=0; i<number_seeds; i++)
    {
        putFoodSeed(&f[i]);
    }
}

void refreshFood(struct food f[], int nfood)
{
    for(size_t i=0; i<nfood; i++)
    {
        if( f[i].put_time )
        {
            if( !f[i].enable || (time(NULL) - f[i].put_time) > FOOD_EXPIRE_SECONDS )
            {
                putFoodSeed(&f[i]);
            }
        }
    }
}

int checkFoodEaten(snake_t* s, struct food f[], size_t n) {
    for (int i = 0; i < SEED_NUMBER; i++) {
        if (f[i].enable) {
            if((s->x == (f[i]).x) && (s->y == (f[i]).y)) {
                f[i].enable = 0;
                return 1;    
            }
        }
    }  
    return 0;
}

void initTail(struct tail_t t[], size_t size)
{
    struct tail_t init_t={0,0};
    for(size_t i=0; i<size; i++)
    {
        t[i]=init_t;
    }
}
void initHead(struct snake_t *head, int x, int y, int dir)
{
    head->x = x;
    head->y = y;
    head->direction = dir;
}
void initSnake(snake_t *head, size_t size, int x, int y, int dir, control_buttons* controls)
{
    tail_t* tail = (tail_t*) malloc(MAX_TAIL_SIZE*sizeof(tail_t));
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head, x, y, dir);
    for (int i = 0; i < size; i++) {
        tail[i].x = x + (2 - head->direction) * i;
        tail[i].y = y;
    }
    head->tail = tail; // прикрепляем к голове хвост
    head->tsize = size+1;
    head->controls = controls;
}
/*
 Движение головы с учетом текущего направления движения
 */
void go(struct snake_t *head, uint8_t n)
{
    char ch = '0'+n;//'@';
    int max_x=0, max_y=0;
    getmaxyx(stdscr, max_y, max_x); // macro - размер терминала
    mvprintw(head->y, head->x, " "); // очищаем один символ
    switch (head->direction)
    {
        case LEFT:
			// Циклическое движение, чтобы не
			// уходить за пределы экрана
            if(head->x <= 0) 
                head->x = max_x;
            mvprintw(head->y, --(head->x), "%c", ch);
        break;
        case RIGHT:
			if(head->x >= max_x) 
                head->x = 0;
            mvprintw(head->y, ++(head->x), "%c", ch);
        break;
        case UP:
			if(head->y <= 1) 
                head->y = max_y;
            mvprintw(--(head->y), head->x, "%c", ch);
        break;
        case DOWN:
            if(head->y >= max_y) 
                head->y = 0;
            mvprintw(++(head->y), head->x, "%c", ch);
        break;
        default:
        break;
    }
    refresh();
}
void changeDirection(struct snake_t* snake, const int32_t key, uint8_t control_options_num)
{
    for(int i = 0; i < control_options_num; i++) {
        if (key == snake->controls[i].down && snake->direction != UP)
            snake->direction = DOWN;
        else if (key == snake->controls[i].up && snake->direction != DOWN)
            snake->direction = UP;
        else if (key == snake->controls[i].right && snake->direction != LEFT)
            snake->direction = RIGHT;
        else if (key == snake->controls[i].left && snake->direction != RIGHT)
            snake->direction = LEFT;
    }
}

/*
 Движение хвоста с учетом движения головы
 */
void goTail(struct snake_t *head)
{
    char ch = '*';
    mvprintw(head->tail[head->tsize-1].y, head->tail[head->tsize-1].x, " ");
    for(size_t i = head->tsize-1; i>0; i--)
    {
        head->tail[i] = head->tail[i-1];
//      if( head->tail[i].y || head->tail[i].x)
//          mvprintw(head->tail[i].y, head->tail[i].x, "%c", ch);

    }
    mvprintw(head->tail[1].y, head->tail[1].x, "%c", ch);

    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
}

int checkSnakeEaten(snake_t * s, snake_t ** snakes) {
    for (int k = 0; k < SNAKES_COUNT; k++) {
        for (int i = 1; i < s->tsize; i++) {
            if((snakes[k]->x == (s->tail[i]).x) && (snakes[k]->y == (s->tail[i]).y)) { 
                return i;
            }
        }
    }
    return 0;
}

uint8_t startMenu() {
    uint8_t menu_items_count = 4;
    const char heading_str[] = "Snake v2.0";
    const char one_player_str[] = "Start one  player";
    const char two_player_str[] = "Start two players";
    const char exit_str[] = "Exit";

    uint8_t menu_y_pos = (LINES-3)/2;
    clear();

    mvprintw(menu_y_pos-2, (COLS-sizeof(heading_str))/2, heading_str);
    mvprintw(menu_y_pos, (COLS-sizeof(one_player_str))/2, one_player_str);
    mvprintw(menu_y_pos+1, (COLS-sizeof(two_player_str))/2, two_player_str);
    mvprintw(menu_y_pos+2, (COLS-sizeof(exit_str))/2, exit_str);

    int key = 0;
    uint8_t i = 0;
    while (key != '\n' && key != KEY_ENTER) {
        mvprintw(menu_y_pos+i, (COLS-sizeof(one_player_str))/2 - 2, "*");
        key = getch();
        if (key == KEY_UP) {
            i < 1 ? i = 2 : i--;
            continue;
        } 
        if (key == KEY_DOWN) {
            i > 1 ? i = 0 : i++;
        } 
    }
    clear();
    return i;
}

int main()
{
    uint8_t players_num = 0;

    initscr();
    keypad(stdscr, TRUE); // Enable F1, F2, arrows etc.
    raw();                // Disable line buffering
    noecho();            // Disable echo() mode on getch call
    curs_set(FALSE);    // Disable cursor
   
    start:
    players_num = startMenu();
    snake_t* snake[SNAKES_COUNT];

    for(int i = 0; i < players_num; i++) {
        snake[i] = (snake_t*)malloc(sizeof(snake_t));
        initSnake(snake[i], START_TAIL_SIZE, SNAKE_START_X(i), SNAKE_START_Y(i), (3 - 2 * (i & 1)), snake_controls[i%2]);
    } 
    mvprintw(1, 0,"Use arrows for control. Press 'F10' for EXIT");
    timeout(150);    // Отключаем таймаут после нажатия клавиши в цикле
    initFood(food, MAX_FOOD_SIZE);
    putFood(food, SEED_NUMBER); // Кладем зерна
    int key_pressed=0;
    while( key_pressed != STOP_GAME )
    {
        key_pressed = getch(); // Считываем клавишу
        for(int i = 0; i < players_num; i++) {
            changeDirection(snake[i], key_pressed, CONTROL_OPTIONS_NUM);
            go(snake[i], i);
            goTail(snake[i]);
//        timeout(150); // Задержка при отрисовке
            if (checkSnakeEaten(snake[i], snake)) {
                initSnake(snake[i], START_TAIL_SIZE, SNAKE_START_X(i), SNAKE_START_Y(i), (3 - 2 * (i & 1)), snake_controls[i%2]);
                break;
            }       
            if (checkFoodEaten(snake[i], food, SEED_NUMBER)) {
                snake[i]->tsize = (snake[i]->tsize < MAX_TAIL_SIZE) ? snake[i]->tsize + 1 : snake[i]->tsize;
            }
        }
        refreshFood(food, SEED_NUMBER); // Обновляем еду
    }
    for(int i = 0; i < players_num; i++) {
        free(snake[i]->tail);
        free(snake[i]);
    }
    if (key_pressed != STOP_GAME) {
        erase();
        refresh();
        goto start;
    }
    endwin(); // Завершаем режим curses mod
    return 0;
}
