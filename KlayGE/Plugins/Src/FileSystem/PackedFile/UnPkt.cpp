// Unpkt.cpp
// KlayGE ����ļ���ȡ�� ʵ���ļ�
// Ver 2.0.0
// ��Ȩ����(C) ������, 2003
// Homepage: http://www.enginedev.com
//
// 2.0.0
// ���ν��� (2003.9.18)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/CommFuncs.hpp>
#include <KlayGE/Memory.hpp>
#include <KlayGE/Engine.hpp>
#include <KlayGE/Crc32.hpp>

#include <cassert>
#include <cctype>
#include <string>

#include <KlayGE/MemFile/MemFile.hpp>
#include <KlayGE/PackedFile/Pkt.hpp>

using std::vector;

namespace
{
	using namespace KlayGE;

	const U32 N(4096);				// size of ring buffer
	const U32 F(18);				// upper limit for match_length
	const U32 THRESHOLD(2);		// encode string into position and length
									//   if match_length is greater than this
	const U32 NIL(N);				// index for root of binary search trees


	static U8 textBuf[N + F - 1];	// ring buffer of size N, 
									// with extra F-1 bytes to facilitate string comparison

	// ���Դ�Сд�Ƚ��ַ���
	/////////////////////////////////////////////////////////////////////////////////
	bool IgnoreCaseCompare(const WString& lhs, const WString& rhs)
	{
		if (lhs.length() != rhs.length())
		{
			return false;
		}

		for (U32 i = 0; i < lhs.length(); ++ i)
		{
			if (std::toupper(lhs[i]) != std::toupper(rhs[i]))
			{
				break;
			}
		}

		if (i != lhs.length())
		{
			return false;
		}

		return true;
	}

	// ��Ŀ¼��ת�������ͽṹ
	/////////////////////////////////////////////////////////////////////////////////
	void Translate(tree<KlayGE::FileDes>* pOut, VFile& In)
	{
		using namespace KlayGE;

		String str;
		FileDes fd;

		U8 tag;
		U8 len;
		while (In.Tell() != In.Length())
		{
			str.clear();
			In.Read(&tag, sizeof(tag));

			switch (tag)
			{
			case DIT_File:
			case DIT_Dir:
				In.Read(&len, sizeof(len));
				str.resize(len);
				In.Read(&str[0], str.length());

				Convert(fd.fileName, str);
				In.Read(&fd.start, sizeof(U32) * 4 + sizeof(U8));
				pOut->AddChild(fd);

				if (DIT_Dir == tag)
				{
					pOut = (pOut->EndChild() - 1)->Get();
				}
				break;

			case DIT_UnDir:
				pOut = pOut->Parent();
				break;
			}
		}
	}
}

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	UnPkt::UnPkt()
			: curFile_(NULL)
	{
		curDir_ = &dirTable_;
	}

	// ��������
	/////////////////////////////////////////////////////////////////////////////////
	UnPkt::~UnPkt()
	{
		this->Close();
	}

	// LZSS��ѹ
	/////////////////////////////////////////////////////////////////////////////////
	void UnPkt::Decode(VFile& Out, VFile& In)
	{
		U32 r(N - F);
		Engine::MemoryInstance().Set(textBuf, ' ', r);

		U32 flags(0);
		U8 c;
		for (;;)
		{
			if (0 == ((flags >>= 1) & 256))
			{
				if (In.Tell() >= In.Length())
				{
					break;
				}

				In.Read(&c, 1);

				flags = c | 0xFF00;		// uses higher byte cleverly
											// to count eight
			}
			if (flags & 1)
			{
				if (In.Tell() >= In.Length())
				{
					break;
				}

				In.Read(&c, 1);

				Out.Write(&c, 1);
				textBuf[r] = c;
				++ r;
				r &= (N - 1);
			}
			else
			{
				if (In.Tell() >= In.Length())
				{
					break;
				}
				In.Read(&c, 1);
				U32 c1 = c;

				if (In.Tell() >= In.Length())
				{
					break;
				}
				In.Read(&c, 1);
				U32 c2 = c;

				c1 |= ((c2 & 0xF0) << 4);
				c2 = (c2 & 0x0F) + THRESHOLD;
				for (U32 k = 0; k <= c2; ++ k)
				{
					c = textBuf[(c1 + k) & (N - 1)];
					Out.Write(&c, 1);
					textBuf[r] = c;
					++ r;
					r &= (N - 1);
				}
			}
		}

		Out.Seek(0, VFile::SM_Begin);
	}

	// �򿪴���ļ�
	/////////////////////////////////////////////////////////////////////////////////
	void UnPkt::Open(const VFile& pktFile)
	{
		Close();

		file_ = pktFile.Clone();

		file_->Read(&mag_, sizeof(mag_));
		Verify(MakeFourCC<'p', 'k', 't', ' '>::value == mag_.magic);
		Verify(3 == mag_.ver);

		file_->Seek(mag_.DTStart, VFile::SM_Begin);

		MemFile DTCom;
		DTCom.CopyFrom(*file_, mag_.DTLength);

		MemFile DT;
		DTCom.Seek(0, VFile::SM_Begin);
		Decode(DT, DTCom);

		DT.Seek(0, VFile::SM_Begin);
		Translate(&dirTable_, DT);
		curDir_ = &dirTable_;
	}

	// �رմ���ļ�
	/////////////////////////////////////////////////////////////////////////////////
	void UnPkt::Close()
	{
		file_.Release();
		curFile_ = NULL;
	}

	// ���ô���ļ��еĵ�ǰ·��
	/////////////////////////////////////////////////////////////////////////////////
	void UnPkt::Dir(const WString& dirName)
	{
		WString theDirName;
		TransPathName(theDirName, dirName);

		// ʶ��·��
		if (L'/' == theDirName[0])
		{
			curDir_ = &dirTable_;
		}
		if ((L'.' == theDirName[0]) && (L'/' == theDirName[1]))
		{
			theDirName = dirName.substr(2, dirName.length() - 2);
		}
		if ((L'.' == theDirName[0]) && (L'.' == theDirName[1]) && (L'/' == theDirName[2]))
		{
			if (curDir_ != NULL)
			{
				curDir_ = curDir_->Parent();
			}
		}

		for (;;)
		{
			WString::size_type nPos = theDirName.find_first_of(L'/');
			WString curDir;
			if (nPos == WString::npos)
			{
				curDir = theDirName;
				theDirName.clear();
			}
			else
			{
				curDir	= theDirName.substr(0, nPos);
				theDirName = theDirName.substr(nPos + 1, theDirName.length());
			}

			if (curDir.empty())
			{
				break;
			}

			// ����Ŀ¼
			FileIterator iter = curDir_->BeginChild();
			while ((iter != curDir_->EndChild())
						&& !IgnoreCaseCompare((*iter)->RootData().fileName, curDir))
			{
				++ iter;
			}
			if (iter == curDir_->EndChild())
			{
				THR(E_FAIL);
			}
			curDir_ = iter->Get();
		}
	}

	// �ڴ���ļ��ж�λ�ļ�
	/////////////////////////////////////////////////////////////////////////////////
	void UnPkt::LocateFile(const WString& pathName)
	{
		WString thePathName;
		TransPathName(thePathName, pathName);

		WString::size_type nPos = thePathName.find_last_of(L'/');

		WString dir(thePathName, 0, nPos == WString::npos ? 0 : nPos);
		WString name(thePathName, nPos + 1, thePathName.length() - 1 - nPos);

		this->Dir(dir);

		// �����ļ�
		FileIterator iter = curDir_->BeginChild();
		while ((iter != curDir_->EndChild())
						&& !IgnoreCaseCompare((*iter)->RootData().fileName, name))
		{
			++ iter;
		}
		if (iter == curDir_->EndChild())
		{
			THR(E_FAIL);
		}

		this->LocateFile(iter);
	}

	void UnPkt::LocateFile(FileIterator iter)
	{
		curFile_ = &((*iter)->RootData());
	}

	// ��ȡ��ǰ�ļ�(��ѹ����)���ֽ���
	/////////////////////////////////////////////////////////////////////////////////
	size_t UnPkt::CurFileSize() const
	{
		assert(curFile_ != NULL);

		return curFile_->DeComLength;
	}

	// ��ȡ��ǰ�ļ�(��ѹ����)
	/////////////////////////////////////////////////////////////////////////////////
	bool UnPkt::ReadCurFile(void* data)
	{
		assert(data != NULL);
		assert(curFile_ != NULL);

		file_->Seek(mag_.FIStart + curFile_->start, VFile::SM_Begin);

		if (curFile_->attr & FA_UnCompressed)
		{
			file_->Read(data, this->CurFileSize());
		}
		else
		{
			MemFile chunk;
			chunk.CopyFrom(*file_, this->CurFileCompressedSize());

			MemFile out;
			chunk.Rewind();
			Decode(out, chunk);

			out.Read(data, out.Length());
		}

		if (Crc32::CrcMem(static_cast<U8*>(data), this->CurFileSize()) != curFile_->crc32)
		{
			return false;	// CRC32 ����
		}

		return true;
	}

	// ��ȡ��ǰ�ļ�(û��ѹ����)���ֽ���
	/////////////////////////////////////////////////////////////////////////////////
	size_t UnPkt::CurFileCompressedSize() const
	{
		assert(curFile_ != NULL);

		return curFile_->length;
	}

	// ��ȡ��ǰ�ļ�(û��ѹ����)
	/////////////////////////////////////////////////////////////////////////////////
	void UnPkt::ReadCurFileCompressed(void* data)
	{
		assert(data != NULL);
		assert(curFile_ != NULL);

		file_->Seek(mag_.FIStart + curFile_->start, VFile::SM_Begin);
		file_->Read(data, this->CurFileCompressedSize());
	}

	// ��ȡ��ǰĿ¼�µĵ�һ���ļ��ĵ�����
	/////////////////////////////////////////////////////////////////////////////////
	UnPkt::FileIterator UnPkt::BeginFile()
	{
		return curDir_->BeginChild();
	}

	// ��ȡ��ǰĿ¼�µ����һ���ļ�����һ��������
	/////////////////////////////////////////////////////////////////////////////////
	UnPkt::FileIterator UnPkt::EndFile()
	{
		return curDir_->EndChild();
	}
}