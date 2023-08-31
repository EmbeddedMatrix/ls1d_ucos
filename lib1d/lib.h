
#define	abs(a)	( ((a) > 0) ? (a) : (-(a)) )

/**命名为  memset 导致编译错误！！ 只能采用与标准库不同的函数命名了！！！！！！**/
void * memset1d(void * s,INT8U c, INT8U count);
void * memcpy1d(void * dst,void * src, INT8U count);

INT8U Hex2Bcd(INT8U hex);


int ftoa(char *str, _fp32 num, int n);


INT8U memchar1d(void * s,INT8U c, char charac);

int printf(char *fmt,...);
