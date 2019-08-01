
// Yiitao_caeserDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Yiitao_caeser.h"
#include "Yiitao_caeserDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CYiitao_caeserDlg 对话框



CYiitao_caeserDlg::CYiitao_caeserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_YIITAO_CAESER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYiitao_caeserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CYiitao_caeserDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CYiitao_caeserDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CYiitao_caeserDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CYiitao_caeserDlg 消息处理程序

BOOL CYiitao_caeserDlg::OnInitDialog()
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

void CYiitao_caeserDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CYiitao_caeserDlg::OnPaint()
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
HCURSOR CYiitao_caeserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CYiitao_caeserDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int check_1 = ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();//选中加密按钮为1，否则为0
	int check_2 = ((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck();//选中解密按钮
	CString message;
	CString cipher;
	CString key;
	GetDlgItemText(IDC_EDIT1, message);//获取文本框内容传给message
	GetDlgItemText(IDC_EDIT3, key);//获取密钥文本框内容传给key
	char *message1 = message.GetBuffer(0);
	char *cipher1;
	int key1 = 0;
	int key2 = 0;
	if (check_1 == 1 && check_2 == 0)//选中加密
	{
		key1 = _ttoi(key) % 26;//将cstring转换成int型
		key2 = _ttoi(key) % 10;
	}
	else if (check_1 == 0 && check_2 == 1)
	{
		key1 = 26 - _ttoi(key) % 26;
		key2 = 10 - _ttoi(key) % 10;

	}
	cipher1 = message1;//默认密文等于明文
	int size = message.GetLength();
	for (int i = 0; i < size; i++)
	{
		if (message1[i] >= 'a'&&message1[i] <= 'z')//小写字母
		{
			if (message1[i] + key1 > 122)//122为‘z'的ascii码
			{
				cipher1[i] = message[i] + key1 - 26;
			}
			else 
			{
				cipher1[i] = message[i] + key1;
			}
		}
		if (message1[i] >= 'A'&&message1[i] <= 'Z')//大写字母
		{
			if (message1[i] + key1 > 90)//90为‘Z'的ascii码
			{
				cipher1[i] = message[i] + key1 - 26;
			}
			else
			{
				cipher1[i] = message[i] + key1;
			}
		}
		if (message1[i] >= '0'&&message[i] <= '9')
		{
			
			if (message1[i] + key2 > 57)//57为‘9'的ascii码
			{
				cipher1[i] = message[i] + key2 - 10;
			}
			else
			{
				cipher1[i] = message[i] + key2;
			}

		}
	}
	cipher = cipher1;
	SetDlgItemText(IDC_EDIT2, cipher);

}


void CYiitao_caeserDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString empty("");
	SetDlgItemText(IDC_EDIT1, empty);
	SetDlgItemText(IDC_EDIT2, empty);
	SetDlgItemText(IDC_EDIT3, empty);
}
