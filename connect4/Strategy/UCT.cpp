#include "UCT.h"

UCT::UCT(int M, int N, int noX, int noY) : row(M), column(N), nox(noX), noy(noY), startTime(clock())
{
}


UCT::~UCT()
{
}

Point *UCT::search(int **boardState, int *topState)
{
	root = new node(boardState, topState, row, column, nox, noy); //�Ե�ǰ״̬�������ڵ� 
	while (clock() - startTime <= TIME_LIMITATION) { //��δ�ľ�����ʱ�� 
		node *selectedNode = treepolicy(root); //�������������Խڵ� 
		double deltaProfit = defaultpolicy(selectedNode); //����ģ����Զ�ѡ�нڵ����һ�����ģ�� 
		backup(selectedNode, deltaProfit); //��ģ�������ݷ����������� 
	}
	node *bestpoint = bestchild(root);
	Point *point = new Point(bestpoint->x(), bestpoint->y());
	root->clear();
	delete root;
	root = NULL;
	return point;
}

int UCT::profit(int **board, int *top, int chesschance, int x, int y) const
{
	if (chesschance == USER_CHANCE && userWin(x, y, row, column, board))
		return USER_WIN_PROFIT;
	if (chesschance == MACHINE_CHANCE && machineWin(x, y, row, column, board))
		return MACHINE_WIN_PROFIT;
	if (isTie(column, top))
		return TIE_PROFIT;
	return UNTERMINAL_STATE; //δ������ֹ״̬
}

void UCT::placechessman(int **board, int *top, int chessman, int &x, int &y)
{
	y = rand() % column; //���ѡ��һ�� 
	while (top[y] == 0) //������������ 
		y = rand() % column; //�����ѡ��һ�� 
	x = --top[y]; //ȷ�����Ӹ߶� 
	board[x][y] = chessman; //���� 
	if (x - 1 == nox && y == noy) //������λ�����Ϸ����ڲ������ӵ�
		top[y] --;
}

int UCT::rightchange(int chessman) const
{
	if (chessman == USER_CHANCE)
		return MACHINE_CHANCE;
	else if (chessman == MACHINE_CHANCE)
		return USER_CHANCE;
	else
		return -1;
}

node *UCT::treepolicy(node *presentNode)
{
	while (!presentNode->isTerminal()) { //�ڵ㲻����ֹ�ڵ� 
		if (presentNode->isExpandable()) //��ӵ��δ�����ʵ���״̬ 
			return expand(presentNode); //��չ�ýڵ� 
		else
			presentNode = bestchild(presentNode); //ѡ�������ӽڵ� 
	}
	return presentNode;
}

node *UCT::expand(node *presentNode)
{
	return presentNode->expand(rightchange(presentNode->whochess()));
}
node *UCT::bestchild(node *father)
{
	return father->bestChild();
}

double UCT::defaultpolicy(node *selectedNode)
{
	int **boardState = selectedNode->BoardState(), *top = selectedNode->TopState();
	int chessman = selectedNode->whochess(), depth = selectedNode->_depth;
	int x = selectedNode->x(), y = selectedNode->y();
	int nowprofit = profit(boardState, top, rightchange(chessman), x, y); //�������� 
	while (nowprofit == UNTERMINAL_STATE) { //����ǰ״̬δ����ֹ״̬ 
		depth++;
		placechessman(boardState, top, chessman, x, y); //������� 
		nowprofit = profit(boardState, top, chessman, x, y); //�������� 
		chessman = rightchange(chessman); //��Ȩ�任 
	}
	for (int i = 0; i != row; i++)
		delete[] boardState[i];
	delete[] boardState;
	delete[] top;
	return double(nowprofit);// / logl(depth + 1); //�����Լ���
}

void UCT::backup(node *selectedNode, double deltaProfit)
{
	selectedNode->backup(deltaProfit);
}