#include "includes.h"


int printbase(long v,int w,int base,int sign)
{
	int i,j;
	int c;
	char buf[64];
	unsigned long value;
	if(sign && v<0)
	{
	value = -v;
	putchar('-');
	}
	else value=v;

	for(i=0;value;i++)
	{
	buf[i]=value%base;
	value=value/base;
	}

#define max(a,b) (((a)>(b))?(a):(b))

	for(j=max(w,i);j>0;j--)
	{
		c=j>i?0:buf[j-1];
		putchar((c<=9)?c+'0':c-0xa+'a');
	}
	return 0;
}

int printf(char *fmt,...)
{
int i;
char c;
void **arg;
void *ap;
int w;
__builtin_va_start(ap,fmt);
arg=ap;
for(i=0;fmt[i];i++)
{
	c=fmt[i];
	if(c=='%')
	{
		w=1;
again:
		switch(fmt[i+1])
		{
			case 's':
				puts(*arg);
				arg++;
				i++;
				break;
			case 'c':
				putchar((long)*arg);
				arg++;
				i++;
				break;
			case 'u':
				printbase((long)*arg,w,10,0);
				arg++;
				i++;
				break;
			case 'd':
				printbase((long)*arg,w,10,1);
				arg++;
				i++;
				break;
			case 'o':
				printbase((long)*arg,w,8,0);
				arg++;
				i++;
				break;
			case 'b':
				printbase((long)*arg,w,2,0);
				arg++;
				i++;
				break;
			case 'p':
			case 'x':
				printbase((long)*arg,w,16,0);
				arg++;
				i++;
				break;
			case '%':
				putchar('%');
				i++;
				break;
			case '0':
				i++;
			case '1' ... '9':
				for(w=0;fmt[i+1]>'0' && fmt[i+1]<='9';i++)
				 w=w*10+(fmt[i+1]-'0');
				goto again;
				 break;

			default:
				putchar('%');
				break;
		}

	}
	else{
		if(c=='\n') putchar('\r');
		putchar(c);
	}
}
	return 0;
}



//// 这个函数应该有很大的优化空间！！ TODO
///  应该可以返回小数点所在的下标这样 以后处理起来就比较方便了 	TODO
/**Float ===> String
 * 把单精度浮点数转换为字符串形式。方便在 lcd 上显示
 * n 代表转换的精度，即是字符串'.'后有几位小数
 * 比如 32.1345 转换为  字符串  	'3 2 . 1 3 4 5'
 * 处理结果是正直还是负值，如果是负值第一个字符是一个'-'
 * 这里怎么使用浮点计算 显示 错误呀。搞不懂。 TODO
 * **/
int ftoa(char *str, _fp32 num, int n)
{
	int     sumI;
	_fp32   sumF;
	int     sign = 0;
	INT8U	temp;
	int     count = 0;

	char *p;
	char *pp;

	if(str == NULL)
		return -1;
	p = str;

	/*Is less than 0*/
	if(num < 0)
	{
		sign = 1;
		num = 0 - num;
	}

	sumI = (int)num;    //sumI is the part of int
	sumF = num - sumI;  //sumF is the part of float

	/*Int ===> String*/
	do
	{
		temp = sumI % 10;
		*(str++) = temp + '0';
	}while((sumI = sumI /10) != 0);

	/*******End*******/
	if(sign == 1)
	{
		*(str++) = '-';
	}

	pp = str;

	pp--;
	while(p < pp)
	{
		*p = *p + *pp;
		*pp = *p - *pp;
		*p = *p -*pp;
		p++;
		pp--;
	}
	*(str++) = '.';     //point

	/*Float ===> String*/
	do
	{
		temp = (INT8U)(sumF*10);
		*(str++) = temp + '0';
		if((++ count) == n)
			break;
		sumF = sumF * 10 - temp;
	}while(1);		//while(!(sumF <= 0.00001));		///TODO  这里如果有 浮点数比较，则链接错误。搞不懂。
	*str = 0;

	return 0;
}

INT8U memchar1d(void * s,INT8U c, char charac)
{
	INT8U	*src;

	src		= (INT8U *)s;
	while(c--){
		if(*src ++ == (INT8U)charac){
			return (INT8U)((INT32U)src - (INT32U)s );
		}
	}
	return 0;
}

void * memset1d(void * s,INT8U c, INT8U count)
{
	INT8U *xs = (INT8U *) s;

	while (count--)
		*xs++ = c;

	return s;
}

void * memcpy1d(void * dst,void * src, INT8U count)
{
	INT8U *xs = (INT8U *) src;

	while (count--)
		*(INT8U *)dst ++ = *xs ++;

	return src;
}

/*******这个可以用mips汇编优化一下， arm x86  51 都有关于bcd 码的指令，在mips上却没有！*******/
INT8U Hex2Bcd(INT8U hex)
{
	INT8U	low,high;

	low = high = 0;
	while(hex >= 10){
		hex	-= 10;
		high ++;
	}
	low	= hex;
	return (high << 4 | low);
}

