# 每个程序员都应该知道的延迟数字

## 参考链接

1. https://gist.github.com/jboner/2841832

2. https://colin-scott.github.io/personal_website/research/interactive_latency.html

3. https://static.googleusercontent.com/media/sre.google/zh-CN//static/pdf/rule-of-thumb-latency-numbers-letter.pdf

链接 1 是 Jeff Dean 总结的表格，不过有很多讨论可以提供表格之外的更多见解，值得阅读，但是数据很老旧（2012 年）, 链接 2 有按年份的交互式的数字，最新的年份是 2020 年，2020 年的数据与链接 3 中的数字相同，本文主要根据链接 3 进行整理。

## 延迟数字表格

### 英文原版

| Operation                                      | Time in ns         | Time in ms (1ms = 1,000,000 ns) |
|------------------------------------------------|--------------------|---------------------------------|
| L1 cache reference                             | 1                  | 0.000001                        |
| Branch misprediction                           | 3                  | 0.000003                        |
| L2 cache reference                             | 4                  | 0.000004                        |
| Mutex lock/unlock                              | 17                 | 0.000017                        |
| Main memory reference                          | 100                | 0.0001                          |
| Compress 1 kB with Zippy                       | 2,000              | 0.002                           |
| Read 1 MB sequentially from memory             | 10,000             | 0.01                            |
| Send 2 kB over 10 Gbps network                 | 1,600              | 0.0016                          |
| SSD 4kB Random Read                            | 20,000             | 0.02                            |
| Read 1 MB sequentially from SSD                | 1,000,000          | 1                               |
| Round trip within same datacenter              | 500,000            | 0.5                             |
| Read 1 MB sequentially from disk               | 5,000,000          | 5                               |
| Read 1 MB sequentially from 1Gbps network      | 10,000,000         | 10                              |
| Disk seek                                      | 10,000,000         | 10                              |
| TCP packet round trip between continents       | 150,000,000        | 150                             |

Therefore, it is possible to read:
* sequentially from HDD at a rate of ~200MB per second
* sequentially from SSD at a rate of ~1 GB per second
* sequentially from main memory at a rate of ~100GB per second (burst rate)
* sequentially from 10Gbps Ethernet at a rate of ~1000MB per second

### 中文翻译

| 操作                                           | 纳秒时间 (ns)      | 毫秒时间 (1ms = 1,000,000 ns)   |
|------------------------------------------------|--------------------|---------------------------------|
| L1 缓存引用                                    | 1                  | 0.000001                        |
| 分支预测失败                                  | 3                  | 0.000003                        |
| L2 缓存引用                                    | 4                  | 0.000004                        |
| 互斥锁加锁/解锁                                | 17                 | 0.000017                        |
| 主存引用                                      | 100                | 0.0001                          |
| 使用 Zippy 压缩 1 kB 数据                      | 2,000              | 0.002                           |
| 从内存中顺序读取 1 MB                         | 10,000             | 0.01                            |
| 通过 10 Gbps 网络发送 2 kB 数据                | 1,600              | 0.0016                          |
| SSD 4kB 随机读取                              | 20,000             | 0.02                            |
| 从 SSD 顺序读取 1 MB                          | 1,000,000          | 1                               |
| 数据中心内往返延迟                            | 500,000            | 0.5                             |
| 从磁盘顺序读取 1 MB                           | 5,000,000          | 5                               |
| 从 1Gbps 网络顺序读取 1 MB                    | 10,000,000         | 10                              |
| 磁盘寻道                                      | 10,000,000         | 10                              |
| 跨大洲的 TCP 数据包往返时间                    | 150,000,000        | 150                             |

因此，可以进行以下读取操作：

* 从 HDD 顺序读取，速率约为每秒 200MB
* 从 SSD 顺序读取，速率约为每秒 1GB
* 从主内存顺序读取，速率约为每秒 100GB（突发速率）
* 从 10Gbps 以太网顺序读取，速率约为每秒 1000MB

## 记忆方法

首先将表格中最上面的部分也就是 ns 级别的部分进行记忆，这些数字都比较小，比较容易记忆。

分别是

| 操作 | 时间（ns） |
|------|------|
| L1 缓存引用 | 1 |
| 分支预测失败 | 3 |
| L2 缓存引用 | 4 |
| 互斥锁加锁/解锁 | 17 |
| 主存引用 | 100 |

然后对于超过 ns 级别的又将其分为主存访问、磁盘访问、网络访问三类，其中

1. 主存访问

可以记忆顺序读取 1MB 数据的时间为主存引用的 100 倍，即 10μs，这个读取速率是相当快的，内存带宽为 100GB/s。

| 操作 | 时间 |
|------|------|
| 主存引用 | 100ns |
| 内存顺序读 1MB | 10μs |

2. 磁盘访问

记住 HDD 和 SSD 的顺序读取速率，分别是 200MB/s 和 1GB/s，日常生活中是会接触到这两个数字的，且比较整齐，方便记忆，顺便可以记住一个 5 倍的速率差距，那么对应顺序读取 1MB 所需的时间就分别为 5ms 和 1ms。

另外 HDD 有个磁盘寻道的时间，一般认为磁盘寻道是个物理操作，速度相当慢，其慢于顺序读取 1MB 的数据就理所当然了，这里记住一个 2 倍即可知道其时间为 10ms。

SSD 4kB 随机读写的速度一般认为和内存顺序读取 1MB 的速度处于同一数量级，但是慢一倍，也就是 20μs。

| 操作 | 时间 |
|------|------|
| SSD 4kB 随机读 | 20μs |
| SSD 顺序读 1MB | 1ms |
| HDD 顺序读 1MB | 5ms |
| HDD 磁盘寻道 | 10ms |

3. 网络访问

网络访问的速度都可以进行计算，比如 10Gbps 的网络，其速率大约为 1250MB/s，那么发送 2kB 的数据所需的时间为 0.002/1250 = 1.6x10^-6 = 1.6μs。

而 1Gbps 的网络，其速率大约为 100MB/s，那么顺序读取 1MB 的数据所需的时间为 10ms。

而数据中心内的往返延迟一般认为在 0.5ms 左右，跨大洲的往返延迟一般认为在 150ms 左右，这两个数字相对来说比较好记忆。

| 操作 | 时间 |
|------|------|
| 10Gbps 网络发送 2kB | 1.6μs |
| 1Gbps 网络读取 1MB | 10ms |
| 数据中心内往返 | 0.5ms |
| 跨大洲往返 | 150ms |

最后还剩一个 Zippy 压缩 1kB 数据的时间，这个数字比较小，一般认为在 2μs 左右。

| 操作 | 时间 |
|------|------|
| Zippy 压缩 1kB | 2μs |

## 注意事项

以上很多估算都并不准确，这些数字只是为了提供大致的数量级方便封底估算，不可以生搬硬套，而要掌握这套数字的大概数量级，尤其是相对的数量级差距。