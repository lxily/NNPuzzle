#include "NNPuzzle.h"

int main() {
#ifdef LOCAL
	clock_t start = clock();
#endif //LOCAL

	srand((unsigned)time(0));
	vector<int>drs;
	NDigit puzzle(boardSize, alpha, beta);
	puzzle.shuffle(shufferLength);
	//nd.printShuffleList();
	printf("初始局面:\n");
	puzzle.printBoard();
	/*
	//原始的dfs暴力搜索
	puzzle.dfs(drs,0);
	printf("After Search:\n");
	printf("Find the number of states: %d\n",(int)puzzle.mp.size());
	if(puzzle.solution.size()==0){
	    printf("No solution found!\n");
	}
	printf("The optimal solution found is as follows:\n");
	puzzle.printBestBoard();
	puzzle.printBoard();
	*/
	//若想动态显示移动过程
	//修改NNPuzzle.h line75、76中的省缺参数
	NNPuzzleSolver solver(puzzle.board);
	string solution = solver.solve(solver.size);
	printf("\n求解局面:\n"); solver.printBoard();
	printf("\nTotal Steps: %zd\n", solution.size());

#ifdef LOCAL
	printf("\n\n");
	clock_t end = clock();
	double endtime = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "Total time:" << endtime << "s" << endl;        //s为单位
	cout << "Total time:" << endtime * 1000 << "ms" << endl;    //ms为单位
#endif //LOCAL

	return 0;
}