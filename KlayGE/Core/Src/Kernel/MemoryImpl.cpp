// MemoryImpl.cpp
// KlayGE �ڴ溯����ʵ�� ʵ���ļ�
// Ver 1.3.8.1
// ��Ȩ����(C) ������, 2002
// Homepage: http://www.enginedev.com
//
// 1.2.8.8
// �Ż��˵��ֽڲ��� (2002.10.1)
//
// 1.3.8.1
// ��������������򿽱����� (2002.12.5)
//
// �޸ļ�¼
///////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Memory.hpp>

#include "MemoryImpl.hpp"

namespace KlayGE
{
	namespace KlayGEImpl
	{
		// ��destָ��Ĵ�СΪcount���ڴ����Ϊc
		/////////////////////////////////////////////////////////////////////
		void* MemStandardLib::Set(void* dest, int c, size_t count) const
		{
			return memset(dest, c, count);
		}

		// ��buf1��buf2�����ݱȽϣ���ͬ����true
		/////////////////////////////////////////////////////////////////////
		bool MemStandardLib::Cmp(const void* dest, const void* src, size_t count) const
		{
			return (0 == memcmp(dest, src, count));
		}

		// ��srcָ��Ĵ�СΪcount���ڴ濽����dest
		//////////////////////////////////////////////////////////////////////////
		void* MemStandardLib::Cpy(void* dest, const void* src, size_t count) const
		{
			return memcpy(dest, src, count);
		}


		// ��destָ��Ĵ�СΪcount���ڴ����Ϊc
		/////////////////////////////////////////////////////////////////////
		void* MemMMXLib::Set(void* dest, int c, size_t count) const
		{
			const size_t ali64Num(count >> 6);
			U8* dstptr(static_cast<U8*>(dest));

			_asm
			{
				movd		mm0, c					// mm0 = 0/0/0/0/0/0/0/c
				punpcklbw	mm0, mm0				// mm0 = 0/0/0/0/0/0/c/c
				punpcklwd	mm0, mm0				// mm0 = 0/0/0/0/c/c/c/c
				punpckldq	mm0, mm0				// mm0 = c/c/c/c/c/c/c/c

				movq		mm1, mm0
				movq		mm2, mm0
				movq		mm3, mm0
				movq		mm4, mm0
				movq		mm5, mm0
				movq		mm6, mm0
				movq		mm7, mm0
			}

			for (size_t i = 0; i < ali64Num; ++ i)
			{
				_asm
				{
					mov			ebx, dstptr

					movq		[ebx],		mm0
					movq		[ebx + 8],  mm1
					movq		[ebx + 16], mm2
					movq		[ebx + 24], mm3
					movq		[ebx + 32], mm4
					movq		[ebx + 40], mm5
					movq		[ebx + 48], mm6
					movq		[ebx + 56], mm7
				}

				dstptr += 64;
			}

			_asm emms;

			memset(dstptr, c, count & 63);
			return dest;
		}

		// ��pBuf1��pBuf2�����ݱȽϣ���ͬ����true
		/////////////////////////////////////////////////////////////////////
		bool MemMMXLib::Cmp(const void* buf1, const void* buf2, size_t count) const
		{
			const size_t ali32Num(count >> 5);

			U8* buf1ptr(static_cast<U8*>(const_cast<void*>(buf1)));
			U8* buf2ptr(static_cast<U8*>(const_cast<void*>(buf2)));

			U32 reg(0);
			for (size_t i = 0; i < ali32Num; ++ i)
			{
				_asm
				{
					mov			ebx, buf1ptr
					mov			ecx, buf2ptr

					movq		mm0, [ebx]
					movq		mm1, [ebx + 8]
					movq		mm2, [ebx + 16]
					movq		mm3, [ebx + 24]

					movq		mm4, [ecx]
					movq		mm5, [ecx + 8]
					movq		mm6, [ecx + 16]
					movq		mm7, [ecx + 24]

					pcmpeqb		mm0, mm4
					pcmpeqb		mm1, mm5
					pcmpeqb		mm2, mm6
					pcmpeqb		mm3, mm7

					pand		mm0, mm1
					pand		mm0, mm2
					pand		mm0, mm3

					movq		mm1, mm0
					psrlq		mm1, 32
					pand		mm0, mm1

					movd		reg, mm0
				}

				buf1ptr += 32;
				buf2ptr += 32;

				if (reg != 0xFFFFFFFF)
				{
					_asm emms;
					return false;
				}
			}

			_asm emms;

			return (0 == memcmp(buf1ptr, buf2ptr, count & 31));
		}

		// ��pSrcָ��Ĵ�СΪCount���ڴ濽����pDest, ʹ��MMXָ�
		//////////////////////////////////////////////////////////////////////////
		void* MemMMXLib::Cpy(void* dest, const void* src, size_t count) const
		{
			const size_t ali64Num(count >> 6);

			U8* dstptr(static_cast<U8*>(dest));
			U8* srcptr(static_cast<U8*>(const_cast<void*>(src)));

			if ((srcptr + count > dest) && (srcptr < dstptr))
			{
				dstptr += count;
				srcptr += count;
				for (size_t i = 0; i < ali64Num; ++ i)
				{
					dstptr -= 64;
					srcptr -= 64;
					this->Cpy64(dstptr, srcptr);
				}
			}
			else
			{
				for (size_t i = 0; i < ali64Num; ++ i)
				{
					this->Cpy64(dstptr, srcptr);
					dstptr += 64;
					srcptr += 64;
				}
			}

			_asm emms;

			memcpy(dstptr, srcptr, count & 63);

			return dest;
		}

		// ����64���ֽ�
		//////////////////////////////////////////////////////////////////////////
		void MemMMXLib::Cpy64(void* dst, const void* src) const
		{
			_asm
			{
				mov			esi, src					// esi = src
				mov			edi, dst					// edi = dst

				lea			esi, [esi + ecx * 8]
				lea			edi, [edi + ecx * 8]
				neg			ecx

				movq		mm0, [esi + ecx * 8]
				movq		mm1, [esi + ecx * 8 + 8]
				movq		mm2, [esi + ecx * 8 + 16]
				movq		mm3, [esi + ecx * 8 + 24]
				movq		mm4, [esi + ecx * 8 + 32]
				movq		mm5, [esi + ecx * 8 + 40]
				movq		mm6, [esi + ecx * 8 + 48]
				movq		mm7, [esi + ecx * 8 + 56]

				movq		[edi + ecx * 8],	  mm0
				movq		[edi + ecx * 8 + 8],  mm1
				movq		[edi + ecx * 8 + 16], mm2
				movq		[edi + ecx * 8 + 24], mm3
				movq		[edi + ecx * 8 + 32], mm4
				movq		[edi + ecx * 8 + 40], mm5
				movq		[edi + ecx * 8 + 48], mm6
				movq		[edi + ecx * 8 + 56], mm7
			}
		}

		void* Mem3DNowExLib::Set(void* dest, int c, size_t count) const
		{
			const size_t ali64Num(count >> 6);
			U8* dstptr(static_cast<U8*>(dest));

			_asm
			{
				movd		mm0, c					// mm0 = 0/0/0/0/0/0/0/c
				punpcklbw	mm0, mm0				// mm0 = 0/0/0/0/0/0/c/c
				pshufw		mm0, mm0, 0				// mm0 = c/c/c/c/c/c/c/c

				movq		mm1, mm0
				movq		mm2, mm0
				movq		mm3, mm0
				movq		mm4, mm0
				movq		mm5, mm0
				movq		mm6, mm0
				movq		mm7, mm0
			}

			for (size_t i = 0; i < ali64Num; ++ i)
			{
				_asm
				{
					mov			ebx, dstptr

					movntq		[ebx],	    mm0
					movntq		[ebx + 8],  mm1
					movntq		[ebx + 16], mm2
					movntq		[ebx + 24], mm3
					movntq		[ebx + 32], mm4
					movntq		[ebx + 40], mm5
					movntq		[ebx + 48], mm6
					movntq		[ebx + 56], mm7
				}

				dstptr += 64;
			}

			_asm
			{
				sfence
				emms
			}

			memset(dstptr, c, count & 63);
			return dest;
		}

		bool Mem3DNowExLib::Cmp(const void* buf1, const void* buf2, size_t count) const
		{
			const size_t ali32Num(count >> 5);

			U8* buf1ptr(static_cast<U8*>(const_cast<void*>(buf1)));
			U8* buf2ptr(static_cast<U8*>(const_cast<void*>(buf2)));

			U32 reg(0);
			for (size_t i = 0; i < ali32Num; ++ i)
			{
				_asm
				{
					mov			ebx, buf1ptr
					mov			ecx, buf2ptr

					prefetchnta	[ebx + 64]
					movq		mm0, [ebx]
					movq		mm1, [ebx + 8]
					movq		mm2, [ebx + 16]
					movq		mm3, [ebx + 24]

					prefetchnta	[ecx + 64]
					movq		mm4, [ecx]
					movq		mm5, [ecx + 8]
					movq		mm6, [ecx + 16]
					movq		mm7, [ecx + 24]

					pcmpeqb		mm0, mm4
					pcmpeqb		mm1, mm5
					pcmpeqb		mm2, mm6
					pcmpeqb		mm3, mm7

					pand		mm0, mm1
					pand		mm0, mm2
					pand		mm0, mm3

					pmovmskb	eax, mm0
					mov			reg, eax
				}

				buf1ptr += 32;
				buf2ptr += 32;

				if (reg != 0xFF)
				{
					_asm emms;
					return false;
				}
			}

			_asm emms;

			return (0 == memcmp(buf1ptr, buf2ptr, count & 31));
		}

		void* Mem3DNowExLib::Cpy(void* dest, const void* src, size_t count) const
		{
			const size_t ali64Num(count >> 6);

			U8* dstptr(static_cast<U8*>(dest));
			U8* srcptr(static_cast<U8*>(const_cast<void*>(src)));

			if ((srcptr + count > dstptr) && (srcptr < dstptr))
			{
				dstptr += count;
				srcptr += count;
				for (size_t i = 0; i < ali64Num; ++ i)
				{
					dstptr -= 64;
					srcptr -= 64;
					this->Cpy64(dstptr, srcptr);
				}
			}
			else
			{
				for (size_t i = 0; i < ali64Num; ++ i)
				{
					this->Cpy64(dstptr, srcptr);
					dstptr += 64;
					srcptr += 64;
				}
			}

			_asm
			{
				sfence
				emms
			}

			memcpy(dstptr, srcptr, count & 63);

			return dest;
		}

		void Mem3DNowExLib::Cpy64(void* dst, const void* src) const
		{
			_asm
			{
				mov			ebx, src
				mov			ecx, dst

				prefetchnta	[ebx + 128]

				movq		mm0, [ebx]
				movq		mm1, [ebx + 8]
				movq		mm2, [ebx + 16]
				movq		mm3, [ebx + 24]
				movq		mm4, [ebx + 32]
				movq		mm5, [ebx + 40]
				movq		mm6, [ebx + 48]
				movq		mm7, [ebx + 56]

				movntq		[ecx],	    mm0
				movntq		[ecx + 8],  mm1
				movntq		[ecx + 16], mm2
				movntq		[ecx + 24], mm3
				movntq		[ecx + 32], mm4
				movntq		[ecx + 40], mm5
				movntq		[ecx + 48], mm6
				movntq		[ecx + 56], mm7
			}
		}
	}
}
