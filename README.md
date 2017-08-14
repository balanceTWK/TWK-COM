
# TWK-COM
串口助手

从以上列表中下载这个文件

小灰灰串口助手.exe 

就可直接使用.

stm32下位机发送波形图数据例子:

int main(void)

{	

  double i;
  
	float val;
	
	while(1)
	{
	
        i+=0.5;
   
        val=20*sin(i);
   
	printf("数据:#-_-#%.3f#^_^#\r\n\r\n",val);//要发送的浮点数据前要加表情 #-_-# 结尾加表情 #^_^# 
	 
        delay_ms(300);
   
	}	 
	
}

