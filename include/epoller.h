#pragma once
#include <vector>

struct epoll_event;

namespace netco
{
	class Coroutine;

	/* 监听事件, 事件的handler为Coroutine */
	class Epoller
	{
	public:
		Epoller();
		~Epoller();

		// create epollFd, return false if failed
		bool init();

		// modify the event of fd
		bool modifyEv(Coroutine* pCo, int fd, int interesEv);

		// add the event of fd
		bool addEv(Coroutine* pCo, int fd, int interesEv);

		// remove the event of fd
		bool removeEv(Coroutine* pCo, int fd, int interesEv);

		// call epoll_wait, store active event to activeEvServs
		int getActEvServ(int timeOutMs, std::vector<Coroutine*>& activeEvServs);

	private:

		inline bool isEpollFdUseful() { return epollFd_ < 0 ? false : true; };

		int epollFd_;

		std::vector<struct epoll_event> activeEpollEvents_;

	};

}
