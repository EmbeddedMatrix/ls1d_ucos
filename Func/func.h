
void	Float2LCDBuf(void);

void	InitTimeDate(void);
BOOLEAN	SystemClock();

void 	PrintLcd(void);

INT32U KeyProc(OS_EVENT *pGMboxKey,INT32U	KeyTime);


void	TdcSampProc(OS_EVENT *pGSemTdcInt);


void	InfraProc(void);

INT8U	TimeTdc();
INT8U	TimeThsens();

INT8U	CapaciDetec(OS_EVENT *pGSemTdcInt,INT8U SampMode);
