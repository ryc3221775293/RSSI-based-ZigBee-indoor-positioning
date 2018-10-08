
// PositioningDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Positioning.h"
#include "PositioningDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern float ComputerX(float ax,float ay,float bx,float by,float cx,float cy,float az,float bz,float cz);
extern float ComputerY(float ax,float ay,float bx,float by,float cx,float cy,float az,float bz,float cz);
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPositioningDlg 对话框




IMPLEMENT_DYNAMIC(CPositioningDlg, CDialogEx);

CPositioningDlg::CPositioningDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPositioningDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
	m_ax = 0.0f;
	m_ay = 0.0f;
	m_bx = 0.0f;
	m_by = 0.0f;
	m_cx = 0.0f;
	m_cy = 0.0f;
	//  m_A = 0.0f;
	//  m_n = 0.0f;
	m_Edit_A = 0.0f;
	m_Edit_n = 0.0f;
	m_BlindToA = 0.0f;
	m_BlindToB = 0.0f;
	m_BlindToC = 0.0f;
	m_BlindX = 0.0f;
	m_BlindY = 0.0f;
	m_anchorA.RSSI = "000";
	m_anchorB.RSSI = "000";
	m_anchorC.RSSI = "000";
	m_bCommunFlag = false;

}

CPositioningDlg::~CPositioningDlg()
{
	// 如果该对话框有自动化代理，则
	//  将此代理指向该对话框的后向指针设置为 NULL，以便
	//  此代理知道该对话框已被删除。
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CPositioningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_COMBO_botelv, m_Botelv);
	//  DDX_Control(pDX, IDC_COMBO_duankou, m_Port);
	DDX_Control(pDX, IDC_COMBO_botelv, m_Botelv);
	DDX_Control(pDX, IDC_COMBO_duankou, m_Duankou);
	DDX_Control(pDX, IDC_COMBO_jiaoyanwei, m_Jiaoyanwei);
	DDX_Control(pDX, IDC_COMBO_shujuwei, m_Shujuwei);
	DDX_Control(pDX, IDC_COMBO_tingzhiwei, m_Tingzhiwei);
	DDX_Text(pDX, IDC_EDIT_AX, m_ax);
	DDX_Text(pDX, IDC_EDIT_AY, m_ay);
	DDX_Text(pDX, IDC_EDIT_BX, m_bx);
	DDX_Text(pDX, IDC_EDIT_BY, m_by);
	DDX_Text(pDX, IDC_EDIT_CX, m_cx);
	DDX_Text(pDX, IDC_EDIT_CY, m_cy);
	//  DDX_Text(pDX, IDC_EDIT_A, m_A);
	//  DDX_Text(pDX, IDC_EDIT_n, m_n);
	DDX_Text(pDX, IDC_EDIT_A, m_Edit_A);
	DDX_Text(pDX, IDC_EDIT_n, m_Edit_n);

	DDX_Text(pDX, IDC_EDIT_BlindToA, m_BlindToA);
	DDX_Text(pDX, IDC_EDIT_BlindToB, m_BlindToB);
	DDX_Text(pDX, IDC_EDIT_BlindToC, m_BlindToC);
	DDX_Text(pDX, IDC_EDIT_BlindX, m_BlindX);
	DDX_Text(pDX, IDC_EDIT_BlindY, m_BlindY);
//	DDX_Text(pDX, IDC_EDIT_Temperature, m_temperature);
}

BEGIN_MESSAGE_MAP(CPositioningDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)
	ON_BN_CLICKED(IDC_BTN_Start, &CPositioningDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_Pause, &CPositioningDlg::OnBnClickedBtnPause)
END_MESSAGE_MAP()


// CPositioningDlg 消息处理程序

BOOL CPositioningDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_Duankou.SetCurSel(0);
	m_Botelv.SetCurSel(0);
	m_Shujuwei.SetCurSel(3);
	m_Jiaoyanwei.SetCurSel(0);
	m_Tingzhiwei.SetCurSel(0);
	

	m_ax = 0.0;
	m_ay = 0.0;
	m_bx = 5.0;
	m_by = 0.0;
	m_cx = 0.0;
	m_cy = 5.0;
	m_Edit_A  = 45.0;
	m_Edit_n	  = 3.25;

	m_DisShow = "";
	pAedit=(CEdit*)GetDlgItem(IDC_EDIT_BlindToA);//这样获取Edit编辑框的指针
	pBedit=(CEdit*)GetDlgItem(IDC_EDIT_BlindToB);
	pCedit=(CEdit*)GetDlgItem(IDC_EDIT_BlindToC);
	pBlindXedit = (CEdit*)GetDlgItem(IDC_EDIT_BlindX);
	pBlindYedit = (CEdit*)GetDlgItem(IDC_EDIT_BlindY);
	UpdateData(FALSE);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPositioningDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPositioningDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPositioningDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 当用户关闭 UI 时，如果控制器仍保持着它的某个
//  对象，则自动化服务器不应退出。这些
//  消息处理程序确保如下情形: 如果代理仍在使用，
//  则将隐藏 UI；但是在关闭对话框时，
//  对话框仍然会保留在那里。

void CPositioningDlg::OnClose()
{
	if (CanExit())
		CDialogEx::OnClose();
}

void CPositioningDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CPositioningDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CPositioningDlg::CanExit()
{
	// 如果代理对象仍保留在那里，则自动化
	//  控制器仍会保持此应用程序。
	//  使对话框保留在那里，但将其 UI 隐藏起来。
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}


//串口初始化
void CPositioningDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bCommunFlag = true;
	UpdateData(TRUE);//获取对话框中输入的数值
	m_anchorA.x = m_ax;
	m_anchorA.y = m_ay;
	m_anchorB.x = m_bx;
	m_anchorB.y = m_by;
	m_anchorC.x = m_cx;
	m_anchorC.y = m_cy;
	//端口号
	int  comID;
	comID = m_Duankou.GetCurSel();//得到选择的端口号的索引
	switch(comID)
	{
	case  0:
		m_PortNum = 1;
		break;
	case  1:
		m_PortNum = 2;
		break;
	case  2:
		m_PortNum = 3;
		break;
	case  3:
		m_PortNum = 4;
		break;
	case  4:
		m_PortNum = 5;
		break;
	case  5:
		m_PortNum = 6;
		break;
	}
	//波特率
	int  btl;
	btl = m_Botelv.GetCurSel();
	switch(btl)
	{
	case  0:
		m_BotelvNum = 9600;
		break;
	case  1:
		m_BotelvNum = 19200;
		break;
	case  2:
		m_BotelvNum = 115200;
		break;
	}

	//此演示程序不进行波特率等数值的更改
	if (m_Port.InitPort(this,m_PortNum,m_BotelvNum,'N',8,1,EV_RXCHAR | EV_RXFLAG,512))
	{
		m_Port.StartMonitoring();
		m_bPortFlag = true;
	}
	else
	{
		AfxMessageBox("串口初始化失败");
		m_bPortFlag = false;
		//	exit(-1);
	}
}
int  a,b,c,temp=0;//表示锚节点的标志位A、B\C在一句报文中所处的位置

//每当串口发来消息，调用此函数进行处理
LRESULT CPositioningDlg::OnCommunication(WPARAM ch, LPARAM port)
{
	if ( true == m_bCommunFlag)
	{
	m_Every_Data +=(char) ch;//接收串口发来的一个字符
	//由于下位机在发送的时候，最后发送了一个'\n'符，
	//所以每次接受到是数据虽然是A012形式，但是最后又一个换行符未显示
	if (ch == '\n')
	{
		//MessageBox("huiche");
		//exit(0);
		if (5 == m_Every_Data.GetLength())//将每次协调器发送来的每个节点的数据整体保存在字符串中，防止接收不完整
		{

		//m_Receive_All_Data +=m_Every_Data;
			
		if (-1 !=( m_Every_Data.Find('A')))//接受的是A节点信息
		{
			for (int i = 1; i <= 3; i++)//将每个节点的数字取出
			{
				m_anchorA.RSSI		+= m_Every_Data.GetAt(i);
				temp++;
			}
			m_anchorA.dis = ComputerDistance(atoi(m_anchorA.RSSI));
			m_anchorA.RSSI.Empty();
		}
		else if (-1 !=( m_Every_Data.Find('B')))
		{
			for (int i = 1; i <= 3; i++)//将每个节点的数字取出
			{
				m_anchorB.RSSI		+= m_Every_Data.GetAt(i);
				temp++;
			}
			m_anchorB.dis = ComputerDistance(atoi(m_anchorB.RSSI));
			m_anchorB.RSSI.Empty();
		}
		else if(-1 !=( m_Every_Data.Find('C')))
		{
			for (int i = 1; i <= 3; i++)//将每个节点的数字取出
			{
				m_anchorC.RSSI		+= m_Every_Data.GetAt(i);
				temp++;
			}
			m_anchorC.dis = ComputerDistance(atoi(m_anchorC.RSSI));
			m_anchorC.RSSI.Empty();
		}
		m_Every_Data.Empty();//将本次接收到的如A节点的数据清除，继续接收B节点数据
		if (temp >= 3)
		{
		
		//计算锚节点跟盲节点之间的相对距离
		//m_anchorA.dis = ComputerDistance(atoi(m_anchorA.RSSI));
		//m_anchorB.dis = ComputerDistance(atoi(m_anchorB.RSSI));
		//m_anchorC.dis = ComputerDistance(atoi(m_anchorC.RSSI));

		//根据相对距离计算盲节点的坐标

		m_blindNode.x=ComputerX(m_ax,m_ay,m_bx,m_by,m_cx,m_cy,m_BlindToA,m_BlindToB,m_BlindToC);		
		m_blindNode.y=ComputerY(m_ax,m_ay,m_bx,m_by,m_cx,m_cy,m_BlindToA,m_BlindToB,m_BlindToC);		

		m_BlindToA = m_anchorA.dis;
		m_BlindToB = m_anchorB.dis;
		m_BlindToC = m_anchorC.dis;
		m_BlindX = m_blindNode.x;
		m_BlindY = m_blindNode.y;
		//计算完成之后，进行下一循环的操作，即再次接受ABC的值
		//		UpdateData(FALSE);
		//目的是只更新显示距离值
		m_DisShow.Format("%f",m_BlindToA);
		pAedit->SetWindowText(m_DisShow);
		m_DisShow.Format("%f",m_BlindToB);
		pBedit->SetWindowText(m_DisShow);
		m_DisShow.Format("%f",m_BlindToC);
		pCedit->SetWindowText(m_DisShow);

		m_DisShow.Format("%f",m_BlindX);
		pBlindXedit->SetWindowText(m_DisShow);
		m_DisShow.Format("%f",m_BlindY);
		pBlindYedit->SetWindowText(m_DisShow);

		//m_Receive_All_Data.Empty();
	/*	m_anchorA.RSSI.Empty();
		m_anchorB.RSSI.Empty();
		m_anchorC.RSSI.Empty();*/
		}
	}
}	
	return 0;
	
	}
}

//锚节点根据接收到的盲节点的RSSI值计算两者之间的相对距离
float CPositioningDlg::ComputerDistance(int rssi)
{
	float  distance;
//	distance = pow(10.0,(abs((double)(rssi-m_Edit_A)))/(10*m_Edit_n));

	//	double  mi = ((double)(rssi-m_A))/(10*m_n);
	//	distance = pow((double)10.0,mi);
	distance = pow((double)10.0,(((double)(rssi-m_Edit_A)))/(10*m_Edit_n));
	return distance;
}
void  CPositioningDlg::Computer_location(double ax,double ay,double la,double bx,double by, double lb,double cx, double cy, double lc)
{ 

	int i = 0,j = 0,n = 0; 
	int okflag = 0; 
	int err = 0; 
	int aberr = 0; 
	int acerr = 0; 
	int abcerr = 0; 
	int len; 
	int abrequery = 0; 
	int acrequery = 0; 
	int requery1 = 0; 
	int loopnum = 1; 
	int option = 0; 

	double cax,cay,cbx,cby,ccx,ccy; 
	double cax_abeyance1[6000],cay_abeyance1[6000]; 
	double cbx_abeyance1[6000],cby_abeyance1[6000]; 
	double ccx_abeyance1[6000],ccy_abeyance1[6000]; 
	double cabx_abeyance1[10],caby_abeyance1[10],cabx_abeyance2[10],caby_abeyance2[10]; 
	double cacx_abeyance1[10],cacy_abeyance1[10],cacx_abeyance2[10],cacy_abeyance2[10]; 

	//遍历圆周上所有点 

	//INPUT: z点的坐标在以A为圆心，以Z到A的距离为半径的圆周上
	for(m_blindNode.x = ax -la;m_blindNode.x <= ax + la;m_blindNode.x++) //以A为圆心以A到Z的距离为半径画圆，m_blindNode.x从圆与x轴的左边交点到右边交点
	{ 
		do 
		{ 
			for(m_blindNode.y = ay -la;m_blindNode.y <= ay + la;m_blindNode.y++) //y轴与圆的交点
			{ 
				cax = m_blindNode.x - ax; 
				cax = fabs(cax); 
				cax = pow(cax,2); 
				cay = m_blindNode.y - ay; 
				cay = fabs(cay); 
				cay = pow(cay,2);//用坐标之间的差值来计算距离 
				if(((cax + cay) > (la*la - err))&&((cax + cay) < (la*la + err))) //当坐标计算的距离平方之和等于给定的距离平方时，保存其值
				{ 
					okflag = 1; 
					cax_abeyance1[i] = m_blindNode.x; 
					cay_abeyance1[i] = m_blindNode.y; 
					n++; 
				} 
			} 
			if(!okflag)err++; 
		} while (okflag == 0); 
		i++; 
		err = 0; 
		okflag = 0; 
	} 
	i = 0; 
	for(m_blindNode.x = bx -lb;m_blindNode.x <= bx + lb;m_blindNode.x++) 
	{ 
		do 
		{ 
			for(m_blindNode.y = by -lb;m_blindNode.y <= by + lb;m_blindNode.y++) 
			{ 
				cbx = m_blindNode.x - bx; 
				cbx = fabs(cbx); 
				cbx = pow(cbx,2); 
				cby = m_blindNode.y - by; 
				cby = fabs(cby); 
				cby = pow(cby,2); 
				if(((cbx + cby) > (lb*lb - err))&&((cbx + cby) < (lb*lb + err))) 
				{ 
					okflag = 1; 
					cbx_abeyance1[i] = m_blindNode.x; 
					cby_abeyance1[i] = m_blindNode.y; 
				} 
			} 
			if(!okflag)err++;       
		} while (okflag == 0); 
		i++; 
		err = 0; 
		okflag = 0; 
	} 
	i = 0; 
	for(m_blindNode.x = cx -lc;m_blindNode.x <= cx + lc;m_blindNode.x++) 
	{ 
		do 
		{ 
			for(m_blindNode.y = cy -lc;m_blindNode.y <= cy + lc;m_blindNode.y++) 
			{ 
				ccx = m_blindNode.x - cx; 
				ccx = fabs(ccx); 
				ccx = pow(ccx,2); 
				ccy = m_blindNode.y - cy; 
				ccy = fabs(ccy); 
				ccy = pow(ccy,2); 
				if(((ccx + ccy) > (lc*lc - err))&&((ccx + ccy) < (lc*lc + err))) 
				{ 
					okflag = 1; 

					ccx_abeyance1[i] = m_blindNode.x; 
					ccy_abeyance1[i] = m_blindNode.y; 
					n++; 
				} 
			} 
			if(!okflag)err++; 
		} while (okflag == 0); 
		i++; 
		err = 0; 
		okflag = 0; 
	}  
	//到目前为止，abeyance中存放的是A,B,C节点所求未知节点Z的可能坐标值
	do 
	{ 
		//找ab 交点，因为距离的偏差，所以坐标会有偏差 
		aberr =0; 
		do 
		{ 
			for(i = 0;i <= 2*la;i++) 
			{ 
				for(j = 0;j <= 2*lb;j++) 
				{ 
					if(fabs(cax_abeyance1[i] - cbx_abeyance1[j])< 10) //在A、B分别计算时算出的Z点坐标值，x坐标相同时
					{ 
						if(fabs(cay_abeyance1[i] - cby_abeyance1[j])< 10) 
						{ 
							cabx_abeyance1[abrequery] = cax_abeyance1[i]; 
							caby_abeyance1[abrequery] = cay_abeyance1[i]; 
							abrequery++; 
						} 
					} 
				} 
			}   
			if(abrequery < loopnum)aberr++; 
		}while(abrequery < loopnum); 
		acerr =0; 
		//找ac 交点 
		do 
		{ 
			for(i = 0;i <= 2*la;i++) 
			{ 
				for(j = 0;j <= 2*lc;j++) 
				{ 
					if(fabs(cax_abeyance1[i] - ccx_abeyance1[j])< 10)
					{ 
						if(fabs(cay_abeyance1[i] - ccy_abeyance1[j])< 10)
						{ 
							cacx_abeyance1[acrequery] = cax_abeyance1[i]; 
							cacy_abeyance1[acrequery] = cay_abeyance1[i]; 
							acrequery++; 
						} 
					} 
				} 
			} 
			if(acrequery < loopnum)acerr++; 
		}while(acrequery < loopnum); 
		//找交点的交点 
		for(i = 0;i < abrequery;i++) 
		{ 
			for(j = 0;j < acrequery;j++) 
			{ 
				if(fabs(cabx_abeyance1[i] - cacx_abeyance1[j]) <10)
				{ 
					requery1 = 1; 
					m_blindNode.x = cabx_abeyance1[i];
					m_blindNode.y = caby_abeyance1[i];
				} 
			} 
		} 
		if(requery1 == 0) 
		{ 
			loopnum++; 
			abcerr++; 
		} 
		if(loopnum == 10) 
		{ 
			AfxMessageBox("chech input figure is right\n"); 
			exit(-1);
		} 
	}while(requery1 ==0); 
}  


void CPositioningDlg::OnBnClickedBtnPause()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bCommunFlag = false;
	m_Port.ClosePort();
	UpdateData(TRUE);//获取控件相关的变量值
}
