#include "stdafx.h"
#include "CChatManager.h"
#include <wchar.h>
#include <corecrt_wstring.h>
#include <atlconv.h>
#include <atlbase.h>
#include < stdlib.h >
#include <atlstr.h>
typedef basic_string<TCHAR> tstring;
void CChatManager::Initialize()
{
	
}

void CChatManager::Update()
{


	if (m_chatContainer.size() > 10)
	{
		m_chatContainer.pop_front();
	}

}

void CChatManager::InputChatting(string str, int cSize)
{
	/*string s = str;

	cout << "s길이: "<<s.length() << endl;

	const char* all = s.c_str();
	size_t len = strlen(all) + 1;
	wchar_t* t = new wchar_t[len];
	if (t == nullptr)
		throw bad_alloc();
	mbstowcs(t, all, len);
	t[len] = '\0';

	int chatLen = MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length(), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length(), t, chatLen);
	t[chatLen] = '\0';

	m_chatContainer.emplace_back(make_pair(t, (UINT32)chatLen));*/

	//wstring wstr(s.length(), L' ');
	//wstr = CA2CT(s.c_str());
	////copy(s.begin(), s.end(), wstr.begin());
	//
	//cout << "--------------hi----------" << endl;
	//wcout.imbue(locale("kor"));
	//wcout << wstr << endl;
	//cout << "--------------buy----------" << endl;
	//tempWstr.assign(s.begin(), s.end());
	//cout <<L"하이 : "<< s << endl;
	/*int n = s.length();
	
	wchar_t* wide_string = new wchar_t[s.length() + 1];
	std::copy(s.begin(), s.end(), wide_string);
	
	wide_string[s.length()] = '/0';
	wcout << wide_string << endl;*/

	
	//tstring tstr;
	//되는버젼1
		//cout << "---maegaelength-------------------------------------" << endl;
	//cout << tt.length() << endl;
	//cout << tt << endl;
	string s {str};
	//s = tt;
	//s.assign(tt.c_str(), tt.length());
	//const char* tempS = tt.c_str();
	//s = tt.c_str();
	//copy(tempS[0], tempS[strlen(tempS)], s);
	//cout << "---daeiplength-------------------------------------" << endl;
	//cout << s.length() << endl;
	const char* all = s.c_str();
	size_t len = strlen(all) + 1;
	wchar_t* t = new wchar_t[len];
	if (t == nullptr)
		throw bad_alloc();
	mbstowcs(t, all, len);
	//t[len] = 0;
	//int chatLen = MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length(), t, chatLen);
	//int n{ 0 };
	//wcout << t << endl;
	ZeroMemory(t, sizeof(512));
	int chattingLen = MultiByteToWideChar(CP_ACP, 0,s.c_str(), s.length(), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length(), t, chattingLen);
	t[chattingLen] = '\0';
	//n = wcslen(wide_string);
	//cout << "----------------------------------------" << endl;
	//wcout  << (UINT32)wcslen(t) << endl;
	//

	//cout  << strlen(t) << endl;
	//wstring a;
	//return (TCHAR*)t;
	//TEXT()
	/*WCHAR* tempWCHAR{ wide_string };
	cout <<"안녕 나는 템프스트링제발~"<< tempWCHAR << endl;


	//cout << "---maegaelength-------------------------------------" << endl;
	////cout << tt.length() << endl;
	////cout << tt << endl;
	////string s{"아니 짜증나잖아 자꾸 안"};
	////s = tt;
	//cout << "---daeiplength-------------------------------------" << endl;
	////cout << s.length() << endl;
	//char* all;// = tt;
	//strcpy(all, tt);
	//size_t len = strlen(all) + 1;
	//wchar_t* t = new wchar_t[len];
	//if (t == nullptr)
	//	throw bad_alloc();
	//mbstowcs(t, all, len);
	////t[len] = 0;
	////int chatLen = MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length(), t, chatLen);
	////int n{ 0 };
	//wcout << t << endl;
	//ZeroMemory(t, sizeof(512));
	//int chattingLen = MultiByteToWideChar(CP_ACP, 0,all, strlen(all), NULL, NULL);
	//MultiByteToWideChar(CP_ACP, 0, all, strlen(all), t, chattingLen);
	//t[chattingLen] = '\0';
	////n = wcslen(wide_string);
	//cout << "----------------------------------------" << endl;
	//wcout  << (UINT32)wcslen(t) << endl;
	//wcout << t << endl;


	//cout  << strlen(t) << endl;
	//wstring a;
	//return (TCHAR*)t;
	//TEXT()
	/*WCHAR* tempWCHAR{ wide_string };*/
	//cout <<"안녕 나는 템프스트링제발~"<< tempWCHAR << endl;


	m_chatContainer.emplace_back(make_pair(t, (UINT32)wcslen(t)));
	
	//return strUnicode;

	//wcout <<"first: " << m_chatContainer.begin()->first<<" - " << m_chatContainer.begin()->second << endl;
	

}

void CChatManager::TextChange(char * pszCode)
{
	BSTR    bstrWide;
	char*   pszAnsi;
	int     nLength;
	// Get nLength of the Wide Char buffer

	nLength = MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, NULL, NULL);
	bstrWide = SysAllocStringLen(NULL, nLength);

	// Change UTF-8 to Unicode (UTF-16)

	MultiByteToWideChar(CP_UTF8, 0, pszCode, strlen(pszCode) + 1, bstrWide,
		nLength);
	// Get nLength of the multi byte buffer

	nLength = WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, NULL, 0, NULL, NULL);
	pszAnsi = new char[nLength];
	// Change from unicode to mult byte

	WideCharToMultiByte(CP_ACP, 0, bstrWide, -1, pszAnsi, nLength, NULL, NULL);
	SysFreeString(bstrWide);

	//cout<<"text trans: " << pszAnsi << endl;
	//m_tempchatContainer.emplace_back(make_pair(pszAnsi, nLength));

	//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

	string s{ pszAnsi };
	//s = tt;
	//s.assign(tt.c_str(), tt.length());
	//const char* tempS = tt.c_str();
	//s = tt.c_str();
	//copy(tempS[0], tempS[strlen(tempS)], s);
	//cout << "---daeiplength-------------------------------------" << endl;
	//cout << s.length() << endl;
	const char* all = s.c_str();
	size_t len = strlen(all) + 1;
	wchar_t* t = new wchar_t[len];
	if (t == nullptr)
		throw bad_alloc();
	mbstowcs(t, all, len);
	//t[len] = 0;
	//int chatLen = MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length(), t, chatLen);
	//int n{ 0 };
	//wcout << t << endl;
	ZeroMemory(t, sizeof(512));
	int chattingLen = MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length(), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.length(), t, chattingLen);
	t[chattingLen] = '\0';
	//n = wcslen(wide_string);
	//cout << "----------------------------------------" << endl;
	////wcout << (UINT32)wcslen(t) << endl;
	//wcout << t << endl;

	//cout << "FINAL TRANS" << t << endl;
	m_chatContainer.emplace_back(make_pair(t, (UINT32)wcslen(t)));

}

CChatManager::CChatManager()
{
}


CChatManager::~CChatManager()
{
}
