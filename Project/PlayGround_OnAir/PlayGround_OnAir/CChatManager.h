#pragma once
#include <list>

class CChatManager : public CSingleTonBase<CChatManager>
{

	wchar_t* ws1[10];
	wstring tempstring;
	bool m_isChatInput;
	int m_maxChatIndex{ 10 };
	int m_curChatIndex{ 0 };
public:
	std::list<pair<wchar_t*, UINT32>> m_chatContainer;
	void Initialize();
	std::list<pair<wchar_t*, UINT32>> GetChatContailner() { return m_chatContainer; }
	void Update();
	void InputChatting(const char* s);
public:
	CChatManager();
	~CChatManager();
};

