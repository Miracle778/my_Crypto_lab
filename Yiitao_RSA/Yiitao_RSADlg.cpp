
// Yiitao_RSADlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Yiitao_RSA.h"
#include "Yiitao_RSADlg.h"
#include "afxdialogex.h"
#include "Bigint.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Bigint p = { 0 };
Bigint q = { 0 };
Bigint e = { 0 };
Bigint d = { 0 };
Bigint PhiN = { 0 };
Bigint N = { 0 };
Bigint c = { 0 };
Bigint m = { 0 };
Bigint M = { 0 };




void PrintNum(unsigned char *out, int &len, Bigint a)
{
	//修改
	len = 0;
	unsigned char temp[2000] = { 0 };
	//unsigned int temp[2000] = { 0 };
	Bigint b = { 0 };
	Bigint c = { 10 };
	while (Compare(a, b) == 1)			//a>0
	{
		temp[len] = Mod(a, c).num[0];
		a = Div(a, c);
		len++;
	}
	
	int j = 0;
	for (int i = len - 1; i >= 0; i--)
	{
		out[j] = temp[i];
		j++;
	}
}

Bigint GCD(Bigint a, Bigint b)		//辗转相除法
{
	Bigint c = { 0 };
	while (getLength(a) > 0)
	{
		Copy(c, Mod(b, a));
		Copy(b, a);
		Copy(a, c);
	}
	return b;
}
Bigint GenE(Bigint PhiN)			//生成公钥e
{
	Bigint e = BigRand(PhiN);		//e取1 - PhiN
	Bigint g = GCD(PhiN, e);
	while (getLength(g) != 1 || g.num[0] != 1)		//g是最大公因数，循环至g=1结束
	{
		e = BigRand(PhiN);
		g = GCD(PhiN, e);
	}
	return e;
}

bool GenD(Bigint e, Bigint PhiN, Bigint &d)
{
	if (Inverse(e, PhiN, d))
		return 1;
	else
		return 0;
}

Bigint Encrypt(Bigint m, Bigint e, Bigint n)
{
	return PowMod(m, e, n);

}
Bigint Decrypt(Bigint c, Bigint d, Bigint n)
{
	return PowMod(c, d, n);
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


// CYiitao_RSADlg 对话框



CYiitao_RSADlg::CYiitao_RSADlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_YIITAO_RSA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYiitao_RSADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CYiitao_RSADlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_EN_CHANGE(IDC_EDIT8, &CYiitao_RSADlg::OnEnChangeEdit8)
	ON_BN_CLICKED(IDC_BUTTON1, &CYiitao_RSADlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CYiitao_RSADlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CYiitao_RSADlg::OnBnClickedButton3)
END_MESSAGE_MAP()


//修改
CString CYiitao_RSADlg::Tansform(unsigned char * Out, int len)		//返回CString,转成十进制输出
{
	CString res;
	CString Temp;
	for (int i = 0; i < len; i++)
	{
		Temp.Format(_T("%u"), Out[i]);
		res = res + Temp;
	}
	return res;
}

// CYiitao_RSADlg 消息处理程序

BOOL CYiitao_RSADlg::OnInitDialog()
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

void CYiitao_RSADlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CYiitao_RSADlg::OnPaint()
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
HCURSOR CYiitao_RSADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CYiitao_RSADlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString P, Q, N, E, D;
	//Bigint p, q, n, e, d;
	srand(time(NULL));
	p = GenPrime(16);			//先来 N 256比特
	q = GenPrime(16);				//SIZE越短，加密越快			//SIZE 65,512比特需要4分50秒,256比特3分多
	N = Narrow(Mul(p, q));				//SIZE 33 256比特 50s
	int len = 0;

	//修改
	
	unsigned char Out1[2000] = { 0 };
	unsigned char Out2[2000] = { 0 };
	unsigned char Out3[2000] = { 0 };
	unsigned char Out4[2000] = { 0 };
	unsigned char Out5[2000] = { 0 };
	/*
	unsigned int Out1[2000] = { 0 };
	unsigned int Out2[2000] = { 0 };
	unsigned int Out3[2000] = { 0 };
	unsigned int Out4[2000] = { 0 };
	unsigned int Out5[2000] = { 0 };*/
	CString res;
	//输出p
	PrintNum(Out1, len, p);
	res = CYiitao_RSADlg::Tansform(Out1, len);		//自己定义的
	SetDlgItemText(IDC_EDIT1, res);

	//输出q
	PrintNum(Out2, len, q);							//len传入之后会重置0
	res = CYiitao_RSADlg::Tansform(Out2, len);		
	SetDlgItemText(IDC_EDIT2, res);

	//输出N
	PrintNum(Out3, len, N);
	res = CYiitao_RSADlg::Tansform(Out3, len);
	SetDlgItemText(IDC_EDIT3, res);

	//生成e,输出e
	Bigint one = { 1 };
	PhiN = Narrow(Mul(Sub(p, one), Sub(q, one)));//计算PhiN
	e = GenE(PhiN);								//生成e
	PrintNum(Out4, len, e);
	res = CYiitao_RSADlg::Tansform(Out4, len);
	SetDlgItemText(IDC_EDIT4, res);

	//生成d,输出d
	Inverse(e, PhiN, d);
	PrintNum(Out5, len, d);
	res = CYiitao_RSADlg::Tansform(Out5, len);
	SetDlgItemText(IDC_EDIT5, res);

}


void CYiitao_RSADlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//加密
	CString msg;
	GetDlgItemText(IDC_EDIT6, msg);
	char *message = msg.GetBuffer(0);
	int len = msg.GetLength();
	for (int i = 0; i < len; i++)
	{
		m.num[i] = message[i];			//m全局变量，Bigint型,待检查中文是否可行
	}
	c = Encrypt(m, e, N);				//体现为什么用全局变量

	CString Temp;
	Temp.Format(_T("%s"), c.num);
	SetDlgItemText(IDC_EDIT7, Temp);
}


void CYiitao_RSADlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	M = Decrypt(c, d, N);
	CString Temp;
	Temp.Format(_T("%s"), M.num);
	SetDlgItemText(IDC_EDIT8, Temp);
}
