#include "stdafx.h"
#include "CChatManager.h"
#include <wchar.h>
#include < stdlib.h >

void CChatManager::Initialize()
{
	
}

void CChatManager::Update()
{
	if (m_chatContainer.size() > 5)
	{
		m_chatContainer.pop_front();
	}

}

void CChatManager::InputChatting(string s)
{

	wchar_t* wide_string = new wchar_t[s.length() + 1];
	std::copy(s.begin(), s.end(), wide_string);
	wide_string[s.length()] = 0;


	m_chatContainer.emplace_back(wide_string);

}

CChatManager::CChatManager()
{
}


CChatManager::~CChatManager()
{
}
