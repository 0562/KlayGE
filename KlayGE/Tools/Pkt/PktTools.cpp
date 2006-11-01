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
#include <KlayGE/LZSS/LZSS.hpp>

#include <iostream>
#include <fstream>

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

	std::ofstream ofs(argv[3], std::ios_base::binary);

	Pkt pkt;
	pkt.Pack(argv[2], ofs);

	cout << "ѹ���ɹ�" << endl;

	return 0;
}
