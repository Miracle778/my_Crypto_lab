#include<iostream>
using namespace std;
#define SIZE 33				//为了实现N  1024比特

typedef struct Bigint {
	unsigned char num[SIZE];
};

typedef struct Bigint2 {	//运算结果数组长度最多为两倍
	unsigned char num[2 * SIZE];
};

Bigint2 Extend(Bigint a);					//扩展数组
//Bigint Init(unsigned char a[], int length);	//初始化
void   Copy(Bigint &a, Bigint b);			//拷贝
int    Compare(Bigint a, Bigint b);			//比较a，b大小
int    getLength(Bigint a);					//获取大整数的数组长度
Bigint Add(Bigint a, Bigint b);				//大整数加法
Bigint Sub(Bigint a, Bigint b);				//减法
Bigint2 Mul(Bigint a, Bigint b);			//乘法结果长度可能为两倍
Bigint Div(Bigint a, Bigint b);				//除法
Bigint MoveLeft(Bigint a, int len);			//左移，除法用到
Bigint Mod(Bigint a, Bigint b);				//模运算

bool Inverse(Bigint e, Bigint N, Bigint &d);//求逆
Bigint AddMod(Bigint a, Bigint b,Bigint n);//加模
Bigint SubMod(Bigint a, Bigint b,Bigint n);
Bigint Sub2Mod(Bigint a, Bigint b,Bigint n);
Bigint MulMod(Bigint a, Bigint b,Bigint n);
Bigint Narrow(Bigint2 a);					//压缩
Bigint2 Mod(Bigint2 a, Bigint2 b);         //Bigint2 mod运算
void BitMoveRight(Bigint &a);


Bigint Narrow(Bigint2 a)   //压缩
{
	Bigint res = { 0 };
	for (int i = 0; i < SIZE ; i++)
		res.num[i] = a.num[i];
	return res;
}
Bigint2 Extend(Bigint a) //扩展
{
	Bigint2 res = { 0 };
	for (int i = 0; i < SIZE ; i++)
		res.num[i] = a.num[i];
	return res;
}
int getLength(Bigint a)
{
	int i = SIZE - 1;//从高位开始
	while (!a.num[i])
		i--;
	return i + 1;
}
int getLength(Bigint2 a)
{
	int i = 2 * SIZE - 1;
	while (!a.num[i])
		i--;
	return i + 1;
}

int Compare(Bigint a, Bigint b) 
{
	int a_len = getLength(a);
	int b_len = getLength(b);
	int max;
	if (a_len > b_len)
		max = a_len;
	else
		max = b_len;
	if (max == 0)		//max = 0表示a ,b都为0，相等
		return 0;
	else
	{
		for (int i = max - 1; i >= 0; i--)
		{
			if (a.num[i] > b.num[i])
				return 1;
			if (a.num[i] < b.num[i])
				return -1;
		}
	}
	return 0;		//若上面for循环都执行完，即a = b，返回0
}
int Compare(Bigint2 a, Bigint2 b)
{
	int a_len = getLength(a);
	int b_len = getLength(b);
	int max;
	if (a_len > b_len)
		max = a_len;
	else
		max = b_len;
	if (max == 0)
		return 0;
	else
	{
		for (int i = max - 1; i >= 0; i--)
		{
			if (a.num[i] > b.num[i])		//因为max = a,b中最大的长度
				return 1;
			if (a.num[i] < b.num[i])
				return -1;
		}
	}
	return 0;
}

Bigint Add(Bigint a, Bigint b)
{
	//修改
	Bigint c;
	unsigned short temp;			//加法和可能会有进位，8位不够用，故定义short 16位
	//unsigned long long temp;
	unsigned char carry = 0;		//初始进位为0;
	//unsigned int carry = 0;
	for (int i = 0; i < SIZE; i++)
	{
		temp = a.num[i] + b.num[i] + carry;
		c.num[i] = temp & 0x00ff;	//低8位
		//c.num[i] = temp & 0xffffffff;
		carry = (temp >> 8) & 0xff; //高于8位的全是进位，进位范围8位足够，不会溢出
		//carry = (temp >> 32) & 0xffffffff;
	}
	return c;
}

Bigint Sub(Bigint a, Bigint b)
{
	//修改
	if (Compare(a, b) == -1) {
		return  a;				//	a < b时返回a
	}
	Bigint c;
	short temp;	//结果用short 16位有符号数表示，要处理借位，使用有符号运算，用补码表示负数再处理
	//long long temp;

	unsigned char carry = 0;//借位
	//unsigned int carry = 0;
	for (int i = 0; i < SIZE ; i++)				//错误，SIZE写成SIZE-1，造成死循环
	{
		temp = a.num[i] - b.num[i] - carry;	//减去前面低位的借位,如果是负数的话，temp为有符号数，用补码表示，低八位相当于加上了256
		c.num[i] = temp & 0x00ff;				//结果等于低8位
		//c.num[i] = temp & 0xffffffff;
		carry = (temp >> 15) & 0x01;			//最高位为符号位，temp如果是负数，则最高位1，产生借位，借位最多借一位
		//carry = (temp >> 63) & 0x1;
	}
	return c;
}
Bigint2 Sub(Bigint2 a, Bigint2 b)
{
	//修改
	if (Compare(a, b) == -1)
		return a;
	Bigint2 c;
	short temp;
	//long long temp;
	unsigned char carry = 0;
	//unsigned int carry = 0;
	for (int i = 0; i < 2 * SIZE; i++)
	{
		temp = a.num[i] - b.num[i] - carry;
		c.num[i] = temp & 0x00ff;
		//c.num[i] = temp & 0xffffffff;
		carry = (temp >> 15) & 0x1;
		//carry = (temp >> 63) & 0x1;
	}
	return c;
}
Bigint2 Mul(Bigint a, Bigint b)
{
	//修改
	Bigint2 c = { 0 };//初始化
	unsigned short temp;	//临时积
	//unsigned long long temp;
	unsigned char carry;
	//unsigned int carry;
	for (int i = 0; i < SIZE; i++)
	{
		carry = 0;
		for (int j = 0; j < SIZE; j++)
		{
			temp = a.num[i] * b.num[j] + c.num[i + j] + carry;		//模拟乘法列式运算，对照计算过程编写
			c.num[i + j] = temp & 0x00ff;				//a.num[i]*b.num[j]结果放在c.num[i+j]
			//c.num[i + j] = temp & 0xffffffff;
			carry = (temp >> 8) & 0xff;			//乘法进位最大为255,因此用unsigned char型
												//a,b都需经初始化置0再参与运算
			//carry = (temp >> 32) & 0xffffffff;
		}
	}
	c.num[SIZE * 2 - 1] = carry;//若是积太大，最高进位循环内未处理，则此处处理
	return c;
}

Bigint MoveLeft(Bigint a, int len)
{
	for (int i = getLength(a) - 1; i >= 0; i--)
	{
		if (i + len >= SIZE)
			continue;
		a.num[i + len] = a.num[i];
	}
	for (int i = len - 1; i >= 0; i--)
		a.num[i] = 0;
	return a;
}
Bigint2 MoveLeft(Bigint2 a, int len)
{
	/*int len_a = getLength(a);
	for (int i = len_a + len - 1; i >= len; i--)
	{
		a.num[i] = a.num[i - len];
	}
	for (int i = 0; i < len; i++)
		a.num[i] = 0;
	return a;*/
	for (int i = getLength(a) - 1; i >= 0; i--)
	{
		if (i + len >= 2*SIZE)
			continue;
		a.num[i + len] = a.num[i];
	}
	for (int i = len - 1; i >= 0; i--)
		a.num[i] = 0;
	return a;

}
Bigint Div(Bigint a, Bigint b)
{
	Bigint B = { 0 };
	Bigint c = { 0 };
	int len = getLength(a) - getLength(b);
	while (len >= 0)
	{
		B = MoveLeft(b, len);	 //临时除数，从高位开始除
		while (Compare(a, B) >= 0)
		{
			a = Sub(a, B);			//更新被除数
			c.num[len]++;			//通过减法得到对应位数的商
		}
		len--;						//除完一次，len减1
	}
	return c;
}

Bigint Mod(Bigint a, Bigint b)
{
	if (Compare(a, b) < 0)
		return a;		//a<b返回a
	else
	{
		Bigint B = { 0 };
		int len = getLength(a) - getLength(b);
		while (len >= 0)
		{
			B = MoveLeft(b, len);		//提高效率，减一个相对大的b的倍数
			while (Compare(a, B) >= 0)
				a = Sub(a, B);
			len--;
		}
		return a;
		
	}
}
Bigint2 Mod(Bigint2 a, Bigint2 b)	//a mod b
{
	if (Compare(a, b) < 0)  //a<b 返回a
		return a;
	else
	{
		Bigint2 B = { 0 };
		int len = getLength(a) - getLength(b);
		while (len >= 0)
		{
			B = MoveLeft(b, len);
			while (Compare(a, B) >= 0)
				a = Sub(a, B);
			len--;
		}
		return a;
	}
}

void Copy(Bigint &a, Bigint b)
{
	for (int i = 0; i < SIZE; i++)
	{
		a.num[i] = b.num[i];
	}
}
void BitMoveRight(Bigint &a)
{											//右移一比特，等于除二
	Bigint temp = { 2 };
	a = Div(a,temp);
}
bool Inverse(Bigint e, Bigint N, Bigint &d)			//e模N的逆，存在d中
{
	Bigint r1 = { 0 };
	Bigint r2 = { 0 };
	Copy(r1, e);
	Copy(r2, N);
	Bigint s1 = { 1 };
	Bigint s2 = { 0 };
	Bigint s = { 0 };
	Bigint r = { 0 };
	Bigint q = { 0 };

	while (1)
	{
		if (getLength(r1) == 0)
			return 0;
		if (getLength(r1) == 1 && r1.num[0] == 1)
		{
			Copy(d, s1);
			return 1;
		}
		q = Div(r1, r2);					
		s = Sub2Mod(s1, MulMod(q, s2, N), N);	
		r = Sub(r1, Narrow(Mul(q, r2)));		
		Copy(r1, r2);
		Copy(s1, s2);
		Copy(s2, s);
		Copy(r2, r);
	}
}

Bigint AddMod(Bigint a, Bigint b, Bigint n)			//a+b mod N
{
	return Mod(Add(a, b), n);
}
Bigint SubMod(Bigint a, Bigint b, Bigint n)		//a>b调用  a-b mod N
{
	return Mod(Sub(a, b), n);
}
Bigint Sub2Mod(Bigint a, Bigint b, Bigint n)
{
	while (Compare(a, b) < 0)
		a = Add(a, n);
	return Sub(a, b);
}

Bigint MulMod(Bigint a, Bigint b, Bigint n)
{
	Bigint2 res = { 0 };
	res = Mul(a, b);
	return Narrow(Mod(res, Extend(n)));
}

Bigint PowMod(Bigint a, Bigint b, Bigint n)
{
	Bigint c = { 1 };
	Bigint temp = { 1 };
	while (getLength(b) > 0)
	{
		while (!(b.num[0] & 1))
		{
			BitMoveRight(b);
			a = MulMod(a, a, n);
		}
		b = Sub(b, temp);				//b-1
		c = MulMod(a, c, n);			//c = a*c mod n
	}
	return c;
}

Bigint BigRand(Bigint n)			//生成0-n之间的数
{	
	//修改
	Bigint res = { 0 };
	for (int i = 0; i < SIZE; i++)
		res.num[i] = rand() % 256;
	//res.num[i] = rand() % 4294967296;
	res = Mod(res, n);
	return res;
}
Bigint BigRand(int bytes)			//生成位数为8*bytes的随机数
{
	//修改
	Bigint res = { 0 };
	for (int i = 0; i < bytes - 1; i++)
		res.num[i] = rand() % 256;
		//res.num[i] = rand() % 4294967296;
	res.num[bytes - 1] = 128 + rand() % 128;		//最高位置1
	//res.num[bytes - 1] = 2147483648 + rand() % 2147483648;
	return res;
}
Bigint BigRandOdd(int bytes)		//生成随机奇数
{
	//修改
	Bigint res = { 0 };
	for (int i = 0; i < bytes - 1; i++)
		res.num[i] = rand() % 256;

	res.num[bytes - 1] = 128 + rand() % 128;
	//res.num[bytes - 1] = 2147483648 + rand() % 2147483648;
	if (!(res.num[0] & 0x01))			//最低位为0,为二的倍数，加一处理
		res.num[0] = res.num[0] + 1;
	return res;
}

bool MillerRabinOnce(Bigint &n)
{
	Bigint b, m, v, temp;
	Bigint j = { 0 };
	Bigint one = { 1 };
	Bigint two = { 2 };
	Bigint three = { 3 };
	m = Sub(n, one);
	while (!(m.num[0] & 0x01))
	{
		j = Add(j, one);				//计算m,j,使得n-1 = 2^j m
		BitMoveRight(m);				//n-1除二j+1，至m为奇数为止
	}
	b = Add(two, BigRand(Sub(n, three)));//选取2-n-1的随机数
	v = PowMod(b, m, n);				//计算v = b^m mod n
	if (Compare(v, one) == 0)
		return 1;
	Bigint i = { 1 };
	temp = Sub(n, one);
	while (Compare(v, temp) < 0)
	{
		if (Compare(i, j) == 0)
			return 0;
		v = MulMod(v, v, n);
		i = Add(i, one);
	}
	return 1;

}

bool MillerRabin(Bigint &n, int loop)
{
	for (int i = 0; i < loop; i++)
	{
		if (!MillerRabinOnce(n))
			return 0;
	}
	return 1;
}

Bigint GenPrime(int bytes)
{
	Bigint res = BigRandOdd(bytes);
	int loop = 20;
	while (!MillerRabin(res, loop))
	{
		res = BigRandOdd(bytes);
	}
	return res;
}