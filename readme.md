该模块使用`stm32f103`系列实现高速交变信号的阈值报警.

* 使用`ledG`报警.
* 使用`adc1`无中断采集电压信号.
* 使用`DMA1`写入.
* 逻辑判断用软件实现. 
* 使用`usart`提供信息打印.
* 使用`TIM6`进行led生命ctr递减, 实现报警常亮.
* 使用`TIM7`控制程序生命周期,可自行定义占空比A/R, 节省能耗.

可以在20kHz~100kHz带宽下监测交变信号0~3.3v阈值. 精度为12bits.


azazel,2025/4/2
