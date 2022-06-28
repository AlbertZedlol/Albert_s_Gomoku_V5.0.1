#include "general.h"
#include "globalFunctionDeclaration.h"
#include "globalVariableDeclaration.h"
#include "Neighbor.h"
#include "chessboard.h"
#include "coordination.h"

//constructor

Neighbor::Neighbor(int value, int R)
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 21; j++)
		{
			neighborArray[i][j] = value;
			if (i == 5)
			{
				continue;
			}
			neighborDanger[i][j] = 0;
		}

	for (int i = 1; i < 3; i++)
	{
		for (int j = 0; j < 10; j++)
		{

			lastMoves[i][j].x = 0;
			lastMoves[i][j].y = 0;
		}

	}

	range = R;
}


inline void Neighbor::clearNeighborDanger()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			neighborDanger[i][j] = 0;
		}
	}
}



void Neighbor::lastMoveStorage(int color, crd lastMove)
{
	int n = (lastMoves[color][10].x);
	if (n < 10)
	{
		lastMoves[color][n] = lastMove;
		(lastMoves[color][10].x)++;
	}
	else
	{
		//��¼����֮�����ǰpushѹջ��
		for (int i = 1; i < 10; i++)
			lastMoves[0][i] = lastMoves[color][i];
		for (int i = 1; i < 10; i++)
			lastMoves[color][i - 1] = lastMoves[0][i];
		lastMoves[color][9] = lastMove;
	}


}


//clear row m to row M of  neighbor. 
//functions processing array int neighbor[2][10];
inline void Neighbor::clearNeighbor(int m = 0, int M = 1)
{
	for (int i = m; i < M + 1; i++)
		for (int j = 0; j < 21; j++)
			neighbor.neighborArray[i][j] = -1;
}

//display neighborArray.For debug.
void Neighbor::neighborOut(int range)//display neighbor[0][0]to[1][2*range+1]
{
	cout << "neighbor:";
	for (int i = 0; i < 5; i++)
	{
		printf("\n");
		for (int k = 0; k < 2 * range + 1; k++)
		{
			if (i < 5)
			{
				if (neighbor.neighborArray[i][k] == chessColor::empty)
					cout << "��";//empty
				else if (neighbor.neighborArray[i][k] == -1)
					cout << "D"; //different color.
				else if (neighbor.neighborArray[i][k] == 1)
					cout << "S";//same color.
				else if (neighbor.neighborArray[i][k] == -2)
					cout << "��";//out of range.
				else
					cout << neighbor.neighborArray[i][k];
			}
			else
			{
				cout << setw(2) << neighbor.neighborArray[i][k];
			}
		}

	}
	printf("\n");
}

//���ڵ�lastMove:�����˸�������[-range, range]���ڹ�2*range+1��ͬɫ������һ�Է���
//�Լ��÷����ϵ�����λ����ɫ�ֲ�(ͬɫ����ɫ���գ�Խ��)
//����neighborArray��.
void Neighbor::testNeighbor(crd lastMove) const
{
	neighbor.clearNeighbor();
	int x = lastMove.x;
	int y = lastMove.y;
	crd point(x, y);
	crd here = point;
	int sameColor = 0, myColor = gameboard.board[x][y], maxSameColor = 0, worstDirection = -1;


	//�������������㷨�Ż�����������//
	for (int i = 0; i < 4; i++)//Four directions.
	{
		/*	std::cout << "\n��v[" << i << "]: ";*/
		sameColor = 0;
		for (int j = -range; j < range + 1; j++)//2*range+1 places to be tested.
		{
			crd here = point + (j * v[i]);

			if ((!(here.x >= 1 && here.x <= 15 && here.y >= 1 && here.y <= 15)))// off the gameboard.
				neighbor.neighborArray[i][j + range] = -2;//off border.
			else
			{
				int color = gameboard.board[here.x][here.y];
				if (color == myColor)//same color.
				{
					neighbor.neighborArray[i][j + range] = 1;
					sameColor++;
				}
				else if (color == chessColor::empty)//empty
					neighbor.neighborArray[i][j + range] = 0;
				else//different color.
					neighbor.neighborArray[i][j + range] = -1;
			}

		}
		
	}
}

////�����Ƿ��л��ģ���������k����array[2],û����Ϊ�������۽���Σ��Ȩ�أ���¼��danger�С���danger��ȡ���ֵ��Ϊ��Σ�յĵط�������ж�������ѡһ����
//inline bool Neighbor::evaluateDanger() Old Version

inline bool Neighbor::evaluateDanger()
{

	int sub[5] = { 0 };
	
	for (int i = 0; i < 4; i++)
	{


		for (int k = 0; k < 2 * range + 1 - 4; k++)
		{
			getsubString(neighborArray[i], k, 5, sub);
			
			for (int j = 0; j < 5; j++)
			{
				if (equal(sub, bad.liveFour[j]))
				{

					neighborArray[4][1] = i;
					neighborArray[4][2] = k + j;
					return 1;
				}
			}

			if (equal(sub, bad.liveThree[0]))
			{
				neighborDanger[i][k]++;
				neighborDanger[i][k + 4]++;

			}
			else if (equal(sub, bad.liveThree[1]))
			{
				neighborDanger[i][k + 1]++;

			}
			else if (equal(sub, bad.liveThree[2]))
			{
				//������  3��λ��
				neighborDanger[i][k + 3]++;
			}
			else if (equal(sub, bad.liveThree[3]))
			{
				//������ 1 �� 3��λ��
				neighborDanger[i][k + 1]++;
				neighborDanger[i][k + 3]++;
			}
			else if (equal(sub, bad.liveThree[4]))
			{
				neighborDanger[i][k + 1]++;
			}
		}
	}
	return 0;
}
//���ݶԷ������һ�������λ�ã�������˸���������Σ�յķ���Ȼ����о�����Σ�յĵ㣬��������Ϊ��һ����������Ľ��顣
// X����һ��λ��  Y�������������
//����㷨��©������Ӧ�ÿ�һ��8*21��array��danger���飬ÿ������decideһ�飬��������danger����ȡ���
//��Ϊ����ĳ���������Ӻܶ࣬���Ƕ�ûɶ��в����һЩ�������٣����Ѿ������ˡ�
//Old version crd Neighbor::nextMove(crd here)
crd Neighbor::nextMove(crd here)
{
	neighbor.testNeighbor(here);

	//clear.danger.
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			neighborDanger[i][j] = 0;
		}
	}
	//Next move on v[worst_direction],sequecne worst_sequence; 
	int worst_direction = 0, worst_sequence = 0;
	if (evaluateDanger())//==1:there is a liveFour.
	{
		worst_direction = neighborArray[4][1];
		worst_sequence = neighborArray[4][2];
	}
	else
	{
		//find the maximum  of neighborDanger at (I,J)
		int I = 0, J = 0;
		int maximumDanger = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				//cout << neighborDanger[m] << " ";
				if (neighborDanger[i][j] > maximumDanger)
				{
					I = i;
					J = j;
					maximumDanger = neighborDanger[i][j];

				}
			}
		}
		//���ֵ������ΪmaxNumber����������choice�С�  choice ,[0]�洢�ڼ�������v[i]��i�� [1]�洢�ڼ�����k
		//cout << "\nmaximumDanger==" << maximumDanger << "\n";
		int choice[2][10] = { 0 }, maxNumber = 0;
		//If all the points are safe,then you can attack freely.//���ȫ����ȫ���򷵻�here����ֵ������һ�����������ӵĽ��顪����ζ��û��Σ�գ�����Խ�����
		if (maximumDanger == 0)
		{
			return here;

		}
		//else, you have to block the other chesser.

		else
		{
			//����������ֵ�㣬�����䷽��i������k����choice[2][10]�С�
			//find all the maximum points, record them in [i][j].  [i]==v[i],direction; [j]==k, sequence;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 21; j++)
				{
					if (neighborDanger[i][j] == maximumDanger)
					{
						choice[0][maxNumber] = i;
						choice[1][maxNumber] = j;
						maxNumber++;
					}
				}
			}
			//���ֵ��Ψһʱ���ѡȡһ����
			if (maxNumber > 1)
			{
				//�������ʵ��Ȼ����ȸ��ʵ�ѹ������������{0,1,2...},����һ���������ֵrandomValue����choice��һ���бꡣ
				std::default_random_engine F;
				std::uniform_real_distribution<double> f(1, maxNumber * 50 - 1);
				F.seed(time(NULL));
				int randomValue = (int)(f(F) / 50);

				worst_direction = choice[0][randomValue];
				worst_sequence = choice[1][randomValue];
				neighborArray[4][1] = worst_direction;
				neighborArray[4][2] = worst_direction;


			}
			else
			{
				worst_sequence = choice[1][0];
				worst_direction = choice[0][0];

			}
		}

	}
	crd temp = (worst_sequence - range) * v[worst_direction];
	crd result = here + temp;
	return result;
}