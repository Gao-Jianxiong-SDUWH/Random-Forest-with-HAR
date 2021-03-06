#include "MPU6050.H" 
uint8   dis[6];					//显示数字(-511至512)的字符数组
uint8   up_zeros[2];					
uint8   belowzeros[4];
int	dis_data;					//变量
//********************************************************************************
//小数部分转字符串
//********************************************************************************
void lcd_printf_below_zeros(uint8 *s,int temp_data)
{	

//	*s =temp_data/10000+0x30;
//	temp_data=temp_data%10000;     //取余运算

	*s =temp_data/1000+0x30;
	temp_data=temp_data%1000;     //取余运算

	*++s =temp_data/100+0x30;
	temp_data=temp_data%100;     //取余运算
	*++s =temp_data/10+0x30;
	temp_data=temp_data%10;      //取余运算
	*++s =temp_data+0x30; 	
}
//********************************************************************************
//整数部分转字符串
//********************************************************************************
void lcd_printf(uint8 *s,int temp_data)
{
//	if(temp_data<0)
//	{
//		temp_data=-temp_data;
//		*s='-';
//	}
//	else *s='+';
	
//	*++s =temp_data/10000+0x30;
//	temp_data=temp_data%10000;     //取余运算

//	*++s =temp_data/1000+0x30;
//	temp_data=temp_data%1000;     //取余运算

//	*s =temp_data/100+0x30;
//	temp_data=temp_data%100;     //取余运算
	*s =temp_data/10+0x30;
	temp_data=temp_data%10;      //取余运算
	*++s =temp_data+0x30; 	
}
//*************************************************************************************************
//I2C起始信号
//*************************************************************************************************
void I2C_Start()
{
    SDA = 1;                    //拉高数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 0;                    //产生下降沿
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
}
//*************************************************************************************************
//I2C停止信号
//*************************************************************************************************
void I2C_Stop()
{
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 1;                    //产生上升沿
    Delay5us();                 //延时
}
//**************************************************************************************************
//I2C发送应答信号
//入口参数:ack (0:ACK 1:NAK)
//**************************************************************************************************
void I2C_SendACK(bit ack)
{
    SDA = ack;                  //写应答信号
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
}
//****************************************************************************************************
//I2C接收应答信号
//****************************************************************************************************
bit I2C_RecvACK()
{
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    CY = SDA;                   //读应答信号
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
    return CY;
}
//*****************************************************************************************************
//向I2C总线发送一个字节数据
//*****************************************************************************************************
void I2C_SendByte(uint8 dat)
{
    uint8  i;
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        SDA = CY;               //送数据口
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    I2C_RecvACK();
}
//*****************************************************************************************************
//从I2C总线接收一个字节数据
//******************************************************************************************************
uint8 I2C_RecvByte()
{
    uint8  i;
    uint8  dat = 0;
    SDA = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        dat |= SDA;             //读数据               
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    return dat;
}
//*****************************************************************************************************
//向I2C设备写入一个字节数据
//*****************************************************************************************************
void Single_WriteI2C(uint8 REG_Address,uint8 REG_data)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
    I2C_SendByte(REG_Address);    //内部寄存器地址，
    I2C_SendByte(REG_data);       //内部寄存器数据，
    I2C_Stop();                   //发送停止信号
}
//*******************************************************************************************************
//从I2C设备读取一个字节数据
//*******************************************************************************************************
uint8 Single_ReadI2C(uint8 REG_Address)
{
	uint8  REG_data;
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress);    //发送设备地址+写信号
	I2C_SendByte(REG_Address);     //发送存储单元地址，从0开始	
	I2C_Start();                   //起始信号
	I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号
	REG_data=I2C_RecvByte();       //读出寄存器数据
	I2C_SendACK(1);                //接收应答信号
	I2C_Stop();                    //停止信号
	return REG_data;
}
//******************************************************************************************************
//初始化MPU6050
//******************************************************************************************************
void InitMPU6050(void)
{
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_WriteI2C(SMPLRT_DIV, 0x07);
	Single_WriteI2C(CONFIG, 0x04);
	Single_WriteI2C(GYRO_CONFIG, 0x18);
	Single_WriteI2C(ACCEL_CONFIG, 0x1f);//0x0a//4g 0.63hz
}
//******************************************************************************************************
//合成数据
//******************************************************************************************************
int GetData(uint8 REG_Address)
{
	uint8  H,L;
	H=Single_ReadI2C(REG_Address);
	L=Single_ReadI2C(REG_Address+1);
	return ((H<<8)+L);   //合成数据
}
//******************************************************************************************************
//超级终端（串口调试助手）上显示10位数据
//******************************************************************************************************
// 大于0部分
void Display_up_zeros(int value)
{ 
	uint8  i;
	lcd_printf(up_zeros, value);			//转换数据显示
	for(i=0;i<2;i++)
	{
    U1SendData(up_zeros[i]);
  }
}
// 小于0部分
void Display_below_zeros(int value)
{ 
	uint8  i;
	lcd_printf_below_zeros(belowzeros, value);			//转换数据显示
	for(i=0;i<4;i++)
	{
    U1SendData(belowzeros[i]);
  }
}
/*********************************END FILE*************************************/
void lcd_printf2(uint8 *s,int temp_data)
{
	if(temp_data<0)
	{
		temp_data=-temp_data;
		*s='-';
	}
	else *s=' ';

	*++s =temp_data/10000+0x30;
	temp_data=temp_data%10000;     //取余运算

	*++s =temp_data/1000+0x30;
	temp_data=temp_data%1000;     //取余运算

	*++s =temp_data/100+0x30;
	temp_data=temp_data%100;     //取余运算
	*++s =temp_data/10+0x30;
	temp_data=temp_data%10;      //取余运算
	*++s =temp_data+0x30; 	
}
void Display10BitData(int value)
{ 
	uint8 i;
	lcd_printf2(dis, value);			//转换数据显示
	for(i=0;i<6;i++)
	{
    U1SendData(dis[i]);
  }
}

