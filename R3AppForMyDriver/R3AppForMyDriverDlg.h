// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。  
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。  
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问 
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// R3AppForMyDriverDlg.h: 头文件
//

#pragma once


// CR3AppForMyDriverDlg 对话框
class CR3AppForMyDriverDlg : public CDialog
{
// 构造
public:
	CR3AppForMyDriverDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_R3APPFORMYDRIVER_DIALOG };
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
    CString m_R0Text, m_R3Text;
    afx_msg void OnBnClickedButtonCreate();
    afx_msg void OnBnClickedButtonStop();
    afx_msg void OnBnClickedButtonDeviceControl();
    afx_msg void OnBnClickedButtonRead();
    afx_msg void OnBnClickedButtonWrite();
};
