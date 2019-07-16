# RISCV_Simulator

RISCV32I指令集模拟器，C++语言实现。

采用五级流水的结构，具体的框架参见[Structure.pdf](https://github.com/LittleQili/RISCV_Simulator/blob/predictor/src/Structure.pdf)

测试数据仅仅采用课程提供的，这些数据保证不会出现Structure Hazard,
所以仅考虑Data Hazard以及Control Hazard。

## Branch : parallel's Notes

我的并行版本的key point：

### 1.并行控制

1）模块与模块之间的开关关系：（不考虑hazard时）

   本条指令的某个模块执行之后可以打开其后面的模块，并且关闭自身模块，这是为了保证一条指令的串行执行。
   
2）处理hazard: 
  
   Control: 停在IF之前，由EX通知恢复执行。
   
   Data： 停在ID之前，由WB通知恢复执行。
   
### 2.寄存器的锁

正常情况下，都在IF阶段判定上锁。

1）某条指令中 rd == rs1||rd == rs2 的解决方案：
   
   将上锁移到ID阶段。若否，那么就会出现在WB阶段应该解锁的时候，判定寄存器仍然被上锁的问题。
   
2）x0寄存器切不可上锁。

### 3.判定data hazard 解除

最终采用了：WB阶段若发现之前的4个阶段全部在这个周期内不能被执行，进行解锁判定操作。


最初写并行版本时，我的思路不是很清晰，导致我中途废弃了一个版本，之后通过（漫长而痛苦的）debug过程发现了一些小问题并解决。

以上。
