// tree.hpp
// KlayGE ��ģ�� ͷ�ļ�
// Ver 1.4.8.5
// ��Ȩ����(C) ������, 2001--2002
// Homepage: http://www.enginedev.com
//
// 1.2.8.9
// ������find_if (2002.10.21)
//
// 1.2.8.8
// �޸���find�ͱ��� (2002.10.10)
// ������DelChild��Bug (2002.10.3)
//
// 1.4.8.5
// ʹ����ResPtr (2003.4.21)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _TREE_HPP
#define _TREE_HPP

#include <cstddef>
#include <vector>
#include <KlayGE/SharedPtr.hpp>
#include <KlayGE/alloc.hpp>

namespace KlayGE
{
	template <typename T>
	class tree
	{
	public:
		typedef T				value_type;
		typedef T*				pointer;
		typedef const T*		const_pointer;
		typedef T&				reference;
		typedef const T&		const_reference;
		typedef std::size_t		size_type;
		typedef std::ptrdiff_t	difference_type;

		typedef SharedPtr<tree<T> >							ChildType;
		typedef std::vector<ChildType, alloc<ChildType> >	ChildrenType;
		typedef typename ChildrenType::iterator				ChildIterator;

	private:
		T				m_Root;

		tree<T>*		m_pParent;
		ChildrenType	m_Children;

	public:
		tree(const T& Root = T(), tree<T>* pPar = NULL)
			: m_Root(Root), m_pParent(pPar)
			{ }

		tree(const tree& rhs)
				: m_Root(rhs.m_Root), m_pParent(rhs.m_pParent),
					m_Children(rhs.m_Children)
			{ }

		~tree()
			{ ClearChildren(); }


		void Parent(const tree<T>* pPar)
			{ m_pParent = pPar; }
		tree<T>*& Parent()
			{ return m_pParent; }
		const tree<T>*& Parent() const
			{ return m_pParent; }

		void RootData(const_reference tData)
			{ m_Root = tData; }
		const_reference RootData() const
			{ return m_Root; }
		reference RootData()
			{ return m_Root; }

		size_type ChildNum()
			{ return m_Children.size(); }

		ChildIterator BeginChild()
			{ return m_Children.begin(); }
		ChildIterator EndChild()
			{ return m_Children.end(); }

		ChildType& Child(size_type index)
			{ return m_Children.at(index); }
		const ChildType& Child(size_type index) const
			{ return m_Children.at(index); }

		void AddChild(const tree<T>& Child)
		{
			ChildType pt(new tree<T>(Child));
			pt->SetParent(this);
			m_Children.push_back(pt);
		}
		void AddChild(const_reference Data)
		{
			m_Children.push_back(ChildType(new tree<T>(Data, this)));
		}
		void DelChild(ChildIterator& iter)
		{
			iter = m_Children.erase(iter);
		}
		void ClearChildren()
		{
			m_Children.clear();
			ChildrenType().swap(m_Children);
		}


		void SetChild(ChildIterator iter, const tree<T>& Child)
		{
			ChildType pt(new tree<T>(Child));
			pt->SetParent(this);
			*iter = pt;
		}
		void SetChildData(ChildIterator iter, const_reference Data)
		{
			if (iter == EndChild())
			{
				AddChild(Data);
			}
			else
			{
				Child(iter).SetRootData(Data);
			}
		}

		// ����
		ChildIterator find(const_reference Data)
		{
			ChildIterator iter = BeginChild();
			while ((iter != EndChild()) && (Data != (*iter)->RootData()))
			{
				++ iter;
			}

			return iter;
		}
		template <typename Predicate>
		ChildIterator find_if(const_reference Data, Predicate op)
		{
			ChildIterator iter = BeginChild();
			while ((iter != EndChild()) && !op(Data, (*iter)->RootData()))
			{
				++ iter;
			}

			return iter;
		}

		// ����
		template <typename Function>
		void PreRoot(Function op)
		{
			op(RootData());

			for (ChildIterator iter = BeginChild(); iter != EndChild(); ++ iter)
			{
				(*iter)->PreRoot(op);
			}
		}

		template <typename Function>
		void PostRoot(Function op)
		{
			for (ChildIterator iter = BeginChild(); iter != EndChild(); ++ iter)
			{
				(*iter)->PostRoot(op);
			}

			op(RootData());
		}
	};
}

#endif		// _TREE_HPP

