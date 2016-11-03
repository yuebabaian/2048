
// 2048Dlg.h : 头文件
//

#pragma once

#include <list>
#include <math.h>
using namespace std;
// CMy2048Dlg 对话框
class CMy2048Dlg : public CDialogEx
{
// 构造
public:
	CMy2048Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY2048_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRebegin();
	afx_msg void OnRedo();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
	void auto_run();
	int getN(int m);
	bool randM();
	void show();
	void moveK(DWORD VK);
	bool move_left(int n[4][4],int &n_score);
	bool move_right(int n[4][4],int &n_score);
	bool move_up(int n[4][4],int &n_score);
	bool move_down(int n[4][4],int &n_score);
	double getCha(int n[4][4]);

	CDC mdc;//设备上下文对象的类
	CBitmap bmp;
	CFont font;
	CPen pen;

	int m[4][4];	//
	int new_pos;	//新出现块
	int count;		//新块放大步骤
	int score;
	bool over;
	int rem[4][4];

	
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
