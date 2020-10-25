#include "console.h"
#include <conio.h>
#include <sstream>
#include <string>
#include "brain.h"
using namespace std;

#define define_multiplayer 1
#define define_single_play 2
#define infinity 100000
#define define_win 10000

// direction : UP LEFT RIGHT DOWN UL UR DL DR
int cc[8] = {0 ,-1,1 ,0 ,-1,1 ,-1,1 };
int cr[8] = {-1,0 ,0 ,1 ,-1,-1,1 ,1 };
int column,row;
menu main_menu({35,20},colorToInt(console_grey,console_darkwhite),colorToInt(console_white,console_black));
int main_now_option = 1;
bool exit_game;
int table[5][5];

int c_table = 1;
int r_table = 1;
// single play value
int computer;

void begin_screen();
void let_computer_move();
void draw_table(int c, int r);
string ChToStr(char c);
void draw_cursor(bool visible, int x, int y);
int check_result_game(int turn);
void print_x_win();
void print_o_win();
void print_draw();

void init_game(){
    _console_init();
    main_menu.add_option("single play");
    main_menu.add_option("multiplayer");
    main_menu.add_option("exit");
}

int menu_game(){
    main_menu.print();
    bool repeat = true;
    while (repeat && !exit_game){
        _console_set_cursor(false,20);
        if (_kbhit()){
            char button = _getch();
            switch (button){
                case 'a': case 'A': // left
                    break;
                case 'd': case 'D': // right
                    break;
                case 'w': case 'W': // up
                    if (main_now_option==1){
                        break;
                    }
                    main_now_option--;
                    main_menu.change_option('p',true);
                    break;
                case 's': case 'S': // down
                    if (main_now_option==main_menu.menu_option.size()){
                        break;
                    }
                    main_now_option ++;
                    main_menu.change_option('n',true);
                    break;
                case VK_RETURN:
                    if (main_now_option == 3)
                        exit_game = true;
                    repeat = false;
                    break;
            }
        }
    }
    if (main_now_option == 1)
        return define_single_play;
    if (main_now_option == 2)
        return define_multiplayer;
}

void single_play(){
    _console_clr();
    for (int i=1;i<=3;i++){
        for (int j=1;j<=3;j++){
            table[i][j] = -1;
        }
    }
    draw_table(c_table,r_table);
    int now_turn = random(1,2);
    computer = random(1,2);
    if (now_turn == computer){
        let_computer_move();
        now_turn = 3-computer;
    }
    int cursor_x, cursor_y;
    cursor_x = 1;
    cursor_y = 1;
    bool visible_cursor = true;
    bool repeat = true;

    my_time now,last;
    last.time_save();
    while (repeat && !exit_game){
        _console_set_cursor(false,20);
        now.time_save();
        if (diff_time(last,now)>=200){
            draw_cursor(visible_cursor,cursor_x,cursor_y);
            visible_cursor = !visible_cursor;
            last.time_save();
        }
        if (_kbhit()){
            char button = _getch();
            switch (button){
                case 'a': case 'A': // left
                    if (cursor_x==1)
                        break;
                    draw_cursor(table[cursor_x][cursor_y]>0,cursor_x,cursor_y);
                    cursor_x--;
                    draw_cursor(true,cursor_x,cursor_y);
                    break;
                case 'd': case 'D': //right
                    if (cursor_x==3)
                        break;
                    draw_cursor(table[cursor_x][cursor_y]>0,cursor_x,cursor_y);
                    cursor_x++;
                    draw_cursor(true,cursor_x,cursor_y);
                    break;
                case 'w': case 'W': // up
                    if (cursor_y==1)
                        break;
                    draw_cursor(table[cursor_x][cursor_y]>0,cursor_x,cursor_y);
                    cursor_y--;
                    draw_cursor(true,cursor_x,cursor_y);
                    break;
                case 's': case 'S': // down
                    if (cursor_y==3)
                        break;
                    draw_cursor(table[cursor_x][cursor_y]>0,cursor_x,cursor_y);
                    cursor_y++;
                    draw_cursor(true,cursor_x,cursor_y);
                    break;
                case 'x': case 'X':
                    exit_game = true;
                    break;
                case 'u': case 'U':
                    break;
                case VK_RETURN:
                    if (table[cursor_x][cursor_y]==-1){
                        table[cursor_x][cursor_y] = now_turn;
                        draw_cursor(true,cursor_x,cursor_y);
                        int r = check_result_game(1);
//                        now_turn = 3-now_turn;
                        switch(check_result_game(1)){
                            case 1: // player 1 win
                                sleep(1000);
                                _console_clr();
                                print_x_win();
                                return;
                                break;
                            case 0: // draw
                                sleep(1000);
                                _console_clr();
                                print_draw();
                                return;
                                break;
                            case -1: // player 2 win
                                sleep(1000);
                                _console_clr();
                                print_o_win();
                                return;
                                break;
                        }
                    let_computer_move();
                    switch(check_result_game(1)){
                            case 1: // player 1 win
                                sleep(1000);
                                _console_clr();
                                print_x_win();
                                return;
                                break;
                            case 0: // draw
                                sleep(1000);
                                _console_clr();
                                print_draw();
                                return;
                                break;
                            case -1: // player 2 win
                                sleep(1000);
                                _console_clr();
                                print_o_win();
                                return;
                                break;
                        }
                    }
                    break;
            }
        }
    }
}

void multiplayer_play(){
    _console_clr();
    for (int i=1;i<=3;i++){
        for (int j=1;j<=3;j++){
            table[i][j] = -1;
        }
    }
    draw_table(c_table,r_table);
    int now_turn = random(1,2);
    int cursor_x, cursor_y;
    cursor_x = 1;
    cursor_y = 1;
    bool visible_cursor = true;
    bool repeat = true;

    my_time now,last;
    last.time_save();
    while (repeat && !exit_game){
        _console_set_cursor(false,20);
        now.time_save();
        if (diff_time(last,now)>=200){
            draw_cursor(visible_cursor,cursor_x,cursor_y);
            visible_cursor = !visible_cursor;
            last.time_save();
        }
        if (_kbhit()){
            char button = _getch();
            switch (button){
                case 'a': case 'A': // left
                    if (cursor_x==1)
                        break;
                    draw_cursor(table[cursor_x][cursor_y]>0,cursor_x,cursor_y);
                    cursor_x--;
                    draw_cursor(true,cursor_x,cursor_y);
                    break;
                case 'd': case 'D': //right
                    if (cursor_x==3)
                        break;
                    draw_cursor(table[cursor_x][cursor_y]>0,cursor_x,cursor_y);
                    cursor_x++;
                    draw_cursor(true,cursor_x,cursor_y);
                    break;
                case 'w': case 'W': // up
                    if (cursor_y==1)
                        break;
                    draw_cursor(table[cursor_x][cursor_y]>0,cursor_x,cursor_y);
                    cursor_y--;
                    draw_cursor(true,cursor_x,cursor_y);
                    break;
                case 's': case 'S': // down
                    if (cursor_y==3)
                        break;
                    draw_cursor(table[cursor_x][cursor_y]>0,cursor_x,cursor_y);
                    cursor_y++;
                    draw_cursor(true,cursor_x,cursor_y);
                    break;
                case 'x': case 'X':
                    exit_game = true;
                    break;
                case 'u': case 'U':
                    break;
                case VK_RETURN:
                    if (table[cursor_x][cursor_y]==-1){
                        table[cursor_x][cursor_y] = now_turn;
                        draw_cursor(true,cursor_x,cursor_y);
                        now_turn = 3-now_turn;
                        switch(check_result_game(1)){
                            case 1: // player 1 win
                                sleep(1000);
                                _console_clr();
                                print_x_win();
                                return;
                                break;
                            case 0: // draw
                                sleep(1000);
                                _console_clr();
                                print_draw();
                                return;
                                break;
                            case -1: // player 2 win
                                sleep(1000);
                                _console_clr();
                                print_o_win();
                                return;
                                break;
                        }
                    }
                    break;
            }
        }
    }
}

int main(){
    init_game();
    int mode_choice;
    while (!exit_game){
        _console_set_cursor(false,20);
        int mode_choice;
        begin_screen();
        mode_choice = menu_game();
        main_menu.clr();
        if (exit_game){
            break;
        }
        if (mode_choice == define_single_play){
            single_play();
        }
        if (mode_choice == define_multiplayer){
            multiplayer_play();
        }
        system("pause");
    }
    system("pause");
}

void draw_table(int c, int r){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut,&csbi);
    column = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    _console_draw(c,r,ChToStr(218),colorToInt(console_white,console_black)); // top-left
    _console_draw(c+6,r,ChToStr(191),colorToInt(console_white,console_black)); // top-right
    _console_draw(c+6,r+6,ChToStr(217),colorToInt(console_white,console_black)); // bot-right
    _console_draw(c,r+6,ChToStr(192),colorToInt(console_white,console_black)); // bot-left
    for (int i=c;i<=c+6;i++){
        for (int j=r;j<=r+6;j++){
            // bien ngang ----------------------------------------
            if (i==c || i == c+6){
                if (j==r || j==r+6){
                    continue;
                }
                if ((r-j)%2==0){
                    if ((c-i)%2==0){// |-
                        if (i == 1){
                            _console_draw(i,j,ChToStr(195),colorToInt(console_white,console_black));
                        }
                        if (i == c+6){
                            _console_draw(i,j,ChToStr(180),colorToInt(console_white,console_black));

                        }
                    }else{

                    }
                }else{// -
                    _console_draw(i,j,ChToStr(179),colorToInt(console_white,console_black));
                }
                continue;
            }
            // bien doc|||||||||||||||||||||||||||||||||||||||||||||
            if (j==r){
                if ((c-i)%2==0){ //T
                    _console_draw(i,j,ChToStr(194),colorToInt(console_white,console_black));
                }else{// -
                    _console_draw(i,j,ChToStr(196),colorToInt(console_white,console_black));
                }
                continue;
            }
            if (j==r+6){
                if ((c-i)%2==0){//-|
                    _console_draw(i,j,ChToStr(193),colorToInt(console_white,console_black));
                }else{//-
                    _console_draw(i,j,ChToStr(196),colorToInt(console_white,console_black));
                }
                continue;
            }
            // middle
            if ((c-i)%2==0){
                if ((r-j)%2==0){//+
                    _console_draw(i,j,ChToStr(197),colorToInt(console_white,console_black));
                }else{// -
                    _console_draw(i,j,ChToStr(179),colorToInt(console_white,console_black));
                }
            }else{
                if ((r-j)%2==0){//-
                    _console_draw(i,j,ChToStr(196),colorToInt(console_white,console_black));
                }else{//space
                    _console_draw(i,j," ",colorToInt(console_white,console_black));
                }
            }
        }
    }
}

string ChToStr(char c){
    stringstream stream;
    stream << c;
    return stream.str();
}

void draw_cursor(bool visible, int x, int y){
    int c = c_table -1 + 2*x;
    int r = r_table -1 + 2*y;
    int color;
    string str;
    if (table[x][y]==-1){
        str = " ";
        color = colorToInt(console_black,console_darkwhite);
    }
    if (table[x][y]==1){
        str = "X";
        color = colorToInt(console_red,console_darkwhite);
    }
    if (table[x][y]==2){
        str = "O";
        color = colorToInt(console_blue,console_darkwhite);
    }
    if (visible){
        _console_draw(c,r,str,color);
    }else{
        _console_draw(c,r,str,colorToInt(console_white,console_black));
    }
}

int check_result_game(int turn){
    int not_go = false;
    for (int i=1;i<=3;i++){
        for (int j=1;j<=3;j++){
            if (table[i][j]==-1){
                not_go = true;
                continue; // not go
            }
            for (int k=0;k<8;k++){
                for (int step = 1;step<=2;step++){
                    int newi = i+cc[k]*step;
                    int newj = j+cr[k]*step;
                    if (0>=newi || newi>=4 || 0>=newj || newj>=4){
                        continue;
                    }
                    if (table[i][j]!=table[newi][newj])
                        break;
                    if (step+1==3){
                        if (table[i][j]==turn){
                            return 1;
                        }else{
                            return -1;
                        }
                    }
                }
            }
        }
    }
    if (!not_go){
        return 0;
    }
    return -1000;
}

void print_x_win(){
    _console_draw(7,1," /$$   /$$                   /$$      /$$       /$$                \n",12);
    _console_draw(7,2,"| $$  / $$                  | $$  /$ | $$      |__/                \n",12);
    _console_draw(7,3,"|  $$/ $$/                  | $$ /$$$| $$       /$$       /$$$$$$$ \n",12);
    _console_draw(7,4," \\  $$$$/                   | $$/$$ $$ $$      | $$      | $$__  $$\n",12);
    _console_draw(7,5,"  >$$  $$                   | $$$$_  $$$$      | $$      | $$  \\ $$\n",12);
    _console_draw(7,6," /$$/\\  $$                  | $$$/ \\  $$$      | $$      | $$  | $$\n",12);
    _console_draw(7,7,"| $$  \\ $$                  | $$/   \\  $$      | $$      | $$  | $$\n",12);
    _console_draw(7,8,"|__/  |__/                  |__/     \\__/      |__/      |__/  |__/\n",12);
}

void print_o_win(){
    _console_draw(7,1,"  /$$$$$$                    /$$      /$$       /$$                \n",9);
    _console_draw(7,2," /$$__  $$                  | $$  /$ | $$      |__/                \n",9);
    _console_draw(7,3,"| $$  \\ $$                  | $$ /$$$| $$       /$$       /$$$$$$$ \n",9);
    _console_draw(7,4,"| $$  | $$                  | $$/$$ $$ $$      | $$      | $$__  $$\n",9);
    _console_draw(7,5,"| $$  | $$                  | $$$$_  $$$$      | $$      | $$  \\ $$\n",9);
    _console_draw(7,6,"| $$  | $$                  | $$$/ \\  $$$      | $$      | $$  | $$\n",9);
    _console_draw(7,7,"|  $$$$$$/                  | $$/   \\  $$      | $$      | $$  | $$\n",9);
    _console_draw(7,8," \\______/                   |__/     \\__/      |__/      |__/  |__/\n",9);
}

void print_draw(){
    _console_draw(9,1," /$$$$$$$        /$$$$$$$         /$$$$$$        /$$      /$$\n",15);
    _console_draw(9,2,"| $$__  $$      | $$__  $$       /$$__  $$      | $$  /$ | $$\n",15);
    _console_draw(9,3,"| $$  \\ $$      | $$  \\ $$      | $$  \\ $$      | $$ /$$$| $$\n",15);
    _console_draw(9,4,"| $$  | $$      | $$$$$$$/      | $$$$$$$$      | $$/$$ $$ $$\n",15);
    _console_draw(9,5,"| $$  | $$      | $$__  $$      | $$__  $$      | $$$$_  $$$$\n",15);
    _console_draw(9,6,"| $$  | $$      | $$  \\ $$      | $$  | $$      | $$$/ \\  $$$\n",15);
    _console_draw(9,7,"| $$$$$$$/      | $$  | $$      | $$  | $$      | $$/   \\  $$\n",15);
    _console_draw(9,8,"|_______/       |__/  |__/      |__/  |__/      |__/     \\__/\n",15);
}
void begin_screen(){
    _console_clr();
    _console_draw(17,1,"  _   _        _               _             \n",9);
    _console_draw(17,2," | | (_)      | |             | |            \n",9);
    _console_draw(17,3," | |_ _  ___  | |_ __ _  ___  | |_ ___   ___ \n",9);
    _console_draw(17,4," | __| |/ __| | __/ _` |/ __| | __/ _ \\ / _ \\\n",9);
    _console_draw(17,5," | |_| | (__  | || (_| | (__  | || (_) |  __/\n",9);
    _console_draw(17,6,"  \\__|_|\\___|  \\__\\__,_|\\___|  \\__\\___/ \\___|\n",9);
    _console_draw(17,13,"Design:             Quan Luong",9);
    _console_draw(17,14,"Computer algorithm: Nguyen Le Minh",9);
}

void let_computer_move(){
	/*
    int n,x,y;
    n=0;
    for (int i=1;i<=3;i++){
        for (int j=1;j<=3;j++){
            if (table[i][j] ==-1){
                n++;
            }
        }
    }
    n = random(1,n);
    for (int i=1;i<=3;i++){
        for (int j=1;j<=3;j++){
            if (table[i][j]==-1){
                if (n>0){
                    x=i;
                    y=j;
                    n--;
                }
            }
        }
    } */
	const int threePowNine = 19683;
	static int scoreTable[threePowNine][3],
		nextMoveR[threePowNine][3],
		nextMoveC[threePowNine][3];
	static bool evaluated[threePowNine][3];

	int tbl[5][5];
	for (int i = 1; i <= 3; ++i)
	for (int j = 1; j <= 3; ++j)
		tbl[i][j] = (table[i][j] == -1) ? 0 : table[i][j];

	int score = optimal_score(tbl, computer, computer, scoreTable, nextMoveR, nextMoveC, evaluated);
	int hash = get_table_hash(tbl);
	int x, y;
	x = nextMoveR[hash][computer];
	y = nextMoveC[hash][computer];

    table[x][y] = computer;
    draw_cursor(true,x,y);
}
