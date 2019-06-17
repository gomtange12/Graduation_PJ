#pragma once

/*
---------------------------------------------------------------------------------------
싱글톤 템플릿 클레스
---------------------------------------------------------------------------------------
*/

template <typename T>
class CSingleTonBase
{
	//객체하나 만들어서 다 씀. 
protected:

	static T* m_Instance;

protected:

	CSingleTonBase() {}
	~CSingleTonBase() {};


public:
	static T* GetInstance()
	{
		if (0 == m_Instance)
			return m_Instance = new T;

		return m_Instance;
	}

	void ReleseInstance()
	{
		if (m_Instance)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}
};

//싱글턴 인스턴스 초기화
template <typename T>
T*	CSingleTonBase<T>::m_Instance = 0;
#pragma once
