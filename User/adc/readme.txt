在项目文件夹的User里创建的一个驱动文件夹.

用来存放adc的驱动库.

里面是板级支持包的c文件和头文件:
bsd_adc.c  (bsd: board support package,板级支持包)
	用来配置和初始化adc. 选择通道, 设置时钟.
	
bsd_adc.h
	声明bsd_adc.c中初始化和读取adc的函数.











