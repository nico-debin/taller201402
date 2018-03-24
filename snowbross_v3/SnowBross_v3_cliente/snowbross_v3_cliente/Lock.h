#ifndef _Lock_H_
#define _Lock_H_

#include <WinSock2.h>

// Singleton
class Lock{
	public:
		virtual ~Lock();
		//static Lock* getLock();
		
		bool isLocked();
		void start();
		void stop();
		CRITICAL_SECTION getHandle();
		Lock();

	private:
		//static Lock* instance;
		CRITICAL_SECTION lock;
		bool locked;
};

#endif