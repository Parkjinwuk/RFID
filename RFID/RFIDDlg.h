﻿
// RFIDDlg.h: 헤더 파일
//

#pragma once
#include "is_d2xx.h"

// CRFIDDlg 대화 상자
class CRFIDDlg : public CDialogEx
{
// 생성입니다.
public:
	CRFIDDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CRFIDDlg();
	IS_HANDLE ftHandle = 0;
	char readSerialNumber[100] = "COM07";
	char changeSerialNumber[100] = "RFID01";
	short usbnumber;
	unsigned char wirteData[1024];
	unsigned short writeLength = 0;
	unsigned char readData[1024];
	unsigned short readLength = 0;
	BOOL flag_r=0;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RFID_DIALOG };
#endif

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
	afx_msg void OnConnect();
	afx_msg void OnDisconnect();
	afx_msg void OnReadOnce();
	CString m_strRfid;
	
//	afx_msg void OnBnClickedButton5();
//	afx_msg void OnBnClickedButton6();
	CStatic m_picture_control;
//	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit1();
	CString m_strGoods;
	CString m_strCost;
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit2();
	CString m_strType;
	CString m_strTemp;
};
