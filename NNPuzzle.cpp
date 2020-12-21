#include "NNPuzzle.h"

void cls(){
	COORD coor = { 0,0 };    //这是Windows API的结构体，用来存储光标位置
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coor);
	//for (int i = 0; i < 30; i++) {
	//	coor.X = i;
	//	coor.Y = 0;
	//	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coor);
	//	printf("                           \n");
	//}
}

NDigit::NDigit(int _size, int _alpha, int _beta){
	size = _size;
	alpha = _alpha;
	beta = _beta;
	solved = false;
	bestTarget = inf;
	board.resize(size);
	correct.resize(size*size);
	for (int i = 0; i < size; ++i) {
		board[i].resize(size);
		for (int j = 0; j < size; ++j) {
			correct[i*size + j].resize(2);
			board[i][j] = i * size + j;
			correct[i*size + j][0] = i;
			correct[i*size + j][1] = j;

		}
	}
	zx = 0; zy = 0;
}

bool NDigit::moveOne(int dr){
	if (!legalMove(dr))return false;
	int nx = zx + dir[dr][0];
	int ny = zy + dir[dr][1];
	swap(board[zx][zy], board[nx][ny]);
	zx = nx; zy = ny;
	return true;
}

bool NDigit::moveList(vector<int> drs) {
	for (int i = 0; i < (int)drs.size(); ++i) {
		if (!legalMove(drs[i])) {
			for (int j = i - 1; j >= 0; --j) {
				moveOne(drs[j]);
			}
			return false;
		}
		moveOne(drs[i]);
	}
	return true;
}

int NDigit::evaluate() {
	int ret = 0;
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			int d = board[i][j];
			int dx = abs(i - correct[d][0]);
			int dy = abs(j - correct[d][1]);
			ret += alpha * (dx + dy) + beta * (dx || dy);
		}
	}
	return ret;
}

void NDigit::shuffle(const int _shuffleLength){
	shuffleList.clear();
	for (int i = 0, dr; i < _shuffleLength; ++i) {
		while (true) {
			dr = rand() % 4;
			if (!legalMove(dr))continue;
			else if (i&&shuffleList[i - 1] == (dr ^ 1))continue;
			else break;
		}
		moveOne(dr);
		shuffleList.push_back(dr);
		shuffleReverse.push_back(dr ^ 1);
	}
	reverse(shuffleReverse.begin(), shuffleReverse.end());
}

ll NDigit::hash() {
	ll ret = 0;
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			ret = ret * size*size + board[i][j];
		}
	}
	return ret;
}

void NDigit::dfs(vector<int>& drs, int dep) {
	ll hashValue = hash();
	if (mp.count(hashValue) && mp[hashValue] <= dep) {
		return;
	}
	mp[hashValue] = dep;
	int target = evaluate();
	solved |= !target;
	if (bestTarget > target) {
		bestBoard = board;
		bestTarget = target;
	}
	if (solved)solution = drs;
	if (dep > min(shufferLength, maxDeep))return;
	vector<pii>nxdrs;
	for (int i = 0; i < 4; ++i) {
		if (legalMove(i)) {
			moveOne(i);
			nxdrs.push_back(mkp(evaluate(), i));
			moveOne(i ^ 1);
		}
	}
	//sort(nxdrs.begin(),nxdrs.end(),greater<pii>());
	sort(nxdrs.begin(), nxdrs.end(), less<pii>());
	for (int i = 0; i < (int)nxdrs.size() && !solved; ++i) {
		drs.push_back(nxdrs[i].second);
		moveOne(nxdrs[i].second);
		dfs(drs, dep + 1);
		moveOne(nxdrs[i].second ^ 1);
		drs.pop_back();
	}
}

void NDigit::printBoard() {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			printf("%3d ", board[i][j]);
		}
		printf("\n");
	}
}

void NDigit::printBestBoard() {
	printf("BestBoard:\n");
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			printf("%3d ", bestBoard[i][j]);
		}
		printf("\n");
	}
}

void NDigit::printSolution() {
	printf("Solution: length=%d\n", (int)solution.size());
	for (int i = 0; i < (int)solution.size(); ++i) {
		if (i&&i % 80 == 0)putchar('\n');
		putchar(dirString[solution[i]]);
	}
	putchar('\n');
}

void NDigit::printShuffleList() {
	printf("ShuffleList: length=%d\n", (int)shuffleList.size());
	for (int i = 0; i < (int)shuffleList.size(); ++i) {
		if (i&&i % 80 == 0)putchar('\n');
		putchar(dirString[shuffleList[i]]);
	}
	putchar('\n');
}

void NDigit::printShuffleReverse() {
	printf("ShuffleReverse: length=%d\n", (int)shuffleReverse.size());
	for (int i = 0; i < (int)shuffleReverse.size(); ++i) {
		if (i&&i % 80 == 0)putchar('\n');
		putchar(dirString[shuffleReverse[i]]);
	}
	putchar('\n');
}

NNPuzzleSolver::NNPuzzleSolver(vector<vector<int>> _board, vector<vector<int>> _target) {
	board = _board;
	initial = _board;
	size = (int)_board.size();
	placed.resize(size);
	position.resize(size*size);
	if (_target.size()) {
		target = _target;
	}
	else {
		target.resize(size);
		for (int i = 0; i < size; ++i) {
			target[i].resize(size);
			for (int j = 0; j < size; ++j) {
				target[i][j] = i * size + j;
			}
		}
	}
	for (int i = 0; i < size; ++i) {
		placed[i].resize(size);
		for (int j = 0; j < size; ++j) {
			placed[i][j] = false;
			position[board[i][j]] = mkp(i, j);
			if (_board[i][j] == 0) { zeroX = i; zeroY = j; }
		}
	}
	mp['U'] = 0; mp['D'] = 1; mp['L'] = 2; mp['R'] = 3;
}

/*
string NNPuzzleSolver::getRoadBetweenPoints(int N, int sx, int sy, int tx, int ty) {
	vector<vector<bool>>vis = placed;
	queue<piis>Q; Q.push(mkp(mkp(sx, sy), ""));
	vis[sx][sy] = true;
	while (!Q.empty()) {
		piis out = Q.front(); Q.pop();
		int x = out.fi.fi;
		int y = out.fi.se;
		if (x == tx && y == ty)
			return out.second;
		for (int i = 0; i < 4; ++i) {
			int ix = x + dir[i][0];
			int iy = y + dir[i][1];
			if (ix >= 0 && ix < N&&iy >= 0 && iy < N && !vis[ix][iy]) {
				vis[ix][iy] = true;
				string road = out.se;
				road.push_back(dirString[i]);
				Q.push(mkp(mkp(ix, iy), road));
				if (ix == tx && iy == ty)return road;
			}
		}
	}
	return "No Road found!!!";
}
*/
string NNPuzzleSolver::getRoadBetweenPoints(int N, int sx, int sy, int tx, int ty) {
	vector<vector<bool>>vis = placed;
	priority_queue<pair<int, piis>>Q;
	int dist = -abs(sx - tx) - abs(sy - ty);
	Q.push(mkp(dist, mkp(mkp(sx, sy), "")));
	vis[sx][sy] = true;
	while (!Q.empty()) {
		pair<int, piis> out = Q.top(); Q.pop();
		int x = out.se.fi.fi;
		int y = out.se.fi.se;
		if (x == tx && y == ty)
			return out.se.se;
		for (int i = 0; i < 4; ++i) {
			int ix = x + dir[i][0];
			int iy = y + dir[i][1];
			int idist = -abs(ix - tx) - abs(iy - ty);
			if (ix >= 0 && ix < N&&iy >= 0 && iy < N && !vis[ix][iy]) {
				vis[ix][iy] = true;
				string road = out.se.se;
				road.push_back(dirString[i]);
				Q.push(mkp(idist, mkp(mkp(ix, iy), road)));
				if (ix == tx && iy == ty)return road;
			}
		}
	}
	return "No Road found!!!";
}


bool NNPuzzleSolver::moveOne(int x, int y, char c, bool obs) {
	int nx = x + dir[mp[c]][0];
	int ny = y + dir[mp[c]][1];
	swap(board[x][y], board[nx][ny]);
	swap(position[board[x][y]], position[board[nx][ny]]);
	if (obs) {
		cls();
		printf("初始局面：\n"); printInitial();
		printf("当前局面：\n"); printBoard();
		Sleep(100);
	}
	
	return true;
}

bool NNPuzzleSolver::moveList(int x, int y, string road, bool obs) {
	for (int i = 0; i < (int)road.size(); ++i) {
		moveOne(x, y, road[i], obs);
		x += dir[mp[road[i]]][0];
		y += dir[mp[road[i]]][1];
	}
	return true;
}

string NNPuzzleSolver::recover(int N, int sx, int sy, int tx, int ty) {
	string road = getRoadBetweenPoints(N, sx, sy, tx, ty);
	if (road == "No Road found!!!") {
		cout << "No Road found!!!" << endl; exit(0);
	}
	vector<pii>coorOfRoad;
	for (int i = 0, a = sx, b = sy; i < (int)road.size(); ++i) {
		a += dir[mp[road[i]]][0];
		b += dir[mp[road[i]]][1];
		coorOfRoad.push_back(mkp(a, b));
	}
	string ret = "";
	for (int i = 0; i < (int)coorOfRoad.size(); ++i) {
		placed[sx][sy] = true;
		pii aim = coorOfRoad[i];
		pii spa = position[0];
		string spaceMove = getRoadBetweenPoints(N, spa.fi, spa.se, aim.fi, aim.se);
		if (spaceMove == "No Road found!!!") {
			cout << "No Road found!!!" << endl; exit(0);
		}
		moveList(spa.fi, spa.se, spaceMove);
		ret += spaceMove;
		int dx = aim.fi - sx;
		int dy = aim.se - sy;
		char dr = dx == 1 ? 'U' : dx == -1 ? 'D' : dy == 1 ? 'L' : 'R';
		moveOne(aim.fi, aim.se, dr);
		ret += dr;
		placed[sx][sy] = false;
		sx = aim.fi;
		sy = aim.se;
	}
	placed[sx][sy] = true;
	return ret;
}

string NNPuzzleSolver::dealWithLeftDownCorner(int N, int x, int y) {
	string ret = "";
	pii cor = position[target[x][y]];
	int sx = cor.first, sy = cor.second;
	ret+=recover(N, sx, sy, x - 1, y);//注意此处被标记为了placed
	placed[x - 1][y] = false;   //消除标记
	pii spa = position[0];
	if (spa.fi == x && spa.se == y) {
		moveOne(x, y, 'U');
		ret += 'U';
		return ret;
	}
	if (spa.fi == x - 1) {
		ret += "UL"; moveList(spa.fi, spa.se, "UL");
	}
	ret += "DDRULURDDLU"; moveList(x - 2, y, "DDRULURDDLU");
	placed[x][y] = true;
	return ret;
}

string NNPuzzleSolver::dealWithRightUpCorner(int N, int x, int y) {
	string ret = "";
	pii cor = position[target[x][y]];
	int sx = cor.first, sy = cor.second;
	ret+=recover(N, sx, sy, x, y - 1);	//注意此处被标记为了placed
	placed[x][y - 1] = false;			//消除标记
	pii spa = position[0];
	if (spa.fi == x && spa.se == y) {
		moveOne(x, y, 'L');
		ret += 'L';
		return ret;
	}
	if (spa.se == y - 1) {
		ret += "LU"; moveList(spa.fi, spa.se, "LU");
	}
	ret += "RRDLULDRRUL"; moveList(x, y - 2, "RRDLULDRRUL");
	placed[x][y] = true;
	return ret;
}

void NNPuzzleSolver::init(vector<vector<int>> _board, vector<vector<int>> _target){
	board = _board;
	initial = _board;
	size = (int)_board.size();
	placed.resize(size);
	position.resize(size*size);
	if (_target.size()) {
		target = _target;
	}
	else {
		target.resize(size);
		for (int i = 0; i < size; ++i) {
			target[i].resize(size);
			for (int j = 0; j < size; ++j) {
				target[i][j] = i * size + j;
			}
		}
	}
	for (int i = 0; i < size; ++i) {
		placed[i].resize(size);
		for (int j = 0; j < size; ++j) {
			placed[i][j] = false;
			position[board[i][j]] = mkp(i, j);
			if (_board[i][j] == 0) { zeroX = i; zeroY = j; }
		}
	}
	//db2(board[zeroX][zeroY], initial[zeroX][zeroY]); exit(0);
}

string NNPuzzleSolver::solve(int N) {
	string ret = "";
	if (N == 2) {
		pii cor = position[target[1][1]];
		int sx = cor.first, sy = cor.second;
		ret += recover(N, sx, sy, 1, 1);
		placed[1][1] = true;
		if (board[0][1] == 0) {
			moveOne(0, 1, 'L');
			ret.push_back('L');
		}
		else if (board[1][0] == 0) {
			moveOne(1, 0, 'U');
			ret.push_back('U');
		}
		placed[1][1] = true;
		placed[0][0] = true;
		return ret;
	}
	int clockCount = 0;
	for (int i = N - 1; i >= 1; --i) {
		pii cor = position[target[N - 1][i]];
		int sx = cor.first, sy = cor.second;
		ret += recover(N, sx, sy, N - 1, i);
	}
	ret += dealWithLeftDownCorner(N, N - 1, 0);
	for (int i = N - 1; i >= 1; --i) {
		pii cor = position[target[i][N - 1]];
		int sx = cor.first, sy = cor.second;
		ret += recover(N, sx, sy, i, N - 1);
	}
	ret += dealWithRightUpCorner(N, 0, N - 1);
	ret += solve(N - 1);

	return ret;
}

void NNPuzzleSolver::printBoard() {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			printf("%3d ", board[i][j]);
		}
		printf("\n");
	}
}

void NNPuzzleSolver::printPlaced() {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			cout << placed[i][j] << " ";
		}
		printf("\n");
	}
}

void NNPuzzleSolver::printInitial(){
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			printf("%3d ", initial[i][j]);
		}
		printf("\n");
	}
}

void NNPuzzleSolver::printPosition(){
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				int val = i * size + j;
				printf("(%3d, %3d) ", position[val].fi, position[val].se);
			}
			printf("\n");
		}
	}
