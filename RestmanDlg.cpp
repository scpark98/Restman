
// RestmanDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Restman.h"
#include "RestmanDlg.h"
#include "afxdialogex.h"

#include <wininet.h>
#pragma comment(lib, "wininet.lib")

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
	DDX_Control(pDX, IDC_TREE_API, m_tree);
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
	ON_WM_TIMER()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_API, &CRestmanDlg::OnTvnSelchangedTreeApi)
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
	m_edit_url.set_text(_T("https://admin.linkmemine.com/lmm/api/v1.0/"));

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

	m_tree.set_imagelist(IDI_EMPTY, IDI_FOLDER_COLLAPSED, IDI_FOLDER_EXPANDED);
	//m_tree.load(get_exe_directory() + _T("\\api_data.txt"));

	//tree에서 json을 load하려면 Collection, Folder, APIs 들을 읽어온 후
	//해당 노드에 데이터를 어떻게 저장할 것인지가 명확해야 한다.
	//struct는 애플리케이션마다 다를 것이다.
	//postman에서는 Collection마다 json으로 export하므로 json 파일도 Collection마다 읽어온다.
	//bin/data 폴더의 모든 json을 읽어온다. 각 json은 하나의 Collection이다.
	//각 노드는 struct로 생성하고 각 트리에 SetItemData를 통해 저장한다.

	RestoreWindowPosition(&theApp, this);

	SetTimer(timer_find_json_files, 1000, NULL);

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
	save_json();

	release_node_data();

	CDialogEx::OnCancel();
}

void CRestmanDlg::release_node_data(HTREEITEM hItem)
{
	if (hItem)
	{
		TRACE(_T("release node data : %s\n"), m_tree.GetItemText(hItem));
		CApiNode* data = (CApiNode*)m_tree.GetItemData(hItem);
		delete data;

		hItem = m_tree.GetNextItem(hItem, TVGN_CHILD);
		while (hItem)
		{
			release_node_data(hItem);
			hItem = m_tree.GetNextItem(hItem, TVGN_NEXT);
		}
	}
	else
	{
		HTREEITEM hItem = m_tree.GetNextItem(NULL, TVGN_ROOT);
		while (hItem)
		{
			release_node_data(hItem);
			hItem = m_tree.GetNextItem(hItem, TVGN_NEXT);
		}
	}
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
	m_tree.ScreenToClient(&pt);
	hItem = m_tree.HitTest(pt);

	TRACE(_T("label = %s\n"), m_tree.GetItemText(hItem));

	//우클릭을 하면 일단 해당 노드를 선택상태로 만들어줘야 한다.
	m_tree.SelectItem(hItem);

	CMenu menu;
	menu.LoadMenu(IDR_MENU_TREE_CONTEXT);

	CMenu* pMenu = menu.GetSubMenu(0);

	//indent level로 노드 생성 가능 여부를 판단하려 했으나 folder는 계속 중첩이 가능하므로 다른 방법을 사용해야 한다.
	//규칙1. hItem = -1, 즉 최상위 루트에서만	collection을 생성할 수 있다.
	//규칙2. folder는 Collection 또는 상위폴더의 하위노드로만 생성된다.
	//규칙3. request도 Collection 또는 폴더의 하위노드로만 생성된다.
	//parent가 collection인지 folder인지를 구분하기 위해 image_index를 참조한다.
	//int indent_level = m_tree.get_indent_level(hItem);
	int parent_image_index = m_tree.get_image_index(hItem);

	//collection은 최상위 루트로 생성되므로 어떤 항목이 선택된 상태에서는 disable로 표시된다.
	pMenu->EnableMenuItem(ID_TREE_MENU_ADD_COLLECTION, (hItem ? MF_DISABLED : MF_ENABLED));

	//folder는 Collection 또는 상위폴더의 하위노드로만 생성된다.
	pMenu->EnableMenuItem(ID_TREE_MENU_ADD_FOLDER, ((parent_image_index >= 0 && parent_image_index < 2) ? MF_ENABLED : MF_DISABLED));

	//request도 Collection 또는 폴더의 하위노드로만 생성된다.
	pMenu->EnableMenuItem(ID_TREE_MENU_ADD_REQUEST, ((parent_image_index >= 0 && parent_image_index < 2) ? MF_ENABLED : MF_DISABLED));

	::GetCursorPos(&pt);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

	*pResult = 0;
}

void CRestmanDlg::OnTreeMenuAddCollection()
{
	HTREEITEM parent = m_tree.GetSelectedItem();
	m_tree.InsertItem(_T("New collection"), parent);
	//TV_INSERTSTRUCT tvInsert;
	//tvInsert.item.mask = TVIF_TEXT;
	//tvInsert.hInsertAfter = TVI_LAST;
	//tvInsert.hParent = parent;
	//tvInsert.item.pszText = _T("New collection");
	//m_tree.InsertItem(&tvInsert);
}

void CRestmanDlg::OnTreeMenuAddFolder()
{
	HTREEITEM parent = m_tree.GetSelectedItem();
	HTREEITEM hItem = m_tree.InsertItem(_T("New folder"), 1, 1, parent);

	m_tree.Expand(parent, TVE_EXPAND);
}

void CRestmanDlg::OnTreeMenuAddRequest()
{
	HTREEITEM parent = m_tree.GetSelectedItem();
	m_tree.InsertItem(_T("New request"), 2, 2, parent);
}

LRESULT CRestmanDlg::on_message_CSCTreeCtrl(WPARAM wParam, LPARAM lParam)
{
	auto msg = (CSCTreeCtrlMessage*)wParam;

	if (msg->message == CSCTreeCtrl::message_edit_item)
	{
		m_tree.edit_item();
	}

	return 0;
}

void CRestmanDlg::OnBnClickedButtonSend()
{
	CString full_url = m_edit_url.get_text();
	CString verb = m_combo_verb.get_cur_text();

	CRequestUrlParams param(full_url, verb);
	param.timeout_ms = 5000;
	request_url(&param);
}

void CRestmanDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == timer_find_json_files)
	{
		KillTimer(timer_find_json_files);

		find_json_files();
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CRestmanDlg::find_json_files()
{
	release_node_data();
	m_tree.DeleteAllItems();

	//data 폴더의 각 json 파일은 하나의 Collection 단위로 저장된다.
	int i;
	std::deque<CString> dq;

	FindAllFiles(get_exe_directory() + _T("\\data"), &dq, _T("*"), _T("*.json"));

	for (i = 0; i < dq.size(); i++)
	{
		load_json(dq[i]);
	}

	m_tree.expand_all();
}

void CRestmanDlg::load_traverse(const rapidjson::Value& v, const CString& indent)
{
	if (v.IsObject())
	{
		for (auto& m : v.GetObj())
		{
			Trace_only(_T("1.%s \"%s\":"), indent, m.name.GetCString());
			load_traverse(m.value, indent + _T("  "));
		}
	}
	else if (v.IsArray()) {
		Trace_only(_T("2-1.%s[\n"), indent);
		for (int i = 0; i < v.Size(); i++) {
			load_traverse(v[i], indent + _T("  "));
		}
		Trace_only(_T("2-2.%s]\n"), indent);
	}
	else if (v.IsString()) {
		Trace_only(_T("3.\"%s\"\n"), v.GetCString());
	}
	/*
	else if (v.IsBool()) {
		//std::cout << (v.GetBool() ? "true" : "false") << "\n";
	}
	else if (v.IsInt()) {
		//std::cout << v.GetInt() << "\n";
	}
	else if (v.IsUint()) {
		std::cout << v.GetUint() << "\n";
	}
	else if (v.IsInt64()) {
		std::cout << v.GetInt64() << "\n";
	}
	else if (v.IsUint64()) {
		std::cout << v.GetUint64() << "\n";
	}
	else if (v.IsDouble()) {
		std::cout << v.GetDouble() << "\n";
	}
	else if (v.IsNull()) {
		std::cout << "null\n";
	}
	*/
}

void CRestmanDlg::load_json(CString json_path)
{
	//Json json;

	//if (json.load(json_path) == false)
	//	return;

	//load_traverse(json.doc);
#if 1
	int i;
	Json json;
	
	if (json.load(json_path) == false)
		return;

	CString all_json = json.get_all_data();

	std::deque<CString> lines;
	get_token_string(all_json, lines, _T("\n"));

	auto node = new CApiNode();
	bool is_collection = true;
	bool is_request = false;
	HTREEITEM hParent = TVI_ROOT;
	HTREEITEM hCollection = NULL;

	for (i = 0; i < lines.size(); i++)
	{
		//=Object는 한 노드 단위의 시작을 알림.
		if (lines[i].Mid(lines[i].Find('=') + 1) == _T("Object"))
		{
			//맨 처음 Object를 만난 경우는 name이 당연히 empty이므로 continue;
			if (node->name.IsEmpty())
				continue;

			//맨 처음 collection 정보일 경우
			if (is_collection)
			{
				node->type = node_collection;

				HTREEITEM hItem = m_tree.InsertItem(node->name, node->type, node->type, hParent);
				m_tree.SetItemData(hItem, (DWORD_PTR)node);
				hParent = hCollection = hItem;

				is_collection = false;
				node = new CApiNode();
			}
			//request인 경우
			else if (is_request)
			{
				//store request data
				TRACE(_T("request = %s, url = %s\n"), node->name, node->url);
				node->type = node_request;

				HTREEITEM hItem = m_tree.InsertItem(node->name, node->type, node->type, hParent);
				m_tree.SetItemData(hItem, (DWORD_PTR)node);

				is_request = false;
				node = new CApiNode();
			}
			//folder인 경우
			else if (lines[i].Find(_T("/request]=Object")) < 0)
			{
				//folder인데 root인 collection의 바로 하위 폴더로 다시 올라가는 경우
				if (get_char_count(lines[i], '/') == 1 && lines[i].Find(_T("item[0]") > 0))
				{
					hParent = hCollection;
					//continue;
				}

				if (node->name.IsEmpty())
					continue;

				TRACE(_T("folder = %s\n"), node->name);
				//store folder data
				node->type = node_folder;

				HTREEITEM hItem = m_tree.InsertItem(node->name, node->type, node->type, hParent);
				m_tree.SetItemData(hItem, (DWORD_PTR)node);
				hParent = hItem;

				node = new CApiNode();
			}
		}
		else if (lines[i].Find(_T("/name]")) > 0)
		{
			node->name = lines[i].Mid(lines[i].Find('=') + 1);
		}
		else if (lines[i].Find(_T("/description]")) > 0)
		{
			node->desc = lines[i].Mid(lines[i].Find('=') + 1);
		}
		else if (lines[i].Find(_T("/url]")) > 0)
		{
			is_request = true;
			node->url = lines[i].Mid(lines[i].Find('=') + 1);
		}
		else if (lines[i].Find(_T("/method]")) > 0)
		{
			node->method = lines[i].Mid(lines[i].Find('=') + 1);
		}
		else if (lines[i].Find(_T("/header]")) > 0)
		{
			node->header = lines[i].Mid(lines[i].Find('=') + 1);
		}
		else if (lines[i].Find(_T("/body]")) > 0)
		{
			node->body = lines[i].Mid(lines[i].Find('=') + 1);
		}
		else
		{
			TRACE(_T("not classified. lines[%d] = %s\n"), i, lines[i]);
		}
	}

	//맨 마지막 데이터 추가
	if (is_request)
	{
		node->type = node_request;

		HTREEITEM hItem = m_tree.InsertItem(node->name, node->type, node->type, hParent);
		m_tree.SetItemData(hItem, (DWORD_PTR)node);
	}
	else //folder인 경우
	{
		node->type = node_folder;

		HTREEITEM hItem = m_tree.InsertItem(node->name, node->type, node->type, hParent);
		m_tree.SetItemData(hItem, (DWORD_PTR)node);
	}
#endif
}

//현재 구조를 json 파일로 저장한다. json_path = _T("")이면 data/api.json에 덮어쓴다.
//현재는 개발단계이므로 api.json에 덮어쓰지 않고 data/saved/api_saved.json에 따로 저장한다.
void CRestmanDlg::save_json(CString json_path)
{
	if (json_path.IsEmpty())
		json_path.Format(_T("%s\\data\\saved\\api_saved.json"), get_exe_directory());

	HTREEITEM hItem = m_tree.GetRootItem();

	/*
	Trace(_T("\n"));

	Json json;
	//rapidjson::Document doc;
	json.doc.SetObject();

	CApiNode* data = (CApiNode*)m_tree.GetItemData(hItem);

	rapidjson::Value items(rapidjson::kArrayType);

	if (data)
	{
		//collection
		rapidjson::Value collection(rapidjson::kObjectType);
		collection.AddMember("name", CString2string(data->name), json.doc.GetAllocator());
		collection.AddMember("description", CString2string(data->desc), json.doc.GetAllocator());
		json.doc.AddMember("info", collection, json.doc.GetAllocator());

		//folders
		HTREEITEM hChildItem = m_tree.GetChildItem(hItem);

		while (hChildItem)
		{
			data = (CApiNode*)m_tree.GetItemData(hChildItem);

			if (data)
			{
				//folder
				if (data->type == node_folder)
				{
					rapidjson::Value folder(rapidjson::kObjectType);
					folder.AddMember("name", CString2string(data->name), json.doc.GetAllocator());
					folder.AddMember("description", CString2string(data->desc), json.doc.GetAllocator());

					rapidjson::Value sub_items(rapidjson::kArrayType);

					//request
					HTREEITEM hRequestItem = m_tree.GetChildItem(hChildItem);
					while (hRequestItem)
					{
						data = (CApiNode*)m_tree.GetItemData(hRequestItem);
						if (data && data->type == node_request)
						{
							folder.AddMember("url", CString2string(data->url), json.doc.GetAllocator());
							folder.AddMember("method", CString2string(data->method), json.doc.GetAllocator());
							folder.AddMember("header", CString2string(data->header), json.doc.GetAllocator());
							folder.AddMember("body", CString2string(data->body), json.doc.GetAllocator());
						}
						hRequestItem = m_tree.GetNextSiblingItem(hRequestItem);
					}
					hChildItem = m_tree.GetNextSiblingItem(hChildItem);

					items.PushBack(folder, json.doc.GetAllocator());
				}
			}
			else
			{
				hChildItem = m_tree.GetNextSiblingItem(hChildItem);
			}
		}
	}

	json.doc.AddMember("item", items, json.doc.GetAllocator());

	json.save(json_path);
	*/

	int tab_count = 0;
	CString label;

	while (hItem)
	{
		CApiNode* data = (CApiNode*)m_tree.GetItemData(hItem);
		if (data)
		{
			//if (!data->name.IsEmpty())

		}

		label = m_tree.GetItemText(hItem);

		Trace_only(make_string(_T("\t"), tab_count));

		Trace_only(_T("%s\n"), label);

		//child가 있다면 child로 이동하고
		if (m_tree.ItemHasChildren(hItem))
		{
			tab_count++;
			hItem = m_tree.GetChildItem(hItem);
			continue;
		}
		//child가 없다면 현재 node의 sibling으로 이동한다.
		else
		{
			HTREEITEM hSiblingItem = m_tree.GetNextSiblingItem(hItem);
			if (hSiblingItem)
			{
				hItem = hSiblingItem;
				continue;
			}
			else
			{
				//만약 sibling도 없다면 parent의 sibling부터 다시 탐색한다.
				while (true)
				{
					tab_count--;
					hItem = m_tree.GetParentItem(hItem);

					//parent가 NULL인 경우는 최상위 node들인 경우이므로 모든 탐색을 중지한다.
					if (hItem == NULL)
						return;

					//sibling이 있으면 그 node부터 다시 탐색을 시작하고 없다면 다시 parent의 sibling을 찾는다.
					hSiblingItem = m_tree.GetNextSiblingItem(hItem);
					if (hSiblingItem)
					{
						hItem = hSiblingItem;
						break;
					}
				}
			}
		}
	}
}

void CRestmanDlg::OnTvnSelchangedTreeApi(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	CApiNode* data = (CApiNode*)m_tree.GetItemData(hItem);
	if (data)
	{
		m_edit_url.set_text(data->url);
		m_combo_verb.SelectString(-1, data->method);

		m_edit_url.EnableWindow(data->type == node_request);
		m_combo_verb.EnableWindow(data->type == node_request);

		//m_list_params.clear();
		//if (data->type == node_request)
		//{
		//	m_list_params.add_item(_T("key name"), data->header, data->desc);
		//	m_list_params.add_item(_T("key name"), data->body, data->desc);
		//}
	}
	*pResult = 0;
}
