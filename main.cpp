#include <chrono>
#include <mutex>
#include <vector>
#include <future>
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

// заполняет массив
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

std::mutex m;
// for_each буду делить на 3 потока
template<class IT, class FN>
FN foreach(IT first, IT last, FN function)
{
	const int num_potok = 3;
	auto sizeContainer{ last - first };
	auto sizebl{ sizeContainer / num_potok };
	auto lbd = [](IT it, IT end, FN& function) -> FN
	{
		for (; it != end; ++it)
		{
			std::lock_guard<std::mutex> grd(m);
			function(*it);
		}
		return function;
	};

	// делю задачу на потоки
	IT it = first;
	int count{ num_potok - 1 };
	while (--count)
	{
		IT end = it + sizebl;
		auto as = std::async(std::launch::async, lbd, it, end, std::ref(function));
		it += sizebl;
	}
	lbd(it, last, std::ref(function));

	// тоже самое в один поток
	//for (; first != last; ++first) function(*first);

	return function;
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
