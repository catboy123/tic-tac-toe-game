#include "brain.h"

int get_table_hash(int tbl[][5]) {
	return tbl[1][1] + tbl[1][2] * 3 + tbl[1][3] * 9
		+ tbl[2][1] * 27 + tbl[2][2] * 81 + tbl[2][3] * 243
		+ tbl[3][1]*729 + tbl[3][2] * 2187 + tbl[3][3] * 6561;
}
int game_ended(int table[][5]) {
	int cc[8] = { 0, -1, 1, 0, -1, 1, -1, 1 };
	int cr[8] = { -1, 0, 0, 1, -1, -1, 1, 1 };
	int not_go = false;
	for (int i = 1; i <= 3; i++){
		for (int j = 1; j <= 3; j++){
			if (table[i][j] == 0){
				not_go = true;
				continue; // not go
			}
			for (int k = 0; k<8; k++){
				for (int step = 1; step <= 2; step++){
					int newi = i + cc[k] * step;
					int newj = j + cr[k] * step;
					if (0 >= newi || newi >= 4 || 0 >= newj || newj >= 4){
						continue;
					}
					if (table[i][j] != table[newi][newj])
						break;
					if (step + 1 == 3){
						return table[i][j];
					}
				}
			}
		}
	}
	if (!not_go){
		return 0;
	}
	return -1;
}
int optimal_score(int table[][5], int computer,  int player, int scoreTbl[][3], int nextMoveR[][3], int nextMoveC[][3], bool evaluated[][3]) {

	const int infinity = 10000;
	int penalty;
	if (player == computer) {
		if (player == 1) penalty = -1;
		else penalty = +1;
	}
	else penalty = 0;

	int hash = get_table_hash(table);
	if (evaluated[hash][player]) return scoreTbl[hash][player];

	int tmp = game_ended(table);
	int ans = 0; nextMoveR[hash][player] = nextMoveC[hash][player] = -1; //default: no next move at all.
	if (tmp == 1) {
		ans = infinity;
	}
	else if (tmp == 2) {
		ans = -infinity;
	}
	else if (tmp == 0) {
		ans = 0;
	}
	else { //tmp==-1
		int traceR = -1, traceC = -1;
		if (player == 1) ans = -infinity;
		else ans = infinity;
		for (int i = 1; i <= 3; ++i)
		for (int j = 1; j <= 3; ++j)
		if (table[i][j] == 0) {
			table[i][j] = player;
			int tmp = optimal_score(table, computer, 3 - player, scoreTbl, nextMoveR, nextMoveC, evaluated);
			if ((player == 1 && tmp > ans) || (player == 2 && tmp < ans)) {
				ans = tmp;
				traceR = i; traceC = j;
			}
			table[i][j] = 0;
		}
		//Penalty for one turn late.
		ans += penalty;
		nextMoveR[hash][player] = traceR;
		nextMoveC[hash][player] = traceC;
	}

	evaluated[hash][player] = 1;
	return scoreTbl[hash][player] = ans;
}