#ifndef __UCT_H__
#define __UCT_H__

#include <iostream>
#include <time.h>
#include "Judge.h" //�����ж�ʤ���ĺ���userWin��machineWin��isTie 

//#define EMPTY 0 //δ���� 
#define USER_CHANCE 1 //�Է���Ȩ 
#define MACHINE_CHANCE 2 //������Ȩ 
#define TIME_LIMITATION 2500 //ʱ������
#define USER_WIN_PROFIT -1 //�Է���ʤ���� 
#define MACHINE_WIN_PROFIT 1 //������ʤ���� 
#define TIE_PROFIT 0 //ƽ������
#define UNTERMINAL_STATE 2 //����ֹ״̬ 
#define VITALITY_COEFFICIENT 0.8 //����ϵ��c��ѡȡ���� 

using namespace std;

class UCT;

class node
{
private:
	int **boardState; //���״̬
	int *topState; //����״̬
	int row, column; //���̴�С��M, N��
	int _noX, _noY; //�������ӵ�λ�� 
	int _chessman; //�ҷ��������� 
	int lastx, lasty; //ǰһ������λ��
	int visitednum; //�����ʴ��� 
	double profit; //��ǰ״̬�ҷ�����
	int _depth; //�ڵ���� 
	node *father; //���ڵ�
	node **children; //�ӽڵ�
	int expandablenum; //����չ�ڵ����� 
	int *expandablenode; //����չ�ڵ��� 
	friend class UCT;

	int *TopState() const { //�������̶���״̬����topState 
		int *presentTop = new int[column];
		for (int i = 0; i < column; i++)
			presentTop[i] = topState[i];
		return presentTop;
	}
	int **BoardState() const { //��������״̬����boardState 
		int **presentBoardState = new int*[row];
		for (int i = 0; i < row; i++) {
			presentBoardState[i] = new int[column];
			for (int j = 0; j < column; j++)
				presentBoardState[i][j] = boardState[i][j];
		}
		return presentBoardState;
	}
	void clear() { //�ռ��ͷ�
		for (int i = 0; i < row; i++)
			delete[] boardState[i];
		delete[] boardState;
		delete[] topState;
		delete[] expandablenode;
		for (int i = 0; i != column; i++)
			if (children[i]) {
				children[i]->clear();
				delete[] children[i];
			}
		delete[] children;
	}

public:
	//���캯�� 
	node(int **board, int *top, int M, int N, int noX, int noY, int depth = 0, int x = -1, int y = -1, int playingRight = MACHINE_CHANCE, node* _father = NULL)
	{
		boardState = new int*[M];
		for (int i = 0; i < M; i++)
		{
			boardState[i] = new int[N];
			for (int j = 0; j < N; j++)
				boardState[i][j] = board[i][j];
		}
		topState = new int[N];
		for (int i = 0; i < N; i++)
			topState[i] = top[i];
		row = M;
		column = N;
		_noX = noX;
		_noY = noY;
		_depth = depth;
		lastx = x;
		lasty = y;
		_chessman = playingRight;
		visitednum = 0;
		profit = 0;
		father = _father;
		expandablenum = 0;
		children = new node*[column]; //��С�����������ӽڵ����� 
		expandablenode = new int[column]; //�ɵ����ӽڵ��ŵ����� 
		for (int i = 0; i != column; i++) {
			if (topState[i] != 0) //����i�п����� 
				expandablenode[expandablenum++] = i;
			children[i] = NULL;
		}
	}
	int x() const { return lastx; }
	int y() const { return lasty; }
	int chessman() const { return _chessman; }
	bool isExpandable() const { return expandablenum > 0; }//�Ƿ����չ
	//�Ƿ�Ϊ��ֹ�ڵ� 
	bool isTerminal() {
		if (lastx == -1 && lasty == -1) //��Ϊ���ڵ� 
			return false;
		if ((_chessman == USER_CHANCE && machineWin(lastx, lasty, row, column, boardState)) || //�����ʤ�� 
			(_chessman == MACHINE_CHANCE && userWin(lastx, lasty, row, column, boardState)) || //���ʤ�� 
			(isTie(column, topState))) //ƽ�� 
			return true;
		return false;
	}
	//��չ�ڵ� 
	node *expand(int playingRight) {
		int index = rand() % expandablenum; //���ȷ��һ������ֵ 
		int **newBoardState = BoardState(); //��������״̬���� 
		int *newTopState = TopState(); //�������̶���״̬���� 
		int newY = expandablenode[index], newX = --newTopState[newY]; //ȷ���������� 
		newBoardState[newX][newY] = chessman(); //���� 
		if (newX - 1 == _noX && newY == _noY) //������λ�õ����Ϸ�λ���ǲ������ӵ� 
			newTopState[newY] --; //�������̶���״̬����
		//Ϊ��ǰ�ڵ㴴����չ�ӽڵ� 
		children[newY] = new node(newBoardState, newTopState, row, column, _noX, _noY, _depth + 1, newX, newY, playingRight, this);
		swap(expandablenode[index], expandablenode[--expandablenum]); //����ѡ���ӽڵ����û���Ŀ¼ĩβ
		return children[newY];
	}
	//�����ӽڵ�
	node *bestChild() {
		node* best;
		double maxProfitRatio = -RAND_MAX;
		for (int i = 0; i != column; i++) {
			if (children[i] == NULL) continue;
			double modifiedProfit = (_chessman == USER_CHANCE ? -1 : 1) * children[i]->profit; //��������ֵ
			int childVisitedNum = children[i]->visitednum; //�ӽڵ������ 
			double tempProfitRatio = modifiedProfit / childVisitedNum +
				sqrtl(2 * logl(visitednum) / childVisitedNum) * VITALITY_COEFFICIENT; //�����ۺ������� 
			if (tempProfitRatio > maxProfitRatio || (tempProfitRatio == maxProfitRatio && rand() % 2 == 0)) { //ѡ���ۺ������������ӽڵ� 
				maxProfitRatio = tempProfitRatio;
				best = children[i];
			}
		}
		return best;
	}
	//���ݸ���
	void backup(double deltaProfit) {
		node *temp = this;
		while (temp) {
			temp->visitednum++; //���ʴ���+1 
			temp->profit += deltaProfit; //��������delta 
			temp = temp->father;
		}
	}
};

class UCT
{
private:
	node *_root; //���ڵ�
	int _row, _column; //����������
	int _noX, _noY; //�������ӵ��λ�� 
	int startTime; //���㿪ʼʱ��

	//���㵱ǰ״̬����
	int profit(int **board, int *top, int chessman, int x, int y) const;
	//������� 
	void placechessman(int **board, int *top, int chessman, int &x, int &y);
	//��Ȩ�任 
	int rightchange(int chessman) const;
	//���������� 
	node *treepolicy(node *presentNode);
	//�Խڵ������չ
	node *expand(node *presentNode);
	//�����ӽڵ� 
	node *bestchild(node *father);
	//ģ����� 
	double defaultpolicy(node *selectedNode);
	//���ݸ�������(���Խ������ԽС)
	void backup(node *selectedNode, double deltaProfit);

public:
	//���캯�� 
	UCT(int row, int column, int noX, int noY);
	//�������������� 
	node *search(int **boardState, int *topState); 
	//�������� 
	~UCT();
};

#endif //__UCT_H__