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

/// 能被4整除且又能不能被 100 整除 是闰年 能  直接被400整除也是闰年
/// 做 求余 操作 %,应该不涉及到浮点寄存器，通过反汇编分析没发现诸如 $fx之类的 。
INT8U	IsLeap (INT32U year)
{
	return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}



/// 一个任务 每隔一段时间调用这个函数，来更新系统的时间。
/// 返回TRUE 	表示OSTimeGet 处理完毕， SumTime 小于1秒
/// 返回FALSE 表示SumTime 大于1秒，仍然需要处理
/// 在一些特定的时刻，比如整小时 时间校正， 在某个日起的电压检测 ，有时候会阻塞起来，
/// 所以这里需要单独的一个任务来调用这个函数
BOOLEAN	SystemClock()
{
	static INT32U 	LastTime;
	static INT32U	SumTime;
	INT32U	TempTime;

	TempTime	= OSTimeGet();
	SumTime		+= TempTime - LastTime;
	LastTime	= TempTime;
	if(SumTime >= (8000000/ TIMERDLY)){								/// 1s 时间
		SumTime	-= 8000000/ TIMERDLY;
		second	++;
		VoltageCtrl();								/// 每1秒种进行一次电压采样，是不是一有些 频繁了？？
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
