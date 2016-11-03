
// 2048Dlg.h : ͷ�ļ�
//

#pragma once

#include <list>
#include <math.h>
using namespace std;
// CMy2048Dlg �Ի���
class CMy2048Dlg : public CDialogEx
{
// ����
public:
	CMy2048Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY2048_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

	CDC mdc;//�豸�����Ķ������
	CBitmap bmp;
	CFont font;
	CPen pen;

	int m[4][4];	//
	int new_pos;	//�³��ֿ�
	int count;		//�¿�Ŵ���
	int score;
	bool over;
	int rem[4][4];

	
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
