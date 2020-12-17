#pragma once
#include<bits/stdc++.h>
#include<unordered_map>
#include<windows.h>
#include <dos.h>
using namespace std;
#define inf 0x3f3f3f3f
#define pii pair<int,int>
#define piis pair<pii,string>
#define mkp(a,b) make_pair(a,b)
#define ll unsigned long long
#define fi first
#define se second
#define db1(x) cerr<<#x<<" = "<<x<<endl
#define db2(x,y) cerr<<#x<<" = "<<x<<" "<<#y<<" = "<<y<<endl;
#define db3(x,y,z) cerr<<#x<<" = "<<x<<" "<<#y<<" = "<<y<<" "<<#z<<" = "<<z<<endl;
const int boardSize = 100;   //棋盘大小
const int alpha = 0;  //距离误差
const int beta = 1;   //位置误差
const int maxDeep = 5;    //搜索最大深度
const int shufferLength = 3000000; //打乱次数
const string dirString = "UDLR";
const int dir[4][2] = { -1,0,1,0,0,-1,0,1 };

//暴力算法可能以后会添加启发式实现
struct NDigit {
	int size;
	int zx, zy;
	bool solved;
	int alpha, beta;
	int bestTarget;
	vector<vector<int>>board;
	vector<vector<int>>bestBoard;
	vector<vector<int>>correct;
	unordered_map<ll, int>mp;
	vector<int>solution;
	vector<int>shuffleList;
	vector<int>shuffleReverse;

	NDigit() {}
	NDigit(int _size, int _alpha, int _beta);
	bool legalPosition(int x, int y) {
		return x >= 0 && x < size&&y >= 0 && y < size;
	}
	bool legalMove(int dr) {
		return legalPosition(zx + dir[dr][0], zy + dir[dr][1]);
	}
	bool moveOne(int dr);
	bool moveList(vector<int>drs);
	int evaluate();
	void shuffle(const int _shuffleLength);
	ll hash();
	void dfs(vector<int>&drs, int dep);

	void printBoard();
	void printBestBoard();
	void printSolution();
	void printShuffleList();
	void printShuffleReverse();
};

//构造+搜索+递归，多项式复杂度得到解
struct NNPuzzleSolver {
	int size, zeroX, zeroY;
	vector<pii>position;
	vector<vector<int>>board;
	vector<vector<int>>target;
	vector<vector<int>>initial;
	vector<vector<bool>>placed;
	unordered_map<char, int>mp;

	NNPuzzleSolver() {}
	NNPuzzleSolver(vector<vector<int>> _board, vector<vector<int>> _target = {});
	string getRoadBetweenPoints(int N, int sx, int sy, int tx, int ty);
	bool moveOne(int x, int y, char c, bool obs = false);
	bool moveList(int x, int y, string road, bool obs = false);
	string recover(int N, int sx, int sy, int tx, int ty);
	string dealWithLeftDownCorner(int N, int x, int y);
	string dealWithRightUpCorner(int N, int x, int y);
	void init(vector<vector<int>>_board, vector<vector<int>> _target = {});
	string solve(int N);
	void printBoard();
	void printPlaced();
	void printInitial();
	void printPosition();
};


