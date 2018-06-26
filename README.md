# MiniSQL

数据库系统DB小组期末工程。

设计并实现一个精简型单用户SQL引擎(DBMS)MiniSQL

允许用户通过字符界面输入SQL语句实现

- 表的`建立`/`删除`
- 索引的`建立`/`删除`
- 表记录的`插入`/`删除`/`查找`

具体要求见大纲

----

### 文件架构

- **src/**目录是各个模块的源代码，负责各个模块的只需要在这里附上源代码

  - **Interpreter**（小孟与小孟的小伙伴）
    - [ ] 完成情况
  - **CatalogManager**（小孟与小孟的小伙伴）
    - [ ] 完成情况
  - **RecordManager**（翔）
    - [ ] 完成情况
  - **IndexManager**（kk）
    - [x] 完成情况
  - **BufferManager**（阿洪）
    - [ ] 完成情况

  每一个模块的功能则从大纲中提取，完成则在此处勾上。在**src/**里的**README.md**里面提供借口说明以及其他比较复杂的说明。

- **doc/**目录是各个模块的说明，包括实现的功能，以及模块报告内容

- **resource/**目录则是MiniSQL大程的大纲以及测试集

---

### 注释风格（选）

在文件头

```c++
/*
 * File: demo.h
 * Version: 1.0
 * Author: kk
 * Modified Date: Wed May 23 16:55:16 DST 2018
 * -------------------------------------------
 * 文件功能
 */
```

在函数头

```c++
/*
 * Function: 函数名
 * Argument: 参数介绍
 * Return: 返回值作用
 * Usage: 如何调用该函数
 * --------------------
 * 函数功能的简要介绍
 */

// 示例
/*
 * Function: sum
 * Argument: a(int) 第一个加数
             b(int) 第二个加数
 * Return: sum(int) 返回参数的和
 * Usage: s = sum(a, b);
 * ---------------------
 * 将输入参数求和，并返回结果。
 */
int sum(int a, int b);
```

在函数内部

对于比较复杂的操作需要注释的话，就在那个操作上面写注释吧，不过如果注释特别短就在后面接着就好。

如果循环或者条件嵌套比较厉害，就在每个循环结束的位置加以注释标识。

```c++
for (int i = 0; i < n; ++i)
{
    for (int j = i; j < m; ++j)
    {
        if (i == j)
        {
            std::cout << "DB小组" << std::endl;
        }// end if-1
    }// end loop-2
}// end loop-1
```

---

