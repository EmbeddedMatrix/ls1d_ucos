
#define	abs(a)	( ((a) > 0) ? (a) : (-(a)) )

/**����Ϊ  memset ���±�����󣡣� ֻ�ܲ������׼�ⲻͬ�ĺ��������ˣ�����������**/
void * memset1d(void * s,INT8U c, INT8U count);
void * memcpy1d(void * dst,void * src, INT8U count);

INT8U Hex2Bcd(INT8U hex);


int ftoa(char *str, _fp32 num, int n);


INT8U memchar1d(void * s,INT8U c, char charac);

int printf(char *fmt,...);
