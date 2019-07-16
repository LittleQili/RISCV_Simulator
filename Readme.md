# RISCV_Simulator

RISCV32I指令集模拟器，C++语言实现。
采用五级流水的结构，具体的框架参见[Structure.pdf](https://github.com/LittleQili/RISCV_Simulator/blob/predictor/src/Structure.pdf)

测试数据仅仅采用课程提供的，这些数据保证不会出现Structure Hazard,
所以仅考虑Data Hazard以及Control Hazard。

本repo的五个branch分别实现的功能如下：

| Branch | Notes |
| 
| [master](https://github.com/LittleQili/RISCV_Simulator/tree/master) | 串行。仅有一个"buffer"，运行效率较高，但不可改成并行，封装效果差 |
| [standard](https://github.com/LittleQili/RISCV_Simulator/tree/standard) | 串行。将buffer拆成4块，分别存储需要的数据 |
| [parallel](https://github.com/LittleQili/RISCV_Simulator/tree/parallel) | 并行，给寄存器上锁(IF/ID)，在遇到hazard之后停止于ID之前(Data)IF之前(Control) |
| [forwarding](https://github.com/LittleQili/RISCV_Simulator/tree/forwarding) | 并行，在MA结束后(Load)/MA开始前(除Load)，WB阶段进行forwarding,无特殊情况 |
| [predictor](https://github.com/LittleQili/RISCV_Simulator/tree/predictor) | 并行，根据本条指令的pc进行（过于naive的）分支预测(意即以本条指令的pc为key值判断是否跳转) |

分支预测的结果（仅摘取部分测试文件）：
basicopt1 : 113912/155139 Rate: 73.4258%
bulgarian : 60951/71493 Rate: 85.2545%
expr : 86/111 Rate: 77.4775%
hanoi : 12823/17457 Rate: 73.4548%
magic : 45047/67869 Rate: 66.3735%
pi : 33068321/39956380 Rate: 82.7611%
qsort: 172288/200045 Rate: 86.1246%
queens : 53052/77116 Rate: 68.7951%
superloop : 251660/435027 Rate: 57.8493%
tak : 15160/60639 Rate: 25.0004%