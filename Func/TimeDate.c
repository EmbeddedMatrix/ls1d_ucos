#include "includes.h"



void	InitTimeDate(void)
{
	year	= 14;
	month	= 6;
	day		= 19;
	hour	= 18;
	minute	= 30;
	second	= 00;
}

/// �ܱ�4���������ܲ��ܱ� 100 ���� ������ ��  ֱ�ӱ�400����Ҳ������
/// �� ���� ���� %,Ӧ�ò��漰������Ĵ�����ͨ����������û�������� $fx֮��� ��
INT8U	IsLeap (INT32U year)
{
	return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}



/// һ������ ÿ��һ��ʱ��������������������ϵͳ��ʱ�䡣
/// ����TRUE 	��ʾOSTimeGet ������ϣ� SumTime С��1��
/// ����FALSE ��ʾSumTime ����1�룬��Ȼ��Ҫ����
/// ��һЩ�ض���ʱ�̣�������Сʱ ʱ��У���� ��ĳ������ĵ�ѹ��� ����ʱ�������������
/// ����������Ҫ������һ�������������������
BOOLEAN	SystemClock()
{
	static INT32U 	LastTime;
	static INT32U	SumTime;
	INT32U	TempTime;

	TempTime	= OSTimeGet();
	SumTime		+= TempTime - LastTime;
	LastTime	= TempTime;
	if(SumTime >= (8000000/ TIMERDLY)){								/// 1s ʱ��
		SumTime	-= 8000000/ TIMERDLY;
		second	++;
		VoltageCtrl();								/// ÿ1���ֽ���һ�ε�ѹ�������ǲ���һ��Щ Ƶ���ˣ���
		if(second >= 60){
			second -= 60;
			minute	++;
			//if(minute == 1)
			//	MonthlySave();
			if(minute >= 60){
				minute -= 60;
				hour	++;
				if(hour >= 24){
					hour -= 24;
					day	++;
					if(day > (MonthDays[month] + ((month == 2) ? IsLeap(year) : 0)) ){
						day	= 1;
						month ++;
						if(month >= 13){
							month -= 12;
							year ++;
						}
					}
				}
			}
		}
	}
	return (SumTime < (8000000/ TIMERDLY) );
}
