//#include "key.h"
//#include "bsp_u8g2.h"
//#include "tim.h"
////int KeyState(int button)

////����������
////���ذ���ֵ
////mode:0,��֧��������;1,֧��������;
////0��û���κΰ�������
////1��WKUP���� WK_UP
////ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
////ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP!!
//int KEY_Scan(u8 mode)
//{
//    static u8 key_up=1;     //�����ɿ���־
//    if(mode==1)key_up=1;    //֧������
//	  if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
//    {
//        HAL_Delay(5);
//        key_up=0;
//        if(KEY1==0)       return KEY1_PRES;
//        else if(KEY2==0)  return KEY2_PRES;
//        else if(KEY3==0)  return KEY3_PRES;
//        else if(KEY4==0)  return KEY4_PRES;          
//    }else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1;
//    return 0;   //�ް�������
//}


//char click(void)
//{
//	static char flag_key=1;//�������ɿ���־
//	if(flag_key&&HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==0)
//	{
//	  flag_key=0;
//		return 1;	// ��������
//	}
//	else if(1==HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin))			flag_key=1;
//	return 0;//�ް�������
//}

//void showscreen()
//{
//		if(KEY_Scan(1)==1)
//		{
//				g_dcV+=2;
//				if(g_dcV>=49) g_dcV=49;
//        u8g2_ClearBuffer(&u8g2);
//		    u8g2_Show(&u8g2,1);
//		}
//		if(KEY_Scan(1)==2)
//		{
//				g_dcV-=2;
//				if(g_dcV<=12) g_dcV=12;
//			u8g2_ClearBuffer(&u8g2);
//		    u8g2_Show(&u8g2,2);
//				
//		}
//		if(KEY_Scan(1)==3)
//		{
//				x_dcV+=2;
//			if(x_dcV>=20) x_dcV=20;
//			u8g2_ClearBuffer(&u8g2);
//		    u8g2_Show(&u8g2,3);
//			
//		}
//		if(KEY_Scan(1)==4)
//		{
//				x_dcV-=2;
//			  if(x_dcV<=5) x_dcV=5;
//			u8g2_ClearBuffer(&u8g2);
//		    u8g2_Show(&u8g2,4);
//		}
//		u8g2_SendBuffer(&u8g2);
////		HAL_Delay(200);
//}

//void key_update()
//{
//	if(KEY_Scan(1)==1)
//	{
//		g_dcV+=2;
//		if(g_dcV>=49) g_dcV=49;
//	}
//	if(KEY_Scan(1)==2)
//	{
//		g_dcV-=2;
//		if(g_dcV<=12) g_dcV=12;		
//	}
//	if(KEY_Scan(1)==3)
//	{
//		x_dcV+=2;
//		if(x_dcV>=20) x_dcV=20;
//		
//	}
//	if(KEY_Scan(1)==4)
//	{
//		x_dcV-=2;
//		if(x_dcV<=5) x_dcV=5;
//	}
//}



