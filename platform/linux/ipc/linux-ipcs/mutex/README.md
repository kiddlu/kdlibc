# 使用方法
```
cd mutex/
make
./_main
make good
./_main

```

对比可以发现，不使用mutex同步的话，两个线程的打印会交叉

# 描述
线程之间是不需要通信的，因为它们共享同一个地址空间，只要使用全局变量既可实现通信，但是为了保证数据的一致性(一个变量在被改写时不可以被其它线程访问)，需要使用同步的机制。

mutex是mutual exclusive的缩写，即互相排斥，因此也叫做互斥量。一般用于线程间同步，通过特殊的设置，也可以用于进程间同步。