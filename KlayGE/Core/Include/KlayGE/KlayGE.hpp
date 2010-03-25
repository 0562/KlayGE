// KlayGE.hpp
// KlayGE ͷ�ļ�
// Ver 3.8.0
// ��Ȩ����(C) ������, 2003-2010
// Homepage: http://klayge.sourceforge.net
//
// 3.8.0
// �ĳ�DLL����ʽ (2008.10.17)
//
// 2.2.0
// ȥ����Safe*���� (2004.10.31)
//
// 2.1.0
// ȥ���˻����� (2004.4.20)
//
// 2.0.0
// ���ν��� (2003.8.10)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#ifndef _KLAYGE_HPP
#define _KLAYGE_HPP

#include <KlayGE/Config.hpp>

#pragma once

#include <KlayGE/Types.hpp>

#define KLAYGE_NAME			KlayGE
#define KLAYGE_VER_MAJOR	3
#define KLAYGE_VER_MINOR	10
#define KLAYGE_VER_RELEASE	0
#define KLAYGE_VER_STR		KLAYGE_STRINGIZE(KLAYGE_NAME)" "KLAYGE_STRINGIZE(KLAYGE_VER_MAJOR)"."KLAYGE_STRINGIZE(KLAYGE_VER_MINOR)"."KLAYGE_STRINGIZE(KLAYGE_VER_RELEASE)

#include <vector>
#include <string>

#include <boost/assert.hpp>

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/Math.hpp>

#include <boost/config/requires_threads.hpp>

#endif		// _KLAYGE_HPP
