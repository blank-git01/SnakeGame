
// SnakeGameDlg.h: 헤더 파일
//

#pragma once
#include"SnakeLinkedList.h"

// CSnakeGameDlg 대화 상자
class CSnakeGameDlg : public CDialogEx
{
	// 생성입니다.
public:
	void SnakePrint(SnakeLinkedList list);
	CSnakeGameDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	UINT nIDEvent = 1; // timer ID set 1
	afx_msg void OnTimer(UINT nIDEvent); // use timer function
	int keytemp;
	int h_col = 12; // current col
	int h_row = 12; // current row
	int c_keytemp;
	int feed_col;
	int feed_row;
	bool check = false;
	bool checkfeed(int feed_row, int feed_col, SnakeLinkedList snake) { // if snake crush with itself, return true
		Node* cur = snake.head;
		do {
			if ((feed_row == cur->Getrow()) && (feed_col == cur->Getcol())) {
				return true;
			}
			cur = cur->Getnext();
		} while (cur != snake.head);
		return false;
	}

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SNAKEGAME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMSG);

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
