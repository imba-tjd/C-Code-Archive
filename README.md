# C Code Archives

一些C语言代码存档，希望对初学者有帮助。

其实我一直对“写正确的代码”有追求，但是C语言写正确的代码实在是太难了：

* for循环的下标要用size_t，不用int；单独看起来没什么，但和有符号数混用就有可能坑
* 容器类要么设计成弱类型(void*)，要么就要全写成宏定义。typedef根本做不到同时使用两个不同类型的容器，只能用宏中的##硬模拟重载
* 错误处理困难，可能出错的地方太多了，比如内存分配失败，参数为null或范围不对。难道所有可能发生错误的地方都返回error_no，用指针取结果，而且往上传染？若是设置errno全局变量，那返回什么呢？没法知道泛型的默认值啊
* 字符串即使分配到堆上，也会要么仍然长度不够，要么空闲浪费太多；这也导致处理用户输入困难
* Windows下处理utf8困难

所以我觉得还是不要想着用C写正常的程序了。别用什么无符号数、void*、int32_t、restrict，就用int和char。不要深入学C，尽早开始学习别的语言。

本仓库尽量保证的：

* 格式化代码，大括号不换行
* 不变的字符串及数组内容用const修饰
* 无参函数不省略void（虽然C23可以省了）
* 区间左闭右开
* 输入输出与逻辑计算分离
* 完全不介意用递归，不过还是尽量尾递归
* CI Lint
