
// SnakeGameDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SnakeGame.h"
#include "SnakeGameDlg.h"
#include "afxdialogex.h"
#include "SnakeLinkedList.h" // headerfile to use snakelinkedlist

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SnakeLinkedList snakelist; // make linkedlist

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


// CSnakeGameDlg 대화 상자


CSnakeGameDlg::CSnakeGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SNAKEGAME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSnakeGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSnakeGameDlg, CDialogEx)
	ON_WM_TIMER() // using timer message 
	ON_WM_KEYDOWN() // using keydown message
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CSnakeGameDlg 메시지 처리기

BOOL CSnakeGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	c_keytemp = keytemp; // save last keytemp value in c_keytemp
	if (MessageBox(L"Snake game\n\n조작법 : W,A,S,D 또는 방향키\n빨간색 먹이를 먹으면 몸통이 늘어납니다\n벽이나 몸에 닿으면 게임이 종료됩니다\n\n게임 진행을 원하면 예를 눌러주세요.\n", L"게임설명", MB_YESNO) != IDYES)
		this->DestroyWindow(); // if user click 'cancle'

	srand(time(NULL)); // make random int value
	SetBackgroundColor(RGB(80, 40, 5)); // change background color
	SetTimer(1, 55, NULL); // set timer
	keytemp = 0x41; // inital keytemp value
	feed_col = rand() % 25;
	feed_row = rand() % 25;

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

void CSnakeGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSnakeGameDlg::OnPaint()
{
	static bool start = true;
	static bool init = true;
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
		if (start == true) { // if start value is true, it is first operation
			CPaintDC pdc(this); // to use paint
			CRect Rect;
			CPen Pen, * oldPen;
			CBrush Brush, * oldBrush;
			Pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255)); // to use pen

			//draw background
			Brush.CreateSolidBrush(RGB(10, 240, 70));
			oldPen = pdc.SelectObject(&Pen);
			oldBrush = pdc.SelectObject(&Brush);
			Rect.SetRect(30, 30, 531, 531);
			pdc.RoundRect(&Rect, CPoint(0, 0));
			pdc.SelectObject(oldPen);
			pdc.SelectObject(oldBrush);
			Pen.DeleteObject();
			Brush.DeleteObject();
			InvalidateRect(&Rect, FALSE);

			//draw line 
			Pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			int tx = 30, ty = 30;
			CPoint point(tx, ty);
			SelectObject(pdc, Pen);
			for (int x = 0; x < 26; x++) { // draw vertical line
				pdc.MoveTo(point.x + (x * 20), point.y);
				pdc.LineTo(point.x + (x * 20), 530);
				Invalidate(FALSE);
			}
			for (int y = 0; y < 26; y++) { // draw horizonal line
				pdc.MoveTo(point.x, point.y + (y * 20));
				pdc.LineTo(530, point.y + (y * 20));
				Invalidate(FALSE);
			}

			Brush.CreateSolidBrush(RGB(0, 0, 0));
			pdc.SelectObject(Brush);

			pdc.SelectObject(oldBrush);
			Brush.DeleteObject();
			start = false;
		}

		// draw feed
		if (check == false) { // if snake is attach to feed

			CClientDC dc(this);

			CBrush brush, * oldBrush;
			brush.CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = dc.SelectObject(&brush);

			//draw same feed
			dc.Rectangle(30 + feed_col * 20, 30 + feed_row * 20, 51 + feed_col * 20, 51 + feed_row * 20);
		}
		else { // if snake is not attach to feed

			CClientDC dc(this);

			CBrush brush, * oldBrush;
			brush.CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = dc.SelectObject(&brush);

			do {
				//make new feed
				feed_col = rand() % 25;
				feed_row = rand() % 25;
			} while (checkfeed(feed_row, feed_col, snakelist));
			dc.Rectangle(30 + feed_col * 20, 30 + feed_row * 20, 51 + feed_col * 20, 51 + feed_row * 20);
			check = false;

		}
		CDialogEx::OnPaint();
	}
	if (init == true) {
		//print snakelist
		SnakePrint(snakelist);
		init = false;
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSnakeGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSnakeGameDlg::OnTimer(UINT nIDEvent) //ontimer function
{

	SendMessage(WM_KEYDOWN, NULL, NULL);
	c_keytemp = keytemp; // save last keytemp to c_keytemp
	if (c_keytemp == 0x57)// if user down W key
	{
		h_row--;
	}
	else if (c_keytemp == 0x41)// if user down A key
	{
		h_col--;
	}
	else if (c_keytemp == 0x44)// if user down D key
	{
		h_col++;
	}
	else if (c_keytemp == 0x53)// if user down S key
	{
		h_row++;
	}

	OnPaint(); // call onpaint function

	if ((h_row == feed_row) && (h_col == feed_col)) { // if snake attach to feed
		check = true;
		snakelist.Append(h_row, h_col); // call append function
		SnakePrint(snakelist);
	}
	else if (snakelist.checkfail(h_row, h_col) == true) { // if snake attach to its body itself
		KillTimer(1); // kill timer
		SnakePrint(snakelist); // print snakelist
		MessageBox(L"몸에 부딪혔습니다", L"게임 종료", MB_ICONSTOP);
		this->DestroyWindow();
	}
	else if ((h_row < 0) || (h_col < 0) || (h_row > 24) || (h_col > 24)) { // if snake is out of range
		KillTimer(1); // kill timer
		SnakePrint(snakelist); // print snakelist
		MessageBox(L"벽에 부딪혔습니다", L"게임 종료", MB_ICONSTOP);
		this->DestroyWindow();
	}
	else if (snakelist.size == 25 * 25) { // if user win
		KillTimer(1); //kill timer
		SnakePrint(snakelist);  // print snakelist
		MessageBox(L"당신이 이겼습니다", L"게임 성공", MB_ICONSTOP);
		this->DestroyWindow();
	}
	else { // move snake
		CClientDC dc(this);

		CBrush brush, * oldBrush;
		brush.CreateSolidBrush(RGB(10, 240, 70));
		oldBrush = dc.SelectObject(&brush);

		//make gray rectangle to cover snake's tail
		dc.Rectangle(30 + snakelist.tail->Getcol() * 20, 30 + snakelist.tail->Getrow() * 20, 51 + snakelist.tail->Getcol() * 20, 51 + snakelist.tail->Getrow() * 20);
		dc.Rectangle(30 + (snakelist.tail->Getprev())->Getcol() * 20, 30 + (snakelist.tail->Getprev())->Getrow() * 20, 51 + (snakelist.tail->Getprev())->Getcol() * 20, 51 + (snakelist.tail->Getprev())->Getrow() * 20);

		snakelist.Move(h_row, h_col); // move sanke
		SnakePrint(snakelist);  // print snakelist

	}

	CDialog::OnTimer(nIDEvent);

}

BOOL CSnakeGameDlg::PreTranslateMessage(MSG* pMSG)
{
	if (pMSG->message == WM_KEYDOWN)
	{
		if (pMSG->wParam == 0x57 || pMSG->wParam == VK_UP) { // if user down W key
			if (c_keytemp != 0x53 && c_keytemp != VK_DOWN)
				keytemp = 0x57;
		}
		else if (pMSG->wParam == 0x41 || pMSG->wParam == VK_LEFT) { // if user down A key
			if (c_keytemp != 0x44 && c_keytemp != VK_RIGHT)
				keytemp = 0x41;
		}
		else if (pMSG->wParam == 0x44 || pMSG->wParam == VK_RIGHT) { // if user down D key
			if (c_keytemp != 0x41 && c_keytemp != VK_LEFT)
				keytemp = 0x44;
		}
		else if (pMSG->wParam == 0x53 || pMSG->wParam == VK_DOWN) { // if user down S key
			if (c_keytemp != 0x57 && c_keytemp != VK_UP)
				keytemp = 0x53;
		}
	}

	return CDialog::PreTranslateMessage(pMSG);
}

void CSnakeGameDlg::SnakePrint(SnakeLinkedList list) // print snake
{
	Node* cur = list.head;
	do {
		if (cur == list.head) { // if cur is haed, draw haed shape
			CClientDC dc(this);

			CBrush brush, * oldBrush;
			brush.CreateSolidBrush(RGB(70, 60, 50));
			oldBrush = dc.SelectObject(&brush);
			dc.Rectangle(30 + 20 * (cur->Getcol()), 30 + 20 * (cur->Getrow()), 30 + 20 * (cur->Getcol()) + 21, 30 + 20 * (cur->Getrow()) + 21);
		}
		else if (cur == list.tail) { // if cur is tail, draw tail shape
			CClientDC dc(this);

			CBrush brush, * oldBrush;
			brush.CreateSolidBrush(RGB(115, 110, 100));
			oldBrush = dc.SelectObject(&brush);

			dc.Rectangle(30 + 20 * (cur->Getcol()), 30 + 20 * (cur->Getrow()), 30 + 20 * (cur->Getcol()) + 21, 30 + 20 * (cur->Getrow()) + 21);
		}
		else { // if cur is body, draw body shape
			CClientDC dc(this);

			CBrush brush, * oldBrush;
			brush.CreateSolidBrush(RGB(110, 100, 80));
			oldBrush = dc.SelectObject(&brush);

			dc.Rectangle(30 + 20 * (cur->Getcol()), 30 + 20 * (cur->Getrow()), 30 + 20 * (cur->Getcol()) + 21, 30 + 20 * (cur->Getrow()) + 21);
		}

		cur = cur->Getnext(); // save next node address in cur
	} while (cur != list.head);
}





