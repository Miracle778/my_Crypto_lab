
// Yiitao_RSADlg.h : 头文件
//

#pragma once


// CYiitao_RSADlg 对话框
class CYiitao_RSADlg : public CDialogEx
{
// 构造
public:
	CYiitao_RSADlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_YIITAO_RSA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit8();
	afx_msg void OnBnClickedButton1();
	CString Tansform(unsigned char* out, int len);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
