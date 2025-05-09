
// RestmanDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Restman.h"
#include "RestmanDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CRestmanDlg 대화 상자



CRestmanDlg::CRestmanDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RESTMAN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRestmanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_API, m_tree_api);
	DDX_Control(pDX, IDC_SPLITTER, m_splitter);
	DDX_Control(pDX, IDC_COMBO_VERB, m_combo_verb);
	DDX_Control(pDX, IDC_EDIT_URL, m_edit_url);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_button_send);
	DDX_Control(pDX, IDC_LIST_PARAMS, m_list_params);
}

BEGIN_MESSAGE_MAP(CRestmanDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CRestmanDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRestmanDlg::OnBnClickedCancel)
	ON_WM_WINDOWPOSCHANGED()
	ON_NOTIFY(NM_RCLICK, IDC_TREE_API, &CRestmanDlg::OnNMRClickTreeApi)
	ON_COMMAND(ID_TREE_MENU_ADD_FOLDER, &CRestmanDlg::OnTreeMenuAddFolder)
	ON_COMMAND(ID_TREE_MENU_ADD_REQUEST, &CRestmanDlg::OnTreeMenuAddRequest)
	ON_COMMAND(ID_TREE_MENU_ADD_COLLECTION, &CRestmanDlg::OnTreeMenuAddCollection)
	ON_REGISTERED_MESSAGE(Message_CSCTreeCtrl, &CRestmanDlg::on_message_CSCTreeCtrl)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CRestmanDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CRestmanDlg 메시지 처리기

BOOL CRestmanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//CRequestUrlParams param(_T("admin.linkmemine.com"), 443, _T("lmm/api/v1.0/"));
	//request_url(&param);

	m_resize.Create(this);
	m_resize.Add(IDC_TREE_API, 0, 0, 0, 100);
	m_resize.Add(IDC_SPLITTER, 0, 0, 0, 100);
	m_resize.Add(IDC_COMBO_VERB, 0, 0, 0, 0);
	m_resize.Add(IDC_EDIT_URL, 0, 0, 100, 0);
	m_resize.Add(IDC_BUTTON_SEND, 100, 0, 0, 0);
	m_resize.Add(IDC_LIST_PARAMS, 0, 0, 100, 0);

	int min_size = 60;
	m_splitter.set_type(CControlSplitter::CS_VERT, true, Gdiplus::Color::LightGray);
	m_splitter.AddToTopOrLeftCtrls(IDC_TREE_API);
	m_splitter.AddToBottomOrRightCtrls(IDC_COMBO_VERB, 0, 0, SPF_LEFT);
	m_splitter.AddToBottomOrRightCtrls(IDC_EDIT_URL, min_size, 0, SPF_LEFT | SPF_RIGHT);
	m_splitter.AddToBottomOrRightCtrls(IDC_LIST_PARAMS, 0, 0, SPF_LEFT | SPF_RIGHT);

	m_combo_verb.set_font_size(9);
	m_combo_verb.set_line_height(14);
	m_combo_verb.set_font_bold(FW_HEAVY);
	m_combo_verb.add(_T("GET"), Gdiplus::Color(18, 135, 63));
	m_combo_verb.add(_T("POST"), Gdiplus::Color(182, 137, 32));
	m_combo_verb.add(_T("PUT"), Gdiplus::Color(39, 109, 195));
	m_combo_verb.SetCurSel(0);

	m_edit_url.set_text_color(Gdiplus::Color::RoyalBlue);
	m_edit_url.set_back_color(Gdiplus::Color::White);
	m_edit_url.set_line_align(DT_BOTTOM);
	m_edit_url.set_auto_font_size(true, 0.60f);
	m_edit_url.set_dim_text(_T("Input Url here..."));

	m_list_params.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FLATSB);
	m_list_params.set_headings(_T("Key,200;Value,200;Description,200"));
	m_list_params.set_header_height(32);
	m_list_params.get_header_ctrl()->set_text_color(Gdiplus::Color::Gray);
	m_list_params.get_header_ctrl()->set_font_bold(FW_SEMIBOLD);
	m_list_params.get_header_ctrl()->use_header_separator(false);
	m_list_params.set_line_height(32);
	m_list_params.allow_sort(false);
	m_list_params.allow_edit();
	m_list_params.add_item(_T("key name"));

	m_tree_api.set_imagelist(IDI_EMPTY, IDI_FOLDER_COLLAPSED, IDI_FOLDER_EXPANDED);
	m_tree_api.load(get_exe_directory() + _T("\\api_data.txt"));

	RestoreWindowPosition(&theApp, this);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CRestmanDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CRestmanDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CRestmanDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRestmanDlg::OnBnClickedOk()
{
}


void CRestmanDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_tree_api.save(get_exe_directory() + _T("\\api_data.txt"));

	CDialogEx::OnCancel();
}

void CRestmanDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveWindowPosition(&theApp, this);
}

void CRestmanDlg::OnNMRClickTreeApi(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM hItem = NULL;// = pNMTreeView->itemNew.hItem;	//얻어오지 못한다.
	CPoint pt;
	::GetCursorPos(&pt);
	m_tree_api.ScreenToClient(&pt);
	hItem = m_tree_api.HitTest(pt);

	TRACE(_T("label = %s\n"), m_tree_api.GetItemText(hItem));

	//우클릭을 하면 일단 해당 노드를 선택상태로 만들어줘야 한다.
	m_tree_api.SelectItem(hItem);

	CMenu menu;
	menu.LoadMenu(IDR_MENU_TREE_CONTEXT);

	CMenu* pMenu = menu.GetSubMenu(0);

	//collection은 최상위 루트로 생성되므로 어떤 항목이 선택된 상태에서는 disable로 표시된다.
	pMenu->EnableMenuItem(ID_TREE_MENU_ADD_COLLECTION, (hItem ? MF_DISABLED : MF_ENABLED));

	//folder는 반드시 Collection의 하위노드로만 생성된다.
	pMenu->EnableMenuItem(ID_TREE_MENU_ADD_FOLDER, (m_tree_api.get_indent_level(hItem) == 0 ? MF_ENABLED : MF_DISABLED));

	//request는 반드시 folder의 하위노드로만 생성된다.
	pMenu->EnableMenuItem(ID_TREE_MENU_ADD_REQUEST, (m_tree_api.get_indent_level(hItem) == 1 ? MF_ENABLED : MF_DISABLED));

	::GetCursorPos(&pt);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

	*pResult = 0;
}

void CRestmanDlg::OnTreeMenuAddCollection()
{
	HTREEITEM parent = m_tree_api.GetSelectedItem();
	m_tree_api.InsertItem(_T("New collection"), parent);
	//TV_INSERTSTRUCT tvInsert;
	//tvInsert.item.mask = TVIF_TEXT;
	//tvInsert.hInsertAfter = TVI_LAST;
	//tvInsert.hParent = parent;
	//tvInsert.item.pszText = _T("New collection");
	//m_tree_api.InsertItem(&tvInsert);
}

void CRestmanDlg::OnTreeMenuAddFolder()
{
	HTREEITEM parent = m_tree_api.GetSelectedItem();
	HTREEITEM hItem = m_tree_api.InsertItem(_T("New folder"), 1, 1, parent);

	m_tree_api.Expand(parent, TVE_EXPAND);
}

void CRestmanDlg::OnTreeMenuAddRequest()
{
	HTREEITEM parent = m_tree_api.GetSelectedItem();
	m_tree_api.InsertItem(_T("New request"), 2, 2, parent);
}

LRESULT CRestmanDlg::on_message_CSCTreeCtrl(WPARAM wParam, LPARAM lParam)
{
	auto msg = (CSCTreeCtrlMessage*)wParam;

	if (msg->message == CSCTreeCtrl::message_edit_item)
	{
		m_tree_api.edit_item();
	}

	return 0;
}

void CRestmanDlg::OnBnClickedButtonSend()
{
	m_tree_api.iterate_tree_in_order();
}
