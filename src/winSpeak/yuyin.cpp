#include "sapi.h"
#include "sphelper.h"
#pragma comment(lib, "sapi.lib")

int yuyin()
{
	TCHAR szFileName[256] = L"D:\\~sys\\Desktop\\0.wav";//���������汣����Ŀ���ļ���·��
	USES_CONVERSION;
	WCHAR m_szWFileName[2048];
	wcscpy(m_szWFileName, T2W(szFileName));//ת���ɿ��ַ���

										   //����һ����������󶨵�wav�ļ�
	CSpStreamFormat OriginalFmt;
	CComPtr<ISpStream>  cpWavStream;
	CComPtr<ISpStreamFormat>    cpOldStream;


	ISpVoice *pSpVoice;
	// ��ҪCOM�ӿ�
	::CoInitialize(NULL);
	// COM��ʼ��
	ISpStreamFormat *is;

	// ��ȡISpVoice�ӿ�
	CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void**)&pSpVoice);

	//pSpVoice->SetVolume(100);//��������



	HRESULT hr = pSpVoice->GetOutputStream(&cpOldStream);
	if (hr == S_OK)
		hr = OriginalFmt.AssignFormat(cpOldStream);
	else
		hr = E_FAIL;
	// ʹ��sphelper.h���ṩ�ĺ������� wav �ļ�
	if (SUCCEEDED(hr))
	{
		hr = SPBindToFile(m_szWFileName, SPFM_CREATE_ALWAYS, &cpWavStream,
			&OriginalFmt.FormatId(), OriginalFmt.WaveFormatExPtr());
	}
	if (SUCCEEDED(hr))
	{
		//��������������� wav �ļ��������� speakers
		pSpVoice->SetOutput(cpWavStream, TRUE);
	}
	//��ʼ�ʶ�
	pSpVoice->Speak(L"�������������������", SPF_ASYNC | SPF_IS_NOT_XML, 0);

	//�ȴ��ʶ�����
	pSpVoice->WaitUntilDone(INFINITE);
	cpWavStream.Release();

	//��������¶�λ��ԭ������
	pSpVoice->SetOutput(cpOldStream, FALSE);

	return 0;
}