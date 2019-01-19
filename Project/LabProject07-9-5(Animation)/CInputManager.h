#pragma once
#include "stdafx.h"
class CInputManager : public CSingleTonBase<CInputManager>
{
public:
	CInputManager();
	~CInputManager();

public:
	void AddKey();
};

