#include "stdafx.h"
#include "CChatManager.h"
#include <wchar.h>
#include < stdlib.h >

void CChatManager::Initialize()
{
	
}

void CChatManager::Update()
{
	ws1[0] = L"Hello, world!";
	tempstring = L"histring";
	//m_chatContainer.emplace_front(wcstombs(tempstring));
	string s{ "leeso" };
	wchar_t* wide_string = new wchar_t[s.length() + 1];
	std::copy(s.begin(), s.end(), wide_string);
	wide_string[s.length()] = 0;

	m_chatContainer.emplace_front(wide_string);


	//delete[] wide_string;
}

void CChatManager::InputChatting()
{
	//ws1[m_curChatIndex] = 
}

CChatManager::CChatManager()
{
}


CChatManager::~CChatManager()
{
}
