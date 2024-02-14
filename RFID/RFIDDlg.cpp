
// RFIDDlg.cpp: 구현 파일
//
#include "is_d2xx.h"
#include "pch.h"
#include "framework.h"
#include "RFID.h"
#include "RFIDDlg.h"
#include "afxdialogex.h"

// sound 출력용
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include "mmsystem.h"
#include <iostream>
#include <mysql.h>

using namespace std;
#pragma comment(lib,"winmm.lib") // 라이브러리 불러오기

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


// CRFIDDlg 대화 상자



CRFIDDlg::CRFIDDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RFID_DIALOG, pParent)
	, m_strRfid(_T(""))
	, m_strGoods(_T(""))
	, m_strCost(_T(""))
	, m_strType(_T(""))
	, m_strTemp(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CRFIDDlg::~CRFIDDlg()
{
	OnDisconnect();
}

void CRFIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strRfid);
	DDX_Control(pDX, IDC_PIC, m_picture_control);
	DDX_Text(pDX, IDC_EDIT2, m_strGoods);
	DDX_Text(pDX, IDC_EDIT3, m_strCost);
	DDX_Text(pDX, IDC_EDIT4, m_strType);
	DDX_Text(pDX, IDC_EDIT5, m_strTemp);
}

BEGIN_MESSAGE_MAP(CRFIDDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CRFIDDlg::OnConnect)
	ON_BN_CLICKED(IDC_BUTTON3, &CRFIDDlg::OnReadOnce)
	ON_BN_CLICKED(IDC_BUTTON2, &CRFIDDlg::OnDisconnect)
//	ON_BN_CLICKED(IDC_BUTTON5, &CRFIDDlg::OnBnClickedButton5)
//	ON_BN_CLICKED(IDC_BUTTON6, &CRFIDDlg::OnBnClickedButton6)
//	ON_EN_CHANGE(IDC_EDIT1, &CRFIDDlg::OnEnChangeEdit1)
ON_EN_CHANGE(IDC_EDIT1, &CRFIDDlg::OnEnChangeEdit1)
ON_EN_CHANGE(IDC_EDIT3, &CRFIDDlg::OnEnChangeEdit3)
ON_EN_CHANGE(IDC_EDIT2, &CRFIDDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CRFIDDlg 메시지 처리기

BOOL CRFIDDlg::OnInitDialog()
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

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CRFIDDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRFIDDlg::OnPaint()
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
HCURSOR CRFIDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRFIDDlg::OnConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		//열린 포트번호 찾기
	if (is_GetDeviceNumber(&usbnumber) == IS_OK)
	{
		printf("FTDI USB To Serial 연결된 개수 : %d\n", usbnumber);
		if (is_GetSerialNumber(0, readSerialNumber) == IS_OK)
		{
			if (memcmp(changeSerialNumber, readSerialNumber, sizeof(changeSerialNumber) != 0))
			{
				if (is_SetSerialNumber(0, changeSerialNumber) == IS_OK)
				{
					printf(" USB To Serial Number 를 변경 하였습니다.\n");
					printf(" FTDI SerialNumber :  %s \n", changeSerialNumber);
				}
			}
			else
				printf(" FTDI SerialNumber :  %s \n", changeSerialNumber);
		}
	}

	//열린 포트번호로 연결
	unsigned long portNumber;
	if (is_GetCOMPort_NoConnect(0, &portNumber) == IS_OK)
	{
		printf("COM Port : %d\n", portNumber);
	}
	if (is_OpenSerialNumber(&ftHandle, readSerialNumber, 115200) != IS_OK)
	{
		printf("USB To Serial과 통신 연결 실패\n");
		//return -1;
	}
	else {
		printf("Serial포트 %d와 통신 연결성공!! \n", portNumber);
	}
	Sleep(100);
	flag_r = 0;

}


void CRFIDDlg::OnReadOnce()
{
	CString temp, temp1=_T("");
	CString cname = _T("");
	CString ccost = _T("");
	CString ctype = _T("");
	CString ctemp = _T("");
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//ISO15693모드로 읽기( 싱글모드 읽기 )

		if (is_WriteReadCommand(ftHandle, CM1_ISO15693, CM2_ISO15693_ACTIVE + BUZZER_ON,
			writeLength, wirteData, &readLength, readData) == IS_OK)
		{
			int i;
			printf("ISO 15693 UID : ");
			for (i = 0; i < readLength; i++)
			{
				printf("%02x ", readData[i]);
				temp.Format(_T("%02x "), readData[i]);
				temp1 += temp;
				
			
			}
			
			m_strRfid = temp1;
			
			printf("\n");

			MYSQL* conn;
			MYSQL* conn_result;
			unsigned int timeout_sec = 1;

			conn = mysql_init(NULL);
			mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout_sec);
			conn_result = mysql_real_connect(conn, "127.0.0.1", "root", "0526", "mysql", 3306, NULL, 0);
			//비밀번호 바꿔주세요!!

			if (NULL == conn_result)
			{
				cout << "DB Connection Fail" << endl;
			}
			else
			{
				cout << "DB Connection Success" << endl;

				char query[1024];
				MYSQL_RES* result;
				MYSQL_ROW row;
				CString Cabernet_Sauvignon = _T("c8 c9 68 cb 50 01 04 e0 ");
				CString Merlot = _T("26 a8 68 cb 50 01 04 e0 ");
				if (!(temp1.Compare(Cabernet_Sauvignon)))
				{
					sprintf_s(query, 1024, "SELECT * FROM wine WHERE num=1");//아까전에 만들어줬던 db 내 테이블 이름

					// Send Query
					if (mysql_query(conn, query))
					{
						cout << "SELECT Fail" << endl;
					}

					// Get Response
					result = mysql_store_result(conn);

					int fields = mysql_num_fields(result);    // 필드 갯수 구함

					while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
					{
						cname = row[1];
						ccost = row[2];
						ctype = row[3];
						ctemp = row[4];

						m_strGoods = cname;
						m_strCost = ccost;
						m_strType = ctype;
						m_strTemp = ctemp;
						
					}

					CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
					m_picture_control.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
					CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
					dc = m_picture_control.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
					CImage image;//불러오고 싶은 이미지를 로드할 CImage 
					image.Load(_T("img\\wine\\Cabernet Sauvignon.png"));//이미지 로드

					image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
					ReleaseDC(dc);//DC 해제

					mysql_free_result(result);
					mysql_close(conn);
				}
				if (!(temp1.Compare(Merlot)))
				{
					sprintf_s(query, 1024, "SELECT * FROM wine WHERE num=2");//아까전에 만들어줬던 db 내 테이블 이름

					// Send Query
					if (mysql_query(conn, query))
					{
						cout << "SELECT Fail" << endl;
					}

					// Get Response
					result = mysql_store_result(conn);

					int fields = mysql_num_fields(result);    // 필드 갯수 구함

					while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
					{
						cname = row[1];
						ccost = row[2];
						ctype = row[3];
						ctemp = row[4];

						m_strGoods = cname;
						m_strCost = ccost;
						m_strType = ctype;
						m_strTemp = ctemp;
					}

					CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
					m_picture_control.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
					CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
					dc = m_picture_control.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
					CImage image;//불러오고 싶은 이미지를 로드할 CImage 
					image.Load(_T("img\\wine\\merlot.jpg"));//이미지 로드

					image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
					ReleaseDC(dc);//DC 해제

					mysql_free_result(result);
					mysql_close(conn);
				}
				PlaySoundW(_T("sound\\glass.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
		}
		/*
	else
	{
		// read continue mode
	}
	*/

	//ISO14443A모드로 읽기
	if (is_WriteReadCommand(ftHandle, CM1_ISO14443AB, CM2_ISO14443A_ACTIVE + BUZZER_ON,
		writeLength, wirteData, &readLength, readData) == IS_OK)
	{
		int i;
		printf("ISO 14443AB UID : ");
		for (i = 0; i < readLength; i++)
		{
			printf("%02x ", readData[i]);
			temp.Format(_T("%02x "), readData[i]);
			temp1 += temp;
		}

		m_strRfid = temp1;
		UpdateData(FALSE);
		printf("\n");

		MYSQL* conn;
		MYSQL* conn_result;
		unsigned int timeout_sec = 1;

		conn = mysql_init(NULL);
		mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout_sec);
		conn_result = mysql_real_connect(conn, "127.0.0.1", "root", "0526", "mysql", 3306, NULL, 0);
		//비밀번호 바꿔주세요!!

		if (NULL == conn_result)
		{
			cout << "DB Connection Fail" << endl;
		}
		else
		{
			cout << "DB Connection Success" << endl;

			char query[1024];
			MYSQL_RES* result;
			MYSQL_ROW row;
			CString Chappellet_Chardonnay = _T("01 31 76 5b ");
			CString Sauvignon_Blanc = _T("c1 44 a0 af ");
			if (!(temp1.Compare(Chappellet_Chardonnay)))
			{
				sprintf_s(query, 1024, "SELECT * FROM wine WHERE num=3");//아까전에 만들어줬던 db 내 테이블 이름

				// Send Query
				if (mysql_query(conn, query))
				{
					cout << "SELECT Fail" << endl;
				}

				// Get Response
				result = mysql_store_result(conn);

				int fields = mysql_num_fields(result);    // 필드 갯수 구함

				while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
				{
					cname = row[1];
					ccost = row[2];
					ctype = row[3];
					ctemp = row[4];

					m_strGoods = cname;
					m_strCost = ccost;
					m_strType = ctype;
					m_strTemp = ctemp;
				}
				CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
				m_picture_control.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
				CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
				dc = m_picture_control.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
				CImage image;//불러오고 싶은 이미지를 로드할 CImage 
				image.Load(_T("img\\wine\\Chappellet Chardonnay.jpg"));//이미지 로드

				image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
				ReleaseDC(dc);//DC 해제

				mysql_free_result(result);
				mysql_close(conn);
			}
			if (!(temp1.Compare(Sauvignon_Blanc)))
			{
				sprintf_s(query, 1024, "SELECT * FROM wine WHERE num=4");//아까전에 만들어줬던 db 내 테이블 이름

				// Send Query
				if (mysql_query(conn, query))
				{
					cout << "SELECT Fail" << endl;
				}

				// Get Response
				result = mysql_store_result(conn);

				int fields = mysql_num_fields(result);    // 필드 갯수 구함

				while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
				{
					cname = row[1];
					ccost = row[2];
					ctype = row[3];
					ctemp = row[4];

					m_strGoods = cname;
					m_strCost = ccost;
					m_strType = ctype;
					m_strTemp = ctemp;
				}

				CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
				m_picture_control.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
				CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
				dc = m_picture_control.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
				CImage image;//불러오고 싶은 이미지를 로드할 CImage 
				image.Load(_T("img\\wine\\Sauvignon Blanc.jpg"));//이미지 로드

				image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
				ReleaseDC(dc);//DC 해제

				mysql_free_result(result);
				mysql_close(conn);
			}
			PlaySoundW(_T("sound\\glass.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
	}
	UpdateData(FALSE);
}

void CRFIDDlg::OnDisconnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		// 무선파워를 끊어요.
	is_RfOff(ftHandle);
	//USB 포트를 Close
	if (is_Close(ftHandle) == IS_OK)
	{
		printf("연결을 닫습니다. ");
	}

}

/*
void CRFIDDlg::OnBnClickedButton5()
{
	/////////////////////////////////  이미지 출력  /////////////////////////////////
	CRect m_rect;
	CBitmap m_bitmap;
	//CString Path_img = "img\\null.bmp";
	CString Path_img = _T("img\\beer.bmp");
	m_bitmap.DeleteObject();
	m_bitmap.Attach((HBITMAP)LoadImage(NULL, Path_img, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	CDC* pDC = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(m_bitmap);
	((CStatic*)GetDlgItem(IDC_STATIC))->GetWindowRect(m_rect);
	ScreenToClient(&m_rect);
	CClientDC   dc(this);

	dc.BitBlt(100, 50, 320, 240, &memDC, 0, 0, SRCCOPY);

	ReleaseDC(pDC);
	DeleteDC(memDC);
}


void CRFIDDlg::OnBnClickedButton6()
{
	//sndPlaySound("sound.wav", SND_ASYNC | SND_NODEFAULT);
	PlaySoundW(_T("sound\\CanOpen.wav"), NULL, SND_FILENAME | SND_ASYNC);
	//_getch();
}
*/

//void CRFIDDlg::OnEnChangeEdit1()
//{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}


void CRFIDDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CRFIDDlg::OnEnChangeEdit3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CRFIDDlg::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
