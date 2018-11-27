//#include "UCT.h"
//
//UCT::UCT(int M, int N, int noX, int noY) : row(M), column(N), nox(noX), noy(noY), startTime(clock())
//{
//}
//
//
//UCT::~UCT()
//{
//}
//
//Point *UCT::search(int **boardState, int *topState)
//{
//	root = new node(boardState, topState, row, column, nox, noy); //�Ե�ǰ״̬�������ڵ� 
//	while (clock() - startTime <= TIME_LIMITATION) { //��δ�ľ�����ʱ�� 
//		node *selectednode = treepolicy(root); //�������������Խڵ� 
//		double deltaProfit = defaultpolicy(selectednode); //����ģ����Զ�ѡ�нڵ����һ�����ģ�� 
//		backup(selectednode, deltaProfit); //��ģ�������ݷ����������� 
//	}
//	node *bestpoint = bestchild(root);
//	Point *point = new Point(bestpoint->x(), bestpoint->y());
//	root->clear();
//	delete root;
//	root = NULL;
//	return point;
//}
//
//int UCT::profit(int **board, int *top, int chesschance, int x, int y) const
//{
//	if (chesschance == USER_CHANCE && userWin(x, y, row, column, board))
//		return USER_WIN_PROFIT;
//	if (chesschance == MACHINE_CHANCE && machineWin(x, y, row, column, board))
//		return MACHINE_WIN_PROFIT;
//	if (isTie(column, top))
//		return TIE_PROFIT;
//	return UNTERMINAL_STATE; //δ������ֹ״̬
//}
//
//void UCT::placechessman(int **board, int *top, int chessman, int &x, int &y)
//{
//	y = rand() % column; //���ѡ��һ�� 
//	while (top[y] == 0) //������������ 
//		y = rand() % column; //�����ѡ��һ�� 
//	x = --top[y]; //ȷ�����Ӹ߶� 
//	board[x][y] = chessman; //���� 
//	if (x - 1 == nox && y == noy) //������λ�����Ϸ����ڲ������ӵ�
//		top[y] --;
//}
//
//int UCT::rightchange(int chessman) const
//{
//	if (chessman == USER_CHANCE)
//		return MACHINE_CHANCE;
//	else if (chessman == MACHINE_CHANCE)
//		return USER_CHANCE;
//	else
//		return -1;
//}
//
//node *UCT::treepolicy(node *presentNode)
//{
//	while (!presentNode->isTerminal()) { //�ڵ㲻����ֹ�ڵ� 
//		if (presentNode->isExpandable()) //��ӵ��δ�����ʵ���״̬ 
//			return expand(presentNode); //��չ�ýڵ� 
//		else
//			presentNode = bestchild(presentNode); //ѡ�������ӽڵ� 
//	}
//	return presentNode;
//}
//
//node *UCT::expand(node *presentNode)
//{
//	return presentNode->expand(rightchange(presentNode->whochess()));
//}
//node *UCT::bestchild(node *rootnode)
//{
//	node* best;
//	double maxProfitRatio = -RAND_MAX;
//	for (int i = 0; i < column; i++)
//	{
//		if (rootnode->children[i] == NULL)
//			continue;
//		double modifiedProfit = (rootnode->chesschance == USER_CHANCE ? -1 : 1) * rootnode->children[i]->profit; //��������ֵ
//		int childVisitedNum = rootnode->children[i]->visitednum; //�ӽڵ������ 
//		double tempProfitRatio = modifiedProfit / childVisitedNum + sqrtl(2 * logl(rootnode->visitednum) / childVisitedNum) * C; //�����ۺ������� 
//		//if (tempProfitRatio > maxProfitRatio || (tempProfitRatio == maxProfitRatio && rand() % 2 == 0)) 
//		if (tempProfitRatio > maxProfitRatio)
//		{ //ѡ���ۺ������������ӽڵ� 
//			maxProfitRatio = tempProfitRatio;
//			best = rootnode->children[i];
//		}
//	}
//	return best;
//	//return father->bestChild();
//}
//
//double UCT::defaultpolicy(node *selectedNode)
//{
//	int **boardState = selectedNode->BoardState(), *top = selectedNode->TopState();
//	int chessman = selectedNode->whochess(), depth = selectedNode->_depth;
//	int x = selectedNode->x(), y = selectedNode->y();
//	int nowprofit = profit(boardState, top, rightchange(chessman), x, y); //�������� 
//	while (nowprofit == UNTERMINAL_STATE) { //����ǰ״̬δ����ֹ״̬ 
//		depth++;
//		placechessman(boardState, top, chessman, x, y); //������� 
//		nowprofit = profit(boardState, top, chessman, x, y); //�������� 
//		chessman = rightchange(chessman); //��Ȩ�任 
//	}
//	for (int i = 0; i != row; i++)
//		delete[] boardState[i];
//	delete[] boardState;
//	delete[] top;
//	return double(nowprofit);// / logl(depth + 1); //�����Լ���
//}
//
//void UCT::backup(node *selectedNode, double deltaProfit)
//{
//	selectedNode->backup(deltaProfit);
//}

#include "UCT.h"

UCT::UCT(int M, int N, int noX, int noY) : row(M), column(N), nox(noX), noy(noY), startTime(clock())
{
}


UCT::~UCT()
{
}

Point *UCT::search(int **board, int *top)
{
	root = new node(board, top, row, column, nox, noy);
	while (clock() - startTime <= TIMELIMITE)
	{
		node *tempnode = treepolicy(root);
		double deltaprofit = defaultpolicy(tempnode);
		backup(tempnode, deltaprofit);
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
	if (chesschance == USERCHANCE && userWin(x, y, row, column, board))
		return USERWIN;
	if (chesschance == MACHINECHANCE && machineWin(x, y, row, column, board))
		return MACHINEWIN;
	if (isTie(column, top))
		return TIE;
	return NOTEND;
}

void UCT::placechess(int **board, int *top, int chesschance, int &x, int &y)
{
	y = rand() % column;
	while (top[y] == 0)
		y = rand() % column;
	x = --top[y];
	board[x][y] = chesschance;
	if (x - 1 == nox && y == noy)
		top[y] --;
}

int UCT::rightchange(int chesschance) const
{
	if (chesschance == USERCHANCE)
		return MACHINECHANCE;
	else
	{
		if (chesschance == MACHINECHANCE)
			return USERCHANCE;
	}
}

node *UCT::treepolicy(node *rootnode)
{
	while (!rootnode->isleaf())
	{
		if (rootnode->isExpandable())
			return expand(rootnode);
		else
			rootnode = bestchild(rootnode);
	}
	return rootnode;
}

node *UCT::expand(node *expandnode)
{
	//int expandablenum = expandnode->expandablenum;
	//int index = rand() % expandablenum; //���ȷ��һ������ֵ 
	//int **tempboardarray = new int*[row];
	//for (int i = 0; i < row; i++)
	//{
	//	tempboardarray[i] = new int[column];
	//	for (int j = 0; j < column; j++)
	//		tempboardarray[i][j] = expandnode->boardarray[i][j];
	//}
	//int *temptoparray = new int[column];
	//for (int i = 0; i < column; i++)
	//	temptoparray[i] = expandnode->toparray[i];
	////*int **tempboardarray = expandnode->getboard();
	//int *temptoparray = expandnode->gettop();*/
	//int newy = expandnode->expandablenode[index], newx = --temptoparray[newy];
	//tempboardarray[newx][newy] = expandnode->chesschance; 
	//if (newx - 1 == nox && newy == noy)
	//	temptoparray[newy] --; 
	////Ϊ��ǰ�ڵ㴴����չ�ӽڵ� 
	//expandnode->children[newy] = new node(tempboardarray, temptoparray, row, column, nox, noy, expandnode->depth + 1, newx, newy, rightchange(expandnode->chesschance), expandnode);
	//for (int i = 0; i < row; i++)
	//	delete[] tempboardarray[i];
	//delete[] tempboardarray;
	//delete[] temptoparray;
	//swap(expandnode->expandablenode[index], expandnode->expandablenode[--expandablenum]); 
	//return expandnode->children[newy];
	return expandnode->expand(rightchange(expandnode->whochess()));
}
node *UCT::bestchild(node *rootnode)
{
	node* bestpoint;
	double maxprofit = -(numeric_limits<double>::max)();
	for (int i = 0; i < column; i++)
	{
		if (rootnode->children[i] == NULL)
			continue;
		double newprofit = (rootnode->chesschance == USERCHANCE ? -1 : 1) * rootnode->children[i]->profit;
		int childvisitednum = rootnode->children[i]->visitednum;
		double tempprofit = newprofit / childvisitednum + sqrtl(2 * logl(rootnode->visitednum) / childvisitednum) * C; //�����ۺ������� 
		//if (tempProfitRatio > maxProfitRatio || (tempProfitRatio == maxProfitRatio && rand() % 2 == 0)) 
		if (tempprofit > maxprofit)
		{
			maxprofit = tempprofit;
			bestpoint = rootnode->children[i];
		}
	}
	return bestpoint;
	//return father->bestChild();
}

double UCT::defaultpolicy(node *tempnode)
{
	int **board = tempnode->getboard(), *top = tempnode->gettop();
	int chesschance = tempnode->whochess(), depth = tempnode->depth;
	int x = tempnode->x(), y = tempnode->y();
	int tempprofit = profit(board, top, rightchange(chesschance), x, y); //�������� 
	while (tempprofit == NOTEND)
	{
		depth++;
		placechess(board, top, chesschance, x, y);
		tempprofit = profit(board, top, chesschance, x, y);
		chesschance = rightchange(chesschance);
	}
	for (int i = 0; i < row; i++)
		delete[] board[i];
	delete[] board;
	delete[] top;
	return double(tempprofit);
}

void UCT::backup(node *leaf, double deltaprofit)
{
	node *temp = leaf;
	while (temp)
	{
		temp->visitednum++;
		temp->profit += deltaprofit;
		temp = temp->father;
	}
	//selectedNode->backup(deltaProfit);
}