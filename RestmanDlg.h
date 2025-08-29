
// RestmanDlg.h: 헤더 파일
//

#pragma once

#include "../Common/CTreeCtrl/SCTreeCtrl/SCTreeCtrl.h"
#include "../Common/CButton/GdiButton/GdiButton.h"
#include "../Common/CComboBox/SCComboBox/SCComboBox.h"
#include "../Common/CEdit/SCEdit/SCEdit.h"
#include "../Common/CEdit/RichEditCtrlEx/RichEditCtrlEx.h"
#include "../Common/CListCtrl/CVtListCtrlEx/VtListCtrlEx.h"
#include "../Common/ResizeCtrl.h"
#include "../Common/ControlSplitter.h"
#include "../Common/Json/rapid_json/json.h"

enum NODE_TYPE
{
	node_collection = 0,
	node_folder,
	node_request,
};

class CApiNode
{
public:
	int			type;	//collection, folder, request
	CString		name;
	CString		desc;
	CString		method;
	CString		url;
	CString		header;
	CString		body;
};


// CRestmanDlg 대화 상자
class CRestmanDlg : public CDialogEx
{
// 생성입니다.
public:
	CRestmanDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CResizeCtrl		m_resize;

	LRESULT			on_message_CSCTreeCtrl(WPARAM wParam, LPARAM lParam);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESTMAN_DIALOG };
#endif

protected:
	enum TIMER_ID
	{
		timer_find_json_files = 0,
	};

	void			find_json_files();
	void			load_json(CString json_path);

	void			load_traverse(const rapidjson::Value& v, const CString& indent = _T(""));

	//현재 구조를 json 파일로 저장한다. json_path = _T("")이면 data/api.json에 덮어쓴다.
	void			save_json(CString json_path = _T(""));

	void			release_node_data(HTREEITEM hItem = NULL);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CSCTreeCtrl m_tree;
	CControlSplitter m_splitter;
	CSCComboBox m_combo_verb;
	CSCEdit m_edit_url;
	CGdiButton m_button_send;
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	CVtListCtrlEx m_list_params;
	afx_msg void OnNMRClickTreeApi(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreeMenuAddFolder();
	afx_msg void OnTreeMenuAddRequest();
	afx_msg void OnTreeMenuAddCollection();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTvnSelchangedTreeApi(NMHDR* pNMHDR, LRESULT* pResult);
};
