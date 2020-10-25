#ifndef _brain_h_
#define _brain_h_

int get_table_hash(int tbl[][5]);
int game_ended(int table[][5]);
int optimal_score(int table[][5], int computer, int player, int scoreTbl[][3], int nextMoveR[][3], int nextMoveC[][3], bool evaluated[][3]);

#endif