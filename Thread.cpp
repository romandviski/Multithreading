#include <iostream>
#include <thread>
#include <string>
#include <chrono>

using namespace std;

class ThreadClass
{
	
public:
	void SimpleAbstractWork(std::string tmp)
	{
		cout << "----- " << "ID Thread - " << this_thread::get_id() << " ---- SimpleAbstractWork Start ----" << endl;
		for (int i = 0; i < 10; i++)
		{
			std::cout << "ID Thread - " << std::this_thread::get_id() << "\t" << "DoWork - " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		cout << "----- " << "ID Thread - " << this_thread::get_id() << " ---- SimpleAbstractWork Done ----" << endl;
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
		std::cout << "----- " << "ID Thread - " << std::this_thread::get_id() << " ---- Sum Start ---- " << a << " + " << b << " ----" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		result = a + b;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "----- " << "ID Thread - " << std::this_thread::get_id() << " ---- Sum Done ---- " << "Result = " << result << " ----" << std::endl;
		return result;
	}

	void Assignment(int &a)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		std::cout << "----- " << "ID Thread - " << std::this_thread::get_id() << " ---- Assignment Start ---- " << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		a = a * a;
		std::cout << "----- " << "ID Thread - " << std::this_thread::get_id() << " ---- Assignment Done ---- "<< a << std::endl;
	}

	void GreatJobWithoutParametr()
		{
			cout << "----- " << "ID Thread- " << this_thread::get_id() << " ---- GreatJobWithoutParametr Start ----" << endl;
			this_thread::sleep_for(chrono::milliseconds(4000));
			cout << "----- " << "ID Thread- " << this_thread::get_id() << " ---- GreatJobWithoutParametr Done ----" << endl;
		}

};

int main()
{
	cout << "Core thread number - " << thread::hardware_concurrency() << endl;
	cout << "Sleep - 0,1 s" << endl;
	this_thread::sleep_for(chrono::milliseconds(100));
	cout << "Main thread ID - " << this_thread::get_id() << endl;

	ThreadClass myThreadClass;
	int Result;
	int SecondResult = 5;

	thread ThreadObjectWithLambda([&Result, &myThreadClass]()
	{
		Result = myThreadClass.Sum(5, 7);
	});

	thread ThreadObjectWithLambda2([&]()
	{
		myThreadClass.Assignment(SecondResult);
	});

	thread ThreadObjectWithLambda3(&ThreadClass::GreatJobWithoutParametr, myThreadClass);

	myThreadClass.SimpleAbstractWork("Main");
	ThreadObjectWithLambda.join();
	ThreadObjectWithLambda2.join();
	ThreadObjectWithLambda3.join();

	cout << endl << Result << endl;
	cout << SecondResult << endl;



	return 0;
}
