#include "thread_safe_queue.h"
#include <thread>
#include <iostream>
#include <functional>

namespace
{
	struct QueueEntry
	{
		static size_t ctr;
		size_t msg;
	};
	size_t QueueEntry::ctr = 0;

	QueueEntry CreateANewEntry() { return QueueEntry{ ++QueueEntry::ctr }; }
}

int main()
{
	ThreadSafeQueue<QueueEntry> TSQ;

	auto producer = [](ThreadSafeQueue<QueueEntry>& TSQ) {
		for (size_t i = 0; i != 1000; ++i)
		{
			TSQ.push(CreateANewEntry());
		}
	};

	auto consumer = [](ThreadSafeQueue<QueueEntry>& TSQ) {
		for (size_t i = 0; i != 1000; ++i)
		{
			std::cout << TSQ.pop().msg << "\n";
		}
	};

	std::thread c1(consumer, std::ref(TSQ));
	std::thread c2(consumer, std::ref(TSQ));
	std::thread p1(producer, std::ref(TSQ));
	std::thread p2(producer, std::ref(TSQ));

	p1.join();
	p2.join();
	c1.join();
	c2.join();

	return 0;
}