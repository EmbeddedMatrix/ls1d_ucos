#include "includes.h"


_u32 func_once_time(TDCINFO *TDC);
_u32 func_once_thsens(TDCINFO *TDC);
_u32 check_res(TDCINFO *TDC);
_u32 calibrate(_u32 start, _u32 stop, TDCINFO *TDC);
_fp32 res_cal(_u32 result, TDCINFO *TDC);
_fp32	ftime_caculate(_fp32 *res, _u32 ptr, _u32 stop_num);
void ptime_caculate(_fp32 *res, _u32 ptr, _fp32 *time, TDCINFO *TDC);
_u32 func_cap_detection(TDCINFO *TDC);


void	DataProcess(INT8U SampMode);

_u32 calibrate(_u32 start, _u32 stop, TDCINFO *TDC)
{
	_u32 cal = 1;
	_u32 cc1, cc2, v1, v2;

	switch(TDC->tdc_mode){
	case S_TDC:  //stdc
		cc1 = (start/0x10000);
		v1 = (start%0x10000);
		cc2 = (stop/0x10000);
		v2 = (stop%0x10000);
		if(TDC->tdc_opt & 0x1)    //dres, "1" -double
			cal = ((v2>>7)*126 + v2%128) - ((v1>>7)*126 + v1%128);
		else
			cal = ((v2>>6)*63 + v2%64) - ((v1>>6)*63 + v1%64);
		break;
	case R_TDC:  //rtdc
		break;
	case H_TDC:  //htdc
		break;
	default:
		break;
	}
	return cal;
}

_u32 func_cap_detection(TDCINFO *TDC)
{
	switch(TDC->tdc_mode){
	case S_TDC:  //stdc
	{
		_u32 err;
		err = check_res(TDC);
		if(err) return 1;

	}
	    break;
	case R_TDC:  //rtdc
	    break;
	case H_TDC:  //htdc
	    break;
	default:
		break;
	}

	return 0;
}

_fp32 res_cal(_u32 result, TDCINFO *TDC)
{
	_fp32 res = 0;
	_u32 cc, v;
	_u32 dres, nsmp;
    dres = (TDC->tdc_opt & 0x1);    			//dres, "1" -double
    nsmp = (((TDC->tdc_opt)>>6) & 0x3);    		//samp

	switch(TDC->tdc_mode){
	case S_TDC:  //stdc
		cc = (result/0x10000);
		v = (result%0x10000);
	    if(nsmp == 3) {
	    	v = v<<1;
	    }
		if(dres)
			res = (_fp32)cc*125000 - (_fp32)((v>>7)*126+v%128)*125000/(TDC->tdc_data.cal*(nsmp+1)); // ps
		else
			res = (_fp32)cc*125000 - (_fp32)((v>>6)*63+v%64)*125000/(TDC->tdc_data.cal*(nsmp+1)); // ps
		break;
	case R_TDC:  //rtdc
		break;
	case H_TDC:  //htdc
		break;
	default:
		break;
	}
	return res;
}

_u32 func_once_time(TDCINFO *TDC)
{
	switch(TDC->tdc_mode){
	case S_TDC:
		{
			_u32 up;
			up = Sonar[0]&0x8000;

			TDC->res_num = TDC->hitin1 + 2 - 2*TDC->disautocal; 			//stop_num+cal
			_u32 err;
			err = check_res(TDC);
			if(err) return 1;

			_u32 i;
			TDC->tdc_data.cal = calibrate(TDC->result[TDC->current_ptr0-2], TDC->result[TDC->current_ptr0-1], TDC);
			for(i=TDC->last_ptr0; i<(TDC->last_ptr0 + TDC->hitin1); i++)
			{
				TDC->res[i] = res_cal(TDC->result[i], TDC);
			}

			_fp32 time = 0;
			time = ftime_caculate(TDC->res, TDC->last_ptr0, TDC->hitin1);
			if(up) {
				(TDC->tdc_data).ftime1 = time;
			}
			else {
				(TDC->tdc_data).ftime2 = time;
			}
		}
	    break;
	case R_TDC:  //rtdc
	    break;
	case H_TDC:  //htdc
	    break;
	default:
		break;
	}

	return 0;
}

_fp32 ftime_caculate(_fp32 *res, _u32 ptr, _u32 stop_num)
{
	_u32 i;
	_fp32 delay[stop_num], average_time = 0;;
	for(i=1;i<stop_num;i++)
	{
		delay[i] = res[i+ptr] - res[0+ptr];
		average_time += delay[i];
	}

	if(stop_num != 1)
		average_time /= (stop_num - 1);

	return average_time;
}

void ptime_caculate(_fp32 *res, _u32 ptr, _fp32 *time, TDCINFO *TDC)
{
	_s32 i, j;
	for(i=0; i<4; i++)
		time[i] = 0;

	switch(TDC->res_num) {
	case 3:
	case 5:
		for(i=0; i<(TDC->res_num - 1); i++)
			time[i] = res[ptr+i+1] - res[ptr+0];
		break;
	case 4:
	case 8:
		for(i=0, j=0; i<(TDC->res_num/2); i++, j += 2)
			time[i] = res[ptr+j+1] - res[ptr+j];
		break;
	}
}

_u32 func_once_thsens(TDCINFO *TDC)
{
	switch(TDC->tdc_mode){
	case S_TDC:  						//stdc
		{
			if(Thsens[0]&0x8)   		//pwrsave
				TDC->res_num = ((Thsens[0]&0x2)>0?4:2)+1;
			else
				TDC->res_num = 2*((Thsens[0]&0x2)>0?4:2);  //4 or 8
			_u32 err;
			err = check_res(TDC);
			if(err) return 1;

			_s32 i;
			for(i=0; i<TDC->current_ptr0; i++)
			{
				TDC->res[i] = res_cal(TDC->result[i], TDC);
			}

			_fp32 time[4];
			ptime_caculate(TDC->res, 0, time, TDC);
			if(Thsens[0]&0x4) {  				//4-3-2-1
				for(i=3; i>=0; i--)
					TDC->tdc_data.ptime[i] = time[3-i];
			}else {
				for(i=0; i<4; i++)
					TDC->tdc_data.ptime[i] = time[i];
			}
		}
	    break;
	case R_TDC:  //rtdc
	    break;
	case H_TDC:  //htdc
	    break;
	default:
		break;
	}

	return 0;
}

_u32 func_twice_time(TDCINFO *TDC)
{
	switch(TDC->tdc_mode){
	case S_TDC:
	{
		_u32 up;
		up = Sonar[0]&0x8000;

		TDC->res_num = TDC->hitin1 + 2 - 2*TDC->disautocal; //stop_num+cal
		_u32 err;
		err = check_res(TDC);
		if(err) return 1;

		_u32 i;

		TDC->tdc_data.cal = calibrate(TDC->result[TDC->last_ptr0-2], TDC->result[TDC->last_ptr0-1], TDC);
			for(i=0; i<TDC->hitin1; i++)
			{
				TDC->res[i] = res_cal(TDC->result[i], TDC);
			}
		TDC->tdc_data.cal = calibrate(TDC->result[TDC->current_ptr0-2], TDC->result[TDC->current_ptr0-1], TDC);
			for(i=TDC->last_ptr0; i<(TDC->last_ptr0 + TDC->hitin1); i++)
			{
				TDC->res[i] = res_cal(TDC->result[i], TDC);
			}

			_fp32 delta_time = 0;
			_fp32 time1 = 0;
			_fp32 time2 = 0;
	    time1 = ftime_caculate(TDC->res, 0, TDC->hitin1);
	    time2 = ftime_caculate(TDC->res, TDC->last_ptr0, TDC->hitin1);
	    if(up) {
			delta_time = time2 - time1;
			(TDC->tdc_data).ftime1 = time1;
			(TDC->tdc_data).ftime2 = time2;
	    }
		else {
			delta_time = time1 - time2;
			(TDC->tdc_data).ftime1 = time2;
			(TDC->tdc_data).ftime2 = time1;
		}
		(TDC->tdc_data).time_delta = delta_time;
	}
	    break;
	case R_TDC:  //rtdc
	    break;
	case H_TDC:  //htdc
	    break;
	default:
		break;
	}

	return 0;
}


static _u32 func_twice_thsens(TDCINFO *TDC)
{
	switch(TDC->tdc_mode){
	case S_TDC:  //stdc
	{
		if(Thsens[0]&0x8)   //pwrsave
			TDC->res_num = ((Thsens[0]&0x2)>0?4:2)+1;
		else
			TDC->res_num = 2*((Thsens[0]&0x2)>0?4:2);  //2 or 4
		_u32 err;
		err = check_res(TDC);
		if(err) return 1;

		_s32 i;
			for(i=0; i<TDC->current_ptr0; i++)
			{
				TDC->res[i] = res_cal(TDC->result[i], TDC);
			}

		_fp32 time[4];
		ptime_caculate(TDC->res, 0, time, TDC);
		if(Thsens[0]&0x4) {  //4-3-2-1
			for(i=3; i>=0; i--)
				TDC->tdc_data.ptime[i] = time[3-i];
		} else {
			for(i=0; i<4; i++)
				TDC->tdc_data.ptime[i] = time[i];
		}

	    ptime_caculate(TDC->res, TDC->last_ptr0, time, TDC);
		if(Thsens[0]&0x4) {  //4-3-2-1
			for(i=3; i>=0; i--)
				TDC->tdc_data.ptime[i] += time[i];
		} else {
			for(i=0; i<4; i++)
				TDC->tdc_data.ptime[i] += time[3-i];
		}

		for(i=0; i<4; i++)
			TDC->tdc_data.ptime[i] /= 2;
	}
	    break;
	case R_TDC:  //rtdc
	    break;
	case H_TDC:  //htdc
	    break;
	default:
		break;
	}

	return 0;
}

_u32 check_res(TDCINFO *TDC)
{
	TDC->current_ptr0 = (Sonar[8]&0xf);
	if(TDC->current_ptr0 == 0)
		TDC->current_ptr0 = 16;
	TDC->current_ptr1 = ((Sonar[8]>>4)&0xf);
	if(TDC->current_ptr1 == 0)
		TDC->current_ptr1 = 16;
	TDC->last_ptr0 = (Sonar[8]>>8)&0xf;
	TDC->last_ptr1 = (Sonar[8]>>12)&0xf;

	/*
	 * 对于两次测量来说，应该对两个指针都进行检查；
	 * 但如果进行的是一次测量，last指针是不变的，只需要检查current指针。
	 */
//	printf("ptr:0x%x\n",Sonar[8]);
	_u32 err = 0;
	err = (((TDC->current_ptr0 - TDC->last_ptr0)%16) != TDC->res_num);
	if((TDC->mop == mode_3)||(TDC->mop == mode_2))
		err = (err || TDC->last_ptr0 != TDC->res_num);

	if(err)
	{
		PMU_SecToken = PMU_SecToken_command;
		PMU_CommandW = 0x80000000;  //0xbfeb003c, clear regs, then you can start next measure
		//printf("\nERROR: wrong result number!\n");
		return 1;
	}

	_u32 *result = TDC->result;
	_u32 i;
	for(i=0; i<(TDC->current_ptr0); i++)
	{
		result[i] = Sonar[16+i];    //0xbfeb4040 Result[0]
	}

	if(TDC->change_flow_direction)
		Sonar[0] ^= 0xc000;
	PMU_SecToken = PMU_SecToken_command;
	PMU_CommandW = 0x80000000;  //0xbfeb003c, clear regs, then you can start next measure

	return 0;
}


void	ResReady(void)
{
	TDCINFO *TDC = &GTdcInfo;
	_u32 tdcsel, tdcsel2, cop;
	tdcsel = ((Sonar[1]>>19)&0x1);    //tdcsel
	tdcsel2 = ((Sonar[1]>>27)&0x1);    //tdcsel2
	cop = ((PMU[1]>>9)&0x7);  	//cop

	TDC->tdc_mode = 2*tdcsel2 + tdcsel;
	TDC->mrange = ((Sonar[1]>>18)&0x1);    //mrange2
	TDC->disautocal = ((Sonar[1]>>20)&0x1);    //disautocal
	TDC->hitin1 = ((Sonar[1]>>21)&0x7);    //hitin1
	TDC->tdc_opt = ((Sonar[2]>>22)&0xff);    //TDCOpt
	TDC->mop = cop;
	TDC->sonar_PulseDef = SONAR_PulseDef;
	TDC->sonar_ResPtr = SONAR_ResPtr;

	if(TDC->mrange == 0)  						//do nothing, if mrange1
	{
		_u32 *result = TDC->result;
		_u32 i;
		for(i=0; i<16; i++)
		{
			result[i] = Sonar[16+i];    		//0xbfeb4040 Result[0]
		}
		PMU_SecToken = PMU_SecToken_command;
		PMU_CommandW = 0x80000000;
		return ;
	}

	switch(cop){  									//cop
	case mode_0:
	//		TDC->change_flow_direction = 1;
		TDC->err = func_once_time(TDC);
		break;
	case mode_1:
		TDC->err = func_once_thsens(TDC);
		break;
	case mode_2:
		TDC->change_flow_direction = 1;
		TDC->err = func_twice_time(TDC);
		break;
	case mode_3:
		TDC->err = func_twice_thsens(TDC);
		break;
#if 0
	case mode_4:
		TDC->res_num = 2;
		TDC->err = func_tdc_cal(TDC);
		break;
	case mode_5:
		TDC->res_num = 4;
		TDC->err = func_clkhs_cal(TDC);
		break;
#endif
	case mode_6:
		TDC->res_num = 4;
		TDC->err = func_cap_detection(TDC);
		break;
	default:
		break;
	}
	return ;
}


INT8U	TimeTdc()
{
	INT32U	TempTime;
	INT8U	ret;
	static	INT32U		LastTimeTdc;

	ret	= 0;
	TempTime	= OSTimeGet();
	if(SampingMode	== USER){
		if( (TempTime - LastTimeTdc) >= (2 * 8000000/ TIMERDLY)){
			LastTimeTdc	= TempTime;
			ret = 1;
		}else{
			ret = 0;
		}
	}else if(SampingMode	== DOCIM){
		if( (TempTime - LastTimeTdc) >= 250 * 8000/TIMERDLY){
			LastTimeTdc	= TempTime;
			ret = 1;
		}else{
			ret = 0;
		}
	}
	return ret;
}


INT8U	TimeThsens()
{
	INT32U	TempTime;
	INT8U	ret;
	static	INT32U	LastThsensTdc;

	ret	= 0;
	TempTime	= OSTimeGet();
	if(SampingMode	== USER){
		if( (TempTime - LastThsensTdc) >= 12 * 8000000/ TIMERDLY){
			LastThsensTdc	= TempTime;
			ret = 1;
		}else{
			ret = 0;
		}
	}else if(SampingMode	== DOCIM){
		if( (TempTime - LastThsensTdc) >= 2 * 8000000/ TIMERDLY){
			LastThsensTdc	= TempTime;
			ret = 1;
		}else{
			ret = 0;
		}
	}
	return ret;
}


/*****************************************************
 * 假如在 Thsens 测试 pend之前满足tdc测试时间，那么这种情况不考虑。
 * 如果在  Thsens 测试 pend cpu再次醒来之后满足了tdc测试，由于此时不允许再测试了。
 * 如果处理完  Thsens 数据之后 就OSTimeDly了，那么就会丢失一次 tdc 测试。
 * 再想下：如果此时 OSTimeDly 了，cpu就休眠，在 休眠之前判断是 res唤醒的。因此不会更改
 * pmu_compare 的数值，因此需等待一段时间之后，pmu把 cpu唤醒，此时再做 TimeTdc() 也不迟！！
 * 实际中最好这里加入打印语句观察 发生这种情况的频率。（估计也极少发生！！）
 * 这个进程优先级设置为最低，只有这样才能当这个进程 OSSemPend 的时候立刻进入idle进程休眠掉！
 * ****************************************************/
void	TdcSampProc(OS_EVENT *pGSemTdcInt)
{
	INT8U err;

//NextTimeTdc:
	///下面两个 pend 应该 pend 的是同一个信号量！！！
	if( (TimeTdc() || CASE_ISSET(TIMEWAKE) ) && (!CASE_ISSET(THSENSWAKE) ) )
	{
		if( !CASE_ISSET(TIMEWAKE) ){
			SetTdcMop(DOUBLETIME);
			StartTdc();
			EnSleep();
		}
		OSSemPend(pGSemTdcInt,0,&err);
		ResReady();
		DataProcess(DOUBLETIME);
	}
	if(TimeThsens () || CASE_ISSET(THSENSWAKE) )
	{
		if(!CASE_ISSET(THSENSWAKE) ){
			SetTdcMop(DOUBLETHSENS);
			StartTdc();
			EnSleep();
		}
		OSSemPend(pGSemTdcInt,0,&err);
		ResReady();
		DataProcess(DOUBLETHSENS);
		if(TimeTdc() && CASE_ISSET(THSENSWAKE) ){
			puts("hp\n");
			//goto  NextTimeTdc
		}
	}
}


INT8U	CapaciDetec(OS_EVENT *pGSemTdcInt,INT8U SampMode)
{
	INT8U		err;
	InitTdc();
	SelectTdc(S_TDC);
	SetTdcMop(SampMode);
	StartTdc();
	OSSemPend(pGSemTdcInt,0,&err);
	if(err == OS_NO_ERR){					/// 这个    换能器电容检测   超过10ms，不到20ms
		ResReady();
	}
	return GTdcInfo.err;
}


/**处理超声波
 * **/
void	ProcTime(_fp32 time)
{
	AccumHeat	+= 3.1382f;
	Power		= 3.14f;
	InteFlux	+= 1.52f;
	InstantFlux	= 4.66f;
	InstantHeat	= 6.23f;
}

/**处理温度测试！
 * **/
void	ProcThsens(_fp32 time1,_fp32 time2)
{
	InTemp	= 64.42f;
	OutTemp	= 43.82f;
	DammingTemp	= (43.82f + 64.42f)/2;
	DiffTemp	= 64.42f - 43.82f;
}

_fp32 time_to_temp(_fp32 time, _fp32 standard)
{
	_fp32 r_input = time/standard;
	_fp32 temp;
    /******************************************************************************************************************************/
	temp = 10.1373f*r_input*r_input + 235.515f*r_input - 245.649725f;

	return temp;
}


/** 随意处理的， 这里的公式没有任何意义！！
 * 使用时还需根据情况修改才行~
 * **/
void	DataProcess(INT8U SampMode)
{
	//_fp32 time1;	,time2
	//_u32 i;
	TDCINFO *TDC	= &GTdcInfo;
	if(SampMode == DOUBLETIME){
		/*if(Sonar[0]&0x8000){
			time1	= GTdcInfo.tdc_data.ftime1;
		}else{
			time1	= GTdcInfo.tdc_data.ftime2;
		}
		ProcTime(time1);*/
		/*for(i=0; i<16; i++)
			if((i == 0)||(i == 1)||(i == 2)||(i == 6)||(i == 7)
					||(i == 8)||(i == 9)||(i == 10)||(i == 14)||(i == 15))
				printf("\nresult[%d]: 0x%x", i, TDC->result[i]);

		printf("\nup:%d\tdown:%d\tdelta:%d",
				(_u32)TDC->tdc_data.ftime1,
				(_u32)TDC->tdc_data.ftime2,
				(_u32)TDC->tdc_data.time_delta
		);*/
	}else if(SampMode == DOUBLETHSENS){
		/*if(Thsens[0]&0x4){
			time1	= GTdcInfo.tdc_data.ptime[3] + GTdcInfo.tdc_data.ptime[2];
			time2	= GTdcInfo.tdc_data.ptime[1] + GTdcInfo.tdc_data.ptime[0];
		}else{
			time1	= GTdcInfo.tdc_data.ptime[0] + GTdcInfo.tdc_data.ptime[1];
			time2	= GTdcInfo.tdc_data.ptime[2] + GTdcInfo.tdc_data.ptime[3];
		}
		ProcThsens(time1,time2);*/
		_fp32 tmp;
		tmp = TDC->tdc_data.ptime[2] + TDC->tdc_data.ptime[3];
		if((tmp < 0.0001f) && (tmp > -0.0001f)){
			puts("err");
			return ;
		}
		_fp32 temp1 = time_to_temp(TDC->tdc_data.ptime[0]*2, tmp);
		_fp32 temp2 = time_to_temp(TDC->tdc_data.ptime[1]*2, tmp);
		static char	buf[64];
		ftoa(buf, temp1, 4);
		puts("\n");
		puts(buf);
		puts("\n");
		ftoa(buf, temp2, 4);
		puts(buf);/**/
		puts("\n");
	}
}
















