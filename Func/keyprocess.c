#include "includes.h"

void ShortKey(void)
{
	MinorMenu ++;
	if( ((MajorMenu == 1) || (MajorMenu == 2)) && (MinorMenu == 6) ){
		MinorMenu	= 0;
	}else if( (MajorMenu == 3) && (MinorMenu == 54) ){
		MinorMenu	= 0;
	}else if( (MajorMenu == 4) && (MinorMenu == 7) ){
		MinorMenu	= 0;
	}
}

void	LongKey(void)
{
	if( (MajorMenu == 1) && (MinorMenu == 5) ){
		MajorMenu	= 4;
		MinorMenu	= 0;
	}else {
		if(MajorMenu == 4){
			SampingMode	= USER;
		}
		MajorMenu ++;
		MinorMenu	= 0xff;			/// 这个为 0xff 代表切换大菜单 时候显示第几菜单，如果为 0 则表示不显示
		if(MajorMenu >= 4){
			MajorMenu	= 1;
		}
	}
	if(MajorMenu == 4){
		SampingMode	= DOCIM;
	}
}

void	KeyTimeOut(void)
{
	if(MajorMenu == 4){			/// 处于检定模式
		KeyTimeOutCnt ++;
		if(KeyTimeOutCnt >= 2){	/// 返回 A1 菜单  这里是2分钟，可以改为 2小时 。	TODO
			KeyTimeOutCnt = 0;
			MajorMenu	= 1;
			MinorMenu	= 0;
			SampingMode	= USER;
		}
	}else {
		MajorMenu	= 1;
		MinorMenu	= 0;
	}
}

INT32U KeyProc(OS_EVENT *pGMboxKey,INT32U	KeyTime)
{
	INT8U	err;
	void	*pmsg;

	if( (OSTimeGet() - LASTKEYPress) > (60 * 8000000/ TIMERDLY) ){		/// 当cpu 休眠的时候，检测按键超时 1分钟
		LASTKEYPress	= OSTimeGet();
		//KeyTimeOut();
		//puts("KeyTimeOut\n");
	}
	pmsg = OSMboxPend(pGMboxKey, 60 * 8000000/ TIMERDLY ,&err);		/// 如果cpu 不休眠，检测按键超时
	if (err == OS_TIMEOUT ){						/// 1 分钟之后超时了，用户没有按键
		//KeyTimeOut();
	}else{											/// 处理按键过程
		if( *(INT8U *)pmsg  & (1<<3) ){				//// 按键 松开！！！！！
			if( (OSTimeGet() - KeyTime)  > (3 * 8000000/ TIMERDLY)){	/// 按键间隔时间超过 3s
					//LcdPoweron();
					puts("lo\n");
					LongKey();
			}else{										/// 按键间隔时间在2s之内，可以认为是短按键
				puts("short\n");
				ShortKey();
				//OSSemPost(pGSemKeyToInfra);			/// 启动红外通信
			}
			LASTKEYPress	= OSTimeGet();
			KeyTimeOutCnt	= 0;
		}else {										/// 按键按下
			KeyTime	= OSTimeGet ();
		}
	}
	return KeyTime;
}


