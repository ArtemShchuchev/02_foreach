#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
#include <random>
#include "SecondaryFunction.h"

/*
Реализовать параллельный вариант оператора for_each.

Функция должна принимать итераторы на начало и конец
контейнера и ссылку обрабатывающую функцию.

При реализации необходимо рекурсивно разделить контейнер
на несколько блоков и для каждого блока запустить
отдельную задачу, применяющую обрабатывающую функцию к
части контейнера.
*/

// заполняет массив случайными значениями от 0 до 100
void fillArr(std::vector<int>& arr)
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	for (auto& data : arr) data = std::rand() % 100;
}
// перегружаю оператор << для wcout (чтобы выводить std::vector<int>)
std::wostream& operator<< (std::wostream& out, const std::vector<int>& vect)
{
	for (const auto& val : vect) out << val << " ";
	return out;
}


int main(int argc, char** argv)
{
	printHeader(L"for_each");

	const size_t arrsize = 20;
	std::vector<int> arr(arrsize);

	fillArr(arr);
	std::wcout << L"Массив до обработки: " << arr << "\n";
	
	std::wcout << "\n";

	return 0;
}
