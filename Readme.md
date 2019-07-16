# RISCV_Simulator

RISCV32I指令集模拟器，C++语言实现。

采用五级流水的结构，具体的框架参见[Structure.pdf](https://github.com/LittleQili/RISCV_Simulator/blob/predictor/src/Structure.pdf)

## Branch : master's Notes

该架构的思路基本是从我自己的理解的角度出发构思出来的。时间有些久了，一些关键的思路可能难以写清楚。

该架构的key point:

### 1. 指令分类：

未采用每个具体指令一个类的想法，这种分类方式过于繁杂而且运行比较耗时（我的理解是一级一级往下new）

采用“两种”分类标准：
1）使用指令类型（opcode）作为分类标准，可以对应到不同的解析指令的方式。主要用于IF、ID阶段，对于不同的类型执行不同的语句。

2）枚举类型+具体操作类型作为分类标准，对ALU的运算类型分类过程有了极大的简化。

### 2. 其他：

1）立即数在IF阶段被解析。
2）PC未放在寄存器类中（虽然这不符合硬件实际情况）。
