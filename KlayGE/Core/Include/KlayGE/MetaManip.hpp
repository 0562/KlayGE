// MetaManip.hpp
// KlayGE Դ��̿� ͷ�ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
// ��Ҫ����Loki
//
// 2.0.0
// ���ν��� (2003.8.6)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _METAMANIP_HPP
#define _METAMANIP_HPP

namespace KlayGE
{
	// �����ڶ���
	////////////////////////////////////////////////////////////////////////////////
	template <bool T>
	class StaticAssert;

	template <>
	class StaticAssert<true>
	{
	};

	// �ѳ���ת����Ψһ������
	// ע��: Int2Type<v> �� v ��һ����������������
	////////////////////////////////////////////////////////////////////////////////
	template <int v>
	struct Int2Type
	{
		enum { value = v };
	};

	// ����bool������������ѡ��һ������
	// ע��: If<flag, T, U>::Result ��
	// flag ��һ��������bool����
	// T �� U ������
	// ���flag == true, Result��T, ������U
	////////////////////////////////////////////////////////////////////////////////
	template <bool flag, typename T, typename U>
	struct If
	{
	private:
		template<bool>
		struct In
		{
			typedef T Result;
		};

		template<>
		struct In<false>
		{
			typedef U Result;
		};

	public:
		typedef typename In<flag>::Result Result;
	};

	// �������������������ͬ������true
	// ע��: SameType<T, U>::value ��
	// T �� U ������
	////////////////////////////////////////////////////////////////////////////////
	template <typename T, typename U>
	struct SameType
	{
	private:
		template<typename>
		struct In 
		{
			enum { value = false };
		};

		template<>
		struct In<T>
		{
			enum { value = true };
		};

	public:
		enum { value = In<U>::value };
	};

	// �ж�T�Ƿ������ʽת��ΪU
	////////////////////////////////////////////////////////////////////////////////
	template <class T, class U>
	class Conversion
	{
		typedef char Small;
		struct Big { char dummy[2]; };
		static Big Test(...);
		static Small Test(U);
		static T MakeT();

	public:
		enum { exists = sizeof(typename Small) == sizeof(Test(MakeT())) };
	};
}

#endif			// _METAMANIP_HPP
