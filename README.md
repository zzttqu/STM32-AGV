# 参数记录
1. HBS54上边的细分调整为1000，也就是1000个脉冲转一圈。
2. 编码器一圈1000个脉冲。
3. 轮子周长150mm，直径471.24mm。一个脉冲是0.47mm
4. z轴旋转的单位是rad/s
5. stm32上传的速度单位为mm/s
6. 车子行进方向为x轴正方向，右手系，左手方向为y轴正方向
   
# 注意点
1. unsigned char是八位一字节，只会截取传入变量的低八位，所以高八位要右移放入
2. 2号定时器做编码器有问题，需要选择为swd调试，不能用jtag，jtag有冲突的



# IO手册
## 普通输出引脚
1. A电机的脉冲引脚为A4，正反转引脚为A5
2. B电机的脉冲引脚为C4，正反转引脚为C5
3. C电机的脉冲引脚为B0，正反转引脚为B1
4. D电机的脉冲引脚为B10，正反转引脚为B11
## 定时器输入引脚
5. A电机编码器tim2，PA15连接A相，PB3连接B相。
6. B电机编码器tim3，PA6连接A相，PA7连接B相。
7. C电机编码器tim4，PB6连接A相，PB7连接B相。
8. D电机编码器tim5，PA0连接A相，PA1连接B相。

# 串口协议
1. 接收的如果以A开头为配置，