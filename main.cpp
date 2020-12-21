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
	printf("��ʼ����:\n");
	puzzle.printBoard();
	/*
	//ԭʼ��dfs��������
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
	//���붯̬��ʾ�ƶ�����
	//�޸�NNPuzzle.h line75��76�е�ʡȱ����
	NNPuzzleSolver solver(puzzle.board);
	string solution = solver.solve(solver.size);
	printf("\n������:\n"); solver.printBoard();
	printf("\nTotal Steps: %zd\n", solution.size());

#ifdef LOCAL
	printf("\n\n");
	clock_t end = clock();
	double endtime = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "Total time:" << endtime << "s" << endl;        //sΪ��λ
	cout << "Total time:" << endtime * 1000 << "ms" << endl;    //msΪ��λ
#endif //LOCAL

	return 0;
}