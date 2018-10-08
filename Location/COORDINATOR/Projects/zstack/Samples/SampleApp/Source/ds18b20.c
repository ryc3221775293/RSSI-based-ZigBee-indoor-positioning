#include"iocc2530.h"
#include"OnBoard.h"
#include "ds18b20.h" 

#define Ds18b20IO P0_7       //温度传感器引脚


void Ds18b20Delay(unsigned int k);
void Ds18b20InputInitial(void);
void Ds18b20OutputInitial(void);
unsigned char Ds18b20Initial(void);
void Ds18b20Write(unsigned char infor);
unsigned char Ds18b20Read(void);


//时钟频率为32M
void Ds18b20Delay(unsigned int k)
{
  while (k--)
  {
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");   
  }
}

void Ds18b20InputInitial(void)//设置端口为输入
{
    P0DIR &= 0x7f;
}

void Ds18b20OutputInitial(void)//设置端口为输出
{
    P0DIR |= 0x80;
}

//ds18b20初始化  初始化成功返回0x00，失败返回0x01
unsigned char Ds18b20Initial(void)
{
    unsigned char Status = 0x00;
    unsigned int CONT_1 = 0;
    unsigned char Flag_1 = 1;
    Ds18b20OutputInitial();
    Ds18b20IO = 1;      //DQ复位
    Ds18b20Delay(260);  //稍做延时
    Ds18b20IO = 0;      //单片机将DQ拉低
    Ds18b20Delay(750);  //精确延时 大于 480us 小于960us
    Ds18b20IO = 1;      //拉高总线
    Ds18b20InputInitial();//设置IO输入
    while((Ds18b20IO != 0)&&(Flag_1 == 1))//等待ds18b20响应，具有防止超时功能
    {                                      //等待约60ms左右
        CONT_1++;
        Ds18b20Delay(10);
        if(CONT_1 > 8000)Flag_1 = 0;
        Status = Ds18b20IO;
    }
    Ds18b20OutputInitial();
    Ds18b20IO = 1;
    Ds18b20Delay(100);
    return Status;       //返回初始化状态
}

void Ds18b20Write(unsigned char infor)
{
    unsigned int i;
    Ds18b20OutputInitial();
    for(i=0;i<8;i++)
    {
        if((infor & 0x01))
        {
            Ds18b20IO = 0;
            Ds18b20Delay(6);
            Ds18b20IO = 1;
            Ds18b20Delay(50);
        }
        else
        {
            Ds18b20IO = 0;
            Ds18b20Delay(50);
            Ds18b20IO = 1;
            Ds18b20Delay(6);
        }
        infor >>= 1;
    }
}

unsigned char Ds18b20Read(void)
{
    unsigned char Value = 0x00;
    unsigned int i;
    Ds18b20OutputInitial();
    Ds18b20IO = 1;
    Ds18b20Delay(10);
    for(i=0;i<8;i++)
    {
        Value >>= 1; 
        Ds18b20OutputInitial();
        Ds18b20IO = 0;// 给脉冲信号
        Ds18b20Delay(3);
        Ds18b20IO = 1;// 给脉冲信号
        Ds18b20Delay(3);
        Ds18b20InputInitial();
        if(Ds18b20IO == 1) Value |= 0x80;
        Ds18b20Delay(15);
    } 
    return Value;
}

//温度读取函数
unsigned char ReadDs18B20(void) 
{
    unsigned char V1,V2;   //定义高低8位 缓冲
    unsigned char temp;    //定义温度缓冲寄存器
    
    Ds18b20Initial();
    Ds18b20Write(0xcc);    // 跳过读序号列号的操作
    Ds18b20Write(0x44);    // 启动温度转换
    
    Ds18b20Initial();
    Ds18b20Write(0xcc);    //跳过读序号列号的操作 
    Ds18b20Write(0xbe);    //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
    
    V1 = Ds18b20Read();    //低位
    V2 = Ds18b20Read();    //高位
    temp = ((V1 >> 4)+((V2 & 0x07)*16)); //转换数据 

    return temp;
}