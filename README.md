"# STM32+ MQTT+eps6288(串口)方便移植gprs使用"
使用刷好固件的wifi模块(默认自动连接网络)，stm32内部解析MQTT协议，仅仅使用串口中断方式与wifi模块连接，这样移植到gprs上回非常方便
（只需要AT命令设置好透传模式）就可以发送MQTT格式的数据了。
//硬件改变世界，代码创造未来 --------jianghu_xiaofan@foxmail.com
