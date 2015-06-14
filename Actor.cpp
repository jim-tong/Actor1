//#include "stdafx.h"
#include "Actor.h"
//#include "windows.h"
std::atomic<int> Active::m_GlobalActiveID(0);

Active::Active(){}

Active::Active(int n32BlockSize, int n32BufferSize): m_PendingWorkNum(0), m_ActiveID(m_GlobalActiveID++), m_ThreadID(0), m_BeginInThreadCallback(NULL){
		m_IfUpdate = false;
}

Active::~Active() {
	Stop();
}

void Active::Send( Buffer* apBuffer ){
	if (NULL != apBuffer){
        m_IOMutex.lock();
		bool bNeedSig = m_Queue.empty();
		m_Queue.push(apBuffer);
		m_IOMutex.unlock();
		++m_PendingWorkNum;
		if (bNeedSig){
			m_ConditionVar.notify_one();
		}
	}
}

void Active::Run() {
	if (m_BeginInThreadCallback){
		m_BeginInThreadCallback();
	}
	while (true){
		Consume();
	}
}

Active* Active::CreateActive(Callback aCallBack, int n32BlockSize, int n32BufferSize){
	Active* aPtr = new Active(n32BlockSize, n32BufferSize);
	aPtr->m_Callback = aCallBack;
	return aPtr;
}

void Active::setCallBack(Callback aCallBack){
	m_Callback = aCallBack;
}

Buffer* Active::GetBuffer(){
    Buffer* pBuffer = new Buffer();
	return pBuffer;
}

bool Active::IfEmpty(){
	return m_Queue.empty();  
}

void Active::Consume(){
	//m_IOMutex.lock();
    std::unique_lock<std::mutex> lock(m_IOMutex);
	while(m_Queue.empty()){
		m_ConditionVar.wait(lock);
	}

	m_SwapQueue.swap(m_Queue);
	lock.unlock();

	while(!m_SwapQueue.empty()){
		Buffer* pBuffer = m_SwapQueue.front();
		m_SwapQueue.pop();

		m_Callback(pBuffer);
		--m_PendingWorkNum;
		if (pBuffer){
			//m_pBufferPool.ReleaseObejct(pBuffer);
            delete pBuffer;
		}
	}
}

void Active::Stop(){
	m_ConditionVar.notify_one();
}

void Active::ReleaseBuffer(Buffer*& pBuffer){
	if (NULL != pBuffer){
		//m_pBufferPool.ReleaseObejct(pBuffer);
		pBuffer = NULL;
	}
}

unsigned Active::GetThreadID(){
	return m_ThreadID;
}

void Active::RunWithUpdate(){
	bool ifNotify = false;
	bool ifQueueEmpty = false;
	while(true){
		ifNotify = false;
		ifQueueEmpty = false;
	}
}

void Active::Start(){
    m_Thread = std::thread(&Active::Run, this);
	//m_ThreadID = get_native_thread_id(m_Thread);
}

void Active::SetThreadStartCallback(BeginInThreadCallback pBeginInThreadCallback){
	m_BeginInThreadCallback = pBeginInThreadCallback;
}

int Buffer::GetMaxLength(){
	return m_MaxLength;
}
