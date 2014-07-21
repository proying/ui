#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include <vector>
#include <algorithm>

namespace DirectUI
{
	template <typename TReturn, typename TParam>
	class IReceiver;

	template <typename TReturn, typename TParam>
	class CReceiver;

	template <typename TReturn, typename TParam>
	class IObserver
	{
	public:
		typedef IReceiver<TReturn, TParam> IReceiverType;
	public:
		virtual bool AddReceiver(IReceiverType* pReceiver) = 0;
		virtual void RemoveReceiver(IReceiverType* pReceiver) = 0;
		virtual TReturn Broadcast(TParam param) = 0;
		virtual TReturn RBroadcast(TParam param) = 0;
		virtual TReturn Notify(TParam param) = 0;
	};

	template <typename TReturn, typename TParam>
	class IReceiver
	{
	public:
		typedef IObserver<TReturn, TParam> IObserverType;
	public:
		virtual bool AddObserver(IObserverType* pObserver) = 0;
		virtual void RemoveObserver() = 0;
		virtual void RemoveObserver(IObserverType* pObserver) = 0;
		virtual TReturn Receive(TParam param) = 0;
		virtual TReturn Respond(TParam param, IObserverType* pObserver) = 0;
	};

	template <typename TReturn, typename TParam>
	class DirectUI_API CObserver : public IObserver<TReturn, TParam>
	{
	public:
		typedef IReceiver<TReturn, TParam> IReceiverType;
		typedef std::vector<IReceiverType*> IReceiverVector;
		typedef typename IReceiverVector::iterator IReceiverIter;
		typedef typename IReceiverVector::reverse_iterator IReceiverRIter;

		template <typename TReturn, typename TParam>
		friend class Iterator;
	public:
		CObserver()
			:m_pReceiver(NULL)
		{
			m_pReceiver = new IReceiverVector;
		}

		virtual ~CObserver() 
		{
			if (m_pReceiver)
			{
				m_pReceiver->clear();
				delete m_pReceiver;
			}
		}

		bool AddReceiver(IReceiverType* pReceiver)
		{
			if (!pReceiver)
				return false;

			pReceiver->AddObserver(this);
			m_pReceiver->push_back(pReceiver);
			return true;
		}

		void RemoveReceiver(IReceiverType* pReceiver)
		{
			if (pReceiver)
			{
				IReceiverIter it = std::find(m_pReceiver->begin(), m_pReceiver->end(), pReceiver);
				if (it != m_pReceiver->end())
					m_pReceiver->erase(it);	
			}
		}

		TReturn Broadcast(TParam param)
		{
			IReceiverIter it = m_pReceiver->begin();
			for (; it != m_pReceiver->end(); ++it)
				(*it)->Receive(param);

			return TReturn();
		}

		TReturn RBroadcast(TParam param)
		{
			IReceiverRIter it = m_pReceiver->rbegin();
			for (; it != m_pReceiver->rend(); ++it)
			{
				(*it)->Receive(param);
			}

			return TReturn();
		}

		TReturn Notify(TParam param)
		{
			IReceiverIter it = m_pReceiver->begin();
			for (; it != m_pReceiver->end(); ++it)
				(*it)->Respond(param, this);

			return TReturn();
		}

		template <typename TReturn, typename TParam>
		class Iterator
		{
			DWORD _index;
			CObserver<TReturn, TParam>& _tbl;
			IReceiver<TReturn, TParam>* _ptr;
		public:
			Iterator(CObserver& table)
				: _tbl(table), _index(0), _ptr(NULL)
			{
			}

			Iterator(const Iterator& v)
				: _tbl(v._tbl), _index(v._index), _ptr(v._ptr)
			{
			}

			IReceiver<TReturn, TParam>* Next()
			{
				if (_index >= _tbl.m_pReceiver->size())
					return NULL;

				for (; _index < _tbl.m_pReceiver->size();)
				{
					_ptr = _tbl.m_pReceiver->at(_index++);
					if (_ptr) return _ptr;
				}
				return NULL;
			}
		};

	protected:
		IReceiverVector *m_pReceiver;
	};

	template <typename TReturn, typename TParam>
	class DirectUI_API CReceiver : public IReceiver<TReturn, TParam>
	{
	public:
		typedef IObserver<TReturn, TParam> IObserverType;
		typedef std::vector<IObserverType*> IObserverVector;
		typedef typename IObserverVector::iterator IObserverIter;

	public:
		CReceiver()
		{
			m_pObserver = new IObserverVector;
		}

		virtual ~CReceiver()
		{
			if (m_pObserver)
			{
				m_pObserver->clear();
				delete m_pObserver;
			}
		}

		bool AddObserver(IObserverType* pObserver)
		{
			if (!pObserver)
				return false;

			m_pObserver->push_back(pObserver);
			return true;
		}

		void RemoveObserver()
		{
			IObserverIter it = m_pObserver->begin();
			for (; it != m_pObserver->end(); ++it)
			{
				(*it)->RemoveReceiver(this);
			}
		}

		void RemoveObserver(IObserverType* pObserver)
		{
			if (pObserver)
			{
				IObserverIter it = std::find(m_pObserver->begin(), m_pObserver->end(), pObserver);
				if (it != m_pObserver->end())
					m_pObserver->erase(it);
			}
		}

		TReturn Receive(TParam param)
		{
			return TReturn();
		}

		TReturn Respond(TParam param, IObserverType* pObserver)
		{
			return TReturn();
		}

	protected:
		IObserverVector *m_pObserver;
	};

}

#endif //__OBSERVER_H__