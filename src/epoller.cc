#pragma once
#include <sys/epoll.h>
#include "../include/epoller.h"
#include "../include/coroutine.h"
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include "stdio.h"
#include <memory>

using namespace netco;

Epoller::Epoller()
    :epollFd_(-1), activeEpollEvents_(16)
{
}

Epoller::~Epoller()
{
    if(isEpollFdUseful()){
        ::close(epollFd_);
    }
}

bool Epoller::init(){
    epollFd_ = epoll_create1(EPOLL_CLOEXEC);
    return isEpollFdUseful();
}


bool Epoller::addEv(Coroutine* pCo, int fd, int interesEv)
{
    if(!isEpollFdUseful()){
        return false;
    }
    epoll_event event;
    memset(&event, 0, sizeof(epoll_event));
    event.events = interesEv;
    event.data.ptr = pCo;
    if(epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event) < 0){
        return false;
    }
    return true;
}

bool Epoller::modifyEv(Coroutine* pCo, int fd, int interesEv)
{
    if(!isEpollFdUseful()){
        return false;
    }
    epoll_event event;
    memset(&event, 0, sizeof(epoll_event));
    event.events = interesEv;
    event.data.ptr = pCo;
    if(epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &event) < 0){
        return false;
    }
    return true;
}

bool Epoller::removeEv(Coroutine* pCo, int fd, int interesEv)
{
    if(!isEpollFdUseful()){
        return false;
    }
    epoll_event event;
    memset(&event, 0, sizeof(epoll_event));
    event.events = interesEv;
    event.data.ptr = pCo;
    if(epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &event) < 0){
        return false;
    }
    return true;
}


int Epoller::getActEvServ(int timeOutMs, std::vector<Coroutine*>& activeEvServs)
{
	if (!isEpollFdUseful())
	{
		return -1;
	}
	int actEvNum = ::epoll_wait(epollFd_, &*activeEpollEvents_.begin(), static_cast<int>(activeEpollEvents_.size()), timeOutMs);
	int savedErrno = errno;
	if (actEvNum > 0)
	{
		if (actEvNum > static_cast<int>(activeEpollEvents_.size()))
		{
			return savedErrno;
		}
		for (int i = 0; i < actEvNum; ++i)
		{
			Coroutine* pCo = static_cast<Coroutine*>(activeEpollEvents_[i].data.ptr);
            if( nullptr != pCo)
            {
                #ifdef DEBUGING
                    printf("coID:%d ,event:%d\n",pCo->id_, activeEpollEvents_[i].events);
                #endif
			    activeEvServs.push_back(pCo);
            }
		}
		if (actEvNum == static_cast<int>(activeEpollEvents_.size()))
		{
			activeEpollEvents_.resize(activeEpollEvents_.size() * 2);
		}
	}
	return savedErrno;
}