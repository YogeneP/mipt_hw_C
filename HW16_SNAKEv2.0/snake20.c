#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses/ncurses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>

//#define DEBUG 
#ifdef DEBUG
#define D if(1)
#define ND if(0)
#else
#define D if(0) 
#endif

#ifdef MESS
#define WHAT_A_MESS players_num = 15;
#else
#define WHAT_A_MESS {}
#endif

#define MIN_Y  2
#define CONTROL_OPTIONS_NUM 2

#define MAX_TAIL_SIZE 50
#define START_TAIL_SIZE 3
#define MAX_FOOD_SIZE 20 
#define FOOD_EXPIRE_SECONDS 10
#define SEED_NUMBER 3
#define SNAKES_COUNT 2
#define WILD_FOOD_GROW 2
#define SALVAGE_FOOD_GROW 1
#define HEAD_CHAR '@'
#define TAIL_CHAR '*'
#define TAIL_TRACE_CHAR ' '
#define WILD_FOOD_CHAR '$'
#define WILD_FOOD_COLOR 3
#define SALVAGE_FOOD_CHAR 'o'
#define SALVAGE_FOOD_COLOR 1
#define RANDOM_Y (rand() % (LINES - 2) + 1)
#define RANDOM_X (rand() % (COLS - 1))

#define SNAKE_START_X(I) (!((I) & 1) * COLS - (!((I) & 1) * 2 - 1) * (START_TAIL_SIZE + 5))
#define SNAKE_START_Y(I) ((((I) / 2) + 1) * (LINES / (2 + ((players_num - 1) / 2))))

enum {LEFT=1, UP, RIGHT, DOWN, STOP_GAME=KEY_F(10)};

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

typedef struct snake_t
{
    uint8_t x;
    uint8_t y;
    uint8_t direction;
    uint8_t to_grow;
    size_t tsize;
    struct tail_t *tail;
    struct control_buttons* controls;

} snake_t;

typedef struct tail_t
{
    uint8_t x;
    uint8_t y;
} tail_t;

typedef struct food_t
{
    int x;
    int y;
    time_t put_time;
    uint8_t enable;
} food_t;

food_t wild_food[MAX_FOOD_SIZE];
void cleanup(void);
void allocErrorExit(int);
void weAreTheChampions(int);

void initFood(food_t f[], size_t start, size_t end)
{
    food_t init = {0,0,0,0};
    for(size_t i = start; i <= end ; i++)
    {
        f[i] = init;
    }
}

void relocateFoodSeed(food_t *fp, uint8_t fy, uint8_t fx, char ch)
{
    mvprintw(fp->y, fp->x, " ");
    fp->x = fx;
    fp->y = fy; 
    fp->put_time = time(NULL);
    fp->enable = 1;
    mvprintw(fp->y, fp->x, "%c", ch);
}

void putFood(food_t f[], size_t number_seeds, char ch)
{
    for(size_t i=0; i<number_seeds; i++)
    {
        relocateFoodSeed(&f[i],RANDOM_Y, RANDOM_X, ch);
    }
}

void refreshFood(food_t f[], size_t nfood, uint8_t relocate, char ch)
{
    for(size_t i=0; i < nfood; i++) {
        if( f[i].put_time ) {
            if(relocate) {
                if(!f[i].enable || (time(NULL) - f[i].put_time) > FOOD_EXPIRE_SECONDS ) {
                    relocateFoodSeed(&f[i], RANDOM_Y, RANDOM_X, ch);
                }
            } else {
                if(f[i].enable && (time(NULL) - f[i].put_time) > FOOD_EXPIRE_SECONDS ) {
                    f[i].enable = 0;
                    mvprintw(f[i].y,f[i].x," ");
                }
            }
        }
    }
}

int checkFoodEaten(snake_t* s, food_t f[], size_t n) {
    for (size_t i = 0; i < n; i++) {
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
    for(size_t i = 0; i < size; i++)
    {
        t[i] = init_t;
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
    tail_t* tail;
    if((tail = (tail_t*) malloc(MAX_TAIL_SIZE*sizeof(tail_t))) == NULL) {
        allocErrorExit(__LINE__);
    }
    initTail(tail, MAX_TAIL_SIZE);
    initHead(head, x, y, dir);
    for (size_t i = 0; i < size; i++) {
        tail[i].x = x + (2 - head->direction) * i;
        tail[i].y = y;
    }
    head->tail = tail; // прикрепляем к голове хвост
    head->tsize = size;
    head->to_grow = 0;
    head->controls = controls;
}

void go(struct snake_t *head)
{
    mvprintw(head->y, head->x, " ");
    switch (head->direction)
    {
        case LEFT:
            (*head).x--;            
            if(head->x <= 0) 
                head->x = COLS - 1;
        break;
        case RIGHT:
            (*head).x++;
			if(head->x >= COLS) 
                head->x = 0;
        break;
        case UP:
            (*head).y--;
			if(head->y <= 0) 
                head->y = LINES - 1;
        break;
        case DOWN:
            (*head).y++;
            if(head->y >= LINES) 
                head->y = 0;
        break;
        default:
        break;
    }
    mvprintw(head->y, head->x, "%c", HEAD_CHAR);
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

void goTail(struct snake_t *head)
{
    if (!(head->to_grow)) {
        mvprintw(head->tail[head->tsize - 1].y, head->tail[head->tsize - 1].x, "%c", TAIL_TRACE_CHAR);
    } else {
        head->tsize++;
        head->to_grow--;
    }
    for(size_t i = head->tsize-1; i > 0; i--) {
        head->tail[i] = head->tail[i-1];
        mvprintw(head->tail[i].y, head->tail[i].x, "%c", TAIL_CHAR);
    }
    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
}

/*
 return:
 -1 if snake[snake_index] is not harmed
 i - eaten tail chain index, if snake's tail is cut by someone else; 0 - headbite
*/ 
int checkSnakeEaten(snake_t **snakes, uint8_t snake_index, uint8_t p_num) {
    for (int k = 0; k < p_num; k++) {
        if(k != snake_index) {
            for (size_t i = 0; i < snakes[snake_index]->tsize - 1; i++) {
                if((snakes[k]->x == (snakes[snake_index]->tail[i]).x) && (snakes[k]->y == (snakes[snake_index]->tail[i]).y)) { 
                    return i;
                }
            }
        }
    }
    return -1;
}

void cutTail(snake_t *snake, uint8_t cut_i, food_t **food, size_t *food_size) {
    uint16_t j = 0;
    for (int i = snake->tsize-1; i > cut_i; i--) {
        for (j = 0; j < *food_size; j++) {
            if((*food)[j].enable == 0) {
                relocateFoodSeed(&((*food)[j]),snake->tail[i].y,snake->tail[i].x, SALVAGE_FOOD_CHAR);
                break;
            }
        }
        if(j == *food_size) {
            uint16_t new_food_size = (*food_size) * 2;
            food_t* temp = realloc(*food, sizeof(food_t) * new_food_size);
            if(!temp) {
                allocErrorExit(__LINE__);
            }
            *food = temp;
            initFood(*food, *food_size, (new_food_size - 1));
            *food_size = new_food_size;
            i++; // repeat the loop step after reallocation
        }
    }
    snake->tsize = cut_i > 0 ? cut_i : 1;
}

void printHelp(void) {
    clear();
    char* str[] = {
        "Main rules:",
        "One player:",
        "- practice mode. No competitions, no dangers. Just grow up consuming a wild food ($).",
        "- the food gets rotten after 10 s and disappears, but a new one grows up somewhere else.",
        "- control snakes movements with arrow keys",
        "Two players:",
        "- competition mode. Grow yourself, cut the opponent.",
        "- control snakes movements with arrow keys for player 1 and 'W','S','A','D' keys for player 2.",
        "- bite your opponents tail to turn it to the pieces of salvage food (o)",
        "- wild food ($) is more nutritious, two tail segments grows after consumption",
        "- salvage food piece (o) - a slice of tail, is small and expands the snake by one segment.",
        "Get 50 segments long to win. ",
        "Good luck on the wilderness!"
    };  
    
    mvprintw(0, 0,"'F10' for EXIT to Main menu");
    for (int i = 0; i < 12; i++)
        mvprintw(3+i,2,"%s", str[i]);
} 

/*
returns leftmost character of menu X position (for marker)
*/
uint8_t printMenu(uint8_t ypos) {
    const char heading_str[] = "Snake v2.0";
    const char one_player_str[] = "Start one  player";
    const char two_player_str[] = "Start two players";
    const char help_str[] = "How to play";
    const char exit_str[] = "Exit";

    clear();

    mvprintw(ypos-2, (COLS-sizeof(heading_str))/2, heading_str);
    mvprintw(ypos, (COLS-sizeof(one_player_str))/2, one_player_str);
    mvprintw(ypos+1, (COLS-sizeof(two_player_str))/2, two_player_str);
    mvprintw(ypos+2, (COLS-sizeof(help_str))/2, help_str);
    mvprintw(ypos+3, (COLS-sizeof(exit_str))/2, exit_str);

    return (COLS-sizeof(two_player_str))/2;
}

uint8_t startMenu(void) {

    uint8_t menu_ypos = (LINES - 4) / 2;
    uint8_t marker_xpos = printMenu(menu_ypos) - 2;

    int key = 0;
    uint8_t i = 0;
    while (true) {
        mvprintw(menu_ypos + i, marker_xpos, "*");
        key = getch();
        mvprintw(menu_ypos + i, marker_xpos, " ");
        if (key == KEY_UP) {
            i <= 0 ? i = 3 : i--;
            continue;
        } 
        if (key == KEY_DOWN) {
            i >= 3 ? i = 0 : i++;
            continue;
        } 
        if (key == '\n' || key == KEY_ENTER) {
            switch(i) {
                case 3:
                    clear();
                    return 0;
                case 2: 
                    printHelp();
                    key = 0;
                    while(key == 0) {
                        key = getch();
                    }
                    printMenu(menu_ypos);
                    break;
                case 0: 
                case 1:
                    clear();
                    return i+1;
            }
        }
    }
}

food_t *salvage_food = NULL;
snake_t** snake = NULL;
uint8_t players_num = 0;

int main(void)
{
    if (atexit(cleanup) != 0) {
        endwin();
        printf("Memory management error, aborting...\n");
        return -5;
    }

    initscr();
    keypad(stdscr, TRUE); // Enable F1, F2, arrows etc.
    raw();                // Disable line buffering
    noecho();            // Disable echo() mode on getch call
    curs_set(FALSE);    // Disable cursor

    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

    start:
    players_num = startMenu();
    if (!players_num) {
        endwin();
        printf("Exiting... ");
        exit(0);
    }

    WHAT_A_MESS
    snake = malloc(sizeof(snake_t*)*players_num);
    size_t salvage_food_size = START_TAIL_SIZE;
    salvage_food = malloc(sizeof(food_t) * salvage_food_size);
    if (salvage_food == NULL || snake == NULL) {
        allocErrorExit(__LINE__);
    }
    for(int i = 0; i < players_num; i++) {
        if((snake[i] = malloc(sizeof(snake_t))) == NULL) {
            allocErrorExit(__LINE__);
        }
        initSnake(snake[i], START_TAIL_SIZE, SNAKE_START_X(i), SNAKE_START_Y(i), (1 + 2 * (i & 1)), snake_controls[i%2]);
    } 

    mvprintw(0, 0,"'F10' for EXIT to Main menu");
    timeout(150);
    initFood(wild_food, 0, MAX_FOOD_SIZE-1);
    initFood(salvage_food, 0, salvage_food_size-1);
    attron(COLOR_PAIR(WILD_FOOD_COLOR));
    putFood(wild_food, SEED_NUMBER, WILD_FOOD_CHAR);
    attroff(COLOR_PAIR(WILD_FOOD_COLOR));
    int key_pressed = 0;
    while( key_pressed != STOP_GAME )
    {
        key_pressed = getch();
        for(int i = 0; i < players_num; i++) { // first loop - reposition and resize
            changeDirection(snake[i], key_pressed, CONTROL_OPTIONS_NUM);
            uint8_t col_pair = (3*(i+1)) % 10; 
            col_pair = col_pair > 0 ? col_pair : 1;
            attron(COLOR_PAIR(col_pair));
            go(snake[i]);
            goTail(snake[i]);
            attroff(COLOR_PAIR(col_pair));
            if (checkFoodEaten(snake[i], wild_food, SEED_NUMBER)) {
                snake[i]->to_grow += WILD_FOOD_GROW;
            }
            if (checkFoodEaten(snake[i], salvage_food, salvage_food_size)) {
                snake[i]->to_grow += SALVAGE_FOOD_GROW;
            }
            if(snake[i]->tsize >= MAX_TAIL_SIZE) {
                weAreTheChampions(i);
                cleanup();
                goto start;
            }
        }
        for(int i = 0; i < players_num; i++) {  //second loop: check collisions and define cut snakes
            int eaten = checkSnakeEaten(snake, i, players_num);
            if (eaten < 0) { continue; }
            if (eaten >= 0) {
                attron(COLOR_PAIR(SALVAGE_FOOD_COLOR));
                cutTail(snake[i], eaten, &salvage_food, &salvage_food_size);
                attroff(COLOR_PAIR(SALVAGE_FOOD_COLOR));
                continue;
            }      
        }
        attron(COLOR_PAIR(WILD_FOOD_COLOR));
        refreshFood(wild_food, SEED_NUMBER, 1, WILD_FOOD_CHAR);
        attroff(COLOR_PAIR(WILD_FOOD_COLOR));
        refreshFood(salvage_food, salvage_food_size, 0, SALVAGE_FOOD_CHAR);
    }

    goto start;
}

void weAreTheChampions(int i) {
    clear();
    char* str[] = {
        "Player ",
        " bit and ate well!!!",
        "Congratulations! You are chosen by evolution to stay alive."
    };
    mvprintw((LINES-2)/2,(COLS - (strlen(str[0])+strlen(str[1])+1))/2,"%s%d%s",str[0], i+1, str[1]);
    mvprintw((LINES-2)/2 + 1,(COLS - strlen(str[2]))/2,"%s",str[2]);
    mvprintw(0,0, "Press 'F10' to get back to the Main menu");
    int c = 0;
    while ((c = getch()) != STOP_GAME) { };
}

void allocErrorExit(int line) {
    endwin();
    printf("Memory allocation error at line %d", line);
    exit(-1);
}

void cleanup(void) {
    int err = 0;
    for(int i = 0; i < players_num; i++) {
        if(snake[i]->tail) free(snake[i]->tail);
        else err = 1;
        if(snake[i]) free(snake[i]);
        else err=2;
    }
    if(snake) free(snake);
    else err = 3;
    if(salvage_food) free(salvage_food);
    else err = 4;
    D if(err) printf("Err %d on exit... ", err);
}
