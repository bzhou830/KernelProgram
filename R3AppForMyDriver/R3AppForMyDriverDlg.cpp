// R3AppForMyDriverDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "R3AppForMyDriver.h"
#include "R3AppForMyDriverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CR3AppForMyDriverDlg 对话框



CR3AppForMyDriverDlg::CR3AppForMyDriverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_R3APPFORMYDRIVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CR3AppForMyDriverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CR3AppForMyDriverDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_CREATE, &CR3AppForMyDriverDlg::OnBnClickedButtonCreate)
    ON_BN_CLICKED(IDC_BUTTON_STOP, &CR3AppForMyDriverDlg::OnBnClickedButtonStop)
    ON_BN_CLICKED(IDC_BUTTON_DEVICE_CONTROL, &CR3AppForMyDriverDlg::OnBnClickedButtonDeviceControl)
    ON_BN_CLICKED(IDC_BUTTON_READ, &CR3AppForMyDriverDlg::OnBnClickedButtonRead)
    ON_BN_CLICKED(IDC_BUTTON_WRITE, &CR3AppForMyDriverDlg::OnBnClickedButtonWrite)
END_MESSAGE_MAP()


// CR3AppForMyDriverDlg 消息处理程序

BOOL CR3AppForMyDriverDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

void CR3AppForMyDriverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CR3AppForMyDriverDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CR3AppForMyDriverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#define DRIVER_SYMBOLLINK_NAME L"\\??\\MySymbolLinkName"
// 驱动对象句柄
HANDLE hDriver = (HANDLE)-1;

void CR3AppForMyDriverDlg::OnBnClickedButtonCreate()
{
    hDriver = CreateFile(
        DRIVER_SYMBOLLINK_NAME,
        GENERIC_WRITE | GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hDriver != (HANDLE)-1)
    {
        TRACE("R3 获取驱动句柄成功：0x%X", hDriver);
    }
}


void CR3AppForMyDriverDlg::OnBnClickedButtonStop()
{
    if (hDriver != (HANDLE)-1)
    {
        CloseHandle(hDriver);
        TRACE("R3 结束对驱动调用成功");
        hDriver = (HANDLE)-1;
    }
}

#include <winioctl.h>
// 定义的控制码
#define IO_TEST_CODE CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_BUFFERED,FILE_ANY_ACCESS)
void CR3AppForMyDriverDlg::OnBnClickedButtonDeviceControl()
{
    UpdateData(TRUE);
    
    TRACE("JJR：%s", m_R0Text.GetBuffer());
    TRACE("JJR：%s", m_R3Text.GetBuffer());
    char szpTest[256] = { 0 };
    strcpy_s(szpTest, sizeof(szpTest), (char*)m_R3Text.GetBuffer());
    char szpResult[256] = { 0 };
    DWORD nSize = 0;
    if ((DWORD)hDriver != 0xFFFFFFFF)
    {
        if (DeviceIoControl(
            hDriver,
            IO_TEST_CODE,
            szpTest,
            sizeof(szpTest),
            szpResult,
            sizeof(szpResult),
            &nSize,
            NULL
        ))
        {
            m_R0Text.Format(_T("%s"), szpResult);
            UpdateData(FALSE);
        }
    }
}


void CR3AppForMyDriverDlg::OnBnClickedButtonRead()
{
    UpdateData(TRUE);

    char szpTest[256] = { 0 };
    strcpy_s(szpTest, sizeof(szpTest), (char*)m_R3Text.GetBuffer());
    char szpResult[256] = { 0 };
    DWORD nSize = 0;
    if ((DWORD)hDriver != 0xFFFFFFFF)
    {
        if (ReadFile(
            hDriver,
            szpTest,
            strlen(szpTest),
            &nSize,
            NULL
        ))
        {
            TRACE("JJR：执行成功");
        }
    }
}


void CR3AppForMyDriverDlg::OnBnClickedButtonWrite()
{
    UpdateData(TRUE);

    char szpTest[256] = { 0 };
    strcpy_s(szpTest, sizeof(szpTest), (char*)m_R3Text.GetBuffer());
    char szpResult[256] = { 0 };
    DWORD nSize = 0;
    if ((DWORD)hDriver != 0xFFFFFFFF)
    {
        if (WriteFile(
            hDriver,
            szpTest,
            strlen(szpTest),
            &nSize,
            NULL
        ))
        {
            TRACE("JJR：执行成功");
        }
    }
}
