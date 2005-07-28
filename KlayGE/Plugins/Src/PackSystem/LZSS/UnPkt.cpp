// Unpkt.cpp
// KlayGE ����ļ���ȡ�� ʵ���ļ�
// Ver 2.2.0
// ��Ȩ����(C) ������, 2003-2004
// Homepage: http://klayge.sourceforge.net
//
// 2.2.0
// ͳһʹ��istream��Ϊ��Դ��ʾ�� (2004.10.26)
// ʹ��boost::crc������crc32 (2004.10.28)
//
// 2.1.0
// ����Ŀ¼��ı�ʾ�� (2004.4.14)
//
// 2.0.0
// ���ν��� (2003.9.18)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>

#include <string>
#include <vector>
#include <sstream>

#include <boost/assert.hpp>
#pragma warning(disable: 4127 4800)
#include <boost/pool/pool_alloc.hpp>
#pragma warning(disable: 4244 4245)
#include <boost/crc.hpp>

#include <KlayGE/LZSS/LZSS.hpp>

namespace
{
	using namespace KlayGE;

	class LZSS
	{
	public:
		void Decode(std::ostream& out, std::istream& in)
		{
			std::ostreambuf_iterator<char> outIter(out);

			uint32_t r(N - F);
			std::fill_n(textBuf_, r, ' ');

			uint32_t flags(0);
			uint8_t c;
			for (;;)
			{
				if (0 == ((flags >>= 1) & 256))
				{
					in.read(reinterpret_cast<char*>(&c), sizeof(c));
					if (in.fail())
					{
						break;
					}

					flags = c | 0xFF00;		// uses higher byte cleverly
											// to count eight
				}
				if (flags & 1)
				{
					in.read(reinterpret_cast<char*>(&c), sizeof(c));
					if (in.fail())
					{
						break;
					}

					*outIter = c;
					++ outIter;

					textBuf_[r] = c;
					++ r;
					r &= (N - 1);
				}
				else
				{
					in.read(reinterpret_cast<char*>(&c), sizeof(c));
					if (in.fail())
					{
						break;
					}
					uint32_t c1(c);

					in.read(reinterpret_cast<char*>(&c), sizeof(c));
					if (in.fail())
					{
						break;
					}
					uint32_t c2(c);

					c1 |= ((c2 & 0xF0) << 4);
					c2 = (c2 & 0x0F) + THRESHOLD;
					for (uint32_t k = 0; k <= c2; ++ k)
					{
						c = textBuf_[(c1 + k) & (N - 1)];
						
						*outIter = c;
						++ outIter;

						textBuf_[r] = c;
						++ r;
						r &= (N - 1);
					}
				}
			}

			in.clear();
		}

	private:
		static int const N = 4096;			// size of ring buffer
		static int const F = 18;			// upper limit for match_length
		static int const THRESHOLD = 2;		// encode string into position and length
											// if match_length is greater than this
		static int const NIL = N;			// index for root of binary search trees


		uint8_t textBuf_[N + F - 1];				// ring buffer of size N, 
											// with extra F-1 bytes to facilitate string comparison
	};

	// ����Ŀ¼��
	/////////////////////////////////////////////////////////////////////////////////
	void ReadDirTable(DirTable& dirTable, std::istream& input)
	{
		for (;;)
		{
			uint32_t len;
			input.read(reinterpret_cast<char*>(&len), sizeof(len));
			if (input.fail())
			{
				break;
			}

			std::vector<char, boost::pool_allocator<char> > str(len);
			input.read(&str[0], static_cast<std::streamsize>(str.size()));
			if (input.fail())
			{
				break;
			}

			FileDes fd;
			input.read(reinterpret_cast<char*>(&fd), sizeof(fd));
			if (input.fail())
			{
				break;
			}

			dirTable.insert(std::make_pair(std::string(str.begin(), str.end()), fd));
		}

		input.clear();
	}
}

namespace KlayGE
{
	// ���캯��
	/////////////////////////////////////////////////////////////////////////////////
	UnPkt::UnPkt()
	{
	}

	// LZSS��ѹ
	/////////////////////////////////////////////////////////////////////////////////
	void UnPkt::Decode(std::ostream& out, std::istream& in)
	{
		LZSS lzss;
		lzss.Decode(out, in);
	}

	// �򿪴���ļ�
	/////////////////////////////////////////////////////////////////////////////////
	void UnPkt::Open(boost::shared_ptr<std::istream> const & pktFile)
	{
		file_ = pktFile;

		file_->read(reinterpret_cast<char*>(&mag_), sizeof(mag_));
		Verify(MakeFourCC<'p', 'k', 't', ' '>::value == mag_.magic);
		Verify(3 == mag_.ver);

		file_->seekg(mag_.DTStart);

		std::stringstream dtCom;
		{
			std::vector<char, boost::pool_allocator<char> > temp(mag_.DTLength);
			file_->read(&temp[0], static_cast<std::streamsize>(temp.size()));
			dtCom.write(&temp[0], static_cast<std::streamsize>(temp.size()));
		}

		std::stringstream dt;
		dtCom.seekp(0);
		Decode(dt, dtCom);

		dt.seekp(0);
		ReadDirTable(dirTable_, dt);
	}

	// �ڴ���ļ��ж�λ�ļ�
	/////////////////////////////////////////////////////////////////////////////////
	void UnPkt::LocateFile(std::string const & pathName)
	{
		DirTable::iterator iter = dirTable_.find(pathName);
		BOOST_ASSERT(iter != dirTable_.end());

		curFile_ = iter;
	}

	// ��ȡ��ǰ�ļ�(��ѹ����)���ֽ���
	/////////////////////////////////////////////////////////////////////////////////
	size_t UnPkt::CurFileSize() const
	{
		BOOST_ASSERT(curFile_ != dirTable_.end());

		return curFile_->second.DeComLength;
	}

	// ��ȡ��ǰ�ļ�(��ѹ����)
	/////////////////////////////////////////////////////////////////////////////////
	bool UnPkt::ReadCurFile(void* data)
	{
		BOOST_ASSERT(data != NULL);
		BOOST_ASSERT(curFile_ != dirTable_.end());

		file_->seekg(mag_.FIStart + curFile_->second.start);

		if (curFile_->second.attr & FA_UnCompressed)
		{
			file_->read(static_cast<char*>(data), static_cast<std::streamsize>(this->CurFileSize()));
		}
		else
		{
			std::stringstream chunk;
			{
				std::vector<char, boost::pool_allocator<char> > temp(this->CurFileCompressedSize());
				file_->read(&temp[0], static_cast<std::streamsize>(temp.size()));
				chunk.write(&temp[0], static_cast<std::streamsize>(temp.size()));
			}

			std::stringstream out;
			chunk.seekp(0);
			Decode(out, chunk);

			std::copy(std::istreambuf_iterator<char>(out), std::istreambuf_iterator<char>(),
				static_cast<char*>(data));
		}

		boost::crc_32_type crc32;
		crc32.process_bytes(data, this->CurFileSize());

		if (crc32.checksum() != curFile_->second.crc32)
		{
			return false;	// CRC32 ����
		}

		return true;
	}

	// ��ȡ��ǰ�ļ�(û��ѹ����)���ֽ���
	/////////////////////////////////////////////////////////////////////////////////
	size_t UnPkt::CurFileCompressedSize() const
	{
		BOOST_ASSERT(curFile_ != dirTable_.end());

		return curFile_->second.length;
	}

	// ��ȡ��ǰ�ļ�(û��ѹ����)
	/////////////////////////////////////////////////////////////////////////////////
	void UnPkt::ReadCurFileCompressed(void* data)
	{
		BOOST_ASSERT(data != NULL);
		BOOST_ASSERT(curFile_ != dirTable_.end());

		file_->seekg(mag_.FIStart + curFile_->second.start);
		file_->read(static_cast<char*>(data), static_cast<std::streamsize>(this->CurFileCompressedSize()));
	}
}