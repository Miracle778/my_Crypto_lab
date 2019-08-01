
// Yiitao_DESDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Yiitao_DES.h"
#include "Yiitao_DESDlg.h"
#include "afxdialogex.h"
#include<fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


char m[1000] = { 0 };//明文
char c[1000] = { 0 };//密文
char M[1000] = { 0 };//解密得到的信息
int block = 0;//分块

bool SubKey[16][48];//子密钥16轮 48bit

//初始置换IP
const static char IP_Table[64] = {
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7
};
//初始逆置换IP
const static char IP_N_Table[64] = {
	40,8,48,16,56,24,64,32,
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25
};
//扩展表E
static const char E_Table[48] = {
	//少打一行
	32,1,2,3,4,5,
	4,5,6,7,8,9,
	8,9,10,11,12,13,
	12,13,14,15,16,17,
	16,17,18,19,20,21,
	/**/20,21,22,23,24,25,
	24,25,26,27,28,29,
	28,29,30,31,32,1
};
//S盒
static const char S_Box[8][4][16] = {
	//S1第一行10书上是0待检验
	14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
	0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
	4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
	15,12,8,2,4,9,1,7,5,11,3,15,10,0,6,13,	//15填成了14
	//S2
	15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10,
	 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5,
	 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15,
	13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9,
	//S3
	10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
	13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
	13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
	1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,
	//S4
	7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
	13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
	10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
	3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,
	//S5
	2, 12,4,1, 7, 10,11,6,8,5,3,15,13,0,14,9,
	14,11,2,12,4, 7, 13,1,5,0,15,10,3,9,8,6,
	4, 2, 1,11,10,13,7,8,15,9,12,5,6,3,0,14,
	11,8,12,7, 1, 14,2,13,6,15,0,9,10,4,5,3,
	//S6
	12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
	10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
	9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
	4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,
	//S7
	4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
	13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
	1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
	6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,
	//S8
	13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
	1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
	7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
	2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
};
//P盒
static const char P_Table[32] = {
	16,7,20,21,
	29,12,28,17,
	1,15,23,26, 
	5,18,31,10,
	2,8,24,14,
	32,27,3,9,
	19,13,30,6,
	22,11,4,25
};
//PC_1置换
static const char PC_1_Table[56] = {
	57,49,41,33,25,17,9,
	1,58,50,42,34,26,18,
	10,2,59,51,43,35,27,
	19,11,3,60,52,44,36,
	63,55,47,39,31,23,15,
	7,62,54,46,38,30,22,
	14,6,61,53,45,37,29,
	21,13,5,28,20,12,4
};
//PC_2置换
static const char PC_2_Table[48] = {
	14, 17, 11, 24,  1,  5, 
	3,  28, 15,  6, 21, 10, 
	23, 19, 12,  4, 26,  8, 
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55, 
	30, 40, 51, 45, 33, 48, 
	44, 49, 39, 56, 34, 53, 
	46, 42, 50, 36, 29, 32,
};
//左移位数表
static const char LS_Table[16] = {
	1, 1, 2, 2, 2, 2, 2, 2,
	1, 2, 2, 2, 2, 2, 2, 1
};

void Transform(bool* Out, bool* In, const char* Table, int len);//表置换
void Des_Run(char Out[8], char In[8], bool flag);//加解密函数，靠flag选择功能
void Des_SetSubkey(char key[8]);//生成子密钥
void F_func(bool In[32], bool Ki[48]);//F轮函数
void S_func(bool Out[32], bool In[48]);//S盒置换
void Xor(bool* InA, bool* InB, int len);//异或
void Rotatel(bool* In, int len, int loop);//循环左移
void ByteToBit(bool* Out, const char* In, int bits);//字符转比特
void HalfByteToBit(bool* Out, const char* In, int bits);//半字符转比特
void BitToByte(char* Out, const bool* In, int bits);//比特转字符
void Des_File(char *msgFile, char *cipherFile); //文件加密，从文本框获取路径
void Des_File_D(char *msgFile, char *cipherFile);//文件解密，从文本框读入文件路径


//void memcpy(bool* Out, bool* In, int len);//复制函数

//void memcpy(bool* Out, bool* In, int len)
//{
	//for (int i = 0; i < len; i++)
		//Out[i] = In[i];
//}

void Des_Run(char Out[8], char In[8], bool flag)
{
	static bool M[64];//存储64位明文，由明文字符转换得来
	static bool Temp[32];//中间变量，用于对Li赋值
	static bool* Li = &M[0];//分为左右两组
	static bool* Ri = &M[32];
	ByteToBit(M, In, 64);
	if (flag)
	{
		Transform(M, M, IP_Table, 64);//初始置换IP
		for (int i = 0; i < 16; i++)
		{
			memcpy(Temp, Ri, 32);//把R(i-1)保存起来
			F_func(Ri, SubKey[i]);
			Xor(Ri, Li, 32);
			memcpy(Li, Temp, 32);//Li=R(i-1)
		}
		memcpy(Temp, Li, 32);
		memcpy(Li, Ri, 32);
		memcpy(Ri, Temp, 32);//最后交换Li,Ri
		Transform(M, M, IP_N_Table, 64);

	}
	else {
		Transform(M, M, IP_Table, 64);
		for (int i = 15; i >= 0; i--)//没有等于0
		{
			memcpy(Temp, Ri, 32);//把R(i-1)保存起来
			F_func(Ri, SubKey[i]);
			Xor(Ri, Li, 32);
			memcpy(Li, Temp, 32);//Li=R(i-1)
		}
		memcpy(Temp, Li, 32);
		memcpy(Li, Ri, 32);
		memcpy(Ri, Temp, 32);//最后交换Li,Ri
		Transform(M, M, IP_N_Table, 64);
	}
	BitToByte(Out, M, 64);
}
void ByteToBit(bool* Out, const char* In, int bits)
{	
	//把各种盒或表里的十进制转成比特
	for (int i = 0; i < bits; i++)
	{
		Out[i] = (In[i / 8] >> (i % 8)) & 1;
	}
}
void HalfByteToBit(bool* Out, const char* In, int bits)//把各种盒或表里的十进制转成比特
{
	for (int i = 0; i < bits; i++)
	{
		Out[i] = (In[i /4] >> (i % 4)) & 1;
	}
}
void Transform(bool* Out, bool* In, const char* Table, int len)//置换
{
	bool Temp[256];
	for (int i = 0; i < len; i++)
	{
		//Out[i] = In[Table[i] - 1];//Table[i]-1，从0开始计数
		Temp[i] = In[Table[i] - 1];
	}
	memcpy(Out, Temp, len);
}
void F_func(bool In[32], bool Ki[48])
{
	static bool MR[48];
	Transform(MR, In, E_Table, 48);
	Xor(MR, Ki, 48);
	S_func(In, MR);
	Transform(In, In, P_Table,32);

}
void Xor(bool* InA, bool* InB, int len)
{
	for (int i = 0; i < len; i++)
		InA[i] = (InA[i] ^ InB[i]);
}
void S_func(bool Out[32], bool In[48])//错误写成了bool* Out[32]
{
	for (char i = 0, j, k; i < 8; i++)//
	{	
		j = (In[0+6*i] << 1) + In[5+6*i];//行等于首尾拼起来组成的十进制数
		k = (In[1+6*i] << 3) + (In[2+6*i] << 2) + (In[3+6*i] << 1) + In[4+6*i];
		HalfByteToBit(Out+4*i, &S_Box[i][j][k], 4);
	}
}
void BitToByte(char* Out, const bool* In, int bits)
{
	memset(Out, 0, bits >> 3);//初始化8个字节的内存
	for (int i = 0; i < bits; i++)
	{
		Out[i >> 3] |= In[i] << (i & 7);
	}
}

void Des_SetSubkey(char key[8])
{
	static bool K[64];
	static bool* KL = &K[0];
	static bool* KR = &K[28];
	ByteToBit(K, key, 64);
	Transform(K, K, PC_1_Table, 56);
	for (int i = 0; i < 16; i++)
	{
		Rotatel(KL, 28, LS_Table[i]);
		Rotatel(KR, 28, LS_Table[i]);
		Transform(SubKey[i], K, PC_2_Table,48);//忘记PC_2置换了

	}
}
void Rotatel(bool* In, int len, int loop)
{
	static bool Temp[28];
	memcpy(Temp, In, loop);//循环位loop位保存在temp中
	memcpy(In, In + loop, len - loop);//左移
	memcpy(In + len - loop, Temp, loop);//补上
}


void Des_File(char *msgFile, char *cipherFile)
{	//文件加密，从文本框获取路径
	//FILE * msg, *cipher;
	char msgBlock[8], cipherBlock[8];
	//int count;
	ifstream msg(msgFile, ios::binary);//读文件
	ofstream cipher(cipherFile, ios::binary);//写文件

	while (/*!msg.eof()*/msg.peek() != EOF)//出错解决	//c++ eof会多读一次
	{
		/*if ((count = fread(msgBlock, sizeof(char), 8, msg)) == 8)
		{
			Des_Run(cipherBlock, msgBlock, 1);
			fwrite(cipherBlock, sizeof(char), 8, cipher);
		}*/
		memset(msgBlock,0,8);//先默认为0，省去填充
		msg.read(msgBlock, 8);
		Des_Run(cipherBlock, msgBlock, 1);
		cipher.write(cipherBlock, 8);
	}
	/*if (count)
	{		//填充
		memset(msgBlock + count, '\0', 8 - count);
		Des_Run(cipherBlock, msgBlock, 1);
		fwrite(cipherBlock, sizeof(char), 8, cipher);
	}
	fclose(msg);
	fclose(cipher);
	*/
	msg.close();
	cipher.close();
}

void Des_File_D(char *msgFile, char *cipherFile)
{	//文件解密，从文本框读入文件路径
	//FILE * msg, *cipher;

	char msgBlock[8], cipherBlock[8];

	//msg = fopen(msgFile, "wb");
	//cipher = fopen(cipherFile, "rb");

	ofstream msg(msgFile, ios::binary);
	ifstream cipher(cipherFile, ios::binary);
	//int count;
	while (/*!cipher.eof()*/cipher.peek() != EOF)
	{
		/*if ((count = fread(cipherBlock, sizeof(char), 8, cipher)) == 8)
		{
			Des_Run(cipherBlock, msgBlock, 0);
			fwrite(msgBlock, sizeof(char), 8, msg);
		}*/
		/*出错，数组越界cipherBlock[8] = { 0 };*/  
		memset(cipherBlock, 0, 8);
		cipher.read(cipherBlock, 8);
		Des_Run(msgBlock, cipherBlock, 0);
		msg.write((char*)&msgBlock, 8);

	}
	msg.close();
	cipher.close();
}




// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CYiitao_DESDlg 对话框



CYiitao_DESDlg::CYiitao_DESDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_YIITAO_DES_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYiitao_DESDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CYiitao_DESDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CYiitao_DESDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CYiitao_DESDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CYiitao_DESDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CYiitao_DESDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CYiitao_DESDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CYiitao_DESDlg 消息处理程序

BOOL CYiitao_DESDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CYiitao_DESDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CYiitao_DESDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CYiitao_DESDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CYiitao_DESDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CString message;
	GetDlgItemText(IDC_EDIT1, message);//读入明文
	int size = message.GetLength();
	block = (size - 1) / 8 + 1;//分组长度,全局变量

	CString k;
	GetDlgItemText(IDC_EDIT2, k);
	char* key = k.GetBuffer(0);
	Des_SetSubkey(key);

	char m_block[8] = { 0 };//分组
	char c_block[8] = { 0 };

	for (int i = 0; i < block - 1; i++)//少加了等于
	{
		for (int j = 0; j < 8; j++)
			m_block[j] = message.GetAt(8 * i + j);//获取明文对应分组字符
		Des_Run(c_block, m_block, 1);
		for (int j = 0; j < 8; j++)
			c[8 * i + j] = c_block[j];//密文汇总
	}

	//最后一组填充并加密
	for (int j = 0; j < 8; j++)
		m_block[j] = 0;
	for (int j = 0; j < (size - 1) % 8 + 1; j++)
		m_block[j] = message.GetAt(8 * (block - 1) + j);
	Des_Run(c_block, m_block, 1);
	for (int j = 0; j < 8; j++)
		c[8 * (block - 1) + j] = c_block[j];//把j写成了1
	
	//显示密文
	CString cipher = c;
	SetDlgItemText(IDC_EDIT3, cipher);



}


void CYiitao_DESDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	char m_block[8] = { 0 };
	char c_block[8] = { 0 };
	//开始修改
	CString k;
	GetDlgItemText(IDC_EDIT2, k);
	char* key = k.GetBuffer(0);
	Des_SetSubkey(key);
	CString Cipher;
	GetDlgItemText(IDC_EDIT3, Cipher);
	block = Cipher.GetLength() / 8;//密文只能是8的整数倍



	for (int i = 0; i < block; i++)//block改成了8
	{
		for (int j = 0; j < 8; j++)
			c_block[j] = Cipher[8 * i + j];
		Des_Run(m_block, c_block, 0);
		for (int j = 0; j < 8; j++)
			M[8 * i + j] = m_block[j];

	}

	//显示解密文
	CString message = M;
	SetDlgItemText(IDC_EDIT4, message);

}


void CYiitao_DESDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString empty("");
	SetDlgItemText(IDC_EDIT1, empty);
	SetDlgItemText(IDC_EDIT2, empty);
	SetDlgItemText(IDC_EDIT3, empty);
	SetDlgItemText(IDC_EDIT4, empty);
	SetDlgItemText(IDC_EDIT6, empty);
	SetDlgItemText(IDC_EDIT7, empty);
	memset(m, 0, 1000);
	memset(c, 0, 1000);
	memset(M, 0, 1000);

}


void CYiitao_DESDlg::OnBnClickedButton4()
{
	
	// TODO: 在此添加控件通知处理程序代码
	CString msgRoute,cipherRoute,k;
	GetDlgItemText(IDC_EDIT6, msgRoute);//读入明文文件路径
	GetDlgItemText(IDC_EDIT7, cipherRoute);

	GetDlgItemText(IDC_EDIT2, k);
	char* key = k.GetBuffer(0);
	char* msgFile = msgRoute.GetBuffer(0);
	char* cipherFile = cipherRoute.GetBuffer(0);
	Des_SetSubkey(key);
	Des_File(msgFile, cipherFile);
	
}


void CYiitao_DESDlg::OnBnClickedButton5()
{
	
	// TODO: 在此添加控件通知处理程序代码
	CString msgRoute, cipherRoute, k;
	GetDlgItemText(IDC_EDIT6, msgRoute);//读入明文文件路径
	GetDlgItemText(IDC_EDIT7, cipherRoute);

	GetDlgItemText(IDC_EDIT2, k);
	char* key = k.GetBuffer(0);
	char* msgFile = msgRoute.GetBuffer(0);
	char* cipherFile = cipherRoute.GetBuffer(0);
	Des_SetSubkey(key);
	Des_File_D(msgFile, cipherFile);
	
}
