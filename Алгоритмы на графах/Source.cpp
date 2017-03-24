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

void printRoutes(int*, int*, int); // вывод путей для алгоритма Дейкстры и алгоритма Беллмана-Форда
void getMatrixFromFile(string); // получить матрицу из файла 
void showWeightMatrix(); // показать матрицу весов
void createNewWeightMatrix(); // создать новую матрицу веов
void dijkstraAlgorithm(); // выполнить алгоритм дейкстры для массива из файла
void floydAlgorithm(); // выполнить алгоритм флойда для массива из файла
void ford_fulkersonAlgorithm(); // выполнить алгоритм форда-фалкерсона для массива из файла
void bellman_fordAlgorithm(); // выполнить алгоритм беллмана-форда для массива из файла 
void findRoute(TypeOfRoute, int, int **, bool *, int *, int *, int, int, int *); // функция поиска потока для форда-фалкерсона методом поиска в глубину

bool found; // Определяет, найден ли очередной маршрут



const int INF = 1000000; // "бесконечность"
int **G; // Матрица смежности взвешенного графа
int N = 0; // количество вершин 

int main()
{
	setlocale(LC_ALL, "RUS");
	srand(time(NULL));
	getMatrixFromFile("input.txt");

	char inp;
	do
	{
		system("cls");
		cout << "Выберите действие: ";
		cout << "\n1 - Ввести матрицу весов";
		cout << "\n2 - Показать матрицу весов";
		cout << "\n3 - Найти кратчайшие маршруты с кратчайшим весом по алгоритму Дейкстры";
		cout << "\n4 - Найти кратчайшие маршруты с кратчайшим весом по алгоритму Флойда";
		cout << "\n5 - Найти кратчайшие маршруты с кратчайшим весом по алгоритму Беллмана-Форда";
		cout << "\n6 - Найти максимальный поток по алгоритму Форда-Фалкерсона";		
		cout << "\n0 - Выйти\n";
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
		default: cout << "\nЧто-то пошло не так в кейсе для меню..."; break;
		}

	} while (inp != '0');
	return 0;
}

void getMatrixFromFile(string fileName)
{
	//string fileName = "input.txt";
	ifstream inputFileStream (fileName, ios::in); // открытие файла fileName с режимом открытия "чтение"
	if (!inputFileStream)
	{
		cerr << "Невозможно открыть файл!";
		exit(1);
	}

	inputFileStream >> N; // считать из файла первую строку = количество элементов в матрице

	// создание массива N*N
	G = new int*[N];
	for (int i = 0; i < N; i++)
	{
		G[i] = new int[N];
	}
	// считывание данных из файла
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			inputFileStream >> G[i][j];
	}
	inputFileStream.close(); // закрыть поток
}

void showWeightMatrix()
{
	//getMatrixFromFile("input.txt");
	system("cls");
	cout << "Матрица весов для заданного взвешенного графа\n";
	for (int i = 0; i < N; i++) // в качестве теста заполним массив рандомными числами
	{
		for (int j = 0; j < N; j++)
		{
			//if (i == j)
			//	G[i][j] = 0;
			//else
			//	G[i][j] = rand() % 20; //от 0 до 9
			cout << G[i][j];
			if (G[i][j] < 10)
				cout << "  ";
			else cout << " ";
		}
		cout << endl;
	}
	cout << endl << "Для продолжения нажмите любую клавишу" << endl;
	getch();
}

void createNewWeightMatrix() // создать новую матрицу смежности
{
	system("cls");
	cout << "Функциия находится в разработке..." << endl << endl;
	cout << "Нажмите любую клавишу для возврата в меню." << endl;
	getch();
	return;
	/*
	cout << "Введите количество вершин в графе: ";
	cin >> N;
	if (N <= 0) // проверка на дурака
	{
		cout << endl << "Неверный ввод! Число вершин в графе должно быть натуральным числом!" << endl << endl;
		cout << "Нажмите любую клавишу для возврата в меню." << endl;
		getch();
		return;
	}
	else
	{
		G = new int *[N]; // создание
		for (int i = 0; i < N; i++) // двумерного динамического
		{
			G[i] = new int[N]; // массива
		}

		cout << "\nОсуществите ввод матрицы весов W (построчно)\n";
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
	int *distance = new int[N]; // массив расстояний от начального ребра до других
	bool *visited = new bool[N]; // массив, отвечающий за то, была ли посещена какая-либо вершина
	int minVertexIndex; // индекс вершины, до которой минимальный маршрут
	int currentVertexIndex; // индекс текущей вершины, от которой идет проверка
	int minDistance; // минимальная длина маршрута между вершинами
	bool finished = false; // переменная, определяющая, закончен ли алгоритм
	int *previous = new int[N]; // массив предыдущих точек для каждой вершины

	cout << "Введите номер вершины, из которой нужно найти минимальные маршруты (нумерация от 1): ";
	cin >> startVertex;
	if (startVertex <= 0 || startVertex > N) // проверка на дурака
	{
		cout << "\nНеверный ввод! Вершины с таким номером в исходном графе не существует! \nКоличество вершин - " << N << endl;
		cout << "Нажмите любую клавишу для возврата в меню." << endl;
		getch();
		return;
	}
	else
	{
		startVertex--; // уменьшаем номер вершины до стандарта языка Си (нумерация массивов с 0, поэтому смещение на -1 относительно фактических)

					   // стартовая инициализация: 
		for (int i = 0; i < N; i++)
		{
			previous[i] = -1; // нет предыдущих точек
			if (i == startVertex) // начальная вершина
			{
				distance[i] = 0; // кратчайший путь = 0
				visited[i] = true; // посещена изначально

			}
			else
			{
				distance[i] = INF; // иначе путь присваиваем "бесконечности"
				visited[i] = false; // и отмечаем как не посещенные
			}
		}
		minVertexIndex = startVertex; // наименьшая вершина изначально равна стартовой 
		currentVertexIndex = startVertex; // текущая вершина задана пользователем 

										  
		for (int i = 0; i < N; i++) // для каждой вершины
		{
			minDistance = INF; // полагаем минимальную дистанцию равной бесконечности для проверки
			for (int j = 0; j < N; j++) // поиск минимального по весу маршрута до следующей вершины
			{
				if (G[currentVertexIndex][j] != 0 && visited[j] == false) // если существует маршрут из текущей вершины до j-ой, и j-ая вершина еще не посещена
				{
					if (distance[currentVertexIndex] + G[currentVertexIndex][j] < distance[j]) // если новый маршрут меньше уже имеющегося
					{
						distance[j] = distance[currentVertexIndex] + G[currentVertexIndex][j]; // То ставим метку маршрута, равную длине маршрута до предыдущей вершины + весу ребра (из графа смежности)
						previous[j] = currentVertexIndex; // сохранить номер предыдущей вершины 
					}
				}
				if (minDistance >= distance[j] && visited[j] == false) // ищем следующую не посещенную вершину по минимальной метке
				{
					minVertexIndex = j;
					minDistance = distance[j];
				}
			}
			visited[minVertexIndex] = true; // помечаем, что до следующей вершины найден минимальный маршрут
			currentVertexIndex = minVertexIndex; // и отмечаем, что дальше нужно искать от нее												 
		}
		delete[] visited;

		cout << "\n**********Результат поиска по алгоритму Дейкстры**********\n";

		cout << endl << "Матрица длин путей из " << startVertex + 1 << "-ой вершины в остальные" << endl;
		for (int i = 0; i < N; i++)
			cout << distance[i] << " ";
		cout << endl;

		cout << "Получены следующие минимальные маршруты с минимальным весом от " << startVertex + 1 << "-ой вершины до остальных:" << endl;
		printRoutes(distance, previous, startVertex);
		delete[] distance;
		delete[] previous;
		cout << endl << "Для продолжения нажмите любую клавишу" << endl;
		getch();
	}
}

void floydAlgorithm()
{
	system("cls");

	if (N <= 0)
	{
		cout << "Для работы алгоритма необходимо существование матрицы весов графа!";
		cout << endl << "Нажмите любую клавишу для перехода к вводу матрицы.";
		getch();
		createNewWeightMatrix();
		system("cls");
	}

	int **W; // матрица весов
	int **H; // матрица "истории" для хранения пути
			 // определение матриц
	W = new int *[N];
	H = new int *[N];
	for (int i = 0; i < N; i++)
	{
		W[i] = new int[N];
		H[i] = new int[N];
	}
	// стартовая инициализация
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (G[i][j] == 0) // если не существует пути из i в j
			{
				W[i][j] = INF; // вес равен бесконечности
				H[i][j] = -1; // а следующий шаг для того чтобы попасть из i в j = -1 (такого индекса не может быть);
			}
			else // если же путь есть
			{
				W[i][j] = G[i][j]; // копируем вес
				H[i][j] = j; // следующий шаг из i в j равен j (+1 для информации "по факту", т.к. индексация в Си с 0)
			}

		}
	}
	// работа алгорита
	for (int k = 0; k < N; k++)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				if (W[i][j] > W[i][k] + W[k][j]) // если путь из i В j через промежуточную вершину k короче, чем прямой из i в j
				{
					W[i][j] = W[i][k] + W[k][j]; // мы нашли более короткий путь из i в j. Фиксируем его в массиве
					H[i][j] = H[i][k]; // следующий шаг для того, чтоы попасть из i в j равен шагу из i в k (т.к. k - промежуточная)
				}
	// вывод результата
	cout << "**************Результат работы алгоритма Флойда**************" << endl;

	cout << endl << "Матрица путей из каждой вершины в каждую:" << endl;
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
			if (i != j) // не рассматриваем петли
			{
				if (W[i][j] == INF)
					cout << "Пути из вершины " << i + 1 << " в вершину " << j + 1 << " не существует!" << endl;
				else
				{
					// индекс, показывающий вершину, в которой сейчас находимся на считывании

					int k = i;
					cout << i + 1;
					while (H[k][j] != -1)
					{
						cout << " -> " << H[k][j] + 1; // + 1 ввиду особенности языка Си (нумерация с 0)
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
	cout << endl << "Для продолжения нажмите любую клавишу" << endl;
	getch();
}

void bellman_fordAlgorithm()
{
	system("cls");
	
	int startVertex; // начальная вершина
	int *dist; // массив кратчайших путей из начальной вершины до i-ой вершины 
	int *path; // массив, каждый элемент которого хранит индекс текущего предка в отношении маршрута (для стартовой вершины - значение -1)

	if (N <= 0)
	{
		cout << "Для работы алгоритма необходимо существование матрицы весов графа!";
		cout << endl << "Нажмите любую клавишу для перехода к вводу матрицы.";
		getch();
		createNewWeightMatrix();
		system("cls");
	}

	cout << "Введите номер вершины, из которой нужно найти минимальные маршруты (нумерация от 1): ";
	cin >> startVertex;
	startVertex--;

	// изначально все пути заданы в бесконечность, предок в отношении пути не определен
	dist = new int[N];
	path = new int[N];
	for (int i = 0; i < N; i++)
	{
		dist[i] = INF;
		path[i] = -1;
	}		
	dist[startVertex] = 0; // у стартового пути вес равен нулю

	
	for (int i = 0; i < N-1; i++)
	{	
		bool distChanged = false; // отслеживает, произошли ли изменения в процессе релаксации. Для того, чтобы досрочно завершить алгоритм, если дальнейшая релаксация невозможна
		for (int u = 0; u < N; u ++)
			for (int v = 0; v < N; v++)
			{
				if (G[u][v]) // если существует путь из вершины u в вершину v, проверим, можно ли уменьшить машрут
				{
					// пересчет длины маршрута: если до вершины v можно добраться быстрее, используя вершину u + дугу (u,v)
					if (dist[v] > dist[u] + G[u][v])
					{
						// значит, мы нашли более короткий путь. Фиксируем его для вершины v, запоминаем предка (вершину u) и говорим, что в матрице произошли изменения
						dist[v] = dist[u] + G[u][v];
						path[v] = u;
						distChanged = true;
					}
				}
			}
		if (!distChanged)
			break;
	}

	cout << "\n**********Результат поиска по алгоритму Беллмана-Форда**********\n";

	cout << endl << "Матрица длин путей из " << startVertex + 1 << "-ой вершины в остальные" << endl;
	for (int i = 0; i < N; i++)
		cout << dist[i] << " ";
	cout << endl;

	cout << "Получены следующие минимальные маршруты с минимальным весом от " << startVertex + 1 << "-ой вершины до остальных:" << endl;
	printRoutes(dist, path, startVertex);
	delete[] dist;
	delete[] path;
	cout << endl << "Для продолжения нажмите любую клавишу" << endl;
	getch();
}

void ford_fulkersonAlgorithm()
{
	system("cls");
	int **W; // копия матрицы весов G
	bool *visited; // массив, определяющий, был ли посещен узел при очередной итерации поиска
	int *route; // массив, определяющий очередной путь из истока в сток
	int *mark; // массив меток, определяющий, пришли мы в очередную вершину по прямой или обратной дуге
	int maxFlow; // максимальный поток (изначально равен 0)
	int minCapacity; // минимальная пропускная способность
	int startVertex; // исток
	int targetVertex; // сток
	TypeOfRoute routeType; // тип текущего маршрута в поиске

	if (N <= 0)
	{
		cout << "Для работы алгоритма необходимо существование матрицы весов графа!";
		cout << endl << "Нажмите любую клавишу для перехода к вводу матрицы.";
		getch();
		createNewWeightMatrix();
		system("cls");
	}

	startVertex = -1;
	targetVertex = -1; // предполагаем, что этих вершин нет
	int *in = new int[N]; // вспомогательные массивы, определяющие количество входящих и исходящих дуг из каждой вершины
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
	if (startVertex == -1 || targetVertex == -1) // ИСПРАВИТЬ ФУНКЦИИ ПОКАЗА МАТРИЦЫ, ЧТОБЫ МОЖНО БЫЛО ЕЕ СЮДА ВСТАВИТЬ (УБРАТЬ cls ОТОВСЮДУ И ДОБАВИТЬ ЕГО В СВИТЧ
	{
		cout << "В текущей матрице не существует истока и\\или стока" << endl;
		cout << "ИЛИ их количество превышает допустимое!" << endl;
		cout << "Нажмите любую клавишу для перехода к вводу матрицы" << endl;
		getch();
		createNewWeightMatrix();
	}
	else if (startVertex != -1 && targetVertex != -1)
	{

		// стартовая инициализация
		W = new int *[N];
		visited = new bool[N];
		route = new int[N];
		mark = new int[N];
		for (int j = 0; j < N; j++)
		{
			W[j] = new int[N];
		}
		maxFlow = 0; // макс.поток равен 0

		for (int i = 0; i < N; i++) // копирование матрицы
		{
			for (int j = 0; j < N; j++)
			{
				W[i][j] = G[i][j];
			}
		}


		visited[startVertex] = true; // первая вершина (исток) всегда является посещенной
		mark[startVertex] = -1; // всегда из нее выходят дуги (какую метку ставить???)
		route[startVertex] = -1; // всегда в нее ничто не приходит


								 // первый этап: поиск всех маршрутов по дугам прямого направления
		routeType = SIMPLE_ROUTE;
		do // пока находятся новые маршруты прямого направления
		{
			for (int i = 0; i < N; i++) // в каждой итерации обнуляем массивы для работы функции поиска маршрута
			{
				if (i != startVertex) // если не стартовая вершина
				{
					visited[i] = false; // не посещены
										//mark[i] = 0; // не помечены
					route[i] = -1; // нет путей ни до какой вершины
				}
			}
			minCapacity = INF;
			found = false; // предполагаем, что маршрута нет
			findRoute(routeType, startVertex, W, visited, route, mark, N, targetVertex, &minCapacity); // поиск маршрута из истока в сток и запись его в массив
			if (found) // если маршрут найден
			{
				int k = targetVertex; // индекс текущей вершины
				while (k != startVertex) // проходимся по найденному маршруту, пока не дойдем до истока
				{
					W[route[k]][k] -= minCapacity;
					k = route[k];
				}
				maxFlow += minCapacity;
			}
		} while (found);

		// второй этап: поиск маршрутов по дугам любого направления
		// при этом должны перебираться те дуги прямого направления, которые не насыщены (т.е. W[i][j] != 0) 
		// и те дуги обратного направления, которые не пусты (то есть W[j][i] != G[j][i])
		routeType = PSEUDO_ROUTE;
		do
		{
			for (int i = 0; i < N; i++) // в каждой итерации обнуляем массивы для работы функции поиска маршрута
			{
				if (i != startVertex) // если не стартовая вершина
				{
					visited[i] = false; // не посещены
					mark[i] = 0; // не помечены
					route[i] = -1; // нет путей ни до какой вершины
				}
			}
			minCapacity = INF;
			found = false; // предполагаем, что маршрута нет
			findRoute(routeType, startVertex, W, visited, route, mark, N, targetVertex, &minCapacity); // поиск маршрута из истока в сток и запись его в массив
			if (found) // если маршрут найден
			{
				int k = targetVertex; // индекс текущей вершины
				while (k != startVertex) // проходимся по найденному маршруту, пока не дойдем до истока
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

		cout << "**********Результат работы алгоритма Форда-Фалкерсона***********" << endl << endl;
		cout << "Исток сети - вершина с номером " << startVertex + 1 << endl;
		cout << "Сток сети - вершина с номером " << targetVertex + 1 << endl;
		cout << "Максимальный поток в данной сети = " << maxFlow << endl;
		cout << endl << "Матрица весов для сети с найденным максимальным потоком" << endl;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				cout << G[i][j] - W[i][j];
				if (G[i][j] - W[i][j] < 10)
					cout << "  "; // для однозначного числа доп.2 пробела
				else cout << " "; // иначе доп. 1 пробел
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
		cout << endl << "Для продолжения нажмите любую клавишу" << endl;
		getch();
	}
}

// функция определяет путь по дугам прямого направления из истока в сток. Если возможно, записывает путь в массив, иначе возвращает false
void findRoute(TypeOfRoute routeType, int currentVertex, int **W, bool *visited, int *route, int *mark, int N, int targetVertex, int *minCap)
{
	if (route[targetVertex] == -1) // условие продолжения рекурсии: не найден маршрут в конечную точку  
	{
		for (int i = 0; i < N && !found; i++)
		{
			if (i == currentVertex)
			{

				for (int j = 0; j < N && !found; j++) // проходимся по всем элементам массива весов, индекс которого равен текущей вершине
				{
					if (visited[j] == false) // если очереная вершина не посещена
					{
						switch (routeType) // проверяем, какой путь мы ищем
						{
						case SIMPLE_ROUTE: // если ищем простой путь
						{
							if (W[i][j] != 0) // то нас интересуют только те дуги, которые еще не насыщены
							{
								visited[j] = true; // помечаем новую как посещенную
								route[j] = currentVertex; // добавляем вершину, от которой мы ищем, в массив пути
								if (*minCap > W[i][j]) // ищем минимальную пропускную способность
									*minCap = W[i][j];
								findRoute(routeType, j, W, visited, route, mark, N, targetVertex, minCap); // и ищем путь от новой найденной вершины
							}
							break;
						}
						case PSEUDO_ROUTE: // если же мы уже перебрали все простые пути и ищем псевдопути (то есть пути по любмы направлениям дуг)
						{
							if (W[i][j] != 0 || W[j][i] != G[j][i]) // можем ходить по дугам прямого направления, если оне не насыщены, или по дугам обратного направления, если они не пусты
							{
								visited[j] = true; // помечаем ее как посещенную
								route[j] = currentVertex; // добавляем вершину, от которой мы ищем, в массив пути
								mark[j] = (W[i][j] != 0) ? 1 : -1; // определяем метку: если мы пришли по прямой дуге, то метка будет 1, иначе -1

																   // по ходу поиска пути также определим минимальную проп.способность
																   // учитываем случаи, когда мы идем от дуги i к j и наоборот
								if (*minCap > W[i][j] && W[i][j] != 0)
									*minCap = W[i][j];
								else if (*minCap > W[j][i] && W[j][i] != 0)
									*minCap = W[j][i];
								findRoute(routeType, j, W, visited, route, mark, N, targetVertex, minCap); // и ищем путь от новой найденной вершины
							}
							break;
						}
						}
						//visited[j] = true; // помечаем ее как посещенную
						//route[j] = currentVertex; // добавляем вершину, от которой мы ищем, в массив пути
						//mark[j] = (W[i][j] != 0) ? -1 : 1;
						//// по ходу поиска пути также определим минимальную проп.способность
						//// учитываем случаи, когда мы идем от дуги i к j и наоборот
						//if (*minCap > W[i][j] && W[i][j] != 0) 
						//	*minCap = W[i][j]; 
						////else if (*minCap > W[j][i] && W[j][i] != 0) // вроде бы и не нужно, т.к. по обратной дуге мы будем прибавлять, значит, по ней можно будет пройти еще раз - лишняя итерация
						////	*minCap = W[j][i];
						//findRoute(routeType, j, W, visited, route, mark, N, targetVertex, minCap); // и ищем путь от новой найденной вершины
					}
				}
			}
			else continue;
		}
	}
	else found = true; // если мы достигли конечной точки, возвращаем true
}

void printRoutes(int *dist, int *path, int startVertex)
{
	for (int i = 0; i < N; i++)
	{
		if (i != startVertex)
		{
			if (dist[i] == INF)
				cout << "Пути из вершины " << startVertex + 1 << " в " << i + 1 << " не существует!" << endl;
			else
			{
				bool found = false;
				int *route = new int[N]; // для правильного отображения маршрута
				int k = 0; // индексация пути
				route[k] = i + 1; // начинаем с конца (номер фактической вершины, поэтому +1)
				k++;
				int index = i; // будет хранить индекс следующего найденного в обратном порядке элемента
				while (!found)
				{

					for (int j = 0; j < N; j++) // перебираем весь массив путей
					{
						if (path[index] == j) // если встретился элемент со значением, равным значению какого-либо индекса, значит в j-ю вершину можно попасть из вершины с индексом index
						{
							index = j; // запоминаем текущий индекс, т.к. мы дальше будем сравнивать с ним
							break; // завершаем поиск в цикле
						}
					}
					if (index != startVertex) // если индекс не равен начальному
					{
						route[k] = index + 1; // добавляем в маршрут еще одну вершину (фактическую, поэтому +1)
						k++; // и увеличиваем индексацию массива пути
					}
					else found = true; // иначе говорим, что маршрут найден
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