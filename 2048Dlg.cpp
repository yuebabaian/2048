
// 2048Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "2048.h"
#include "2048Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





CMy2048Dlg::CMy2048Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy2048Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy2048Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMy2048Dlg, CDialogEx)

	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CMy2048Dlg::OnRedo)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON1, &CMy2048Dlg::OnRebegin)
END_MESSAGE_MAP()

typedef struct nodeC
{
	int num;
	COLORREF bkc;
	COLORREF tc;
}nodeC;


void CMy2048Dlg::show()
{
	const static nodeC color[]={
		0,		RGB(204,192,179),	RGB(204,192,179),
		2,		RGB(238,230,210),	RGB(119,110,100),
		4,		RGB(237,224,200),	RGB(119,110,100),
		8,		RGB(242,177,121),	RGB(249,242,242),
		16,		RGB(245,148,100),	RGB(249,242,242),
		32,		RGB(246,124,95),	RGB(249,242,242),
		64,		RGB(246,94,59),		RGB(249,242,242),
		128,	RGB(237,207,114),	RGB(249,242,242),
		256,	RGB(237,204,97),	RGB(249,242,242),
		512,	RGB(237,200,80),	RGB(249,242,242),
		1024,	RGB(237,197,63),	RGB(249,242,242),
		2048,	RGB(43,132,98),		RGB(249,242,242),
		4096,	RGB(250,56,108),	RGB(249,242,242),
		8192,	RGB(129,148,200),	RGB(249,242,242),
		16384,	RGB(255,0,0),		RGB(249,242,242),
		32768,	RGB(0,255,0),		RGB(249,242,242),
		65536,	RGB(128,128,0),		RGB(249,242,242),
	};
	static const int kw=120;			//���С
	static const int pos_x=14;		//��ʾλ��
	static const int pos_y=14;
	static const int kj=10;			//��϶��С

	RECT wrt;
	GetClientRect(&wrt);//��ȡ���ڿͻ����Ĵ�С
	mdc.FillSolidRect(&wrt,RGB(255,255,255));


	//����һ���ܱ���
	mdc.SelectObject(&font);//ѡ��һ����ָ�����豸�����Ļ����У����¶����滻��ǰ����ͬ���͵Ķ���
	CBrush bkbrush(RGB(187,173,160));
	mdc.SelectObject(&bkbrush);
	RECT qrt={pos_x-kj,   pos_y-kj,   pos_x+4*kj+4*kw,   pos_y+4*kj+4*kw};
	mdc.RoundRect(&qrt,CPoint(8,8));

	for(int i=0;i<4;i++)
	{
		for(int k=0;k<4;k++)
		{
			int num=m[i][k];
			int n=getN(num);//������������±�
			CBrush brush(color[n].bkc);
			mdc.SelectObject(&brush);//���ø��ӵı���ɫ
			mdc.SetTextColor(color[n].tc);//�������ֵ���ɫ

			//С����
			RECT rt;
			rt.left=pos_x+k*(kw+kj);
			rt.top=pos_y+i*(kw+kj);
			rt.right=rt.left+kw;
			rt.bottom=rt.top+kw;

			
			if(i*4+k == new_pos)
			{
				const static int b[]={
					-24,-20,-16,-12,-8,-4,0,
				};
				rt.left  -= b[count];
				rt.right += b[count];
				rt.top   -= b[count];
				rt.bottom+= b[count];
				count++;
				if(count > sizeof(b)/sizeof(int))
				{
					new_pos=-1;
				}
			}


			mdc.RoundRect(rt.left,rt.top,rt.right,rt.bottom,160,160);
			//д����
			if(num>0)
			{
				CString str;
				str.Format(_T("%d"),num);
				mdc.DrawText(str,&rt,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
			}
		}
	}


	//����
	CBrush brush(RGB(187,173,160));
	mdc.SelectObject(&brush);
	mdc.SetTextColor(RGB(238,235,232));
	RECT rt={580,170,760,300};
	mdc.RoundRect(rt.left,rt.top,rt.right,rt.bottom,8,8);
	CString str;
	str="score";
	rt.bottom = (rt.bottom - rt.top)/2 + rt.top;
	rt.top += 10;
	rt.bottom += 10;
	mdc.DrawText(str,&rt,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	str.Format(_T("%d"),score);
	rt.top += 50;
	rt.bottom += 50;
	mdc.DrawText(str,&rt,DT_SINGLELINE|DT_CENTER|DT_VCENTER);

	if(over)
	{
		CFont fo;
		fo.CreateFont(80,32,0,0,FW_EXTRABOLD,false,false,false,ANSI_CHARSET,OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,FF_MODERN,_T("Arial"));
		mdc.SelectObject(&fo);
		mdc.SetTextColor(RGB(250,0,0));
		str="Game Over";
		mdc.DrawText(str,&qrt,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	}

	CDC *dc=(CDC*)GetDC();
	dc->BitBlt(0,0,wrt.right,wrt.bottom,&mdc,0,0,SRCCOPY);
	ReleaseDC(dc);
}


// CMy2048Dlg ��Ϣ�������

BOOL CMy2048Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	srand(time(NULL));

	RECT rt;
	GetClientRect(&rt);
	CDC *dc=GetDC();
	mdc.CreateCompatibleDC(dc);//����һ����ָ���豸���ݵ��ڴ��豸�����Ļ���
	bmp.CreateCompatibleBitmap(dc,rt.right,rt.bottom);//������ָ�����豸������ص��豸���ݵ�λͼ��

	mdc.SetBkMode(TRANSPARENT);//͸��
	mdc.SelectObject(&bmp);

	font.CreateFont(56,16,0,0,FW_EXTRABOLD,false,false,false,ANSI_CHARSET,OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,FF_MODERN,_T("Arial"));
	mdc.SelectObject(&pen);
	ReleaseDC(dc);

	OnRebegin();
	SetTimer(1,1000/60,NULL);//1�봥��һ��

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}



// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMy2048Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMy2048Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CMy2048Dlg::OnRedo()
{
	int i,k;
	for(i=0;i<4;i++)
		for(k=0;k<4;k++)
			m[i][k]=rem[i][k];
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	SetFocus();
}


void CMy2048Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if(1==nIDEvent)
		show();
	else if(5==nIDEvent)
	{
		for(int i=0;i<5;i++)
			auto_run();
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CMy2048Dlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return 1;
}


void CMy2048Dlg::OnRebegin()
{
	int i,k;
	for(i=0;i<4;i++)
		for(k=0;k<4;k++)
			m[i][k]=0;
	score=0;
	over=0;
	randM();
	randM();
	GetDlgItem(IDC_BUTTON2)->EnableWindow(true);//�����������

	for(i=0;i<4;i++)
		for(k=0;k<4;k++)
			rem[i][k]=m[i][k];
}


BOOL CMy2048Dlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			return 1;
		case VK_SPACE:
			auto_run();
			return 1;
		case 'G':
			SetTimer(5,10,NULL);
			return 1;
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
			moveK(pMsg->wParam);
			return 1;
		case VK_ESCAPE:
			Sleep(2000);
			exit(1);
		default:
			return 1;
		}
	}
	else if(pMsg->message == WM_KEYUP)
		if(pMsg->wParam == 'G')
			KillTimer(5);

	return CDialog::PreTranslateMessage(pMsg);
}

bool CMy2048Dlg::randM()
{
	list<int> h;
	int i,k;
	for(i=0;i<4;i++)
	{
		for(k=0;k<4;k++)
			if(m[i][k]==0)
			{
				//��С����=0�Ĵ���
				h.push_back(i*4+k);
			}
	}
	int num=rand()%h.size();//�����һ��
	list<int>::iterator q=h.begin();
	for(i=0;i<num;i++)
		q++;
	num=rand()%1000;
	if(num > 800)
		num=4;
	else num=2;

	m[0][*q]=num;//��ôǿ������

	count=0;
	new_pos=*q;//�����µ�����һ��

	return 1;
}

int CMy2048Dlg::getN(int m)
{
	int n=0;
	while(m>0)
	{
		n++;
		m/=2;
	}
	return n;
}



void CMy2048Dlg::moveK(DWORD VK)
{
	if(over)
		return ;
	int new_M[4][4];
	int i,k;
	for(i=0;i<4;i++)
		for(k=0;k<4;k++)
			new_M[i][k]=m[i][k];
	int n_score;
	bool r;
	GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	switch(VK)
	{
	case VK_LEFT:
		r=move_left(new_M,n_score);
		break;
	case VK_RIGHT:
		r=move_right(new_M,n_score);
		break;
	case VK_UP:
		r=move_up(new_M,n_score);
		break;
	case VK_DOWN:
		r=move_down(new_M,n_score);
		break;
	}
	if(r)
	{
		for(i=0;i<4;i++)
			for(k=0;k<4;k++)
			{
				rem[i][k]=m[i][k];
				m[i][k]=new_M[i][k];
			}
			randM();
			score=n_score;
			//�������
			for(i=0;i<4*4;i++)
				if(m[0][i]==0)
					break;
			if(i==4*4)
			{
				for(i=0;i<4;i++)
				{
					for(k=0;k<3;k++)
						if(m[i][k] == m[i][k+1])
							break;
					if(k<3)
						break;
				}
				if(i==4)
				{
					for(k=0;k<4;k++)
					{
						for(i=0;i<3;i++)
							if(m[i][k] == m[i+1][k])
								break;
						if(i<3)
							break;
					}
					if(k==4)
						over=1;
				}
			}
	}
}

bool CMy2048Dlg::move_up(int n[4][4],int &n_score)
{
	int i,k,k1;
	bool r=0;
	n_score = score;

	//for(k=1;k<4;++k)//����
	//{
	//	for(k1=k;k1>=1;--k1)
	//	{
	//		for(i=0;i<4;++i)//����
	//		{
	//			if(n[k1-1][i]==0)
	//			{
	//				if(n[k1][i]!=0)
	//					r=1;
	//				n[k1-1][i]=n[k1][i];
	//				n[k1][i]=0;
	//			}
	//			else
	//			{
	//				if(n[k1][i]==n[k1-1][i])
	//				{
	//					r=1;

	//					n[k1-1][i]*=2;
	//					n[k1][i]=0;
	//					n_score+=n[k1-1][i];
	//				}
	//			}
	//		}
	//	}

	//}


	//��Ϊ������ƶ���ʽ
	for(k=0;k<4;k++)
	{
		for(i=0;i<3;i++)
		{
			if(n[i][k]==0)
			{
				int w;
				for(w=i;w<3;w++)
				{
					n[w][k]=n[w+1][k];
					if(n[w][k] != 0)
						r=1;
				}
				n[3][k]=0;

				for(w=i;w<4;w++)
					if(n[w][k] != 0)
						break;
				if(w!=4)
					i--;
			}
			else if(n[i+1][k] == 0)
			{
				int w;
				for(w=i+1;w<3;w++)
				{
					n[w][k]=n[w+1][k];
					if(n[w][k] != 0)
						r=1;
				}
				n[3][k]=0;

				for(w=i+1;w<4;w++)
					if(n[w][k] != 0)
						break;
				if(w!=4)
					i--;
			}
			else if(n[i][k]==n[i+1][k])
			{
				if(n[i][k] != 0)
					r=1;
				n[i][k] *= 2;
				n_score += n[i][k];
				for(int w=i+1;w<3;w++)
				{
					n[w][k]=n[w+1][k];
				}
				n[3][k]=0;
			}
		}
	}
	return r;
}
bool CMy2048Dlg::move_down(int n[4][4],int &n_score)
{
	int i,k,k1;
	bool r=0;
	n_score = score;

	for(k=0;k<4;k++)
	{
		for(i=3;i>0;i--)
		{
			if(n[i][k]==0)
			{
				int w;
				for(w=i;w>0;w--)
				{
					n[w][k]=n[w-1][k];
					if(n[w][k] != 0)
						r=1;
				}
				n[0][k]=0;

				for(w=i;w>0;w--)
					if(n[w][k] != 0)
						break;
				if(w>0)
					i++;
			}
			else if(n[i-1][k] == 0)
			{
				int w;
				for(w=i-1;w>0;w--)
				{
					n[w][k]=n[w-1][k];
					if(n[w][k] != 0)
						r=1;
				}
				n[0][k]=0;

				for(w=i-1;w>0;w--)
					if(n[w][k] != 0)
						break;
				if(w>0)
					i++;
			}
			else if(n[i][k]==n[i-1][k])
			{
				if(n[i][k] != 0)
					r=1;
				n[i][k] *= 2;
				n_score += n[i][k];
				for(int w=i-1;w>0;w--)
				{
					n[w][k]=n[w-1][k];
				}
				n[0][k]=0;
			}
		}
	}

	/*for(k=2;k>=0;--k)
	{
		for(k1=k;k1<=2;++k1)
		{
			for(i=0;i<4;++i)
			{
				if(n[k1+1][i]==0)
				{
					if(n[k1][i]!=0)
						r=1;
					n[k1+1][i]=n[k1][i];
					n[k1][i]=0;
				}
				else
				{
					if(n[k1][i]==n[k1+1][i])
					{
						r=1;

						n[k1+1][i]*=2;
						n[k1][i]=0;
						n_score+=n[k1+1][i];
					}
				}
			}
		}

	}*/
	return r;
}
bool CMy2048Dlg::move_left(int n[4][4],int &n_score)
{

	int i,k,k1;
	bool r=0;
	n_score = score;


	for(i=0;i<4;i++)
	{
		for(k=0;k<3;k++)
		{
			if(n[i][k]==0)//
			{
				int w;
				for(w=k;w<3;w++)
				{
					n[i][w]=n[i][w+1];
					if(n[i][w] != 0)
						r=1;
				}
				n[i][3]=0;

				for(w=k;w<4;w++)
					if(n[i][w] != 0)
						break;
				if(w!=4)
					k--;
			}
			else if(n[i][k+1] == 0)
			{
				int w;
				for(w=k+1;w<3;w++)
				{
					n[i][w]=n[i][w+1];
					if(n[i][w] != 0)
						r=1;
				}
				n[i][3]=0;

				for(w=k+1;w<4;w++)
					if(n[i][w] != 0)
						break;
				if(w!=4)
					k--;
			}
			else if(n[i][k]==n[i][k+1])
			{
				if(n[i][k] != 0)
					r=1;
				n[i][k] *= 2;
				n_score += n[i][k];
				for(int w=k+1;w<3;w++)
				{
					n[i][w]=n[i][w+1];
				}
				n[i][3]=0;
			}
		}
	}


	//for(k=1;k<4;++k)
	//{
	//	for(k1=k;k1>=1;--k1)
	//	{
	//		for(i=0;i<4;++i)//����
	//		{
	//			if(n[i][k1-1]==0)
	//			{
	//				if(n[i][k1]!=0)
	//					r=1;
	//				n[i][k1-1]=n[i][k1];
	//				n[i][k1]=0;
	//			}
	//			else
	//			{
	//				if(n[i][k1]==n[i][k1-1])
	//				{
	//					r=1;

	//					n[i][k1-1]*=2;
	//					n[i][k1]=0;
	//					n_score+=n[i][k1-1];
	//				}
	//			}
	//		}
	//	}

	//}
	
	return r;
}
bool CMy2048Dlg::move_right(int n[4][4],int &n_score)
{
	int i,k,k1;
	bool r=0;
	n_score = score;

	for(i=0;i<4;i++)
	{
		for(k=3;k>0;k--)
		{
			if(n[i][k]==0)
			{
				int w;
				for(w=k;w>0;w--)
				{
					n[i][w]=n[i][w-1];
					if(n[i][w] != 0)
						r=1;
				}
				n[i][0]=0;

				for(w=k;w>0;w--)
					if(n[i][w] != 0)
						break;
				if(w>0)
					k++;
			}
			else if(n[i][k-1] == 0)
			{
				int w;
				for(w=k-1;w>0;w--)
				{
					n[i][w]=n[i][w-1];
					if(n[i][w] != 0)
						r=1;
				}
				n[i][0]=0;

				for(w=k-1;w>0;w--)
					if(n[i][w] != 0)
						break;
				if(w>0)
					k++;
			}
			else if(n[i][k]==n[i][k-1])
			{
				if(n[i][k] != 0)
					r=1;
				n[i][k] *= 2;
				n_score += n[i][k];
				for(int w=k-1;w>0;w--)
				{
					n[i][w]=n[i][w-1];
				}
				n[i][0]=0;
			}
		}
	}

	/*for(k=2;k>=0;--k)
	{
		for(k1=k;k1<=2;++k1)
		{
			for(i=0;i<4;++i)
			{
				if(n[i][k1+1]==0)
				{
					if(n[i][k1]!=0)
						r=1;
					n[i][k1+1]=n[i][k1];
					n[i][k1]=0;
				}
				else
				{
					if(n[i][k1]==n[i][k1+1])
					{
						r=1;

						n[i][k1+1]*=2;
						n[i][k1]=0;
						n_score+=n[i][k1+1];
					}
				}
			}
		}

	}*/


	return r;
}

//�õ����ʺϵ��Զ�����
double CMy2048Dlg::getCha(int n[4][4])
{
	double c=0;
	int i,k,u;

	for(i=0;i<4;i++)
		for(k=0;k<3;k++)
		{
			u=abs(n[i][k]-n[i][k+1]);
			if(u==0)
				c -= 3*n[i][k];
			else c += u*1.5;
		}
		for(k=0;k<4;k++)
			for(i=0;i<3;i++)
			{
				u=abs(n[i][k]-n[i+1][k]);
				if(u==0)
					c -= 3*n[i][k];
				else c += u*1.5;
			}
	for(i=0;i<4;i++)
		for(k=0;k<4;k++)
		{
			int u=n[i][k];
			c =c- u*(getN(u)-1)*4;
	}
	return c;
}

void CMy2048Dlg::auto_run()
{
	int new_M[4][4];
	int n_score;
	DWORD VK=0;
	double mincha=1.0e+20;
	int i,k;
	for(i=0;i<4;i++)
		for(k=0;k<4;k++)
			new_M[i][k]=m[i][k];
	if(move_down(new_M,n_score))
	{
		int c=getCha(new_M);
		if(c<mincha)
		{
			mincha=c;
			VK=VK_DOWN;
		}
	}
	for(i=0;i<4;i++)
		for(k=0;k<4;k++)
			new_M[i][k]=m[i][k];
	if(move_right(new_M,n_score))
	{
		int c=getCha(new_M);
		if(c<mincha)
		{
			mincha=c;
			VK=VK_RIGHT;
		}
	}
	for(i=0;i<4;i++)
		for(k=0;k<4;k++)
			new_M[i][k]=m[i][k];
	if(move_left(new_M,n_score))
	{
		int c=getCha(new_M);
		if(c<mincha)
		{
			mincha=c;
			VK=VK_LEFT;
		}
	}
	
	for(i=0;i<4;i++)
		for(k=0;k<4;k++)
			new_M[i][k]=m[i][k];
	if(move_up(new_M,n_score))
	{
		int c=getCha(new_M);
		if(c<mincha)
		{
			mincha=c;
			VK=VK_UP;
		}
	}
	moveK(VK);
}
