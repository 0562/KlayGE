// Pkt.cpp
// KlayGE �ļ��������
// Ver 1.3.8.1
// ��Ȩ����(C) ������, 2002
// Homepage: http://www.enginedev.com
//
// 1.3.8.1
// ��д�˴������ (2002.10.24)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/VFile.hpp>
#include <KlayGE/DiskFile/DiskFile.hpp>
#include <KlayGE/PackedFile/Pkt.hpp>

#include <iostream>
#include <string>

using namespace std;
using namespace KlayGE;

int main(int argc, char* argv[])
{
	if (4 != argc)
	{
		cout << "ʹ�÷���:" << endl
			<< "ѹ������Ŀ¼: Pkt p Ŀ¼�� �ļ���.pkt" << endl;
		return 1;
	}

	cout << "����ѹ�������Ժ�..." << endl;

	Pkt pkt;
	pkt.Pack(argv[2], DiskFile(argv[3], VFile::OM_Write));

	cout << "ѹ���ɹ�" << endl;

	return 0;
}
