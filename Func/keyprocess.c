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
		MinorMenu	= 0xff;			/// ���Ϊ 0xff �����л���˵� ʱ����ʾ�ڼ��˵������Ϊ 0 ���ʾ����ʾ
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
	if(MajorMenu == 4){			/// ���ڼ춨ģʽ
		KeyTimeOutCnt ++;
		if(KeyTimeOutCnt >= 2){	/// ���� A1 �˵�  ������2���ӣ����Ը�Ϊ 2Сʱ ��	TODO
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

	if( (OSTimeGet() - LASTKEYPress) > (60 * 8000000/ TIMERDLY) ){		/// ��cpu ���ߵ�ʱ�򣬼�ⰴ����ʱ 1����
		LASTKEYPress	= OSTimeGet();
		//KeyTimeOut();
		//puts("KeyTimeOut\n");
	}
	pmsg = OSMboxPend(pGMboxKey, 60 * 8000000/ TIMERDLY ,&err);		/// ���cpu �����ߣ���ⰴ����ʱ
	if (err == OS_TIMEOUT ){						/// 1 ����֮��ʱ�ˣ��û�û�а���
		//KeyTimeOut();
	}else{											/// ����������
		if( *(INT8U *)pmsg  & (1<<3) ){				//// ���� �ɿ�����������
			if( (OSTimeGet() - KeyTime)  > (3 * 8000000/ TIMERDLY)){	/// �������ʱ�䳬�� 3s
					//LcdPoweron();
					puts("lo\n");
					LongKey();
			}else{										/// �������ʱ����2s֮�ڣ�������Ϊ�Ƕ̰���
				puts("short\n");
				ShortKey();
				//OSSemPost(pGSemKeyToInfra);			/// ��������ͨ��
			}
			LASTKEYPress	= OSTimeGet();
			KeyTimeOutCnt	= 0;
		}else {										/// ��������
			KeyTime	= OSTimeGet ();
		}
	}
	return KeyTime;
}


