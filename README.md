# willingOS
williingOS

##### willing： 愿意，自愿的自发的，同时带有意志毅力和干劲，最重要的是希望和喜爱。本是要用joy、play、interest、education through entertainment等词表达快乐的，寓教于乐之意的。但都觉着不妥当。当看到willing一词，顿觉惊艳，于是就取名为willingOS。    
##### 希望willingOS以及之后搭载willingOS的设备，都能提供给人们足够的精神价值，不止步于解决生产和生活问题本身。人生到最后活得只是一个体验，我们解决了多少问题，都不如体验了多少悲欢离合来得实在，记得深沉，当然希望所有人体会到更多乐趣而非悲伤。（O是一个圆圈，可以象征圆满，恭祝圆满）



##  timer设计
```c
  // 全局：  
    uint32_t tickCount; // 用于定时器周期计数
                                   // 最大定时时间：  (1 / SYS_TICK_RATE)  * 0xFFFFFFFF / 60 / 60 / 24 小时
                                   // 当SYS_TICK_RATE = 1000时，最大可以定时49.71026961805556天
                                   // 当SYS_TICK_RATE = 100时，最大可以定时497.1026961805556天
    uint8_t tickSession; // 计时器的会话，取值只有0和1，用于处理tickCount溢出问题，当delayCount=tickCount+任务的delay大于uint32_t的最大值时，比如任务的delayCount会溢出，这时候应该记录delayCount的tickSession为当前！tickSession，这样就不会在本次tickCount超时之前调用溢出的delayCount


   // iterm
   uint32_t expireAt; // 超时时间
   uint8_t tickSession; // 计时器会话
   uint8_t mod; // 0单次定时器，1循环定时器，循环定时器在触发函数调用时，同时令expireAt = tickCount + interval
   uint32_t interval; // 定时周期
   func   taskFunc； // 任务执行函数


    //itermList
    // 根据tickSession排序，然后根据expireAt排序

    // 在OSStart的时候插入timer任务，在timer任务中去进行timer处理
``` 


## deley设计
```c
   // 在taskControlBlock中添加expireAt和tickSession，用于计算超时
   // 分就绪队列，等待队列
   // 每个tickCount去检查delay

```

## 代码整理
```c
  // 将delay相关函数，timer相关函数整理到相应的文件和文件夹中去
  // 将宏定义分开成文件，不同用处的宏定义在自己的文件中
```


## 问题：
 - 1.当前delay的task切换是存在问题的，在delay的时候从ready中删除了currentPCB，并让currentPCB指向了下一个任务，但是其实现在上下文中，cpu还在执行上一个currentPCB的内容，切换的时候会将现场保存到currentTCB->next->stack中，导致问题。考虑一下怎么解决这个问题？比如加个切换变量？
 加一个currentPCBSleeped变量，在switch函数中去

  - sleep的时候从ready中删除，插入suspend，switch的时候按照优先级查找，不一定接着上一个往下走，这样就不会产生问题

  - 任务的优先级提升问题--当任务超过n个tick没有被调用，应该提升优先级，避免任务永远不被执行，每一个提升周期提升一级，直到被执行之后降回去

  - 通过优先级来保证硬delay的准确性，当优先级最高的任务进行硬delay的时候不会被打断，所以是准确的，但是如果优先级不够高，会有被打断的风险。当然，这要求~~~（对不起突然搞忘了）switch在判断要不要切换的时候，进行优先级判断，所以其实延时还是不准的