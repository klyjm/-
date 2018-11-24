#include "UCT.h"

UCT::UCT(int row, int column, int noX, int noY) : _row(row), _column(column), _noX(noX), _noY(noY), startTime(clock())
{
}


UCT::~UCT()
{
	_root->clear();
	delete _root;
}

node *UCT::search(int **boardState, int *topState)
{
	_root = new node(boardState, topState, _row, _column, _noX, _noY); //�Ե�ǰ״̬�������ڵ� 
	while (clock() - startTime <= TIME_LIMITATION) { //��δ�ľ�����ʱ�� 
		node *selectedNode = treepolicy(_root); //�������������Խڵ� 
		double deltaProfit = defaultpolicy(selectedNode); //����ģ����Զ�ѡ�нڵ����һ�����ģ�� 
		backup(selectedNode, deltaProfit); //��ģ�������ݷ����������� 
	}
	return bestchild(_root);
}

int UCT::profit(int **board, int *top, int chessman, int x, int y) const
{
	if (chessman == USER_CHANCE && userWin(x, y, _row, _column, board))
		return USER_WIN_PROFIT;
	if (chessman == MACHINE_CHANCE && machineWin(x, y, _row, _column, board))
		return MACHINE_WIN_PROFIT;
	if (isTie(_column, top))
		return TIE_PROFIT;
	return UNTERMINAL_STATE; //δ������ֹ״̬ 
}

void UCT::placechessman(int **board, int *top, int chessman, int &x, int &y)
{
	y = rand() % _column; //���ѡ��һ�� 
	while (top[y] == 0) //������������ 
		y = rand() % _column; //�����ѡ��һ�� 
	x = --top[y]; //ȷ�����Ӹ߶� 
	board[x][y] = chessman; //���� 
	if (x - 1 == _noX && y == _noY) //������λ�����Ϸ����ڲ������ӵ� 
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
	return presentNode->expand(rightchange(presentNode->chessman()));
}
node *UCT::bestchild(node *father)
{
	return father->bestChild();
}

double UCT::defaultpolicy(node *selectedNode)
{
	int **boardState = selectedNode->BoardState(), *top = selectedNode->TopState();
	int chessman = selectedNode->chessman(), depth = selectedNode->_depth;
	int x = selectedNode->x(), y = selectedNode->y();
	int nowprofit = profit(boardState, top, rightchange(chessman), x, y); //�������� 
	while (nowprofit == UNTERMINAL_STATE) { //����ǰ״̬δ����ֹ״̬ 
		depth++;
		placechessman(boardState, top, chessman, x, y); //������� 
		nowprofit = profit(boardState, top, chessman, x, y); //�������� 
		chessman = rightchange(chessman); //��Ȩ�任 
	}
	for (int i = 0; i != _row; i++)
		delete[] boardState[i];
	delete[] boardState;
	delete[] top;
	return double(nowprofit);// / logl(depth + 1); //�����Լ���
}

void UCT::backup(node *selectedNode, double deltaProfit)
{
	selectedNode->backup(deltaProfit);
}