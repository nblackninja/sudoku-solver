// Sudoku-Solver.cpp: определяет точку входа для консольного приложения.
//

/*
*	Файл: Sudoku-Solver.exe
*
*		Создан: 05, Декабря 2018
*		Автор: Юдин Никита
*
*	Описание:	Эта утилита решает судоку (размером: 9*9)
*				значения представлены в двумерном массиве
*				и ищет первое правильное решение
*				методом рекурсивного перебора.
*/

//========= Библиотеки =========//
#include "stdafx.h"
#include <iostream>
#include <time.h>

//========= Пространство имен ==========//
using namespace std;

//========= Константы =========//
const size_t	N = 3;		// Размер одного квадрата 3 на 3.
const size_t	EMPTY = 0;	// Обозначает пустую ячейку.

							//====== Декларация функций ======//
bool fill_sud(size_t sud[][N*N], size_t row, size_t col);
void print_sud(const size_t sud[][N*N]);
bool is_legal(const size_t sud[][N*N], size_t row, size_t col, size_t val);
bool is_row_ok(const size_t row[], size_t col, size_t val);
bool is_col_ok(const size_t sud[][N*N], size_t row, size_t col, size_t val);
bool is_sqr_ok(const size_t sud[][N*N], size_t row, size_t col, size_t val);

//========== Main ===========//
int main()
{
	setlocale(LC_ALL, "Russian");
	clock_t start = clock();

	size_t sud[N*N][N*N] = {
		0,0,5   ,3,0,0    ,0,0,0,
		8,0,0   ,0,0,0    ,0,2,0,
		0,7,0   ,0,1,0    ,5,0,0,

		4,0,0   ,0,0,5    ,3,0,0,
		0,1,0   ,0,7,0    ,0,0,6,
		0,0,3   ,2,0,0    ,0,8,0,

		0,6,0   ,5,0,0    ,0,0,9,
		0,0,4   ,0,0,0    ,0,3,0,
		0,0,0   ,0,0,9    ,7,0,0
	};  // Доска судоку.

	fill_sud(sud, 0, 0);
	print_sud(sud);

	clock_t end = clock();
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "Время: " << seconds << " сек" << endl;

	cout << endl;
	return 0;
}

//======== Заполнение Судоку =========//
// Описание:	Пытаемся временно заполнить судоку в соответствие с правилами.
// Аргументы:	судоку, начало отсчета по линиям, начало отсчета по колонкам
// Возвращает:	удалось ли заполнить судоку (true - удалось, false - нет)
bool fill_sud(size_t sud[][N*N], size_t row, size_t col)
{
	// Указывает на номер строки следующей ячейки.
	size_t next_row = row;
	if (col == N*N - 1)
		++next_row;

	// Указываем на номер столбца следующей ячейки.
	size_t next_col = (col + 1) % (N*N);

	// Если это условие верно, то мы решили судоку.
	if (row == N*N)
		return true;

	// Проверяем, заполнена ли ячейка
	// если заполнена, то пропускаем
	// если не заполнена, то снова вызываем эту функцию заполнения
	if (sud[row][col] != EMPTY)
		return fill_sud(sud, next_row, next_col);

	// Пытаемся найти правильное и соответсвующее значение
	// и поставить в текущую ячейку.
	for (size_t value = 1; value <= N*N; ++value)
	{
		sud[row][col] = value;

		// Проверяем может ли значение(value) быть в ячейке,
		// и возвращает true, если может.
		if (is_legal(sud, row, col, value) && fill_sud(sud, next_row, next_col))
			return true;

		// Не удалось заполнить ячейку!
		sud[row][col] = EMPTY;
	}

	// Судоку решить не получилось.
	return false;
}

//======== Вывод Судоку ========//
// Вывод судоку в виде красивой таблицы.
void print_sud(const size_t sud[][N*N])
{
	for (size_t i = 0; i < N*N; ++i)
	{
		for (size_t j = 0; j < N*N; ++j)
		{
			cout << sud[i][j] << ' ';
			if (j == 2 || j == 5)
				cout << "| ";
		}
		cout << endl;
		if (i == 2 || i == 5)
			cout << "------+-------+------" << endl;
	}
}

//========== Доступное значение ==========//
// Проверяем и возвращаем, если значение value возможно в этой ячейке
bool is_legal(const size_t sud[][N*N], size_t row, size_t col, size_t value)
{
	return (is_row_ok(sud[row], col, value) &&
		is_col_ok(sud, row, col, value) &&
		is_sqr_ok(sud, row, col, value));
}

//========= Проверка ряда =========//
// Проверяем и возвращаем true, если значение value возможно в ряду
bool is_row_ok(const size_t row[], size_t col, size_t val)
{
	for (size_t i = 0; i < N*N; ++i)
		if (i != col && row[i] == val)
			return false;       // Found the same value again!

	return true;
}

//========= Проверка столбца =========//
// Проверяем и возвращаем true, если значение value возможно в строке
bool is_col_ok(const size_t sud[][N*N], size_t row, size_t col, size_t val)
{
	for (size_t i = 0; i < N*N; ++i)
		if (i != row && sud[i][col] == val)
			return false;       // Found the same value again!

	return true;
}

//========= Проверка квадрата =========//
// Проверяем и возвращаем true, если значение value возможно в квадрате
bool is_sqr_ok(const size_t sud[][N*N], size_t row, size_t col, size_t val)
{
	size_t row_corner = (row / N) * N;
	// Удерживаем номер строки текущей квадратной угловой ячейки.

	size_t col_corner = (col / N) * N;
	// Удерживаем номер столбца текущей квадратной угловой ячейки.

	for (size_t i = row_corner; i < (row_corner + N); ++i)
		for (size_t j = col_corner; j < (col_corner + N); ++j)
			if ((i != row || j != col) && sud[i][j] == val)
				return false;       // Нашли тоже значение снова!

	return true;
}
