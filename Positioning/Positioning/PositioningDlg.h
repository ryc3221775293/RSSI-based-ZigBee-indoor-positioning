
// PositioningDlg.h : 头文件
//
#include "SerialPort.h"
#pragma once

typedef struct
{
	float x;
	float y;
	float dis;
	CString     RSSI;
}Anchor;
typedef struct
{
	float x;
	float y;
}BlindNode;


class CPositioningDlgAutoProxy;


// CPositioningDlg 对话框
class CPositioningDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPositioningDlg);
	friend class CPositioningDlgAutoProxy;

// 构造
public:
	CPositioningDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CPositioningDlg();

// 对话框数据
	enum { IDD = IDD_POSITIONING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	CPositioningDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg LRESULT OnCommunication(WPARAM ch, LPARAM port);
	DECLARE_MESSAGE_MAP()
public:
//	CComboBox m_Botelv;
//	CComboBox m_Port;
	CComboBox m_Botelv;
	CComboBox m_Duankou;
	CComboBox m_Jiaoyanwei;
	CComboBox m_Shujuwei;
	CComboBox m_Tingzhiwei;
	float m_ax;
	float m_ay;
	float m_bx;
	float m_by;
	float m_cx;
	float m_cy;
	Anchor m_anchorA;
	Anchor m_anchorB;
	Anchor m_anchorC;
	BlindNode m_blindNode;
	int   m_PortNum;//串口号
	int   m_BotelvNum;
	bool  m_bPortFlag;
	CSerialPort m_Port;
	CString m_Receive_All_Data;
	CString m_Every_Data;
	afx_msg void OnBnClickedBtnStart();
	float CPositioningDlg::ComputerDistance(int rssi);
	void  CPositioningDlg::Computer_location(double ax,double ay,double la,double bx,double by, double lb,double cx, double cy, double lc);
	afx_msg void OnBnClickedBtnPause();
	float m_Edit_A;
	float m_Edit_n;
	CStatic m_picDraw;
	float m_BlindToA;
	float m_BlindToB;
	float m_BlindToC;
	float m_BlindX;
	float m_BlindY;
	CString m_DisShow;

	CEdit *pAedit;
	CEdit *pBedit;
	CEdit *pCedit;
	CEdit *pBlindXedit;
	CEdit *pBlindYedit;

//	float m_temperature;
private:
//	int m_AData;
//	int m_BData;
//	int m_CData;
public:
	bool m_bCommunFlag;
};
