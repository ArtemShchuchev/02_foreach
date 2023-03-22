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
	//std::srand(static_cast<unsigned int>(std::time(nullptr)));
	//for (auto& data : arr) data = std::rand() % 100;

	int count(0);
	for (auto& data : arr) data = ++count;
}
// перегружаю оператор << для wcout (чтобы выводить std::vector<int>)
std::wostream& operator<< (std::wostream& out, const std::vector<int>& vect)
{
	for (const auto& val : vect) out << val << " ";
	return out;
}

// for_each буду делить на 3 потока
template<class IT, class FN>
FN foreach(IT first, IT last, FN function)
{
	const int num_potok = 3;
	auto sizeContainer{ last - first };
	auto sizebl{ sizeContainer / num_potok };
	auto lbd = [](IT it, IT end, FN& function) {for (; it != end; ++it) function(*it); return function; };
	std::future<FN> as;

	//auto start = std::chrono::steady_clock::now();
	/*
	*/
	for (int k = 0; k < num_potok; ++k)
	{
		IT it = first + sizebl * k, end;
		if (k != (num_potok - 1)) end = it + sizebl;
		else end = last;
		as = std::async(lbd, it, end, function);
	}

	//for (; first != last; ++first) function(*first);

	//auto endt = std::chrono::steady_clock::now();
	//std::chrono::duration<double, std::micro> delta = endt - start;
	//std::wcout << "\ntime: " << delta.count();

	return as.get();
}


int main(int argc, char** argv)
{
	printHeader(L"for_each");
	

	const size_t arrsize = 20;
	std::vector<int> arr(arrsize);

	fillArr(arr);

	std::wcout << L"Массив до обработки: ";
	auto print = [](const int& n) { std::wcout << n << ' '; };
	foreach(arr.begin(), arr.end(), print);
	std::wcout << "\n";
	
	struct Sum
	{
		void operator()(int n) { sum += n; }
		int sum{ 0 };
	}s;
	s = foreach(arr.begin(), arr.end(), s);
	std::wcout << L"Сумма массива: " << s.sum << "\n";

	foreach(arr.begin(), arr.end(), [](int& n) { n++; });
	std::wcout << L"Массив после n + 1:  " << arr << "\n";

	s.sum = 0;
	s = foreach(arr.begin(), arr.end(), s);
	std::wcout << L"Сумма массива: " << s.sum << "\n";
	
	std::wcout << "\n";

	return 0;
}
