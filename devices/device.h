
#define KEY_OFF		0x800        //when the key is off, the key_bit of gpio_in will be high


void LcdPoweron();

void LcdPowerdown();

void WriteLcdAll(void );

void Lcd_HW_Init(void);



void Uart0_38400B(void);

int puts(char *s);

int putchar(long c);

void WriteLcdRegs(INT8U *buf);

void ls1d_key_init();


INT32U  ChkWakeUp(void);
INT8U 	InitWaker(void);
void	WakeCompen();
INT32U	SetPDTime(INT32U PDTime);
void  	CpuSleep( );
void	EnSleep();
void	DisSleep();
void	WakeWaitInt();


void InitTdc(void);
void SelectTdc(INT32U mode);
void SetTdcMop(INT32U mode);
void StartTdc(void);


void Infrared1Init(void);
void Infrared1Disable(void);


void adc_init0(void);
_u32 adc_measure0();
INT32U	adc_test();
void	VoltageCtrl(void);


_u32 Flash_Write(_u32 addr, _u32 *data, _u32 num);
_u32 Flash_Erase_page(_u32 addr);
void	MonthlySave(void);




