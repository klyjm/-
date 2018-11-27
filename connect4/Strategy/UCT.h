//#ifndef __UCT_H__
//#define __UCT_H__
//
//#include <iostream>
//#include <time.h>
//#include "Point.h"
//#include "Judge.h"
//
//#define USER_CHANCE 1 //�Է�����
//#define MACHINE_CHANCE 2 //�������� 
//#define TIME_LIMITATION 2500 //ʱ������
//#define USER_WIN_PROFIT -1 //�Է���ʤ���� 
//#define MACHINE_WIN_PROFIT 1 //������ʤ���� 
//#define TIE_PROFIT 0 //ƽ������
//#define UNTERMINAL_STATE 2 //����ֹ״̬ 
//#define C 0.8 //����ϵ��c��ѡȡ���� 
//
//using namespace std;
//
//class UCT;
//
//class node
//{
//private:
//	int **boardState; //�������
//	int *topState; //��������
//	int row, column; //����������
//	int nox, noy; //��������λ�� 
//	int chesschance; //��Ȩ 
//	int lastx, lasty; //��һ������λ��
//	int visitednum; //�����ʴ��� 
//	double profit; //��ǰ״̬�ҷ�����
//	int _depth; //�ڵ���� 
//	node *father; //���ڵ�
//	node **children; //�ӽڵ�
//	int expandablenum; //����չ�ڵ����� 
//	int *expandablenode; //����չ�ڵ��� 
//	friend class UCT;
//
//	int *TopState() const { //�������̶���״̬����topState 
//		int *presentTop = new int[column];
//		for (int i = 0; i < column; i++)
//			presentTop[i] = topState[i];
//		return presentTop;
//	}
//	int **BoardState() const { //��������״̬����boardState 
//		int **presentBoardState = new int*[row];
//		for (int i = 0; i < row; i++) {
//			presentBoardState[i] = new int[column];
//			for (int j = 0; j < column; j++)
//				presentBoardState[i][j] = boardState[i][j];
//		}
//		return presentBoardState;
//	}
//	void clear() { //�ռ��ͷ�
//		for (int i = 0; i < row; i++)
//			delete[] boardState[i];
//		delete[] boardState;
//		delete[] topState;
//		delete[] expandablenode;
//		for (int i = 0; i < column; i++)
//			if (children[i])
//			{
//				children[i]->clear();
//				//cout << "OK" << endl;
//				delete[] children[i];
//			}
//		delete[] children;
//	}
//
//public:
//	//���캯�� 
//	node(int **board, int *top, int M, int N, int noX, int noY, int depth = 0, int x = -1, int y = -1, int chessright = MACHINE_CHANCE, node* _father = NULL)
//	{
//		boardState = new int*[M];
//		for (int i = 0; i < M; i++)
//		{
//			boardState[i] = new int[N];
//			for (int j = 0; j < N; j++)
//				boardState[i][j] = board[i][j];
//		}
//		topState = new int[N];
//		for (int i = 0; i < N; i++)
//			topState[i] = top[i];
//		row = M;
//		column = N;
//		nox = noX;
//		noy = noY;
//		_depth = depth;
//		lastx = x;
//		lasty = y;
//		chesschance = chessright;
//		visitednum = 0;
//		profit = 0;
//		father = _father;
//		expandablenum = 0;
//		children = new node*[column]; //��С�����������ӽڵ����� 
//		expandablenode = new int[column]; //�ɵ����ӽڵ��ŵ����� 
//		for (int i = 0; i < column; i++) {
//			if (topState[i] != 0) //����i�п����� 
//				expandablenode[expandablenum++] = i;
//			children[i] = NULL;
//		}
//	}
//	int x() const { return lastx; }
//	int y() const { return lasty; }
//	int whochess() const { return chesschance; }
//	bool isExpandable() const { return expandablenum > 0; }//�Ƿ����չ
//	//�Ƿ�Ϊ��ֹ�ڵ� 
//	bool isTerminal() 
//	{
//		if (lastx == -1 && lasty == -1) //��Ϊ���ڵ� 
//			return false;
//		if ((chesschance == USER_CHANCE && machineWin(lastx, lasty, row, column, boardState)) || //�����ʤ�� 
//			(chesschance == MACHINE_CHANCE && userWin(lastx, lasty, row, column, boardState)) || //���ʤ�� 
//			(isTie(column, topState))) //ƽ�� 
//			return true;
//		return false;
//	}
//	//��չ�ڵ� 
//	node *expand(int chessright)
//	{
//		int index = rand() % expandablenum; //���ȷ��һ������ֵ 
//		//int **newBoardState = BoardState(); //��������״̬���� 
//		int **newBoardState = new int*[row];
//		for (int i = 0; i < row; i++) {
//			newBoardState[i] = new int[column];
//			for (int j = 0; j < column; j++)
//				newBoardState[i][j] = boardState[i][j];
//		}
//		int *newTopState = TopState(); //�������̶���״̬���� 
//		int newy = expandablenode[index], newx = --newTopState[newy]; //ȷ���������� 
//		newBoardState[newx][newy] = chesschance; //���� 
//		if (newx - 1 == nox && newy == noy) //������λ�õ����Ϸ�λ���ǲ������ӵ� 
//			newTopState[newy] --; //�������̶���״̬����
//		//Ϊ��ǰ�ڵ㴴����չ�ӽڵ� 
//		children[newy] = new node(newBoardState, newTopState, row, column, nox, noy, _depth + 1, newx, newy, chessright, this);
//		for (int i = 0; i < row; i++)
//			delete[] newBoardState[i];
//		delete[] newBoardState;
//		delete[] newTopState;
//		swap(expandablenode[index], expandablenode[--expandablenum]); //����ѡ���ӽڵ����û���Ŀ¼ĩβ
//		return children[newy];
//	}
//	//�����ӽڵ�
//	//node *bestChild() {
//	//	node* best;
//	//	double maxProfitRatio = -RAND_MAX;
//	//	for (int i = 0; i != column; i++) {
//	//		if (children[i] == NULL) continue;
//	//		double modifiedProfit = (chesschance == USER_CHANCE ? -1 : 1) * children[i]->profit; //��������ֵ
//	//		int childVisitedNum = children[i]->visitednum; //�ӽڵ������ 
//	//		double tempProfitRatio = modifiedProfit / childVisitedNum +
//	//			sqrtl(2 * logl(visitednum) / childVisitedNum) * C; //�����ۺ������� 
//	//		//if (tempProfitRatio > maxProfitRatio || (tempProfitRatio == maxProfitRatio && rand() % 2 == 0)) { //ѡ���ۺ������������ӽڵ� 
//	//		if (tempProfitRatio > maxProfitRatio)
//	//		{
//	//			maxProfitRatio = tempProfitRatio;
//	//			best = children[i];
//	//		}
//	//	}
//	//	return best;
//	//}
//	//���ݸ���
//	void backup(double deltaProfit) {
//		node *temp = this;
//		while (temp) {
//			temp->visitednum++; //���ʴ���+1 
//			temp->profit += deltaProfit; //��������delta 
//			temp = temp->father;
//		}
//	}
//};
//
//class UCT
//{
//private:
//	node *root; //���ڵ�
//	int row, column; //����������
//	int nox, noy; //�������ӵ��λ�� 
//	int startTime; //���㿪ʼʱ��
//
//	//���㵱ǰ״̬����
//	int profit(int **board, int *top, int chessman, int x, int y) const;
//	//������� 
//	void placechessman(int **board, int *top, int chessman, int &x, int &y);
//	//��Ȩ�任 
//	int rightchange(int chessman) const;
//	//���������� 
//	node *treepolicy(node *presentNode);
//	//�Խڵ������չ
//	node *expand(node *presentNode);
//	//�����ӽڵ� 
//	node *bestchild(node *father);
//	//ģ����� 
//	double defaultpolicy(node *selectedNode);
//	//���ݸ�������(���Խ������ԽС)
//	void backup(node *selectedNode, double deltaProfit);
//
//public:
//	//���캯�� 
//	UCT(int M, int N, int noX, int noY);
//	//�������������� 
//	Point *search(int **boardState, int *topState);
//	//�������� 
//	virtual ~UCT();
//};
//
//#endif //__UCT_H__

#ifndef __UCT_H__
#define __UCT_H__

#include <iostream>
#include <time.h>
#include "Point.h"
#include "Judge.h"

#define USERCHANCE 1
#define MACHINECHANCE 2
#define TIMELIMITE 2600
#define USERWIN -1
#define MACHINEWIN 1
#define TIE 0
#define NOTEND 2
#define C 0.7 //����ϵ��c��ѡȡ���� 

using namespace std;

class UCT;

class node
{
private:
	int **boardarray, *toparray, row, column, nox, noy, depth, chesschance, lastx, lasty, visitednum, expandablenum, *expandablenode;
	double profit;
	node *father, **children;
	friend class UCT;

	int *gettop() const
	{
		int *temp = new int[column];
		for (int i = 0; i < column; i++)
			temp[i] = toparray[i];
		return temp;
	}
	int **getboard() const
	{
		int **temp = new int*[row];
		for (int i = 0; i < row; i++) {
			temp[i] = new int[column];
			for (int j = 0; j < column; j++)
				temp[i][j] = boardarray[i][j];
		}
		return temp;
	}
	void clear()
	{
		for (int i = 0; i < row; i++)
			delete[] boardarray[i];
		delete[] boardarray;
		delete[] toparray;
		delete[] expandablenode;
		for (int i = 0; i < column; i++)
			if (children[i])
			{
				children[i]->clear();
				delete[] children[i];
			}
		delete[] children;
	}

public:
	node(int **board, int *top, int M, int N, int noX, int noY, int nodedepth = 0, int x = -1, int y = -1, int chessright = MACHINECHANCE, node* fathernode = NULL)
	{
		boardarray = new int*[M];
		for (int i = 0; i < M; i++)
		{
			boardarray[i] = new int[N];
			for (int j = 0; j < N; j++)
				boardarray[i][j] = board[i][j];
		}
		toparray = new int[N];
		for (int i = 0; i < N; i++)
			toparray[i] = top[i];
		row = M;
		column = N;
		nox = noX;
		noy = noY;
		depth = nodedepth;
		lastx = x;
		lasty = y;
		chesschance = chessright;
		visitednum = 0;
		profit = 0;
		father = fathernode;
		expandablenum = 0;
		children = new node*[column];
		expandablenode = new int[column];
		for (int i = 0; i < column; i++)
		{
			if (toparray[i] != 0)
				expandablenode[expandablenum++] = i;
			children[i] = NULL;
		}
	}
	int x() const 
	{ 
		return lastx; 
	}
	int y() const 
	{ 
		return lasty; 
	}
	int whochess() const 
	{ 
		return chesschance; 
	}
	bool isExpandable() const 
	{ 
		return expandablenum > 0; 
	}
	bool isleaf()
	{
		if (lastx == -1 && lasty == -1)
			return false;
		if ((chesschance == USERCHANCE && machineWin(lastx, lasty, row, column, boardarray)) || (chesschance == MACHINECHANCE && userWin(lastx, lasty, row, column, boardarray)) || (isTie(column, toparray)))
			return true;
		return false;
	}
	node *expand(int chessright)
	{
		int index = rand() % expandablenum;
		int **tempboardarray = new int*[row];
		for (int i = 0; i < row; i++)
		{
			tempboardarray[i] = new int[column];
			for (int j = 0; j < column; j++)
				tempboardarray[i][j] = boardarray[i][j];
		}
		int *temptoparray = new int[column];
		for (int i = 0; i < column; i++)
			temptoparray[i] = toparray[i];
		int newy = expandablenode[index], newx = --temptoparray[newy];
		tempboardarray[newx][newy] = chesschance;
		if (newx - 1 == nox && newy == noy)
			temptoparray[newy] --;
		children[newy] = new node(tempboardarray, temptoparray, row, column, nox, noy, depth + 1, newx, newy, chessright, this);
		for (int i = 0; i < row; i++)
			delete[] tempboardarray[i];
		delete[] tempboardarray;
		delete[] temptoparray;
		swap(expandablenode[index], expandablenode[--expandablenum]);
		return children[newy];
	}
	//�����ӽڵ�
	//node *bestChild() 
	//{
	//	node* best;
	//	double maxProfitRatio = -RAND_MAX;
	//	for (int i = 0; i < column; i++) {
	//		if (children[i] == NULL) continue;
	//		double modifiedProfit = (chesschance == USERCHANCE ? -1 : 1) * children[i]->profit; //��������ֵ
	//		int childVisitedNum = children[i]->visitednum; //�ӽڵ������ 
	//		double tempProfitRatio = modifiedProfit / childVisitedNum + sqrtl(2 * logl(visitednum) / childVisitedNum) * C; //�����ۺ������� 
	//		//if (tempProfitRatio > maxProfitRatio || (tempProfitRatio == maxProfitRatio && rand() % 2 == 0)) 
	//		if (tempProfitRatio > maxProfitRatio)
	//		{ //ѡ���ۺ������������ӽڵ� 
	//			maxProfitRatio = tempProfitRatio;
	//			best = children[i];
	//		}
	//	}
	//	return best;
	//}
	//���ݸ���
	//void backup(double deltaProfit) 
	//{
	//	node *temp = this;
	//	while (temp) 
	//	{
	//		temp->visitednum++; //���ʴ���+1 
	//		temp->profit += deltaProfit; //��������delta 
	//		temp = temp->father;
	//	}
	//}
};

class UCT
{
private:
	node *root; //���ڵ�
	int row, column; //����������
	int nox, noy; //�������ӵ��λ�� 
	int startTime; //���㿪ʼʱ��
	int profit(int **board, int *top, int chessman, int x, int y) const;
	void placechess(int **board, int *top, int chesschance, int &x, int &y);
	int rightchange(int chessman) const;
	node *treepolicy(node *presentNode);
	node *expand(node *expandnode);
	node *bestchild(node *father);
	double defaultpolicy(node *selectedNode);
	void backup(node *selectedNode, double deltaProfit);

public:
	UCT(int M, int N, int noX, int noY);
	Point *search(int **board, int *top);
	virtual ~UCT();
};

#endif //__UCT_H__