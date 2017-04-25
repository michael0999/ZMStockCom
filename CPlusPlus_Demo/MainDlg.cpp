// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"

/// �����Ƿ�ͬ����������
//#define	ZM_TDXSERVER_SYNC

void CMainDlg::AdviseTradeClient(int nIndex)
{
	if(NULL != m_spiTradeClientEvent[nIndex])
		return;
	m_nTradeEventCookie[nIndex] = NULL;
	this->m_spiTradeClientEvent[nIndex] = new CComObject <CStockComEvent>;
	ATLASSERT(this->m_spiTradeClientEvent[nIndex]);
	if(NULL == this->m_spiTradeClientEvent[nIndex])
		return;	
	/// ���ûص������������¼�����ʱƥ�����ĸ�����������
	m_spiTradeClientEvent[nIndex]->SetCallPara(this->m_hWnd,nIndex);
	//�����¼�����
	HRESULT hRet = AtlAdvise(m_spiTrade[nIndex],this->m_spiTradeClientEvent[nIndex]->GetUnknown(),
		DIID__ITradeEvents,&this->m_nTradeEventCookie[nIndex]);
}

void CMainDlg::UnAdviseTradeClient(int nIndex)
{
	if(0 == m_nTradeEventCookie[nIndex])
		return;
	AtlUnadvise(m_spiTrade[nIndex],DIID__ITradeEvents,m_nTradeEventCookie[nIndex]);
	m_nTradeEventCookie[nIndex] = 0;
	m_spiTradeClientEvent[nIndex] = NULL;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	m_wndTradeList.Attach(GetDlgItem(IDC_COMBO_TRADETYPE).m_hWnd);
	int nItemIndex = m_wndTradeList.AddString(L"�ʽ�");
	m_wndTradeList.SetItemData(nItemIndex,STOCKQUERYTYPE_CAPITAL);
	nItemIndex = m_wndTradeList.AddString(L"�ɷ�");
	m_wndTradeList.SetItemData(nItemIndex,STOCKQUERYTYPE_STOCK);
	nItemIndex = m_wndTradeList.AddString(L"����ί��");
	m_wndTradeList.SetItemData(nItemIndex,STOCKQUERYTYPE_TODAYORDER);
	nItemIndex = m_wndTradeList.AddString(L"���ճɽ�");
	m_wndTradeList.SetItemData(nItemIndex,STOCKQUERYTYPE_TODAYTRADE);
	nItemIndex = m_wndTradeList.AddString(L"���տɳ�ί��");
	m_wndTradeList.SetItemData(nItemIndex,STOCKQUERYTYPE_TODAYCANREVOKE);
	nItemIndex = m_wndTradeList.AddString(L"�ɶ�����");
	m_wndTradeList.SetItemData(nItemIndex,STOCKQUERYTYPE_SHAREHOLDERCODE);
	nItemIndex = m_wndTradeList.AddString(L"�������");
	m_wndTradeList.SetItemData(nItemIndex,STOCKQUERYTYPE_BALANCEOFFINANCING);
	nItemIndex = m_wndTradeList.AddString(L"��ȯ���");
	m_wndTradeList.SetItemData(nItemIndex,STOCKQUERYTYPE_BORROWFUNDLEFT);
	nItemIndex = m_wndTradeList.AddString(L"����֤ȯ");
	m_wndTradeList.SetItemData(nItemIndex,STOCKQUERYTYPE_CANBORROWFUND);
	nItemIndex = m_wndTradeList.AddString(L"���깺�¹�");
	m_wndTradeList.SetItemData(nItemIndex,STOCKQUERYTYPE_CANNEWSTOCK);
	nItemIndex = m_wndTradeList.AddString(L"�¹��깺���");
	m_wndTradeList.SetItemData(nItemIndex,STOCKQUERYTYPE_CANNEWSTOCKLIMIT);
	nItemIndex = m_wndTradeList.AddString(L"���");
	m_wndTradeList.SetItemData(nItemIndex,STOCKQUERYTYPE_ASSIGNNUMBER);
	nItemIndex = m_wndTradeList.AddString(L"��ǩ");
	m_wndTradeList.SetItemData(nItemIndex,STOCKQUERYTYPE_BALLOT);
	m_wndTradeList.SetCurSel(0);

	m_wndHistoryList.Attach(GetDlgItem(IDC_COMBO_HISLIST).m_hWnd);
	nItemIndex = m_wndHistoryList.AddString(L"��ʷί��");
	m_wndHistoryList.SetItemData(nItemIndex,HISORDERTYPE_TRUST);
	nItemIndex = m_wndHistoryList.AddString(L"��ʷ�ɽ�");
	m_wndHistoryList.SetItemData(nItemIndex,HISORDERTYPE_TRADE);
	nItemIndex = m_wndHistoryList.AddString(L"���");
	m_wndHistoryList.SetItemData(nItemIndex,HISORDERTYPE_DELIVERYORDER);
	m_wndHistoryList.SetCurSel(0);

	m_nTradeEventCookie[0] = 0;
	m_spiTradeClientEvent[0] = NULL;
	m_nTradeEventCookie[1] = 0;
	m_spiTradeClientEvent[1] = NULL;

	/// ����COM����ǰ����ע�ᣬ����ԱȨ��ִ��ZMStockCom.exe /RegServer��ж��ִ��ZMStockCom.exe /UnregServer
	HRESULT hRet = m_spiTrade[0].CreateInstance(__uuidof(StockTrade));
	ATLASSERT(m_spiTrade[0]);
	if(NULL != m_spiTrade[0])
	{
		/// �����¼�����
		AdviseTradeClient(0);

		/// ���õ�����־���
		hRet = m_spiTrade[0]->put_EnableLog(VARIANT_TRUE);
		/// ��ʼ��ʹ�û���������ʵ��ֻ��Ҫ����һ�Σ����ݿͻ��˰汾�ţ����������(Ĭ��Ϊ1)����ú����õķ���������ƥ��
		/// Ϊ���ִ��Ч�ʣ�ʵ��Ϊ�첽��������Ҫ�����¼�֪ͨ�õ���ʼ�����
		m_spiTrade[0]->Init(CComBSTR(L"6.4"),1);
	}

	/// �������ʧ�ܣ����ȼ���Ƿ�ע��ɹ��������������Ҫ������DLL�Ƿ񶼺ͱ�������һ��Ŀ¼
	this->GetDlgItem(IDC_EDIT_TRADESERVERADDR).SetWindowText(L"mock.tdx.com.cn");
	this->GetDlgItem(IDC_EDIT_TRADESERVERPORT).SetWindowText(L"7708");
	this->GetDlgItem(IDC_EDIT_YYBID).SetWindowText(L"9000");
	this->GetDlgItem(IDC_EDIT_LOGINACCOUNTNO).SetWindowText(L"1852983998@qq.com");
	this->GetDlgItem(IDC_EDIT_TRADEACCOUNT).SetWindowText(L"001001001020115");
	this->GetDlgItem(IDC_EDIT_TRADEPASSWORD).SetWindowText(L"");
	this->GetDlgItem(IDC_EDIT_COMMPASSWORD).SetWindowText(L"");
	this->GetDlgItem(IDC_EDIT_STOCKCODE).SetWindowText(L"000001");

	return TRUE;
}

LRESULT CMainDlg::OnInitReturn(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	IDispatch* piTrade = (IDispatch* )lParam;
	IStockTradePtr spiTrade(piTrade);
	if(NULL == spiTrade)
		return 0;/// �����׽ӿ�
	if(wParam)
	{
#ifdef _DEBUG
		/// �¼�֪ͨ�о������ⵯ��MessageBox�����������������Ƚ����ڵ��Գ��򷽱�
		if(m_spiTrade[0] == spiTrade)
		{
			this->MessageBox(L"����1��ʼ���ӿڳɹ���");

		}
		else if(m_spiTrade[1] == spiTrade)
			this->MessageBox(L"����2��ʼ���ӿڳɹ���");
#endif
	}
	else
	{
		/// ����ʧ�ܣ�������������
		CComBSTR bstrErrInfo;
		spiTrade->get_LastErrDesc(&bstrErrInfo);
#ifdef _DEBUG
		/// �¼�֪ͨ�о������ⵯ��MessageBox�����������������Ƚ����ڵ��Գ��򷽱�
		this->MessageBox(bstrErrInfo.m_str);
#endif
		bstrErrInfo.Empty();
	}

	return 0;
}

LRESULT CMainDlg::OnLoginReturn(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	IDispatch* piTrade = (IDispatch* )lParam;
	IStockTradePtr spiTrade(piTrade);
	if(NULL == spiTrade)
		return 0;/// �����׽ӿ�
	if(wParam)
	{
		VARIANT_BOOL bConnectValid = VARIANT_FALSE;
		HRESULT hRet = spiTrade->get_ConnectValid(&bConnectValid);
		USHORT nTradeID = 0;
		hRet = spiTrade->get_CurTradeID(&nTradeID);
		/// �¼�֪ͨ�о������ⵯ��MessageBox�����������������Ƚ����ڵ��Գ��򷽱�
		this->MessageBox(L"�첽��¼�������ɹ������濪ʼ��ȡ�ɶ�������Ϣ��");
		ITradeRecordPtr spiRecord = NULL;
		/// ��ùɶ��˺���Ϣ
		spiTrade->get_ShareHolderCode(&spiRecord);
		if(NULL != spiRecord)
		{
			/// ��ȡJSON�������������
			CComBSTR bstrJsonVal;
			spiRecord->GetJsonString(&bstrJsonVal);
			this->MessageBox(bstrJsonVal.m_str);
			bstrJsonVal.Empty();
			/// �ɶ���������ڲ��Ỻ�����ݱ��ã������뾡����Ҫ����Clear��գ�����Ӱ��ײ㹦��
//			spiRecord->Clear();
			spiRecord = NULL;
		}
#ifdef ZM_TDXSERVER_SYNC
		/// ͬ������ĳֻ��Ʊ��ʵʱ5������
		spiRecord = NULL;
		this->MessageBox(L"���濪ʼ��ȡָ����Ʊ��ʵʱ5�����飡");
		spiTrade->GetStockQuote(nTradeID,CComBSTR(L"000001"),&spiRecord);
		ATLASSERT(spiRecord);
		ULONG nFieldCount = 0,nRecordCount = 0,nIndex = 0;
		if(NULL != spiRecord)
		{
			hRet = spiRecord->get_FieldCount(&nFieldCount);
			hRet = spiRecord->get_RecordCount(&nRecordCount);
			CComBSTR bstrJsonVal;
			spiRecord->GetJsonString(&bstrJsonVal);
			this->MessageBox(bstrJsonVal.m_str);
			bstrJsonVal.Empty();
		}
#else
		/// �첽�����ֻ��Ʊ��ʵʱ5�����飬��ָ�����ȼ��ͷ��ص��������кš�nReqID�����¼��ص��д���
		ULONG nReqID = 0;
		spiTrade->GetStockQuotes(nTradeID,CComBSTR(L"000001;000002;600001"),RUNPRITYPE_ABOVE_NORMAL,&nReqID);
#endif
	}
	else
	{
		/// ����ʧ�ܣ�������������
		CComBSTR bstrErrInfo;
		spiTrade->get_LastErrDesc(&bstrErrInfo);
#ifdef _DEBUG
		/// �¼�֪ͨ�о������ⵯ��MessageBox�����������������Ƚ����ڵ��Գ��򷽱�
		this->MessageBox(bstrErrInfo.m_str);
#endif
		bstrErrInfo.Empty();
	}

	return 0;
}

LRESULT CMainDlg::OnOrderOK(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(NULL == m_spiTradeClientEvent[lParam] || NULL == m_spiTrade[lParam])
		return 0;/// �����Ѿ��ͷ�
	ITradeRecordPtr spiRecord = m_spiTradeClientEvent[lParam]->GetReqData();
	if(NULL == spiRecord)
		return 0;
	ULONG nRecordCount = 0,nFieldCount = 0;
	spiRecord->get_FieldCount(&nFieldCount);
	spiRecord->get_RecordCount(&nRecordCount);
	if(!nFieldCount || !nRecordCount)
		return 0;/// �쳣����
	CComBSTR bstrJsonVal;
	spiRecord->GetJsonString(&bstrJsonVal);
	this->MessageBox(bstrJsonVal.m_str);
	bstrJsonVal.Empty();
	/// ��ȡ��ί����ţ�����ҵ��������Ҫ��¼����ReqID�ͳɹ��ر�OrderID�Ķ�Ӧ��ϵ�����ں�����г���
	LONG nOrderID = 0;
	spiRecord->GetValueInt(0,0,&nOrderID);
	spiRecord->Clear();
	spiRecord = NULL;
	/// ִ��ȡ��ί�У���Ҫ�����г����ͣ���ί�гɹ�ʱ֪ͨ���أ�Ҳ�����Լ����㣬�Ϻ�������2�����ڽ�����Ϊ1
	m_spiTrade[lParam]->CancelOrder((EZMExchangeType)wParam,nOrderID,&spiRecord);
	return 0;
}

LRESULT CMainDlg::OnOrderSuccess(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(NULL == m_spiTradeClientEvent[lParam] || NULL == m_spiTrade[lParam])
		return 0;/// �����Ѿ��ͷ�
	/// ί�гɹ�֪ͨ,�����wParam
	return 0;
}

LRESULT CMainDlg::OnOrderError(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(NULL == m_spiTradeClientEvent[lParam] || NULL == m_spiTrade[lParam])
		return 0;/// �����Ѿ��ͷ�
	CComBSTR bstrErrInfo((BSTR)lParam);
	this->MessageBox(bstrErrInfo.m_str);
	bstrErrInfo.Empty();
	return 0;
}

LRESULT CMainDlg::OnStockQuote(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if(NULL == m_spiTradeClientEvent[lParam])
		return 0;/// �����Ѿ��ͷ�
	ITradeRecordPtr spiRecord = m_spiTradeClientEvent[lParam]->GetReqData();
	if(NULL == spiRecord)
		return 0;
	/// ���ݽӿڷ�����������
	ULONG nFieldCount = 0,nRecordCount = 0,nIndex = 0;
	HRESULT hRet = spiRecord->get_FieldCount(&nFieldCount);
	hRet = spiRecord->get_RecordCount(&nRecordCount);

	/// ��ȡJSON�������������
	CComBSTR bstrJsonVal;
	spiRecord->GetJsonString(&bstrJsonVal);
	this->MessageBox(bstrJsonVal.m_str);
	bstrJsonVal.Empty();

	CComBSTR bstrName;
	for(nIndex = 0;nIndex < nFieldCount;nIndex++)
	{
		/// ������ؼ�¼���ֶ�����
		EZMDataType eDataType;
		spiRecord->GetDataType(nIndex,&eDataType);
		/// ������ؼ�¼���ֶ�����
		hRet = spiRecord->GetFieldName(nIndex,&bstrName);
		bstrName.Empty();
	}
	///  ������м�¼
	for(nIndex = 0;nIndex < nRecordCount;nIndex++)
	{
		for(ULONG n = 0;n < nFieldCount;n++)
		{
			/// ��ȡÿ���ֶ�ֵ
			CComVariant varVal;
			hRet = spiRecord->GetValue(nIndex,n,&varVal);
			varVal.Clear();
		}
	}
	spiRecord->Clear();
	spiRecord = NULL;

	return 0;
}

LRESULT CMainDlg::OnChangeServer(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{

	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	ReleaseCom();

	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ReleaseCom();
	EndDialog(wID);
	return 0;
}

LONG StrToNum(const ATL::CString& strTemp)
{
	LONG nValue = 0;
	if(strTemp.IsEmpty())
		return nValue;
	BOOL bLoss = FALSE;
	if(strTemp.GetAt(0) == '-')
		bLoss = TRUE;
	int j = 0;
	if(bLoss)
		j++;
	for (; j < strTemp.GetLength(); j++)
	{
		TCHAR c = strTemp[j];
		if (c <= '9' && c >= '0')
		{
			nValue *= 10;
			nValue = (nValue + (c - '0')) ; 
		}
	}
	if(bLoss)
		return -nValue;
	return nValue;
}

void CMainDlg::ReleaseCom()
{
	/// �ͷ�COM�������
	if(NULL != m_spiTrade[0])
	{
		m_spiTrade[0]->LogOut(0);
		/// �Ͽ��¼�����
		UnAdviseTradeClient(0);
		m_spiTrade[0] = NULL;
	}

	if(NULL != m_spiTrade[1])
	{
		m_spiTrade[1]->LogOut(0);
		/// �Ͽ��¼�����
		UnAdviseTradeClient(1);
		m_spiTrade[1] = NULL;
	}
}

LRESULT CMainDlg::OnBnClickedUnInit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	if(NULL != m_spiTrade[0])
	{
		VARIANT_BOOL bConnectValid = VARIANT_FALSE;
		m_spiTrade[0]->get_ConnectValid(&bConnectValid);
		if(VARIANT_FALSE == bConnectValid)
		{
			this->MessageBox(L"��û�н����뽻�׷����������ӣ�");
			return 0;
		}
		m_spiTrade[0]->LogOut(0);
	}

	this->MessageBox(L"�Ͽ�ͨѶ�ɹ���");
	return 0;
}

LRESULT CMainDlg::OnBnClickedInit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	if(NULL == m_spiTrade)
	{
		this->MessageBox(L"����COM�������û�д����ɹ���");
		return 0;
	}
	/// ��ȡ��ʼ������
	CComBSTR bstrServerAddr,bstrServerPort,bstrYybID;
	this->GetDlgItem(IDC_EDIT_TRADESERVERADDR).GetWindowText(&bstrServerAddr);
	this->GetDlgItem(IDC_EDIT_TRADESERVERPORT).GetWindowText(&bstrServerPort);
	this->GetDlgItem(IDC_EDIT_YYBID).GetWindowText(&bstrYybID);
	ATLASSERT(bstrServerAddr.Length());
	ATLASSERT(bstrServerPort.Length());
	ATLASSERT(bstrYybID.Length());
	CComBSTR bstrLoginID,bstrTradeAccount;
	this->GetDlgItem(IDC_EDIT_LOGINACCOUNTNO).GetWindowText(&bstrLoginID);
	this->GetDlgItem(IDC_EDIT_TRADEACCOUNT).GetWindowText(&bstrTradeAccount);
	ATLASSERT(bstrLoginID.Length());
	CComBSTR bstrTradePassword,bstrCommPassword;
	this->GetDlgItem(IDC_EDIT_TRADEPASSWORD).GetWindowText(&bstrTradePassword);
	this->GetDlgItem(IDC_EDIT_COMMPASSWORD).GetWindowText(&bstrCommPassword);
	if(!bstrTradePassword.Length())
	{
		this->MessageBox(L"��û�����ý������룬�޷���¼��������");
		return 0;
	}

#ifdef ZM_TDXSERVER_DEMO
	m_spiTrade[0]->put_BrokerType(BROKERTYPE_MNCS);
#else
	m_spiTrade[0]->put_BrokerType(BROKERTYPE_ZXZQ);
#endif

#ifndef ZM_ADV_VERSION
	/// ���ý��׷�������ַ
	m_spiTrade[0]->put_CurServerHost(bstrServerAddr);
	/// ���ý��׷������˿�
	m_spiTrade[0]->put_CurServerPort((USHORT)StrToNum(bstrServerPort.m_str));
#else

#ifdef ZM_TDXSERVER_DEMO
	/// ���ý��׷�������ַ
	m_spiTrade[0]->put_CurServerHost(bstrServerAddr);
	/// ���ý��׷������˿�
	m_spiTrade[0]->put_CurServerPort((USHORT)StrToNum(bstrServerPort.m_str));
	m_spiTrade[0]->put_AccountType(LOGINIACCOUNTTYPE_MNCS);
#else
	m_spiTrade[0]->put_AccountType(LOGINIACCOUNTTYPE_CAPITAL);
	/// �߼��棬ͨ��JSON��������������ַ���ײ�֧��ͬʱ������ӣ�һ������ĳ���������������ϣ�������ʱ�л�ʹ��
	CString strJsonServer;
	/// ����IP���˿ڣ�main��ʾ��Ҫʹ��ʹ�ã�����Ϊ���ã�����5����������ַ����һ������
	strJsonServer = L"[{\"host\":\"202.108.253.186\",\"port\":7708,\"main\":1},{\"host\":\"202.108.253.141\",\"port\":7708,\"main\":0},{\"host\":\"27.221.115.134\",\"port\":7708,\"main\":0},{\"host\":\"124.160.88.188\",\"port\":7708,\"main\":0},{\"host\":\"124.160.88.188\",\"port\":7708,\"main\":0}]";
	m_spiTrade[0]->put_ServerConfig(CComBSTR(strJsonServer));
#endif
#endif

	/// ����Ӫҵ��ID
	m_spiTrade[0]->put_DepartmentID((USHORT)StrToNum(bstrYybID.m_str));
	/// ���õ�¼�˻�
	m_spiTrade[0]->put_LoginID(bstrLoginID);
	if(bstrTradeAccount.Length())
		m_spiTrade[0]->put_TradeAccount(bstrTradeAccount);/// ��ֵһ���bstrLoginIDһ�������Բ�����
	/// ���ý�������
	m_spiTrade[0]->put_TradePassword(bstrTradePassword);
	/// ����ͨ������
	if(bstrCommPassword.Length())
		m_spiTrade[0]->put_CommPassword(bstrCommPassword);/// ���Բ����ã�����ȯ����Ҫ���óɺͽ�������һ����

	VARIANT_BOOL bRetFlag = VARIANT_FALSE;
#ifndef ZM_TDXSERVER_SYNC
	///  �첽ִ�е�¼���׷���������¼���ͨ���¼�֪ͨ���
	HRESULT hRet = m_spiTrade[0]->LogIn(VARIANT_TRUE,&bRetFlag);
#else
	///  ͬ��ִ�е�¼���׷�������ֱ�����ؽ��
	HRESULT hRet = m_spiTrade[0]->LogIn(VARIANT_FALSE,&bRetFlag);
	if(VARIANT_FALSE == bRetFlag)
	{
		/// ��ȡ������Ϣ
		EZMTradeErrType nErrCode = TRADEERRTYPE_SUCCESS;
		CComBSTR bstrErrDesc;
		m_spiTrade[0]->get_LastErrType(&nErrCode);
		m_spiTrade[0]->get_LastErrDesc(&bstrErrDesc);
		this->MessageBox(bstrErrDesc.m_str);
		bstrErrDesc.Empty();
		return 0;
	}
	this->MessageBox(L"ͬ����¼���׷������ɹ���");
#endif
	return 1;
}

ITradeRecordPtr CMainDlg::GetCategoryData(EZMStockQueryType eCategoryType)
{
	ITradeRecordPtr spiRecord = NULL;
	USHORT nTradeID = 0;
	HRESULT hRet = m_spiTrade[0]->get_CurTradeID(&nTradeID);
	if(!nTradeID)
	{
		this->MessageBox(L"���ȳ�ʼ����¼��");
		return spiRecord;
	}
	if(nTradeID)
	{
		/// һ���ǳ�ʼ���ɹ���ʹ��һ��ʱ�䣬���ύ���׺Ͳ�ѯ����ǰ������Щ����ʧ��ʱ�ż���������Ƿ�����
		VARIANT_BOOL bConnectValid = VARIANT_FALSE;
		hRet = m_spiTrade[0]->get_ConnectValid(&bConnectValid);
		ATLASSERT(VARIANT_TRUE == bConnectValid);
		if(VARIANT_FALSE == bConnectValid)
		{
			VARIANT_BOOL bRetFlag = VARIANT_FALSE;
			/// ����ִ��ͬ����¼����ԭ�����õ���ز������Զ��ٴ�ʹ��
			hRet = m_spiTrade[0]->LogIn(VARIANT_FALSE,&bRetFlag);
		}
	}
	if(FAILED(hRet))
		return spiRecord;/// ͨ�Ų�����
	hRet = m_spiTrade[0]->QueryTradeData(nTradeID,eCategoryType,&spiRecord);
	if(NULL == spiRecord)
	{
		/// ��ȡ������Ϣ
		EZMTradeErrType nErrCode = TRADEERRTYPE_SUCCESS;
		CComBSTR bstrErrDesc;
		m_spiTrade[0]->get_LastErrType(&nErrCode);
		m_spiTrade[0]->get_LastErrDesc(&bstrErrDesc);
		this->MessageBox(bstrErrDesc.m_str);
		bstrErrDesc.Empty();
		return spiRecord;
	}
	ATLASSERT(spiRecord);
	return spiRecord;
}

LRESULT CMainDlg::OnBnClickedGetTradeData(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndTradeList.GetCurSel();
	DWORD dwType = (DWORD )m_wndTradeList.GetItemData(nIndex);
	if(NULL == m_spiTrade)
		return 0;
	ITradeRecordPtr spiRecord = GetCategoryData((EZMStockQueryType)dwType);
	if(NULL != spiRecord)
	{
		ULONG nFieldCount = 0,nRecordCount = 0,nIndex = 0;
		HRESULT hRet = spiRecord->get_FieldCount(&nFieldCount);
		hRet = spiRecord->get_RecordCount(&nRecordCount);
		CComBSTR bstrJsonVal;
		spiRecord->GetJsonString(&bstrJsonVal);
		this->MessageBox(bstrJsonVal.m_str);
		bstrJsonVal.Empty();

		CComBSTR bstrName;
		for(nIndex = 0;nIndex < nFieldCount;nIndex++)
		{
			/// ������ؼ�¼���ֶ�����
			EZMDataType eDataType;
			spiRecord->GetDataType(nIndex,&eDataType);
			/// ������ؼ�¼���ֶ�����
			hRet = spiRecord->GetFieldName(nIndex,&bstrName);
			bstrName.Empty();
		}
		///  ������м�¼
		for(nIndex = 0;nIndex < nRecordCount;nIndex++)
		{
			for(ULONG n = 0;n < nFieldCount;n++)
			{
				/// ��ȡÿ���ֶ�ֵ
				CComVariant varVal;
				hRet = spiRecord->GetValue(nIndex,n,&varVal);
				varVal.Clear();
			}
		}
		spiRecord->Clear();
		spiRecord = NULL;
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedGetFiveMarket(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	CComBSTR bstrStockCode;
	this->GetDlgItem(IDC_EDIT_STOCKCODE).GetWindowText(&bstrStockCode);
	ATLASSERT(bstrStockCode.Length());
	ITradeRecordPtr spiRecord = GetStockMarket(bstrStockCode.m_str);
	if(NULL == spiRecord)
		return 0;
	ULONG nFieldCount = 0,nRecordCount = 0,nIndex = 0;
	HRESULT hRet = spiRecord->get_FieldCount(&nFieldCount);
	hRet = spiRecord->get_RecordCount(&nRecordCount);
	CComBSTR bstrJsonVal;
	spiRecord->GetJsonString(&bstrJsonVal);
	this->MessageBox(bstrJsonVal.m_str);
	bstrJsonVal.Empty();

	CComBSTR bstrName;
	for(nIndex = 0;nIndex < nFieldCount;nIndex++)
	{
		/// ������ؼ�¼���ֶ�����
		EZMDataType eDataType = ZMDATATYPE_UNKNOW;
		spiRecord->GetDataType(nIndex,&eDataType);
		/// ������ؼ�¼���ֶ�����
		hRet = spiRecord->GetFieldName(nIndex,&bstrName);
		bstrName.Empty();
	}
	///  ������м�¼
	for(nIndex = 0;nIndex < nRecordCount;nIndex++)
	{
		for(ULONG n = 0;n < nFieldCount;n++)
		{
			/// ��ȡÿ���ֶ�ֵ
			CComVariant varVal;
			hRet = spiRecord->GetValue(nIndex,n,&varVal);
			varVal.Clear();
		}
	}
	spiRecord->Clear();
	spiRecord = NULL;
	return 0;
}

ITradeRecordPtr CMainDlg::GetHisData(EZMHisOrderType eCategoryType)
{
	ITradeRecordPtr spiRecord = NULL;
	USHORT nTradeID = 0;
	HRESULT hRet = m_spiTrade[0]->get_CurTradeID(&nTradeID);
	if(!nTradeID)
	{
		this->MessageBox(L"���ȳ�ʼ����¼��");
		return spiRecord;
	}
	if(nTradeID)
	{
		/// һ���ǳ�ʼ���ɹ���ʹ��һ��ʱ�䣬���ύ���׺Ͳ�ѯ����ǰ������Щ����ʧ��ʱ�ż���������Ƿ�����
		VARIANT_BOOL bConnectValid = VARIANT_FALSE;
		hRet = m_spiTrade[0]->get_ConnectValid(&bConnectValid);
		ATLASSERT(VARIANT_TRUE == bConnectValid);
		if(VARIANT_FALSE == bConnectValid)
		{
			VARIANT_BOOL bRetFlag = VARIANT_FALSE;
			/// ����ִ��ͬ����¼����ԭ�����õ���ز������Զ��ٴ�ʹ��
			hRet = m_spiTrade[0]->LogIn(VARIANT_FALSE,&bRetFlag);
		}
	}
	if(FAILED(hRet))
		return spiRecord;/// ͨ�Ų�����
	CComBSTR bstrStartDay(L"20170415"),bstrEndDay(L"20170421");
	hRet = m_spiTrade[0]->QueryHisTradeData(nTradeID,eCategoryType,bstrStartDay,bstrEndDay,&spiRecord);
	if(NULL == spiRecord)
	{
		/// ��ȡ������Ϣ
		EZMTradeErrType nErrCode = TRADEERRTYPE_SUCCESS;
		CComBSTR bstrErrDesc;
		m_spiTrade[0]->get_LastErrType(&nErrCode);
		m_spiTrade[0]->get_LastErrDesc(&bstrErrDesc);
		this->MessageBox(bstrErrDesc.m_str);
		bstrErrDesc.Empty();
		return spiRecord;
	}
	ULONG nFieldCount = 0,nRecordCount = 0,nIndex = 0;
	hRet = spiRecord->get_FieldCount(&nFieldCount);
	hRet = spiRecord->get_RecordCount(&nRecordCount);
	CComBSTR bstrJsonVal;
	spiRecord->GetJsonString(&bstrJsonVal);
	this->MessageBox(bstrJsonVal.m_str);
	bstrJsonVal.Empty();

	CComBSTR bstrName;
	for(nIndex = 0;nIndex < nFieldCount;nIndex++)
	{
		/// ������ؼ�¼���ֶ�����
		EZMDataType eDataType = ZMDATATYPE_UNKNOW;
		spiRecord->GetDataType(nIndex,&eDataType);
		/// ������ؼ�¼���ֶ�����
		hRet = spiRecord->GetFieldName(nIndex,&bstrName);
		bstrName.Empty();
	}
	///  ������м�¼
	for(nIndex = 0;nIndex < nRecordCount;nIndex++)
	{
		for(ULONG n = 0;n < nFieldCount;n++)
		{
			/// ��ȡÿ���ֶ�ֵ
			CComVariant varVal;
			hRet = spiRecord->GetValue(nIndex,n,&varVal);
			varVal.Clear();
		}
	}
	spiRecord->Clear();
	spiRecord = NULL;
	return spiRecord;
}

LRESULT CMainDlg::OnBnClickedGetHisData(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	DWORD dwType = (DWORD )m_wndHistoryList.GetItemData(m_wndHistoryList.GetCurSel());
	if(NULL == m_spiTrade)
		return 0;
	ITradeRecordPtr spiRecord = GetHisData((EZMHisOrderType)dwType);
	if(NULL == spiRecord)
		return 0;
	ULONG nFieldCount = 0,nRecordCount = 0,nIndex = 0;
	HRESULT hRet = spiRecord->get_FieldCount(&nFieldCount);
	hRet = spiRecord->get_RecordCount(&nRecordCount);
	CComBSTR bstrJsonVal;
	spiRecord->GetJsonString(&bstrJsonVal);
	this->MessageBox(bstrJsonVal.m_str);
	bstrJsonVal.Empty();

	CComBSTR bstrName;
	for(nIndex = 0;nIndex < nFieldCount;nIndex++)
	{
		/// ������ؼ�¼���ֶ�����
		EZMDataType eDataType = ZMDATATYPE_UNKNOW;
		spiRecord->GetDataType(nIndex,&eDataType);
		/// ������ؼ�¼���ֶ�����
		hRet = spiRecord->GetFieldName(nIndex,&bstrName);
		bstrName.Empty();
	}
	///  ������м�¼
	for(nIndex = 0;nIndex < nRecordCount;nIndex++)
	{
		for(ULONG n = 0;n < nFieldCount;n++)
		{
			/// ��ȡÿ���ֶ�ֵ
			CComVariant varVal;
			hRet = spiRecord->GetValue(nIndex,n,&varVal);
			varVal.Clear();
		}
	}
	spiRecord->Clear();
	spiRecord = NULL;
	return 0;
}

ITradeRecordPtr CMainDlg::GetStockMarket(const CString& strStockCode)
{
	ITradeRecordPtr spiRecord = NULL;
	if(NULL == m_spiTrade)
	{
		this->MessageBox(L"�����δ�����ɹ���");
		return spiRecord;
	}
	USHORT nTradeID = 0;
	HRESULT hRet = m_spiTrade[0]->get_CurTradeID(&nTradeID);
	if(!nTradeID)
	{
		this->MessageBox(L"���ȳ�ʼ����¼��");
		return spiRecord;
	}
	if(nTradeID)
	{
		/// һ���ǳ�ʼ���ɹ���ʹ��һ��ʱ�䣬���ύ���׺Ͳ�ѯ����ǰ������Щ����ʧ��ʱ�ż���������Ƿ�����
		VARIANT_BOOL bConnectValid = VARIANT_FALSE;
		hRet = m_spiTrade[0]->get_ConnectValid(&bConnectValid);
		ATLASSERT(VARIANT_TRUE == bConnectValid);
		if(VARIANT_FALSE == bConnectValid)
		{
			VARIANT_BOOL bRetFlag = VARIANT_FALSE;
			/// ����ִ��ͬ����¼����ԭ�����õ���ز������Զ��ٴ�ʹ��
			hRet = m_spiTrade[0]->LogIn(VARIANT_FALSE,&bRetFlag);
		}
	}
	if(FAILED(hRet))
	{
		this->MessageBox(L"ͨ�Ų�������");
		return spiRecord;/// ͨ�Ų�����
	}
	hRet = m_spiTrade[0]->GetStockQuote(nTradeID,CComBSTR(strStockCode),&spiRecord);
	if(NULL == spiRecord)
	{
		/// ��ȡ������Ϣ
		EZMTradeErrType nErrCode = TRADEERRTYPE_SUCCESS;
		CComBSTR bstrErrDesc;
		m_spiTrade[0]->get_LastErrType(&nErrCode);
		m_spiTrade[0]->get_LastErrDesc(&bstrErrDesc);
		this->MessageBox(bstrErrDesc.m_str);
		bstrErrDesc.Empty();
		return spiRecord;
	}
	ATLASSERT(spiRecord);
	return spiRecord;
}

LRESULT CMainDlg::OnBnClickedBuy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	CComBSTR bstrStockCode;
	this->GetDlgItem(IDC_EDIT_STOCKCODE).GetWindowText(&bstrStockCode);
	ATLASSERT(bstrStockCode.Length());
	ITradeRecordPtr spiRecord = GetStockMarket(bstrStockCode.m_str);
	if(NULL == spiRecord)
		return 0;
	/// ���нӿ���Ҫ�������쳣�����COM���������̱�����������ӿڹ��ܵ�ʱ�򣬻᷵�ش�����0x800706BA����˼�ǡ�RPC �����������á�
	/// ��Ҫ�ͷŵ�ǰʹ�õĽ��׽ӿڶ�������´��� HRESULT hRet = m_spiTrade.CreateInstance(__uuidof(Trade)) ������������

	CComVariant varVal;
	/// ȡ��һ�۸���������
	HRESULT hRet(E_FAIL);
	/// ������Ż�ȡ���ܴ��ڵ����⣺��Ҫ����ȯ�̣�ȯ�̲�ͬ�����ܷ��ص��ֶ�˳��һ��
//	hRet = spiRecord->GetValue(0,16,&varVal);
	/// �������ֶα����ȡ
	hRet = spiRecord->GetValueByName(0,CComBSTR(L"�����"),&varVal);
	ULONG nReqID1 = 0;/// ����Ψһ��ʶ��ί��ID����������¼���ʽ֪ͨ������������Ҫ����ÿ��ί�еĽ��֪ͨ����Ҫ��¼���������¼�֪ͨ��ID����ƥ��
	m_spiTrade[0]->AddOrder(STOCKORDERTYPE_BUY,ORDERPRICETYPE_LIMIT,bstrStockCode,varVal.fltVal,500,&nReqID1);
	varVal.Clear();
	/// ȡ��ǰ������
//	hRet = spiRecord->GetValue(0,5,&varVal);
	hRet = spiRecord->GetValueByName(0,CComBSTR(L"���ļ�"),&varVal);
	ULONG nReqID2 = 0;/// ����Ψһ��ʶ��ί��ID����������¼���ʽ֪ͨ���
//	hRet = m_spiTrade[0]->AddOrder(STOCKORDERTYPE_BUY,ORDERPRICETYPE_LIMIT,bstrStockCode,varVal.fltVal,500,&nReqID2);
	bstrStockCode.Empty();

	/// �ύί�У���һ���������ΪVARIANT_TRUE,�ײ���Զ���鵱ǰͨ��״̬����������ʱ����Զ���¼һ�����ύί��
	VARIANT_BOOL bCommitRet = VARIANT_FALSE;
	/// �������ύ�����ȼ�
	USHORT nTradeID = 0;
	m_spiTrade[0]->get_CurTradeID(&nTradeID);
	hRet = m_spiTrade[0]->CommitOrder(nTradeID,VARIANT_FALSE,RUNPRITYPE_NORMAL,&bCommitRet);
	spiRecord->Clear();
	spiRecord = NULL;
	if(VARIANT_FALSE == bCommitRet)
	{
		/// ��ȡ������Ϣ
		EZMTradeErrType nErrCode = TRADEERRTYPE_SUCCESS;
		CComBSTR bstrErrDesc;
		m_spiTrade[0]->get_LastErrType(&nErrCode);
		m_spiTrade[0]->get_LastErrDesc(&bstrErrDesc);
		this->MessageBox(bstrErrDesc.m_str);
		bstrErrDesc.Empty();
		return 0;
	}
	if(NULL != spiRecord)
	{
		/// ���ί�лر�
		spiRecord->Clear();
		spiRecord = NULL;
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedSell(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	ITradeRecordPtr spiRecord = NULL;
	USHORT nTradeID = 0;
	HRESULT hRet = m_spiTrade[0]->get_CurTradeID(&nTradeID);
	if(!nTradeID)
	{
		this->MessageBox(L"���ȳ�ʼ����¼��");
		return spiRecord;
	}
	if(nTradeID)
	{
		/// һ���ǳ�ʼ���ɹ���ʹ��һ��ʱ�䣬���ύ���׺Ͳ�ѯ����ǰ������Щ����ʧ��ʱ�ż���������Ƿ�����
		VARIANT_BOOL bConnectValid = VARIANT_FALSE;
		hRet = m_spiTrade[0]->get_ConnectValid(&bConnectValid);
		ATLASSERT(VARIANT_TRUE == bConnectValid);
		if(VARIANT_FALSE == bConnectValid)
		{
			VARIANT_BOOL bRetFlag = VARIANT_FALSE;
			/// ����ִ��ͬ����¼����ԭ�����õ���ز������Զ��ٴ�ʹ��
			hRet = m_spiTrade[0]->LogIn(VARIANT_FALSE,&bRetFlag);
		}
	}
	if(FAILED(hRet))
		return spiRecord;/// ͨ�Ų�����
	/// �����Ʊ�б�
	hRet = m_spiTrade[0]->QueryTradeData(nTradeID,STOCKQUERYTYPE_STOCK,&spiRecord);
	if(NULL == spiRecord)
	{
		/// ��ȡ������Ϣ
		EZMTradeErrType nErrCode = TRADEERRTYPE_SUCCESS;
		CComBSTR bstrErrDesc;
		m_spiTrade[0]->get_LastErrType(&nErrCode);
		m_spiTrade[0]->get_LastErrDesc(&bstrErrDesc);
		this->MessageBox(bstrErrDesc.m_str);
		bstrErrDesc.Empty();
		return spiRecord;
	}
	ULONG nFieldCount = 0,nRecordCount = 0,nIndex = 0;
	hRet = spiRecord->get_FieldCount(&nFieldCount);
	hRet = spiRecord->get_RecordCount(&nRecordCount);
	if(!nFieldCount || !nRecordCount)
	{
		spiRecord = NULL;
		return 0;
	}
	CComBSTR bstrJsonVal;
	spiRecord->GetJsonString(&bstrJsonVal);
	this->MessageBox(bstrJsonVal.m_str);
	bstrJsonVal.Empty();

	/// ���ҿ������Ĺ�Ʊ
	CComVariant varVal;
	for(nIndex = 0;nIndex < nRecordCount;nIndex++)
	{
		spiRecord->GetValueByName(nIndex,CComBSTR(L"���ùɷ�"),&varVal);
		varVal.ChangeType(VT_I2);
		if(!varVal.iVal)
			continue;/// �޿��ã��޷�������
		int nSellCount = varVal.iVal;
		varVal.Clear();
		spiRecord->GetValueByName(nIndex,CComBSTR(L"֤ȯ����"),&varVal);
		CString strStockName(varVal.bstrVal);
		varVal.Clear();
		spiRecord->GetValueByName(nIndex,CComBSTR(L"֤ȯ����"),&varVal);
		CString strStockCode(varVal.bstrVal);
		varVal.Clear();
		spiRecord->GetValueByName(nIndex,CComBSTR(L"�ɶ�����"),&varVal);
		CString strHolderCode(varVal.bstrVal);
		varVal.Clear();

		/// ��ȡ��ǰ����۸�
		ITradeRecordPtr spiQuote = NULL;
		m_spiTrade[0]->GetStockQuote(nTradeID,CComBSTR(strStockCode),&spiQuote);
		if(NULL == spiQuote)
			continue;
		ULONG nQuoteCount = 0;
		spiQuote->get_RecordCount(&nQuoteCount);
		if(!nQuoteCount)
		{
			/// û�л�ȡ����ǰ����
			spiQuote = NULL;
			continue;
		}
		CComBSTR bstrJsonVal;
		spiQuote->GetJsonString(&bstrJsonVal);
		this->MessageBox(bstrJsonVal.m_str);
		bstrJsonVal.Empty();

		/// ����5������
		spiQuote->GetValueByName(0,CComBSTR(L"�����"),&varVal);
		varVal.ChangeType(VT_R4);
		spiQuote = NULL;
		float fSell = varVal.fltVal;
		varVal.Clear();
		/// �޼��������п��ù�Ʊ
#ifndef ZM_TDXSERVER_SYNC
		/// �첽�ύ�������Ҫ��¼���ص�����ID�����ں��¼�֪ͨ���ί�лر����Ӧ���Ӷ�����֪��ʵ��ί�н��
		ULONG nReqID = 0;
		m_spiTrade[0]->AddOrder(STOCKORDERTYPE_SALE,ORDERPRICETYPE_LIMIT,\
			CComBSTR(strStockCode),fSell,nSellCount,&nReqID);
#else
		/// ͬ���ύ
		EZMExchangeType eExchangeType = EXCHANGETYPE_UNKNOWN;
		ITradeRecordPtr spiSell = NULL;
		m_spiTrade[0]->SyncCommitOrder(VARIANT_TRUE,STOCKORDERTYPE_SALE,ORDERPRICETYPE_LIMIT,\
			CComBSTR(strStockCode),fSell,nSellCount,&eExchangeType,&spiSell);
		if(NULL == spiSell)
		{
			/// �ύʧ�ܣ���ȡ��������
			CComBSTR bstrErrInfo;
			m_spiTrade[0]->get_LastErrDesc(&bstrErrInfo);
			continue;
		}
		ULONG nSellRecord = 0;
		spiSell->get_RecordCount(&nSellRecord);
		if(nSellRecord)
		{
			CComBSTR bstrJsonVal;
			spiSell->GetJsonString(&bstrJsonVal);
			this->MessageBox(bstrJsonVal.m_str);
			bstrJsonVal.Empty();

			/// ��ý��׷������Ϸ��ص�ί�лر����ID��ͨ����ID���Խ��г�������
			spiSell->GetValue(0,0,&varVal);
			varVal.ChangeType(VT_I4);
			ITradeRecordPtr spiCancle = NULL;
			m_spiTrade[0]->CancelOrder(eExchangeType,varVal.lVal,&spiCancle);
			/// ͨ����ȡspiCancle��ķ���ֵ��֪�������
			spiCancle = NULL;
			varVal.Clear();
		}
		spiSell = NULL;
#endif
	}
	spiRecord->Clear();
	spiRecord = NULL;
#ifndef ZM_TDXSERVER_SYNC
	VARIANT_BOOL bCommitRet = VARIANT_FALSE;
	m_spiTrade[0]->CommitOrder(nTradeID,VARIANT_TRUE,RUNPRITYPE_NORMAL,&bCommitRet);
#endif
	return 0;
}