#include <iostream>
#include <thread>
#include <string>
#include <chrono>

#include <mutex>
#include <ctime>
#include <vector>

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

mutex myMutex;
//recursive_mutex myRecursiveMutex;

void PrintVector(vector<int> VectorToPrint)
{
	for (int i : VectorToPrint)
	{
		printf("%d ", i);
	}
	printf("\n");
}

void RandomFillVectorBySize(vector<int> &VectorToFill, int sizeVector)
{
	//myMutex.try_lock();
	//unique_lock<mutex> myUnique_lock(myMutex, defer_lock);
	myMutex.lock();

	VectorToFill.clear();
	for (int i = 0; i < sizeVector; i++)
	{
		int randNumber = rand() % 100;
		VectorToFill.push_back(randNumber);
		printf("%d ", randNumber);
		this_thread::sleep_for(chrono::milliseconds(300));
	}
	printf("\n");

	myMutex.unlock();
}

void FillVectorBySize(vector<int>& VectorToFill, int Value, int sizeVector)
{
	//this_thread::sleep_for(chrono::milliseconds(1000));

	{
		lock_guard<mutex> muGuard(myMutex);

		VectorToFill.clear();
		for (int i = 0; i < sizeVector; i++)
		{
			VectorToFill.push_back(Value + 1);
			printf("%d ", Value + i);
			this_thread::sleep_for(chrono::milliseconds(300));
		}
		printf("\n");

	}
	//this_thread::sleep_for(chrono::milliseconds(1000));
}

int main()
{
	chrono::time_point<chrono::steady_clock> StartClock, EndClock;
	chrono::duration<float> DurationClock;
	StartClock = chrono::high_resolution_clock::now();

	srand(static_cast<unsigned int>(time(0)));

	vector<int> myVector;

	thread FillVectorThread([&]()
	{
			FillVectorBySize(myVector, 55, 5);
	});

	thread FillVectorThread2([&]()
		{
			RandomFillVectorBySize(myVector, 5);
		});

	FillVectorThread.join();
	FillVectorThread2.join();

	EndClock = chrono::high_resolution_clock::now();
	DurationClock = EndClock - StartClock;
	printf("%f", DurationClock.count());

	cout << endl;
	//system("pause");

	return 0;
}
