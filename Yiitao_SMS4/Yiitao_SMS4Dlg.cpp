
// Yiitao_SMS4Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Yiitao_SMS4.h"
#include "Yiitao_SMS4Dlg.h"
#include "afxdialogex.h"
#include<fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

unsigned int m[1000][4] = { 0 };//明文
unsigned int c[1000][4] = { 0 };//密文
unsigned int M[1000][4] = { 0 };//解密文
//char cipher[1000] = { 0 };

int block = 0;//分组密码分组数

unsigned int rK[32] = { 0 };//轮密钥



const static unsigned int FK[4] = { 0XA3B1BAC6, 0X56AA3350, 0X677D9197, 0XB27022DC };//无符号int，正好32位,系统参数

//固定参数
const static unsigned int CK[32] = {
	0X00070E15, 0X1C232A31, 0X383F464D, 0X545B6269,
	0X70777E85, 0X8C939AA1, 0XA8AFB6BD, 0XC4CBD2D9,
	0XE0E7EEF5, 0XFC030A11, 0X181F262D, 0X343B4249,
	0X50575E65, 0X6C737A81, 0X888F969D, 0XA4ABB2B9,
	0XC0C7CED5, 0XDCE3EAF1, 0XF8FF060D, 0X141B2229,
	0X30373E45, 0X4C535A61, 0X686F767D, 0X848B9299,
	0XA0A7AEB5, 0XBCC3CAD1, 0XD8DFE6ED, 0XF4FB0209,
	0X10171E25, 0X2C333A41, 0X484F565D, 0X646B7279
};

static const /*unsigned*/ char S_Box[256]{   //为什么用char，虽然读不出，但因为转成bit运算，参与运算转换成数型//加了[1]
	0xd6,0x90,0xe9,0xfe,0xcc,0xe1,0x3d,0xb7,0x16,0xb6,0x14,0xc2,0x28,0xfb,0x2c,0x05,
	0x2b,0x67,0x9a,0x76,0x2a,0xbe,0x04,0xc3,0xaa,0x44,0x13,0x26,0x49,0x86,0x06,0x99,
	0x9c,0x42,0x50,0xf4,0x91,0xef,0x98,0x7a,0x33,0x54,0x0b,0x43,0xed,0xcf,0xac,0x62,
	0xe4,0xb3,0x1c,0xa9,0xc9,0x08,0xe8,0x95,0x80,0xdf,0x94,0xfa,0x75,0x8f,0x3f,0xa6,
	0x47,0x07,0xa7,0xfc,0xf3,0x73,0x17,0xba,0x83,0x59,0x3c,0x19,0xe6,0x85,0x4f,0xa8,
	0x68,0x6b,0x81,0xb2,0x71,0x64,0xda,0x8b,0xf8,0xeb,0x0f,0x4b,0x70,0x56,0x9d,0x35,
	0x1e,0x24,0x0e,0x5e,0x63,0x58,0xd1,0xa2,0x25,0x22,0x7c,0x3b,0x01,0x21,0x78,0x87,
	0xd4,0x00,0x46,0x57,0x9f,0xd3,0x27,0x52,0x4c,0x36,0x02,0xe7,0xa0,0xc4,0xc8,0x9e,
	0xea,0xbf,0x8a,0xd2,0x40,0xc7,0x38,0xb5,0xa3,0xf7,0xf2,0xce,0xf9,0x61,0x15,0xa1,
	0xe0,0xae,0x5d,0xa4,0x9b,0x34,0x1a,0x55,0xad,0x93,0x32,0x30,0xf5,0x8c,0xb1,0xe3,
	0x1d,0xf6,0xe2,0x2e,0x82,0x66,0xca,0x60,0xc0,0x29,0x23,0xab,0x0d,0x53,0x4e,0x6f,
	0xd5,0xdb,0x37,0x45,0xde,0xfd,0x8e,0x2f,0x03,0xff,0x6a,0x72,0x6d,0x6c,0x5b,0x51,
	0x8d,0x1b,0xaf,0x92,0xbb,0xdd,0xbc,0x7f,0x11,0xd9,0x5c,0x41,0x1f,0x10,0x5a,0xd8,
	0x0a,0xc1,0x31,0x88,0xa5,0xcd,0x7b,0xbd,0x2d,0x74,0xd0,0x12,0xb8,0xe5,0xb4,0xb0,
	0x89,0x69,0x97,0x4a,0x0c,0x96,0x77,0x7e,0x65,0xb9,0xf1,0x09,0xc5,0x6e,0xc6,0x84,
	0x18,0xf0,0x7d,0xec,0x3a,0xdc,0x4d,0x20,0x79,0xee,0x5f,0x3e,0xd7,0xcb,0x39,0x48
};

unsigned int* SMS4_Run(unsigned int msg[], bool flag);//主要加密函数
//unsigned int F_func(unsigned int M[4] ,unsigned int subkey);//轮函数
void SMS4_SetSubKey(unsigned int In[4]);//密钥扩展,生成全局变量rK[32],子密钥
unsigned int T_func(unsigned int t);//T置换
unsigned int S_func(unsigned int In);
unsigned int L_func(unsigned int In);
unsigned int leftmove(unsigned int In,int len);
unsigned int T1_func(unsigned int In);
unsigned int L1_func(unsigned int In);
void SMS4_File_En(char* msgFile, char* cipherFile);//需加密文件路径，加密生成路径
void SMS4_File_De(char* msgFile, char* cipherFile);

void SMS4_File_En(char* msgFile, char* cipherFile)
{
	char msgBlock[16] = { 0 };
	char cipherBlock[16] = { 0 };
	unsigned int msgTemp[4] = { 0 };
	unsigned int cipherTemp[4] = { 0 };
	ifstream msg(msgFile, ios::binary);
	ofstream cipher(cipherFile, ios::binary);

	while (msg.peek() != EOF)
	{
		memset(msgBlock, 0, 16);//初始化置0
		msg.read(msgBlock, 16);//读完文件指针会后移
		//memcpy(msgTemp, StrToHex(msgBlock), 16);

		memset(msgTemp, 0, 16);		//出错，解密多了填充的0
		//转16进制
		for (int i = 0; i < 16; i++)
			msgTemp[i / 4] += (msgBlock[i] & 0xff) << (24 - (i % 4) * 8);
		memcpy(cipherTemp, SMS4_Run(msgTemp, 1), 16);//加密

		//密文转换字符串
		for (int i = 0; i < 16; i++)
			cipherBlock[i] = (cipherTemp[i / 4] >> (24 - (i % 4) * 8))&0xff;
		cipher.write(cipherBlock, 16);
	}
	msg.close();
	cipher.close();
}

void SMS4_File_De(char* msgFile, char* cipherFile)
{
	char msgBlock[16] = { 0 };
	char cipherBlock[16] = { 0 };   //错误，解密之后文件有填充的0
	unsigned int msgTemp[4] = { 0 };
	unsigned int cipherTemp[4] = { 0 };
	ifstream cipher(cipherFile, ios::binary);
	ofstream msg(msgFile, ios::binary);
	
	//int flag = 0;//尝试解决解密后多余0错误


	while (cipher.peek() != EOF)
	{
		memset(cipherBlock, 0, 16);
		cipher.read(cipherBlock, 16);//密文必为16整数
		memset(cipherTemp, 0, 16);
		for (int i = 0; i < 16; i++)//转16进制解密
			cipherTemp[i / 4] += (cipherBlock[i] & 0xff) << (24 - 8 * (i % 4));
		memcpy(msgTemp, SMS4_Run(cipherTemp, 0), 16);//解密
		for (int i = 0; i < 16; i++)
			msgBlock[i] = (msgTemp[i / 4] >> (24 - 8 * (i % 4))) & 0xff;
		/*
		flag = 0;
		for (int i = 0; i < 16; i++)
			if (msgBlock[i] == 0)
				flag++;
		*/
		msg.write((char*)&msgBlock, 16);
	}
	msg.close();
	cipher.close();
}

/*
unsigned int* StrToHex(char Str[16])
{
	unsigned int Out[4];
	for (int i = 0; i < 16; i++)
		Out[i / 4] += ((Str[i]&0xff) << (24 - (i%4) * 8));//生成十六进制数 //错误改成%
	return Out;
}

*/


unsigned int* SMS4_Run(unsigned int msg[] , bool flag)
{
	unsigned int X[36] = { 0 };
	//初始化
	for (int i = 0; i < 4; i++)
	{
		X[i] = msg[i];
	}
	for (int i = 0; i < 32; i++)
	{
		if (flag)
			X[i + 4] = X[i] ^ (T_func(X[i + 1] ^ X[i + 2] ^ X[i + 3] ^ rK[i]));
		else
			X[i + 4] = X[i] ^ (T_func(X[i + 1] ^ X[i + 2] ^ X[i + 3] ^ rK[31 - i]));
	}
	unsigned int cipher[4] = { 0 };
	for (int i = 0; i < 4; i++)
		cipher[i] = X[35 - i];
	return cipher;
}

unsigned int T_func(unsigned int In)//S函数和L函数，输入32，输出32
{
	unsigned int Out = 0;//养成良好习惯
	Out = S_func(In);
	Out = L_func(Out);
	return Out;
}

unsigned int S_func(unsigned int In)
{
	unsigned int Out = 0;
	/*unsigned*/ char Temp = { 0 };
	for (int i = 0; i < 4; i++)
	{
		Temp = ((In >> (24 - 8 * i)) & 0xFF);//8 bit保存在Temp中,由高位到低位
		Out = Out + (S_Box[Temp] << (24 - 8 * i));
		//上一步是由高位到低位，因此此处右移扩大，迎合高位到低位，
		//S_Box index为什么不需减1，因为8bit 最高1111 1111，从0开始计数
	}
	return Out;
}

unsigned int L_func(unsigned int In)
{
	unsigned int Out = 0;
	Out = In^leftmove(In, 2) ^ leftmove(In, 10) 
		^ leftmove(In, 18) ^ leftmove(In, 24);
	return Out;
}

unsigned int leftmove(unsigned int In, int len)
{
	return (In << len) | (In >> (32 - len));//循环移位
}

void SMS4_SetSubKey(unsigned int In[4])
{
	unsigned int K[36];//中间变量
	for (int i = 0; i < 4; i++)
		K[i] = In[i] ^ FK[i];
	for (int i = 0; i < 32; i++)
	{
		K[i + 4] = K[i] ^ T1_func(K[i + 1] ^ K[i + 2] ^ K[i + 3] ^ CK[i]);
		rK[i] = K[i + 4];
	}
}

unsigned int T1_func(unsigned int In)
{
	unsigned int Out = 0;
	Out = S_func(In);
	Out = L1_func(Out);
	return Out;
}

unsigned int L1_func(unsigned int In)
{
	return In^leftmove(In, 13) ^ leftmove(In, 23);
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


// CYiitao_SMS4Dlg 对话框



CYiitao_SMS4Dlg::CYiitao_SMS4Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_YIITAO_SMS4_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYiitao_SMS4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CYiitao_SMS4Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_BUTTON1, &CYiitao_SMS4Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CYiitao_SMS4Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CYiitao_SMS4Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CYiitao_SMS4Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CYiitao_SMS4Dlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CYiitao_SMS4Dlg 消息处理程序

BOOL CYiitao_SMS4Dlg::OnInitDialog()
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

void CYiitao_SMS4Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CYiitao_SMS4Dlg::OnPaint()
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
HCURSOR CYiitao_SMS4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CYiitao_SMS4Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//加密
	//明文的处理
	
	CString message;
	GetDlgItemText(IDC_EDIT1, message);

	//char* msg = message.GetBuffer(0);

	char * char_m = message.GetBuffer(0);//出错更改，原语句char *char_m

	//char char_m[] = { 0 };
	//char char_m[1000] = { 0 };
	int len = message.GetLength();
	block = (len - 1) / 16 + 1;//分块

	//更改,错误更改，改完问题还是出现
	//for (int i = 0; i < len; i++)
	//	char_m[i] = message.GetAt(i);
	/*
	if (len % 16 != 0)	//填充
		memset(msg + len, 0, block * 16 - len);
	for (int i = 0; i < block; i++)
		memcpy(m[i], StrToHex(msg + i * 16), 16);//将明文十六进制形式存在m中，block组
	*/

	for (int i = 0; i < block - 1; i++)
	{
		for (int j = 0; j < 4; j++)
			for (int t = 0; t < 4; t++)
				m[i][j] += (char_m[16 * i + 4 * j + t] & 0xff) << (24 - 8 * t);//本来中文加密错误，加了&0xff改正
														//未加&之前，‘这个’的十六进制是d5e2 b8f6,未加的话，程序算成d4e2 b7f6
	}
	//最后一组处理，不需填充，/*m数组中初始化0，*/ 只需控制char_m不越界//出错导致16整数倍加解密错误
	//memset(char_m + len, 0, 16 * block - len);  //出错，
	for (int j = 0; j < 4; j++)
	{
		for (int t = 0; t < 4; t++)
		{
			if (16 * (block - 1) + 4 * j + t < len)//自动完成填充
				m[block - 1][j] += (char_m[16 * (block - 1) + 4 * j + t] & 0xff) << (24 - 8 * t);//本来中文加密错误，加了&0xff改正
		}
	}



	//密钥的处理
	CString k;
	unsigned int SubKey[4] = { 0 };
	GetDlgItemText(IDC_EDIT2, k);
	char* key = k.GetBuffer(0);
	/*if (int k_len = k.GetLength() != 16)
		memset(key + k_len, 0, 16 - k_len);//密钥不足填充*/
	for (int i = 0; i < 16; i++)
		SubKey[i / 4] += key[i] << (24 - 8 * (i % 4));
	SMS4_SetSubKey(SubKey);//设置密钥


	/*for (int i = 0; i < block; i++)
		memcpy(c[i], SMS4_Run(m[i], 1), 16);//密文放在c里*/


	//密文处理
	CString Cipher;
	char Temp;
	for (int i = 0; i < block; i++)
	{
		memcpy(c[i], SMS4_Run(m[i], 1), 16);
		for (int j = 0; j < 4; j++)
		{
			for (int t = 0; t < 4; t++)
			{
				//cipher[i * 4 + j * 4 + t] = ((c[i][j] >> (24 - 8 * t)) & 0xFF);//转字符串
				Temp = (c[i][j] >> (24 - 8 * t)) & 0xFF;
				Cipher = Cipher + Temp;
			}
		}
	}
	//CString Cipher = cipher;
	SetDlgItemText(IDC_EDIT3, Cipher);
}

void CYiitao_SMS4Dlg::OnBnClickedButton2()
{
	CString message;
	char Temp;
	for (int i = 0; i < block; i++)
	{
		memcpy(M[i], SMS4_Run(c[i],0), 16);//拷贝16字节，4个unsigned int
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				Temp = (M[i][j] >> (24 - 8 * k)) & 0xFF;
				message = message + Temp;			//中文解密出错，M和m都一样，为什么出错
			}
		}
	}
	SetDlgItemText(IDC_EDIT4, message);
}

void CYiitao_SMS4Dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//清空
	CString empty = "";
	SetDlgItemText(IDC_EDIT1, empty);
	SetDlgItemText(IDC_EDIT2, empty);
	SetDlgItemText(IDC_EDIT3, empty);
	SetDlgItemText(IDC_EDIT4, empty);
	SetDlgItemText(IDC_EDIT5, empty);
	SetDlgItemText(IDC_EDIT6, empty);
	memset(m, 0, 4 * 4 * 1000);
	memset(M, 0, 4 * 4 * 1000);
	memset(c, 0, 4 * 4 * 1000);
	memset(rK, 0, 4 * 32);
}


void CYiitao_SMS4Dlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//密钥的处理
	CString k;
	unsigned int SubKey[4] = { 0 };
	GetDlgItemText(IDC_EDIT2, k);
	char* key = k.GetBuffer(0);
	for (int i = 0; i < 16; i++)
		SubKey[i / 4] += key[i] << (24 - 8 * (i % 4));
	SMS4_SetSubKey(SubKey);//设置密钥

	CString msgFile;
	CString cipherFile;
	GetDlgItemText(IDC_EDIT5, msgFile);
	GetDlgItemText(IDC_EDIT6, cipherFile);

	char *msg = msgFile.GetBuffer(0);
	char *cipher = cipherFile.GetBuffer(0);

	SMS4_File_En(msg, cipher);

}


void CYiitao_SMS4Dlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	//密钥的处理
	CString k;
	unsigned int SubKey[4] = { 0 };
	GetDlgItemText(IDC_EDIT2, k);
	char* key = k.GetBuffer(0);
	for (int i = 0; i < 16; i++)
		SubKey[i / 4] += key[i] << (24 - 8 * (i % 4));
	SMS4_SetSubKey(SubKey);//设置密钥

	CString msgFile;
	CString cipherFile;
	GetDlgItemText(IDC_EDIT5, msgFile);
	GetDlgItemText(IDC_EDIT6, cipherFile);

	char *msg = msgFile.GetBuffer(0);
	char *cipher = cipherFile.GetBuffer(0);

	SMS4_File_De(msg, cipher);

}
