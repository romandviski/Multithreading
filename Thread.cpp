#include <iostream>
#include <thread>
#include <string>
#include <chrono>

#include <mutex>
#include <ctime>
#include <vector>

#include <condition_variable>
#include <random>

#include <future>

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

int GenerateValue(int value)
{
	int i = 0;
	while (i < 2)
	{
		printf("work Second GenerateValue \n");
		this_thread::sleep_for(chrono::milliseconds(1000));
		i++;
	}
	return GetRandomInRange(0, 100) + value;
}

void GetFutureValue(shared_future<int> myFuture)
{
	printf("\nGetFutureValue Thread - %d \n", myFuture.get());
}

void GenerateValueWithPromise(int value, promise<int> &promise)
{
	int i = 0;
	while (i < 2)
	{
		printf("work GenerateValueWithPromise\n");
		this_thread::sleep_for(chrono::milliseconds(1000));
		i++;
	}
	promise.set_value(GetRandomInRange(0, 100) + value);
}

unsigned long long NonAtomicValue = 0;
atomic<unsigned long long> AtomicValue = 0;

void reWriteVariable(unsigned long long& value)
{
	int i = 0;
	while(i < 100000)
	{
		value++;
		i++;
	}
	cout << "Thread Finish" << endl;
}

void reWriteVariableAtomic(atomic<unsigned long long>& value)
{
	int i = 0;
	while (i < 100000)
	{
		value++;
		i++;
	}
	cout << "Atomic Thread Finish" << endl;
}

int main()
{
	packaged_task<int(int)> myTask(GenerateValue);
	future<int> myFuture = myTask.get_future();
	thread myThread(ref(myTask), 5);
	myFuture.get();

	promise<int> FuturePromise;
	shared_future<int> SharedFuture = FuturePromise.get_future().share();

	thread PromiseThread(GenerateValueWithPromise, 5, ref(FuturePromise));


	int FutureValue1 = 0;

	//future<int> SimpleFuture1 = async(GenerateValue, 5);
	shared_future<int> SimpleFuture1 = async(GenerateValue, 5);
	//SimpleFuture1.valid();
	//SimpleFuture1.wait();
	//printf("\n FutureValue1 - %d", FutureValue1);

	thread GetFutureValueThread(GetFutureValue, ref(SimpleFuture1));

	int i = 0;
	while (i < 3)
	{
		printf("Work main \n");
		this_thread::sleep_for(chrono::milliseconds(1100));
		i++;
	}

	//FutureValue1 = SimpleFuture1.get();
	//printf("\n FutureValue1 - %d \n", FutureValue1);

	GetFutureValueThread.join();
	PromiseThread.join();
	myThread.join();

	thread myThread1(reWriteVariable, ref(NonAtomicValue));
	thread myThread2(reWriteVariable, ref(NonAtomicValue));
	thread myThread3(reWriteVariable, ref(NonAtomicValue));
	myThread1.detach();
	myThread2.detach();
	myThread3.detach();

	thread myAtomicThread1(reWriteVariableAtomic, ref(AtomicValue));
	thread myAtomicThread2(reWriteVariableAtomic, ref(AtomicValue));
	thread myAtomicThread3(reWriteVariableAtomic, ref(AtomicValue));
	myAtomicThread1.detach();
	myAtomicThread2.detach();
	myAtomicThread3.detach();

	this_thread::sleep_for(chrono::milliseconds(1000));
	cout << "Value ATOMIC - " << AtomicValue << endl;
	cout << "Value nonATOMIC - " << NonAtomicValue << endl;

	return 0;
}
