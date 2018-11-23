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
#define VITALITY_COEFFICIENT 1.38 //����ϵ��c��ѡȡ���� 

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
		for (int i = 0; i != column; i++)
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
		for (int i = 0; i != row; i++)
			delete[] boardState[i];
		delete[] boardState;
		delete[] topState;
		delete[] expandablenode;
		for (int i = 0; i != column; i++)
			if (children[i]) {
				children[i]->clear();
				delete children[i];
			}
		delete[] children;
	}

public:
	//���캯�� 
	node(int **board, int *top, int r, int c, int noX, int noY, int depth = 0, int x = -1, int y = -1, int playingRight = MACHINE_CHANCE, node* _father = NULL) :
		boardState(board), topState(top), row(r), column(c), _noX(noX), _noY(noY), _depth(depth), lastx(x), lasty(y), _chessman(playingRight), visitednum(0), profit(0), father(_father) {
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
	int Profit(int **board, int *top, int chessman, int x, int y) const {
		if (chessman == USER_CHANCE && userWin(x, y, _row, _column, board))
			return USER_WIN_PROFIT;
		if (chessman == MACHINE_CHANCE && machineWin(x, y, _row, _column, board))
			return MACHINE_WIN_PROFIT;
		if (isTie(_column, top))
			return TIE_PROFIT;
		return UNTERMINAL_STATE; //δ������ֹ״̬ 
	}
	//������� 
	void placeChessman(int **board, int *top, int chessman, int &x, int &y) {
		y = rand() % _column; //���ѡ��һ�� 
		while (top[y] == 0) //������������ 
			y = rand() % _column; //�����ѡ��һ�� 
		x = --top[y]; //ȷ�����Ӹ߶� 
		board[x][y] = chessman; //���� 
		if (x - 1 == _noX && y == _noY) //������λ�����Ϸ����ڲ������ӵ� 
			top[y] --;
	}
	//��Ȩ�任 
	int rightChange(int chessman) const {
		if (chessman == USER_CHANCE)
			return MACHINE_CHANCE;
		else if (chessman == MACHINE_CHANCE)
			return USER_CHANCE;
		else
			return -1;
	}

	//���������� 
	node *TreePolicy(node *presentNode) {
		while (!presentNode->isTerminal()) { //�ڵ㲻����ֹ�ڵ� 
			if (presentNode->isExpandable()) //��ӵ��δ�����ʵ���״̬ 
				return Expand(presentNode); //��չ�ýڵ� 
			else
				presentNode = BestChild(presentNode); //ѡ�������ӽڵ� 
		}
		return presentNode;
	}
	//�Խڵ������չ
	node *Expand(node *presentNode) { return presentNode->expand(rightChange(presentNode->chessman())); }
	//�����ӽڵ� 
	node *BestChild(node *father) { return father->bestChild(); }
	//ģ����� 
	double DefaultPolicy(node *selectedNode) {
		int **boardState = selectedNode->BoardState(), *top = selectedNode->TopState();
		int chessman = selectedNode->chessman(), depth = selectedNode->_depth;
		int x = selectedNode->x(), y = selectedNode->y();
		int profit = Profit(boardState, top, rightChange(chessman), x, y); //�������� 
		while (profit == UNTERMINAL_STATE) { //����ǰ״̬δ����ֹ״̬ 
			depth++;
			placeChessman(boardState, top, chessman, x, y); //������� 
			profit = Profit(boardState, top, chessman, x, y); //�������� 
			chessman = rightChange(chessman); //��Ȩ�任 
		}
		for (int i = 0; i != _row; i++)
			delete[] boardState[i];
		delete[] boardState;
		delete[] top;
		return double(profit);// / logl(depth + 1); //�����Լ���
	}
	//���ݸ�������(���Խ������ԽС)
	void Backup(node *selectedNode, double deltaProfit) { selectedNode->backup(deltaProfit); }

public:
	//���캯�� 
	UCT(int row, int column, int noX, int noY) : _row(row), _column(column), _noX(noX), _noY(noY), startTime(clock()) {}
	//�������������� 
	node *search(int **boardState, int *topState) {
		_root = new node(boardState, topState, _row, _column, _noX, _noY); //�Ե�ǰ״̬�������ڵ� 
		while (clock() - startTime <= TIME_LIMITATION) { //��δ�ľ�����ʱ�� 
			node *selectedNode = TreePolicy(_root); //�������������Խڵ� 
			double deltaProfit = DefaultPolicy(selectedNode); //����ģ����Զ�ѡ�нڵ����һ�����ģ�� 
			Backup(selectedNode, deltaProfit); //��ģ�������ݷ����������� 
		}
		return BestChild(_root);
	}
	//�������� 
	~UCT() { _root->clear(); delete _root; }
};

#endif //__UCT_H__