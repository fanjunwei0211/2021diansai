//#include "key.h"
//#include "bsp_u8g2.h"
//#include "tim.h"
////int KeyState(int button)

////按键处理函数
////返回按键值
////mode:0,不支持连续按;1,支持连续按;
////0，没有任何按键按下
////1，WKUP按下 WK_UP
////注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
////注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
//int KEY_Scan(u8 mode)
//{
//    static u8 key_up=1;     //按键松开标志
//    if(mode==1)key_up=1;    //支持连按
//	  if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
//    {
//        HAL_Delay(5);
//        key_up=0;
//        if(KEY1==0)       return KEY1_PRES;
//        else if(KEY2==0)  return KEY2_PRES;
//        else if(KEY3==0)  return KEY3_PRES;
//        else if(KEY4==0)  return KEY4_PRES;          
//    }else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1;
//    return 0;   //无按键按下
//}


//char click(void)
//{
//	static char flag_key=1;//按键按松开标志
//	if(flag_key&&HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==0)
//	{
//	  flag_key=0;
//		return 1;	// 按键按下
//	}
//	else if(1==HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin))			flag_key=1;
//	return 0;//无按键按下
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



