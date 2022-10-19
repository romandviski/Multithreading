#include <iostream>
#include <thread>
#include <string>
#include <chrono>

//using namespace std;

void SimpleAbstractWork(std::string tmp)
{
	for (int i = 0; i < 10; i++)
	{
		std::cout << "ID Thread - " << std::this_thread::get_id() << "\t" << "DoWork - " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void LoopSimpleAbstractWork(std::string tmp)
{
	int i = 0;
	while(true)
	{
		std::cout << "ID Thread Loop - " << std::this_thread::get_id() << "\t" << "DoLoopWork - " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		i++;
	}
}

int Sum(int a, int b)
{
	int result = -1;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "----- " << "ID Thread - " << std::this_thread::get_id() << " ---Sum Start--- " << a << " + " << b << " -----" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	result = a + b;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	std::cout << "----- " << "ID Thread - " << std::this_thread::get_id() << " ---Sum Done--- " << "Result = " << result << " -----" << std::endl;
	return result;
}

void Assignment(int &a)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	std::cout << "----- " << "ID Thread - " << std::this_thread::get_id() << " ---Assignment Start--- " << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	a = a * a;
	std::cout << "----- " << "ID Thread - " << std::this_thread::get_id() << " ---Assignment Done--- "<< a << std::endl;
}

int main()
{
	std::cout << "Core thread number - " << std::thread::hardware_concurrency() << std::endl;
	std::cout << "Sleep - 0,1 s" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "Main thread ID - " << std::this_thread::get_id() << std::endl;
	std::cout << std::endl;

	int GlobalVar = 8;
	std::cout << GlobalVar << std::endl;
	//std::thread SecondThread(Assignment, std::ref(GlobalVar));
	std::thread SecondThread([&GlobalVar]
	{
			GlobalVar = Sum(4, 6);
	});
	//SecondThread.detach();

	SimpleAbstractWork("Main");

	SecondThread.join();
	std::cout << GlobalVar << std::endl;
	return 0;
}
