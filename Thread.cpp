#include <iostream>
#include <thread>
#include <string>
#include <chrono>

#include <mutex>
#include <ctime>
#include <vector>

#include <condition_variable>>
#include <random>

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
//recursive_mutex myRecursiveMutex; // wikipedia

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

int GetRandomInRange(const int min, const int max)
{
	static default_random_engine gen(static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count()));
	uniform_int_distribution<int> distribution(min, max);

	return distribution(gen);
}

condition_variable CV_FillVector;
//condition_variable_any
//condition_variable CV_ReadVector;
bool EndConsume = false;

void Consume(int& value, vector<int>& Vector, int neededInt, mutex& locker)
{
	unique_lock<mutex> lock(locker);

	bool bIsConsumeFill = false;
	while (!bIsConsumeFill)
	{
		//CV_FillVector.wait_until(lock, chrono::high_resolution_clock::now() + chrono::milliseconds(1000));
		CV_FillVector.wait(lock);
		if (value < 10)
		{
			unsigned int i = 0;
			while (i < Vector.size())
			{
				if (Vector[i] == neededInt)
				{
					value++;
					Vector.erase(Vector.begin() + i);
					i--;
					printf("Hey i found what i want - %d", neededInt);
					printf("\n");
					if (value > 10)
					{
						bIsConsumeFill = true;
					}
				}
				i++;
			}
			Vector.clear();
			//CV_ReadVector.notify_all(); //all threads
		}
	}
	EndConsume = true;
}

void Produce(vector<int>& VectorToFill, mutex &locker)
{
	unique_lock<mutex> UL(locker, defer_lock);

	while (true)
	{
		this_thread::sleep_for(chrono::milliseconds(100));
		int randNumber = GetRandomInRange(0, 10);

		UL.lock();
		VectorToFill.push_back(randNumber);
		UL.unlock();
		if (VectorToFill.size() > 50)
		{
			CV_FillVector.notify_one();
		}
	}
}

int FutureValue1;
void GenerateValue()
{
	int i = 0;
	while (i < 2)
	{
		printf("work Second GenerateValue \n");
		this_thread::sleep_for(chrono::milliseconds(1000));
		i++;
	}
	FutureValue1 = GetRandomInRange(0, 100);
}

int main()
{
	//srand(static_cast<unsigned>(time(0))); //bad work in thread

	vector<int> myVector;
	vector<thread> threads;

	int SuccessConsumeCout = 0;

	mutex m;
	mutex& lockerFormyVector = m;

	for (int i = 0; i < 2; i++)
	{
		threads.push_back(thread([&]()
		{
			Produce(myVector, lockerFormyVector);
		}));
	}

	for (thread& t : threads)
	{
		t.detach();
	}
	thread ConsumeThread(Consume, ref(SuccessConsumeCout), ref(myVector), 5, ref(lockerFormyVector));

	while (!EndConsume)
	{
		printf("%d ", SuccessConsumeCout);

		this_thread::sleep_for(chrono::milliseconds(100));
	}

	printf("\n");
	printf("End work %d ", SuccessConsumeCout);
	ConsumeThread.join();

	cout << endl;
	//system("pause");

	return 0;
}
