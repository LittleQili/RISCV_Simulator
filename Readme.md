# RISCV_Simulator

RISCV32I指令集模拟器，C++语言实现。

采用五级流水的结构，具体的框架参见[Structure.pdf](https://github.com/LittleQili/RISCV_Simulator/blob/predictor/src/Structure.pdf)

## Branch : standard's Notes

在master的基础上将数据存储的缓冲区拆成四份，为了并行而作此改动。

参考资料：[recommended_Structure.png](https://github.com/LittleQili/RISCV_Simulator/blob/standard/src/recommended_Structure.png)
