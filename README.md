# libCoNet

## 介绍
---
Linux 下 C++ 基于 `ucontext` 库实现轻量级对称协程，HOOK 了常用 SOCKET 的 API，构建了日志系统。

## 特性
---
* 构建了扩展能力强的日志系统，支持配置类似log4j的输出格式，继承`netco::LoggerAppender`类实现`log(LogLeven, shared_ptr<LogEvent>)`方法即可实现新的输出器
* 基于 `boost::ucontext` 函数集开发了协程对象
* 实现了线程/协程调度器，自带负载均衡，同时支持用户指定 CPU 核心数和特定 CPU 核心来运行任务
* 可根据实际需要动态配置协程栈大小，同时配置了内存池提升多协程的调度速度
* 将复杂的异步处理流程隐藏在框架底层，通过类似 Golang 接口的 `netco::co_go()` 完成协程接口封装，上层用户可以使用业务同步开发的方式获得异步的高性能，避免了异步回调和回调分离引发的代码支离破碎
* HOOK 了 SOCKET 类，用户可以使用同步开发的方式调用非阻塞的 `Socket::send`, `Socket::read`, `Socket::accept`, `Socket::connect` 方法，同时实现了非阻塞的协程 TCP 服务器

## 使用
---
编译 `src` 文件夹源码，使用时包含 `include/netco_api.h`，即可使用 `netco` 协程库接口：

```sh
cd src
make
```
编译src文件夹源码，使用时包含include/netco_api.h，即可使用netco协程库接口
``` cpp
netco::co_go(func)；   //执行一条协程
netco::co_sleep(1000);//当前协程睡眠1000毫秒后继续执行
```
使用时包含include/socket.h，即可使用socket族函数接口
``` cpp
Socket s;                             //创建一个socket类
s.bind(80);                           //绑定端口号80
s.bind("127.0.0.1", 80);              //绑定127.0.0.1:80
s.listen();                           //开始监听
SocketPtr conSock(s.accept());        //accept一个连接
conSock->read(buf, 1024);             //读连接上的数据
conSock->send(buf, 1024);             //往对端写数据
Socket clientSock;
clientSock.connect("127.0.0.1", 80);  //建立一个连接,之后即可如上述read和send
```
使用时包含include/log.h，即可使用日志模块
``` cpp
shared_ptr<Logger>root = NETCO_LOG_NAME("root")                                  //返回名为root的Logger, 不存在则新建
StdoutLoggerAppender::Ptr stdApp = std::make_shared<StdoutLoggerAppender>();     //标准输出
FileLoggerAppender::Ptr fileApp = std::make_shared<FileLoggerAppender>("./log"); //输出到文件./log
root->addAppender(stdApp);  //添加标准输出器到root:Logger
root->addAppender(fileApp); //添加文件输出器到root:Logger

NETCO_LOG_DEBUG(root) << "hello";                   //流式输出DEBUG级别
NETCO_LOG_FMT_DEBUG(mylog, "%s", "hello myargs");   //格式化输出DEBUG级别
NETCO_LOG_XXLEVEL(root) << "hello";                 //流式输出XXLEVEL级别
NETCO_LOG_FMT_XXLEVEL(mylog, "%s", "hello myargs"); //格式化输出XXLEVEL级别
```
## 类图
---
### 日志系统
![log drawio](https://github.com/bobobobn/libNetCo/assets/145976151/6cda3728-0b86-438c-a443-b8eaac114f2a)
### netco协程库
![netco drawio](https://github.com/bobobobn/libNetCo/assets/145976151/c3603d48-9697-4596-aec0-cada6e540b3d)
