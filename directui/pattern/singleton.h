#ifndef __NS_SINGLETON_H__
#define __NS_SINGLETON_H__

namespace DirectUI
{
	class DirectUI_API CNoCopy 
	{
	private:
		CNoCopy(const CNoCopy& c);
		CNoCopy& operator = (const CNoCopy& c);
	protected:
		CNoCopy() { NULL; }
		//virtual ~CNoCopy() {}
	};

	template <typename T>
	class DirectUI_API CSingleton : public CNoCopy
	{
	public:
		static T* Instance()
		{
			if (!m_pSingletonInstance)
				m_pSingletonInstance = new T;
			return m_pSingletonInstance;
		}

		static void Destroy()
		{
			if (m_pSingletonInstance)
				delete m_pSingletonInstance;
			m_pSingletonInstance = NULL;
		}
	private:
		static T* m_pSingletonInstance; 
	};

	template <typename T> T* CSingleton<T>::m_pSingletonInstance = NULL;
}

#endif //__NS_SINGLETON_H__