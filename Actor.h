#pragma once
#include <thread>
#include <functional>
#include <memory>
#include <queue>
#include <mutex>
#include <string>
//#include "ThreadSafeQueue.h"
#include "Buffer.h"
#include <atomic>
#include <vector>
//#include <boost/date_time/posix_time/posix_time.hpp>
//#include "boost/date_time/gregorian/gregorian.hpp"
#include <set>
//#include "Timer.h"

typedef std::function<void(Buffer*&)> Callback;
typedef std::function<void()> BeginInThreadCallback;

//using boost::gregorian::date;
//using boost::posix_time::ptime;
//using boost::posix_time::microsec_clock;

class Active {
private:
	Active(const Active&);
	Active& operator=(const Active&);
	Active();
	Active(int n32BlockSize, int n32BufferSize);
	void Run();
	void RunWithUpdate();
	void setCallBack(Callback aCallBack);
	bool IfEmpty();
	void Consume();
	std::queue<Buffer*> m_Queue;
	std::thread	m_Thread;
	Callback		m_Callback;
	std::mutex	m_IOMutex;
	//CThreadSafeObejctPool<Buffer> m_pBufferPool;
	std::condition_variable m_ConditionVar;
	std::atomic<int>		m_PendingWorkNum;
	int		m_ActiveID;
	static std::atomic<int> m_GlobalActiveID;
    std::atomic<bool>	m_IfNeedSig;
	unsigned		m_ThreadID;
	std::queue<Buffer*> m_SwapQueue;
	//Concurrency::concurrent_queue<Buffer*> m_SafeQueue;
	bool m_IfUpdate;
public:
	~Active();
	Buffer*			GetBuffer();
	int				GetActorID(){return m_ActiveID;}
	void			ReleaseBuffer(Buffer*& pBuffer);
	void			Send(Buffer* apBuffer);
	void			Stop();
	static Active*	CreateActive(Callback aCallBack, int n32BlockSize, int b32BufferSize);
	int				GetPendingWorkNum()const{return m_PendingWorkNum;}
	unsigned		GetThreadID();

	//CBattleTimer&			GetTimer(){return m_ThreadTimer;}

	void			Start();
	void			SetThreadStartCallback(BeginInThreadCallback pBeginInThreadCallback);

private:
	//CBattleTimer	m_ThreadTimer;
	BeginInThreadCallback m_BeginInThreadCallback;
};

