// SetVector.hpp
// KlayGE ������������ģ�� ͷ�ļ�
// Ver 2.0.2
// ��Ȩ����(C) ������, 2003
// Homepage: http://enginedev.home.g365.net
//
// 2.0.2
// ���ν��� (2003.12.15)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _SETVECTOR_HPP
#define _SETVECTOR_HPP

#include <KlayGE/alloc.hpp>

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

namespace KlayGE
{
	template <typename Key, class Traits = std::less<Key>, class Allocator = alloc<Key> >
	class SetVector
	{
		typedef std::vector<Key, Allocator> ContainerType;
		typedef Traits CompareType;

	public:
		typedef Traits			key_compare;
		typedef Key				key_type;
		typedef Key				value_type;
		typedef Allocator		allocator_type;

		typedef typename ContainerType::const_iterator			const_iterator;
		typedef typename ContainerType::const_reverse_iterator	const_reverse_iterator;
		typedef typename ContainerType::iterator				iterator;
		typedef typename ContainerType::reverse_iterator		reverse_iterator;

		typedef typename ContainerType::size_type				size_type;
		typedef typename ContainerType::difference_type			difference_type;
		typedef typename ContainerType::value_type				value_type;

		typedef typename Allocator::pointer					pointer;
		typedef typename Allocator::const_pointer			const_pointer;
		typedef typename Allocator::reference				reference;
		typedef typename Allocator::const_reference			const_reference;

	public:
		explicit SetVector(const key_compare& comp = key_compare(), 
								const Allocator& alloc = Allocator())
					: container_(alloc), compare_(comp)
			{ }

		template <class InputIterator>
        SetVector(InputIterator first, InputIterator last, 
						const key_compare& comp = key_compare(), 
						const Allocator& alloc = Allocator())
					: container_(first, last, alloc), compare_(comp)
			{ std::sort(this->begin(), this->end(), compare_); }
        
        SetVector& operator=(const SetVector& rhs)
        { 
            SetVector(rhs).swap(*this); 
            return *this;
        }

		iterator begin()
			{ return container_.begin(); }
		const_iterator begin() const
			{ return container_.begin(); }

		iterator end()
			{ return container_.end(); }
		const_iterator end() const
			{ return container_.end(); }

		void clear()
			{ container_.clear(); }

		bool empty() const
			{ container_.empty(); }

		size_type max_size() const
			{ return container_.max_size(); }

		size_type size() const
			{ return container_.size(); }

		size_type count(const Key& key) const
			{ return find(key) != end() }

		std::pair<iterator, iterator> equal_range(const Key& key)
			{ return std::equal_range(begin(), end(), k, compare_); }
		std::pair<const_iterator, const_iterator> equal_range(const Key& key) const
			{ return std::equal_range(begin(), end(), k, compare_); }

		iterator erase(iterator where)
			{ container_.erase(where); }
		iterator erase(iterator first, iterator last)
			{ container_.erase(first, last); }
		size_type erase(const key_type& key)
		{
			iterator iter(this->find(key));
			if (iter != this->end())
			{
				this->erase(iter);
				return 1;
			}
			return 0;
		}

		iterator find(const Key& key)
		{
			iterator iter(this->lower_bound(key));
			if (iter != this->end() && compare_(key, iter->first))
			{
				iter = end();
			}
			return iter;
		}
		const_iterator find(const Key& key) const
		{
			const_iterator iter(this->lower_bound(key));
			if (iter != this->end() && compare_(key, iter->first))
			{
				iter = end();
			}
			return iter;
		}

		Allocator get_allocator() const
		{
			return container_.get_allocator();
		}

		std::pair<iterator, bool> insert(const value_type& val)
		{
			bool found(true);
			iterator iter(this->lower_bound(val.first));

			if ((iter == end()) || compare_(val.first, iter->first))
			{
				iter = container_.insert(iter, val);
				found = false;
			}
			return std::make_pair(iter, !found);
		}
		iterator insert(iterator where, const value_type& val)
		{
			if (where != this->end() && compare_(*pos, val) &&
				(where == this->end() - 1 ||
					!compare_(val, where[1]) &&
					compare_(where[1], val)))
			{
				return container_.insert(where, val);
			}
			return this->insert(val).first;
		}
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			for (; first != last; ++ first)
			{
				this->insert(*first);
			}
		}

		key_compare key_comp() const
			{ return compare_; }

		iterator lower_bound(const Key& key)
			{ return std::lower_bound(this->begin(), this->end(), key, compare_); }
		const_iterator lower_bound(const Key& key) const
			{ return std::lower_bound(this->begin(), this->end(), key, compare_); }

		iterator upper_bound(const Key& key)
			{ return std::upper_bound(this->begin(), this->end(), key, compare_); }
		const_iterator upper_bound(const Key& key) const
			{ return std::upper_bound(this->begin(), this->end(), key, compare_); }

		reverse_iterator rbegin()
			{ return container_.rbegin(); }
		const_reverse_iterator rbegin() const
			{ return container_.rbegin(); }

		reverse_iterator rend()
			{ return container_.rend(); }
		const_reverse_iterator rend() const
			{ return container_.rend(); }

		void swap(SetVector& rhs)
		{
			std::swap(container_, rhs.container_);
			std::swap(compare_, rhs.compare_);
		}

		value_compare value_comp() const
			{ return value_compare(compare_); }

			
		friend bool operator==(const SetVector& lhs, const SetVector& rhs)
			{ return lhs.container_ == rhs.container_; } 

		friend bool operator<(const SetVector& lhs, const SetVector& rhs)
			{ return lhs.container_ < rhs.container_; } 

	private:
		ContainerType	container_;
		CompareType		compare_;
	};

	template <class Key, class Type, class Traits, class Allocator>
	inline void
	swap(SetVector<Key, Type, Traits, Allocator>& lhs, SetVector<Key, Type, Traits, Allocator>& rhs)
	{
		lhs.swap(rhs);
	}

	template <class Key, class Type, class Traits, class Allocator>
	inline bool
	operator!=(const SetVector<Key, Type, Traits, Allocator>& lhs,
					const SetVector<Key, Type, Traits, Allocator>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class Key, class Type, class Traits, class Allocator>
	inline bool
	operator>(const SetVector<Key, Type, Traits, Allocator>& lhs,
					const SetVector<Key, Type, Traits, Allocator>& rhs)
	{
		return rhs < lhs;
	}

	template <class Key, class Type, class Traits, class Allocator>
	inline bool
	operator>=(const SetVector<Key, Type, Traits, Allocator>& lhs,
					const SetVector<Key, Type, Traits, Allocator>& rhs)
	{
		return !(lhs < rhs);
	}

	template <class Key, class Type, class Traits, class Allocator>
	inline bool
	operator<=(const SetVector<Key, Type, Traits, Allocator>& lhs,
					const SetVector<Key, Type, Traits, Allocator>& rhs)
	{
		return !(rhs < lhs);
	}
}

#endif			// _SETVECTOR_HPP
