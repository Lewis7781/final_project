#include <iostream>
#include <stdlib.h>
//#include <algorithm>
#include <time.h>
#include <climits>
#include <queue>
#include "../include/algorithm.h"
//#include "../include/rules.h"
//
//
using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions
 * 1. ~ 4. are listed in next block)
 *
 * The STL library functions is not allowed to use.
 ******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 *
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
 *************************************************************************/
char color;

class boardcell{
private:
    int orbs_num;   // The number of the orbs on the cell
    int capacity;   // The max number of the orbs of a cell
    char color;     // The color of the cell, 'w'(No one has placed the orbs), 'r'(Red Player) and 'b'(Blue player)
    bool explode;
    
public:
    boardcell();
    
    // The basic functions of the Cell
    int get_orbs_num();
    int get_capacity();
    char get_color();
    bool get_explode();
    
    void set_orbs_num(int orbs);
    void set_capacity(int cap);
    void set_color(char col);
    void set_explode(bool tof);
};

boardcell::boardcell(){
    orbs_num = 0, capacity = 8, color='w', explode = false;
}
int boardcell::get_capacity(){
    return this->capacity;
}

int boardcell::get_orbs_num(){
    return this->orbs_num;
}

char boardcell::get_color(){
    return this->color;
}

bool boardcell::get_explode(){
    return this->explode;
}

void boardcell::set_orbs_num(int orbs){
    this->orbs_num = orbs;
}

void boardcell::set_capacity(int cap){
    this->capacity = cap;
}

void boardcell::set_color(char col){
    this->color = col;
}

void boardcell::set_explode(bool tof){
    this->explode = tof;
}

class gameboard{
private:
    
    void cell_reaction_marker();                // After the explosion, mark all the cell that  will explode in next iteration
    bool cell_is_full(boardcell* cell);         // Check wether the cell is full of orbs and set the explosion variable to be true
    void cell_reset(int i, int j);              // Reset the cell to the initial state (color = 'w', orb_num = 0)
    void cell_explode(int i, int j);            // The explosion of cell (i, j). It will add the orb into neighbor cells
    void cell_chain_reaction(Player player);    // If there is a explosion, check wether the chain reaction is active
    
public:
    gameboard();
    gameboard(gameboard &rhs);
    // The basic functions of the Board
    int get_orbs_num(int i, int j);
    int get_capacity(int i, int j);
    char get_cell_color(int i, int j);
    void add_orb(int i, int j, char color);     // Add orb into the cell (i, j)
    bool place_orb(int i, int j, Player* player);      // Use this function to place a orb into a cell
    
    bool win_the_game(Player player);       // The function that is used to check wether the player wins the game after his/her placemnet operation
    boardcell boardcells[ROW][COL];         // The 5*6 board whose index (0,0) is start from the upper left corner
    //int score = 0;
    
};
gameboard::gameboard(gameboard &rhs){
    //gameboard b;
    for(int i=0;i<5;i++){
        for(int j=0;j<6;j++){
            this->boardcells[i][j].set_capacity(rhs.boardcells[i][j].get_capacity())  ;
            this->boardcells[i][j].set_color(rhs.boardcells[i][j].get_color());
            this->boardcells[i][j].set_orbs_num(rhs.boardcells[i][j].get_orbs_num());
            this->boardcells[i][j].set_explode(rhs.boardcells[i][j].get_explode());
        }
    }
    
    /*  b.boardcells[0][0].set_capacity(3), b.boardcells[0][5].set_capacity(3),
     b.boardcells[4][0].set_capacity(3), b.boardcells[4][5].set_capacity(3);
     
     // The edges of the board
     b.boardcells[0][1].set_capacity(5), b.boardcells[0][2].set_capacity(5), b.boardcells[0][3].set_capacity(5), b.boardcells[0][4].set_capacity(5),
     b.boardcells[1][0].set_capacity(5), b.boardcells[2][0].set_capacity(5), b.boardcells[3][0].set_capacity(5),
     b.boardcells[1][5].set_capacity(5), b.boardcells[2][5].set_capacity(5), b.boardcells[3][5].set_capacity(5),
     b.boardcells[4][1].set_capacity(5), b.boardcells[4][2].set_capacity(5), b.boardcells[4][3].set_capacity(5), b.boardcells[4][4].set_capacity(5);*/
    
}
gameboard::gameboard(){
    
    ////// Initialize the borad with correct capacity //////
    // The corners of the board
    boardcells[0][0].set_capacity(3), boardcells[0][5].set_capacity(3),
    boardcells[4][0].set_capacity(3), boardcells[4][5].set_capacity(3);
    
    // The edges of the board
    boardcells[0][1].set_capacity(5), boardcells[0][2].set_capacity(5), boardcells[0][3].set_capacity(5), boardcells[0][4].set_capacity(5),
    boardcells[1][0].set_capacity(5), boardcells[2][0].set_capacity(5), boardcells[3][0].set_capacity(5),
    boardcells[1][5].set_capacity(5), boardcells[2][5].set_capacity(5), boardcells[3][5].set_capacity(5),
    boardcells[4][1].set_capacity(5), boardcells[4][2].set_capacity(5), boardcells[4][3].set_capacity(5), boardcells[4][4].set_capacity(5);
    
}
bool gameboard::place_orb(int i, int j, Player * player){
    
    if( (i > 4 || j > 5) || boardcells[i][j].get_color() != player->get_color() ){
        
        int temp = boardcells[i][j].get_orbs_num();
        temp += 1;
        boardcells[i][j].set_orbs_num(temp);
        boardcells[i][j].set_color(player->get_color());
        
    }
    else{
        player->set_illegal();
        return false;
    }
    
    if(cell_is_full(&boardcells[i][j])){
        cell_explode(i, j);
        cell_reaction_marker();
        cell_chain_reaction(*player);
    }
    
    return true;
}

bool gameboard::cell_is_full(boardcell* cell){
    if(cell->get_orbs_num() >= cell->get_capacity()){
        cell->set_explode(true);
        return true;
    }
    else return false;
}

void gameboard::add_orb(int i, int j, char color){
    int orb_num = boardcells[i][j].get_orbs_num();
    orb_num ++;
    boardcells[i][j].set_orbs_num(orb_num);
    boardcells[i][j].set_color(color);
}

void gameboard::cell_reset(int i, int j){
    boardcells[i][j].set_orbs_num(0);
    boardcells[i][j].set_explode(false);
    boardcells[i][j].set_color('w');
}

void gameboard::cell_explode(int i, int j){
    
    int orb_num;
    char color = boardcells[i][j].get_color();
    
    cell_reset(i, j);
    
    if( i + 1 < ROW){
        add_orb(i+1, j, color);
    }
    
    if( j + 1 < COL){
        add_orb(i, j+1, color);
    }
    
    if( i - 1 >= 0){
        add_orb(i-1, j, color);
    }
    
    if( j - 1 >= 0){
        add_orb(i, j-1, color);
    }
    if( i + 1 < ROW && j - 1 >= 0){
        add_orb(i+1, j-1, color);
    }
    if( i - 1 >= 0 && j - 1 >= 0){
        add_orb(i-1, j-1, color);
    }
    if( i + 1 < ROW && j + 1 < COL){
        add_orb(i+1, j+1, color);
    }
    if( i - 1 >= 0 && j + 1 < COL){
        add_orb(i-1, j+1, color);
    }
}

void gameboard::cell_reaction_marker(){
    
    // Mark the next cell whose number of orbs is equal to the capacity
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            cell_is_full(&boardcells[i][j]);
        }
    }
}

void gameboard::cell_chain_reaction(Player player){
    
    bool chain_reac = true;
    
    while(chain_reac){
        
        chain_reac = false;
        
        for(int i = 0; i < ROW; i++){
            for(int j = 0; j < COL; j++){
                if(boardcells[i][j].get_explode()){
                    cell_explode(i ,j);
                    chain_reac = true;
                }
            }
        }
        
        if(win_the_game(player)){
            return;
        }
        
        cell_reaction_marker();
    }
}

bool gameboard::win_the_game(Player player){
    
    char player_color = player.get_color();
    bool win = true;
    
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if(boardcells[i][j].get_color() == player_color || boardcells[i][j].get_color() == 'w') continue;
            else{
                win = false;
                break;
            }
        }
        if(!win) break;
    }
    return win;
}

int gameboard::get_orbs_num(int i, int j){
    return boardcells[i][j].get_orbs_num();
}

int gameboard::get_capacity(int i, int j){
    return boardcells[i][j].get_capacity();
}

char gameboard::get_cell_color(int i, int j){
    return boardcells[i][j].get_color();
}
#define maxdepth 4
int setval(gameboard b){
    int value = 0;
    int cnt_my_color = 0;
    int cnt_enemy_color = 0;
    int cnt_my = 0;
    int cnt_enemy = 0;
    int cnt_my_critical_mass = 0;
    int cnt_enemy_critical_mass = 0;
    char enemy_col = '\0';
    char my_col = '\0';
    if(color == RED){
        enemy_col = BLUE;
        my_col = RED;
    }
    else{
        my_col = BLUE;
        enemy_col = RED;
    }
    
    for(int i=0;i<5;i++){
        for(int j=0;j<6;j++){
            if(my_col ==b.boardcells[i][j].get_color()){
                if(b.boardcells[i][j].get_orbs_num() == b.boardcells[i][j].get_capacity() - 1) cnt_my_critical_mass++;
                
                cnt_my_color += b.boardcells[i][j].get_orbs_num();
                cnt_my ++;
            }
            else if(enemy_col == b.boardcells[i][j].get_color()){
                if(b.boardcells[i][j].get_orbs_num() == b.boardcells[i][j].get_capacity() - 1)
                    cnt_enemy_critical_mass ++;
                
                cnt_enemy_color += b.boardcells[i][j].get_orbs_num();
                cnt_enemy++;
            }
        }
    }
    
    if(cnt_enemy_color == 0) return 99999999;
    else if(cnt_my_color == 0) return -9999999;
    else {
        if(b.boardcells[0][0].get_color() == my_col ){
            value += 600;
            if( b.boardcells[0][1].get_color() == my_col && b.boardcells[1][1].get_color() == my_col && b.boardcells[1][0].get_color() == my_col){
                value += 5000;
            }
            
        }
        if(b.boardcells[0][5].get_color() == my_col ){
            value += 600;
            if(b.boardcells[0][4].get_color() == my_col && b.boardcells[1][4].get_color() == my_col && b.boardcells[1][5].get_color() == my_col){
                value += 5000;
            }
        }
        if( b.boardcells[4][0].get_color() == my_col){
            value += 600;
            if(b.boardcells[3][0].get_color() == my_col && b.boardcells[3][1].get_color() == my_col && b.boardcells[4][1].get_color() == my_col ){
                value += 5000;
            }
        }
        if(b.boardcells[4][5].get_color() == my_col){
            value += 600;
            if(b.boardcells[3][4].get_color() == my_col && b.boardcells[3][5].get_color() == my_col && b.boardcells[4][4].get_color() == my_col ){
                value += 5000;
            }
        }
        
        if(b.boardcells[0][0].get_color() == enemy_col ){
            value -= 100;
            if( b.boardcells[0][1].get_color() == enemy_col && b.boardcells[1][1].get_color() == enemy_col && b.boardcells[1][0].get_color() == enemy_col){
                value -= 2000;
            }
            
        }
        if(b.boardcells[0][5].get_color() == enemy_col ){
            value -= 100;
            if(b.boardcells[0][4].get_color() == enemy_col && b.boardcells[1][4].get_color() == enemy_col && b.boardcells[1][5].get_color() == enemy_col){
                value -= 2000;
            }
        }
        if( b.boardcells[4][0].get_color() == enemy_col){
            value -= 100;
            if(b.boardcells[3][0].get_color() == enemy_col && b.boardcells[3][1].get_color() == enemy_col && b.boardcells[4][1].get_color() == enemy_col ){
                value -= 2000;
            }
        }
        if(b.boardcells[4][5].get_color() == enemy_col){
            value -= 100;
            if(b.boardcells[3][4].get_color() == enemy_col && b.boardcells[3][5].get_color() == enemy_col && b.boardcells[4][4].get_color() == enemy_col ){
                value -= 2000;
            }
        }
        
        value += 8000 * (cnt_my_color - cnt_enemy_color);
        value += (cnt_my / cnt_enemy) * 40 ;
        value -= cnt_my_critical_mass * 20000 ;
        value -= cnt_enemy_critical_mass *22000;
        
        
    }
    return value;
}
class mapping{
public:
    int x;
    int y;
    mapping(int x,int y){
        this->x = x;          //     x ==> value    y ==> axis
        this->y = y;
    }
    mapping (const mapping &rhs){
        this->x = rhs.x;
        this->y = rhs.y;
    }
};

/*bool operator> (const mapping& lhs,const mapping& rhs){
 return lhs.x < rhs.x;
 }*/
struct comp{
    bool operator()(mapping &lhs,mapping &rhs){
        return lhs.x < rhs.x;
    }
};

//     red  ==>  r  ==>  O     blue  ==>  b  ==>  X
int minimax(gameboard b,int depth,bool isMaximizingPlayer,int alpha,int beta,Player play){
    int val = 0;
    int bestval = 0;
    if(maxdepth == depth){
        return setval(b);
    }
    Player *p;
    
    if(play.get_color() == RED){                    // p is the player of the next layer
        p = new Player(BLUE);
    }
    else{
        p = new Player(RED);
    }
    
    if(isMaximizingPlayer == true){
        bestval = INT_MIN;
        for(int i=0;i<5;i++){
            for(int j=0;j<6;j++){
                if( b.get_cell_color(i, j) == play.get_color() || b.get_cell_color(i, j) == 'w'){   //  can place orb
                    gameboard nb(b);
                    nb.place_orb(i, j, &play);
                    val = minimax(nb, depth+1, false, alpha, beta,*p);
                    bestval = max(bestval,val);
                    alpha = max(alpha,bestval);
                    if(beta <= alpha) break;
                    
                }
            }
        }
        return bestval;
    }
    else {
        bestval = INT_MAX;
        for(int i=0;i<5;i++){
            for(int j=0;j<6;j++){
                if( b.get_cell_color(i, j) == play.get_color() || b.get_cell_color(i, j) == 'w'){   //  can place orb
                    gameboard nb(b);
                    nb.place_orb(i, j, &play);
                    val = minimax(nb, depth+1, true, alpha, beta,*p);
                    bestval = min(bestval,val);
                    beta = min(beta,bestval);
                    if(beta <= alpha) break;
                    
                }
            }
        }
        return bestval;
    }
    return bestval;
}

void algorithm_A(Board board, Player player, int index[]){
    
    //////your algorithm design///////////
    color = player.get_color();
    gameboard tmpboard;
    priority_queue< mapping, vector<mapping> , comp >  pq;
    for(int i=0 ; i<5 ; i++){
        for(int j=0 ; j<6 ; j++){
            tmpboard.boardcells[i][j].set_color(board.get_cell_color(i, j));
            tmpboard.boardcells[i][j].set_capacity(board.get_capacity(i, j));
            tmpboard.boardcells[i][j].set_orbs_num(board.get_orbs_num(i, j));
        }
    }
    int val = 0 ;
    int axis = 0 ;
    Player *p;
    if(player.get_color() == RED){                    // p is the player of the next layer
        p = new Player(BLUE);
    }
    else{
        p = new Player(RED);
    }
    
    for(int i=0 ; i<5 ; i++){
        for(int j=0 ; j<6 ; j++){
            axis = 6 * i + j;
            if(tmpboard.get_cell_color(i,j) == 'w'){
                gameboard newboard(tmpboard);
                newboard.place_orb(i, j, &player);
                val = minimax(newboard, 0, false, INT_MIN, INT_MAX,*p);
                pq.push(mapping(val,axis));
            }
            else {
                if( tmpboard.get_cell_color(i,j) == color ){
                    gameboard newboard(tmpboard);
                    newboard.place_orb(i, j, &player);
                    val = minimax(newboard, 0, false, INT_MIN, INT_MAX,*p);
                    pq.push(mapping(val,axis));
                }
            }
            
        }
    }
    
    mapping mp = pq.top();
    index[0] = mp.y / 6;
    index[1] = mp.y % 6;
    
}
/*
 
 void algorithm_A(Board board, Player player, int index[]){
 srand(time(NULL)*time(NULL));
 int row, col;
 int color = player.get_color();
 
 while(1){
 row = rand() % 5;
 col = rand() % 6;
 if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
 }
 
 index[0] = row;
 index[1] = col;
 }
 */
