// MemoryImpl.cpp
// KlayGE �ڴ溯����ʵ�� ʵ���ļ�
// Ver 2.0.4
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.0.4
// �Ż���3DNowEx��Cpy
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
					mov			edi, dstptr

					movq		[edi],		mm0
					movq		[edi + 8],  mm1
					movq		[edi + 16], mm2
					movq		[edi + 24], mm3
					movq		[edi + 32], mm4
					movq		[edi + 40], mm5
					movq		[edi + 48], mm6
					movq		[edi + 56], mm7
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
					mov			esi, buf1ptr
					mov			edi, buf2ptr

					movq		mm0, [esi]
					movq		mm1, [esi + 8]
					movq		mm2, [esi + 16]
					movq		mm3, [esi + 24]

					movq		mm4, [edi]
					movq		mm5, [edi + 8]
					movq		mm6, [edi + 16]
					movq		mm7, [edi + 24]

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

				movq		mm0, [esi]
				movq		mm1, [esi + 8]
				movq		mm2, [esi + 16]
				movq		mm3, [esi + 24]
				movq		mm4, [esi + 32]
				movq		mm5, [esi + 40]
				movq		mm6, [esi + 48]
				movq		mm7, [esi + 56]

				movq		[edi],		mm0
				movq		[edi + 8],  mm1
				movq		[edi + 16], mm2
				movq		[edi + 24], mm3
				movq		[edi + 32], mm4
				movq		[edi + 40], mm5
				movq		[edi + 48], mm6
				movq		[edi + 56], mm7
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
					mov			edi, dstptr

					movntq		[edi],	    mm0
					movntq		[edi + 8],  mm1
					movntq		[edi + 16], mm2
					movntq		[edi + 24], mm3
					movntq		[edi + 32], mm4
					movntq		[edi + 40], mm5
					movntq		[edi + 48], mm6
					movntq		[edi + 56], mm7
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
					mov			esi, buf1ptr
					mov			edi, buf2ptr

					prefetchnta	[esi + 128]
					movq		mm0, [esi]
					movq		mm1, [esi + 8]
					movq		mm2, [esi + 16]
					movq		mm3, [esi + 24]

					prefetchnta	[edi + 128]
					movq		mm4, [edi]
					movq		mm5, [edi + 8]
					movq		mm6, [edi + 16]
					movq		mm7, [edi + 24]

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
				mov			esi, src
				mov			edi, dst

				prefetchnta	[esi + 256]

				movq		mm0, [esi]
				movq		mm1, [esi + 8]
				movq		mm2, [esi + 16]
				movq		mm3, [esi + 24]
				movq		mm4, [esi + 32]
				movq		mm5, [esi + 40]
				movq		mm6, [esi + 48]
				movq		mm7, [esi + 56]

				movntq		[edi],	    mm0
				movntq		[edi + 8],  mm1
				movntq		[edi + 16], mm2
				movntq		[edi + 24], mm3
				movntq		[edi + 32], mm4
				movntq		[edi + 40], mm5
				movntq		[edi + 48], mm6
				movntq		[edi + 56], mm7
			}
		}
	}
}
