#pragma once
#include <list>

class CChatManager : public CSingleTonBase<CChatManager>
{

	std::list<wchar_t*> m_chatContainer;
	wchar_t* ws1[10];
	wstring tempstring;
	bool m_isChatInput;
	int m_maxChatIndex{ 10 };
	int m_curChatIndex{ 0 };
public:
	void Initialize();
	std::list<wchar_t*> GetChatContailner() { return m_chatContainer; }
	void Update();
	void InputChatting();
public:
	CChatManager();
	~CChatManager();
};

