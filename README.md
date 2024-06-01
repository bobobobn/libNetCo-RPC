# libCoNet
## 介绍
---
linux下C++基于ucontext库实现轻量级对称协程，HOOK了常用SOCKET的API
## 特性
---
* 基于boost::ucontext函数集开发了协程对象
* 实现了线程/协程调度器，自带负载均衡，同时支持用户指定CPU核心数和特定CPU核心来运行任务
* 可根据实际需要动态的配置协程栈大小，同时配置了内存池提升多协程的调度速度
* 将复杂的异步处理流程隐藏在框架底层，通过类似Golang接口的netco::co_go()完成协程接口封装，上层用户可以使用业务同步开发的方式获得异步的高性能，避免了异步回调和回调分离引发的代码支离破碎
* HOOK了SOCKET类，用户可以使用同步开发的方式调用非阻塞的Socket::send, Socket::read, Socket::accpet, Socket::connect方法，同时实现了非阻塞的协程TCP服务器
## 使用
---
编译src文件夹源码，使用时包含include/netco_api.h，即可使用netco协程库接口
netco::co_go(func)执行一条协程；
netco::co_sleep(time)当前协程time毫秒后继续执行;
'''
cd src
make
'''

''' cpp
使用时包含include/socket.h，即可使用socket族函数接口
Socket s; //创建一个socket类
s.bind(80); //绑定端口号80
s.bind("127.0.0.1", 80); //绑定127.0.0.1:80
s.listen(); //开始监听
SocketPtr conSock(s.accept()); //accept一个连接
conSock->read(buf, 1024); //读连接上的数据
conSock->send(buf, 1024); //往对端写数据
Socket clientSock;
clientSock.connect("127.0.0.1", 80); //建立一个连接,之后即可如上述read和send
'''