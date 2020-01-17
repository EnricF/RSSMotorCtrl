
// RSSMotorCtrlDlg.h : header file
//

#pragma once


// CRSSMotorCtrlDlg dialog
class CRSSMotorCtrlDlg : public CDialogEx
{
// Construction
public:
	CRSSMotorCtrlDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RSSMOTORCTRL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStaticRampUpperpos();
	afx_msg void OnBnClickedButtonRampSend();
};