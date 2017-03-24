#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <Windows.h> 
#include <cstdio>
#include <fstream>
#include <vector>
using namespace std;

enum TypeOfRoute { SIMPLE_ROUTE = 1, PSEUDO_ROUTE = 2 };

void printRoutes(int*, int*, int); // ����� ����� ��� ��������� �������� � ��������� ��������-�����
void getMatrixFromFile(string); // �������� ������� �� ����� 
void showWeightMatrix(); // �������� ������� �����
void createNewWeightMatrix(); // ������� ����� ������� ����
void dijkstraAlgorithm(); // ��������� �������� �������� ��� ������� �� �����
void floydAlgorithm(); // ��������� �������� ������ ��� ������� �� �����
void ford_fulkersonAlgorithm(); // ��������� �������� �����-���������� ��� ������� �� �����
void bellman_fordAlgorithm(); // ��������� �������� ��������-����� ��� ������� �� ����� 
void findRoute(TypeOfRoute, int, int **, bool *, int *, int *, int, int, int *); // ������� ������ ������ ��� �����-���������� ������� ������ � �������

bool found; // ����������, ������ �� ��������� �������



const int INF = 1000000; // "�������������"
int **G; // ������� ��������� ����������� �����
int N = 0; // ���������� ������ 

int main()
{
	setlocale(LC_ALL, "RUS");
	srand(time(NULL));
	getMatrixFromFile("input.txt");

	char inp;
	do
	{
		system("cls");
		cout << "�������� ��������: ";
		cout << "\n1 - ������ ������� �����";
		cout << "\n2 - �������� ������� �����";
		cout << "\n3 - ����� ���������� �������� � ���������� ����� �� ��������� ��������";
		cout << "\n4 - ����� ���������� �������� � ���������� ����� �� ��������� ������";
		cout << "\n5 - ����� ���������� �������� � ���������� ����� �� ��������� ��������-�����";
		cout << "\n6 - ����� ������������ ����� �� ��������� �����-����������";		
		cout << "\n0 - �����\n";
		inp = getch();
		switch (inp)
		{
		case '1': createNewWeightMatrix(); break;
		case '2': showWeightMatrix(); break;
		case '3': dijkstraAlgorithm(); break;
		case '4': floydAlgorithm(); break;
		case '5': bellman_fordAlgorithm(); break;
		case '6': ford_fulkersonAlgorithm(); break;
		case '0': return 0;
		default: cout << "\n���-�� ����� �� ��� � ����� ��� ����..."; break;
		}

	} while (inp != '0');
	return 0;
}

void getMatrixFromFile(string fileName)
{
	//string fileName = "input.txt";
	ifstream inputFileStream (fileName, ios::in); // �������� ����� fileName � ������� �������� "������"
	if (!inputFileStream)
	{
		cerr << "���������� ������� ����!";
		exit(1);
	}

	inputFileStream >> N; // ������� �� ����� ������ ������ = ���������� ��������� � �������

	// �������� ������� N*N
	G = new int*[N];
	for (int i = 0; i < N; i++)
	{
		G[i] = new int[N];
	}
	// ���������� ������ �� �����
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			inputFileStream >> G[i][j];
	}
	inputFileStream.close(); // ������� �����
}

void showWeightMatrix()
{
	//getMatrixFromFile("input.txt");
	system("cls");
	cout << "������� ����� ��� ��������� ����������� �����\n";
	for (int i = 0; i < N; i++) // � �������� ����� �������� ������ ���������� �������
	{
		for (int j = 0; j < N; j++)
		{
			//if (i == j)
			//	G[i][j] = 0;
			//else
			//	G[i][j] = rand() % 20; //�� 0 �� 9
			cout << G[i][j];
			if (G[i][j] < 10)
				cout << "  ";
			else cout << " ";
		}
		cout << endl;
	}
	cout << endl << "��� ����������� ������� ����� �������" << endl;
	getch();
}

void createNewWeightMatrix() // ������� ����� ������� ���������
{
	system("cls");
	cout << "�������� ��������� � ����������..." << endl << endl;
	cout << "������� ����� ������� ��� �������� � ����." << endl;
	getch();
	return;
	/*
	cout << "������� ���������� ������ � �����: ";
	cin >> N;
	if (N <= 0) // �������� �� ������
	{
		cout << endl << "�������� ����! ����� ������ � ����� ������ ���� ����������� ������!" << endl << endl;
		cout << "������� ����� ������� ��� �������� � ����." << endl;
		getch();
		return;
	}
	else
	{
		G = new int *[N]; // ��������
		for (int i = 0; i < N; i++) // ���������� �������������
		{
			G[i] = new int[N]; // �������
		}

		cout << "\n����������� ���� ������� ����� W (���������)\n";
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				printf("W[%d][%d] = ", i + 1, j + 1);
				scanf("%d", &G[i][j]);
			}
			cout << endl;
		}
		showWeightMatrix();
	}
	*/
}

void dijkstraAlgorithm()
{
	system("cls");
	
	int startVertex;
	int *distance = new int[N]; // ������ ���������� �� ���������� ����� �� ������
	bool *visited = new bool[N]; // ������, ���������� �� ��, ���� �� �������� �����-���� �������
	int minVertexIndex; // ������ �������, �� ������� ����������� �������
	int currentVertexIndex; // ������ ������� �������, �� ������� ���� ��������
	int minDistance; // ����������� ����� �������� ����� ���������
	bool finished = false; // ����������, ������������, �������� �� ��������
	int *previous = new int[N]; // ������ ���������� ����� ��� ������ �������

	cout << "������� ����� �������, �� ������� ����� ����� ����������� �������� (��������� �� 1): ";
	cin >> startVertex;
	if (startVertex <= 0 || startVertex > N) // �������� �� ������
	{
		cout << "\n�������� ����! ������� � ����� ������� � �������� ����� �� ����������! \n���������� ������ - " << N << endl;
		cout << "������� ����� ������� ��� �������� � ����." << endl;
		getch();
		return;
	}
	else
	{
		startVertex--; // ��������� ����� ������� �� ��������� ����� �� (��������� �������� � 0, ������� �������� �� -1 ������������ �����������)

					   // ��������� �������������: 
		for (int i = 0; i < N; i++)
		{
			previous[i] = -1; // ��� ���������� �����
			if (i == startVertex) // ��������� �������
			{
				distance[i] = 0; // ���������� ���� = 0
				visited[i] = true; // �������� ����������

			}
			else
			{
				distance[i] = INF; // ����� ���� ����������� "�������������"
				visited[i] = false; // � �������� ��� �� ����������
			}
		}
		minVertexIndex = startVertex; // ���������� ������� ���������� ����� ��������� 
		currentVertexIndex = startVertex; // ������� ������� ������ ������������� 

										  
		for (int i = 0; i < N; i++) // ��� ������ �������
		{
			minDistance = INF; // �������� ����������� ��������� ������ ������������� ��� ��������
			for (int j = 0; j < N; j++) // ����� ������������ �� ���� �������� �� ��������� �������
			{
				if (G[currentVertexIndex][j] != 0 && visited[j] == false) // ���� ���������� ������� �� ������� ������� �� j-��, � j-�� ������� ��� �� ��������
				{
					if (distance[currentVertexIndex] + G[currentVertexIndex][j] < distance[j]) // ���� ����� ������� ������ ��� ����������
					{
						distance[j] = distance[currentVertexIndex] + G[currentVertexIndex][j]; // �� ������ ����� ��������, ������ ����� �������� �� ���������� ������� + ���� ����� (�� ����� ���������)
						previous[j] = currentVertexIndex; // ��������� ����� ���������� ������� 
					}
				}
				if (minDistance >= distance[j] && visited[j] == false) // ���� ��������� �� ���������� ������� �� ����������� �����
				{
					minVertexIndex = j;
					minDistance = distance[j];
				}
			}
			visited[minVertexIndex] = true; // ��������, ��� �� ��������� ������� ������ ����������� �������
			currentVertexIndex = minVertexIndex; // � ��������, ��� ������ ����� ������ �� ���												 
		}
		delete[] visited;

		cout << "\n**********��������� ������ �� ��������� ��������**********\n";

		cout << endl << "������� ���� ����� �� " << startVertex + 1 << "-�� ������� � ���������" << endl;
		for (int i = 0; i < N; i++)
			cout << distance[i] << " ";
		cout << endl;

		cout << "�������� ��������� ����������� �������� � ����������� ����� �� " << startVertex + 1 << "-�� ������� �� ���������:" << endl;
		printRoutes(distance, previous, startVertex);
		delete[] distance;
		delete[] previous;
		cout << endl << "��� ����������� ������� ����� �������" << endl;
		getch();
	}
}

void floydAlgorithm()
{
	system("cls");

	if (N <= 0)
	{
		cout << "��� ������ ��������� ���������� ������������� ������� ����� �����!";
		cout << endl << "������� ����� ������� ��� �������� � ����� �������.";
		getch();
		createNewWeightMatrix();
		system("cls");
	}

	int **W; // ������� �����
	int **H; // ������� "�������" ��� �������� ����
			 // ����������� ������
	W = new int *[N];
	H = new int *[N];
	for (int i = 0; i < N; i++)
	{
		W[i] = new int[N];
		H[i] = new int[N];
	}
	// ��������� �������������
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (G[i][j] == 0) // ���� �� ���������� ���� �� i � j
			{
				W[i][j] = INF; // ��� ����� �������������
				H[i][j] = -1; // � ��������� ��� ��� ���� ����� ������� �� i � j = -1 (������ ������� �� ����� ����);
			}
			else // ���� �� ���� ����
			{
				W[i][j] = G[i][j]; // �������� ���
				H[i][j] = j; // ��������� ��� �� i � j ����� j (+1 ��� ���������� "�� �����", �.�. ���������� � �� � 0)
			}

		}
	}
	// ������ ��������
	for (int k = 0; k < N; k++)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (W[i][j] > W[i][k] + W[k][j]) // ���� ���� �� i � j ����� ������������� ������� k ������, ��� ������ �� i � j
				{
					W[i][j] = W[i][k] + W[k][j]; // �� ����� ����� �������� ���� �� i � j. ��������� ��� � �������
					H[i][j] = H[i][k]; // ��������� ��� ��� ����, ���� ������� �� i � j ����� ���� �� i � k (�.�. k - �������������)
				}
	// ����� ����������
	cout << "**************��������� ������ ��������� ������**************" << endl;

	cout << endl << "������� ����� �� ������ ������� � ������:" << endl;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			W[i][j] == INF ? cout << "0  " : (W[i][j] < 10 ? cout << W[i][j] << "  " : cout << W[i][j] << " ");

		}
		cout << endl;
	}
	cout << endl;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			if (i != j) // �� ������������� �����
			{
				if (W[i][j] == INF)
					cout << "���� �� ������� " << i + 1 << " � ������� " << j + 1 << " �� ����������!" << endl;
				else
				{
					// ������, ������������ �������, � ������� ������ ��������� �� ����������

					int k = i;
					cout << i + 1;
					while (H[k][j] != -1)
					{
						cout << " -> " << H[k][j] + 1; // + 1 ����� ����������� ����� �� (��������� � 0)
						k = H[k][j];
					}
					cout << " = " << W[i][j] << endl;
				}
			}
		}

	for (int i = 0; i < N; i++)
	{
		delete[] H[i];
		delete[] W[i];
	}
	delete[] W;
	delete[] H;
	cout << endl << "��� ����������� ������� ����� �������" << endl;
	getch();
}

void bellman_fordAlgorithm()
{
	system("cls");
	
	int startVertex; // ��������� �������
	int *dist; // ������ ���������� ����� �� ��������� ������� �� i-�� ������� 
	int *path; // ������, ������ ������� �������� ������ ������ �������� ������ � ��������� �������� (��� ��������� ������� - �������� -1)

	if (N <= 0)
	{
		cout << "��� ������ ��������� ���������� ������������� ������� ����� �����!";
		cout << endl << "������� ����� ������� ��� �������� � ����� �������.";
		getch();
		createNewWeightMatrix();
		system("cls");
	}

	cout << "������� ����� �������, �� ������� ����� ����� ����������� �������� (��������� �� 1): ";
	cin >> startVertex;
	startVertex--;

	// ���������� ��� ���� ������ � �������������, ������ � ��������� ���� �� ���������
	dist = new int[N];
	path = new int[N];
	for (int i = 0; i < N; i++)
	{
		dist[i] = INF;
		path[i] = -1;
	}		
	dist[startVertex] = 0; // � ���������� ���� ��� ����� ����

	
	for (int i = 0; i < N-1; i++)
	{	
		bool distChanged = false; // �����������, ��������� �� ��������� � �������� ����������. ��� ����, ����� �������� ��������� ��������, ���� ���������� ���������� ����������
		for (int u = 0; u < N; u ++)
			for (int v = 0; v < N; v++)
			{
				if (G[u][v]) // ���� ���������� ���� �� ������� u � ������� v, ��������, ����� �� ��������� ������
				{
					// �������� ����� ��������: ���� �� ������� v ����� ��������� �������, ��������� ������� u + ���� (u,v)
					if (dist[v] > dist[u] + G[u][v])
					{
						// ������, �� ����� ����� �������� ����. ��������� ��� ��� ������� v, ���������� ������ (������� u) � �������, ��� � ������� ��������� ���������
						dist[v] = dist[u] + G[u][v];
						path[v] = u;
						distChanged = true;
					}
				}
			}
		if (!distChanged)
			break;
	}

	cout << "\n**********��������� ������ �� ��������� ��������-�����**********\n";

	cout << endl << "������� ���� ����� �� " << startVertex + 1 << "-�� ������� � ���������" << endl;
	for (int i = 0; i < N; i++)
		cout << dist[i] << " ";
	cout << endl;

	cout << "�������� ��������� ����������� �������� � ����������� ����� �� " << startVertex + 1 << "-�� ������� �� ���������:" << endl;
	printRoutes(dist, path, startVertex);
	delete[] dist;
	delete[] path;
	cout << endl << "��� ����������� ������� ����� �������" << endl;
	getch();
}

void ford_fulkersonAlgorithm()
{
	system("cls");
	int **W; // ����� ������� ����� G
	bool *visited; // ������, ������������, ��� �� ������� ���� ��� ��������� �������� ������
	int *route; // ������, ������������ ��������� ���� �� ������ � ����
	int *mark; // ������ �����, ������������, ������ �� � ��������� ������� �� ������ ��� �������� ����
	int maxFlow; // ������������ ����� (���������� ����� 0)
	int minCapacity; // ����������� ���������� �����������
	int startVertex; // �����
	int targetVertex; // ����
	TypeOfRoute routeType; // ��� �������� �������� � ������

	if (N <= 0)
	{
		cout << "��� ������ ��������� ���������� ������������� ������� ����� �����!";
		cout << endl << "������� ����� ������� ��� �������� � ����� �������.";
		getch();
		createNewWeightMatrix();
		system("cls");
	}

	startVertex = -1;
	targetVertex = -1; // ������������, ��� ���� ������ ���
	int *in = new int[N]; // ��������������� �������, ������������ ���������� �������� � ��������� ��� �� ������ �������
	int *out = new int[N];
	for (int i = 0; i < N; i++)
	{
		in[i] = 0;
		out[i] = 0;
	}
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			if (G[i][j] != 0)
			{
				out[i]++;
				in[j]++;
			}
		}
	for (int i = 0; i < N; i++)
	{
		if (in[i] == 0)
			startVertex = i;
		if (out[i] == 0)
			targetVertex = i;
	}
	delete[] in;
	delete[] out;
	if (startVertex == -1 || targetVertex == -1) // ��������� ������� ������ �������, ����� ����� ���� �� ���� �������� (������ cls �������� � �������� ��� � �����
	{
		cout << "� ������� ������� �� ���������� ������ �\\��� �����" << endl;
		cout << "��� �� ���������� ��������� ����������!" << endl;
		cout << "������� ����� ������� ��� �������� � ����� �������" << endl;
		getch();
		createNewWeightMatrix();
	}
	else if (startVertex != -1 && targetVertex != -1)
	{

		// ��������� �������������
		W = new int *[N];
		visited = new bool[N];
		route = new int[N];
		mark = new int[N];
		for (int j = 0; j < N; j++)
		{
			W[j] = new int[N];
		}
		maxFlow = 0; // ����.����� ����� 0

		for (int i = 0; i < N; i++) // ����������� �������
		{
			for (int j = 0; j < N; j++)
			{
				W[i][j] = G[i][j];
			}
		}


		visited[startVertex] = true; // ������ ������� (�����) ������ �������� ����������
		mark[startVertex] = -1; // ������ �� ��� ������� ���� (����� ����� �������???)
		route[startVertex] = -1; // ������ � ��� ����� �� ��������


								 // ������ ����: ����� ���� ��������� �� ����� ������� �����������
		routeType = SIMPLE_ROUTE;
		do // ���� ��������� ����� �������� ������� �����������
		{
			for (int i = 0; i < N; i++) // � ������ �������� �������� ������� ��� ������ ������� ������ ��������
			{
				if (i != startVertex) // ���� �� ��������� �������
				{
					visited[i] = false; // �� ��������
										//mark[i] = 0; // �� ��������
					route[i] = -1; // ��� ����� �� �� ����� �������
				}
			}
			minCapacity = INF;
			found = false; // ������������, ��� �������� ���
			findRoute(routeType, startVertex, W, visited, route, mark, N, targetVertex, &minCapacity); // ����� �������� �� ������ � ���� � ������ ��� � ������
			if (found) // ���� ������� ������
			{
				int k = targetVertex; // ������ ������� �������
				while (k != startVertex) // ���������� �� ���������� ��������, ���� �� ������ �� ������
				{
					W[route[k]][k] -= minCapacity;
					k = route[k];
				}
				maxFlow += minCapacity;
			}
		} while (found);

		// ������ ����: ����� ��������� �� ����� ������ �����������
		// ��� ���� ������ ������������ �� ���� ������� �����������, ������� �� �������� (�.�. W[i][j] != 0) 
		// � �� ���� ��������� �����������, ������� �� ����� (�� ���� W[j][i] != G[j][i])
		routeType = PSEUDO_ROUTE;
		do
		{
			for (int i = 0; i < N; i++) // � ������ �������� �������� ������� ��� ������ ������� ������ ��������
			{
				if (i != startVertex) // ���� �� ��������� �������
				{
					visited[i] = false; // �� ��������
					mark[i] = 0; // �� ��������
					route[i] = -1; // ��� ����� �� �� ����� �������
				}
			}
			minCapacity = INF;
			found = false; // ������������, ��� �������� ���
			findRoute(routeType, startVertex, W, visited, route, mark, N, targetVertex, &minCapacity); // ����� �������� �� ������ � ���� � ������ ��� � ������
			if (found) // ���� ������� ������
			{
				int k = targetVertex; // ������ ������� �������
				while (k != startVertex) // ���������� �� ���������� ��������, ���� �� ������ �� ������
				{
					if (mark[k] == 1)
						W[route[k]][k] -= minCapacity;
					else if (mark[k] == -1)
						W[k][route[k]] += minCapacity;
					k = route[k];
				}
				maxFlow += minCapacity;
			}
		} while (found);

		cout << "**********��������� ������ ��������� �����-����������***********" << endl << endl;
		cout << "����� ���� - ������� � ������� " << startVertex + 1 << endl;
		cout << "���� ���� - ������� � ������� " << targetVertex + 1 << endl;
		cout << "������������ ����� � ������ ���� = " << maxFlow << endl;
		cout << endl << "������� ����� ��� ���� � ��������� ������������ �������" << endl;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				cout << G[i][j] - W[i][j];
				if (G[i][j] - W[i][j] < 10)
					cout << "  "; // ��� ������������ ����� ���.2 �������
				else cout << " "; // ����� ���. 1 ������
			}
			cout << endl;
		}
		delete[] visited;
		delete[] mark;
		delete[] route;
		for (int i = 0; i < N; i++)
		{
			delete[] W[i];
		}
		delete[] W;
		cout << endl << "��� ����������� ������� ����� �������" << endl;
		getch();
	}
}

// ������� ���������� ���� �� ����� ������� ����������� �� ������ � ����. ���� ��������, ���������� ���� � ������, ����� ���������� false
void findRoute(TypeOfRoute routeType, int currentVertex, int **W, bool *visited, int *route, int *mark, int N, int targetVertex, int *minCap)
{
	if (route[targetVertex] == -1) // ������� ����������� ��������: �� ������ ������� � �������� �����  
	{
		for (int i = 0; i < N && !found; i++)
		{
			if (i == currentVertex)
			{

				for (int j = 0; j < N && !found; j++) // ���������� �� ���� ��������� ������� �����, ������ �������� ����� ������� �������
				{
					if (visited[j] == false) // ���� �������� ������� �� ��������
					{
						switch (routeType) // ���������, ����� ���� �� ����
						{
						case SIMPLE_ROUTE: // ���� ���� ������� ����
						{
							if (W[i][j] != 0) // �� ��� ���������� ������ �� ����, ������� ��� �� ��������
							{
								visited[j] = true; // �������� ����� ��� ����������
								route[j] = currentVertex; // ��������� �������, �� ������� �� ����, � ������ ����
								if (*minCap > W[i][j]) // ���� ����������� ���������� �����������
									*minCap = W[i][j];
								findRoute(routeType, j, W, visited, route, mark, N, targetVertex, minCap); // � ���� ���� �� ����� ��������� �������
							}
							break;
						}
						case PSEUDO_ROUTE: // ���� �� �� ��� ��������� ��� ������� ���� � ���� ���������� (�� ���� ���� �� ����� ������������ ���)
						{
							if (W[i][j] != 0 || W[j][i] != G[j][i]) // ����� ������ �� ����� ������� �����������, ���� ��� �� ��������, ��� �� ����� ��������� �����������, ���� ��� �� �����
							{
								visited[j] = true; // �������� �� ��� ����������
								route[j] = currentVertex; // ��������� �������, �� ������� �� ����, � ������ ����
								mark[j] = (W[i][j] != 0) ? 1 : -1; // ���������� �����: ���� �� ������ �� ������ ����, �� ����� ����� 1, ����� -1

																   // �� ���� ������ ���� ����� ��������� ����������� ����.�����������
																   // ��������� ������, ����� �� ���� �� ���� i � j � ��������
								if (*minCap > W[i][j] && W[i][j] != 0)
									*minCap = W[i][j];
								else if (*minCap > W[j][i] && W[j][i] != 0)
									*minCap = W[j][i];
								findRoute(routeType, j, W, visited, route, mark, N, targetVertex, minCap); // � ���� ���� �� ����� ��������� �������
							}
							break;
						}
						}
						//visited[j] = true; // �������� �� ��� ����������
						//route[j] = currentVertex; // ��������� �������, �� ������� �� ����, � ������ ����
						//mark[j] = (W[i][j] != 0) ? -1 : 1;
						//// �� ���� ������ ���� ����� ��������� ����������� ����.�����������
						//// ��������� ������, ����� �� ���� �� ���� i � j � ��������
						//if (*minCap > W[i][j] && W[i][j] != 0) 
						//	*minCap = W[i][j]; 
						////else if (*minCap > W[j][i] && W[j][i] != 0) // ����� �� � �� �����, �.�. �� �������� ���� �� ����� ����������, ������, �� ��� ����� ����� ������ ��� ��� - ������ ��������
						////	*minCap = W[j][i];
						//findRoute(routeType, j, W, visited, route, mark, N, targetVertex, minCap); // � ���� ���� �� ����� ��������� �������
					}
				}
			}
			else continue;
		}
	}
	else found = true; // ���� �� �������� �������� �����, ���������� true
}

void printRoutes(int *dist, int *path, int startVertex)
{
	for (int i = 0; i < N; i++)
	{
		if (i != startVertex)
		{
			if (dist[i] == INF)
				cout << "���� �� ������� " << startVertex + 1 << " � " << i + 1 << " �� ����������!" << endl;
			else
			{
				bool found = false;
				int *route = new int[N]; // ��� ����������� ����������� ��������
				int k = 0; // ���������� ����
				route[k] = i + 1; // �������� � ����� (����� ����������� �������, ������� +1)
				k++;
				int index = i; // ����� ������� ������ ���������� ���������� � �������� ������� ��������
				while (!found)
				{

					for (int j = 0; j < N; j++) // ���������� ���� ������ �����
					{
						if (path[index] == j) // ���� ���������� ������� �� ���������, ������ �������� ������-���� �������, ������ � j-� ������� ����� ������� �� ������� � �������� index
						{
							index = j; // ���������� ������� ������, �.�. �� ������ ����� ���������� � ���
							break; // ��������� ����� � �����
						}
					}
					if (index != startVertex) // ���� ������ �� ����� ����������
					{
						route[k] = index + 1; // ��������� � ������� ��� ���� ������� (�����������, ������� +1)
						k++; // � ����������� ���������� ������� ����
					}
					else found = true; // ����� �������, ��� ������� ������
				}
				cout << startVertex + 1;
				for (int i = k - 1; i >= 0; i--)
				{
					cout << " -> " << route[i];
				}
				cout << " = " << dist[i] << endl;
			}
		}
	}
}