# RISCV_Simulator

RISCV32I指令集模拟器，C++语言实现。

采用五级流水的结构，具体的框架参见[Structure.pdf](https://github.com/LittleQili/RISCV_Simulator/blob/predictor/src/Structure.pdf)

测试数据仅仅采用课程提供的，这些数据保证不会出现Structure Hazard,
所以仅考虑Data Hazard以及Control Hazard。

## Branch : forwarding's Notes

在parallel基础上进行如下改动：

1. 将寄存器的锁全部去掉。

2. 在MA开始前传递非LOAD的rd_value,MA结束之后传递LOAD的rd_value;在WB阶段传递rd_value.

   接收端为ID与EX之间的buffer.
   
3. 并未出现LOAD之后紧接着使用该寄存器的hazard.
