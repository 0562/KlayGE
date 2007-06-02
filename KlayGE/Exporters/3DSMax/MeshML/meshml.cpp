// meshml.cpp
// KlayGE meshml������ ʵ���ļ�
// Ver 3.6.0
// ��Ȩ����(C) ������, 2005-2007
// Homepage: http://klayge.sourceforge.net
//
// 3.6.0
// ���˶Ի��� (2007.6.2)
//
// 2.5.0
// ���ν��� (2005.5.1)
//
// �޸ļ�¼
/////////////////////////////////////////////////////////////////////////////////

#pragma conform(forScope, push, i, off)
#include <max.h>
#include <Interval.h>
#pragma conform(forScope, pop, i)

#include <vector>
#include <sstream>

#include "util.hpp"
#include "export_main.hpp"
#include "mesh_extractor.hpp"
#include "meshml.hpp"

namespace KlayGE
{
	meshml_export::meshml_export()
	{
	}

	meshml_export::~meshml_export()
	{
	}

	int meshml_export::ExtCount()
	{
		return 1;
	}

	TCHAR const * meshml_export::Ext(int n)
	{
		switch (n)
		{
		case 0:
			return TEXT("meshml");

		default:
			return NULL;
		}
	}

	TCHAR const * meshml_export::LongDesc()
	{
		return TEXT("Mesh XML File");
	}

	TCHAR const * meshml_export::ShortDesc()
	{
		return TEXT("MeshML File");
	}

	TCHAR const * meshml_export::AuthorName()
	{
		return TEXT("Minmin Gong");
	}

	TCHAR const * meshml_export::CopyrightMessage()
	{
		return TEXT("Copyright 2005-2007");
	}

	TCHAR const * meshml_export::OtherMessage1()
	{
		return TEXT("");
	}

	TCHAR const * meshml_export::OtherMessage2()
	{
		return TEXT("");
	}

	unsigned int meshml_export::Version()
	{
		return 010;
	}

	void meshml_export::ShowAbout(HWND hWnd)
	{
		::MessageBox(hWnd, TEXT("Minmin Gong, Copyright 2005-2007"), TEXT("About"), MB_OK);
	}

	BOOL meshml_export::SupportsOptions(int ext, DWORD options)
	{
		if ((options & SCENE_EXPORT_SELECTED) && (0 == ext))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	int meshml_export::DoExport(TCHAR const * name, ExpInterface* exp_interface, Interface* max_interface,
							BOOL suppress_prompts, DWORD options)
	{
		file_name_ = tstr_to_str(name);

		max_interface_ = max_interface;

		export_nodes_.clear();
		if (options & SCENE_EXPORT_SELECTED)
		{
			int count = max_interface_->GetSelNodeCount();
			for (int i = 0; i < count; ++ i)
			{
				this->enum_node(max_interface_->GetSelNode(i));
			}
		}
		else
		{
			this->export_nodes_.clear();
			INode* root_node = this->max_interface_->GetRootNode();
			int const num_children = root_node->NumberOfChildren();
			for (int i = 0; i < num_children; ++ i)
			{
				this->enum_node(root_node->GetChildNode(i));
			}
		}

		HWND max_wnd = max_interface->GetMAXHWnd();
		if (::DialogBoxParam(dll_instance, MAKEINTRESOURCE(IDD_MESHML_EXPORT), max_wnd,
			export_wnd_proc, reinterpret_cast<LPARAM>(this)))
		{
			MessageBox(max_wnd, TEXT("�����ɹ���"), TEXT("MeshML Export"), MB_OK);
		}

		return 1;
	}

	void meshml_export::enum_node(INode* node)
	{
		if (!node->IsNodeHidden())
		{
			export_nodes_.push_back(node);
		}

		int num_children = node->NumberOfChildren();
		for (int i = 0; i < num_children; ++ i)
		{
			this->enum_node(node->GetChildNode(i));
		}
	}

	INT_PTR WINAPI meshml_export::export_wnd_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (WM_INITDIALOG == msg)
		{
			meshml_export* instance = reinterpret_cast<meshml_export*>(lparam);
			assert(instance != NULL);

			::SetWindowLongPtr(wnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(instance));
		}
		else
		{
			meshml_export* instance = reinterpret_cast<meshml_export*>(::GetWindowLongPtr(wnd, GWL_USERDATA));

			switch (msg)
			{
			case WM_COMMAND:
				switch (LOWORD(wparam))
				{
				case IDOK:
					{
						assert(instance != NULL);

						int const joint_per_ver = ::SendMessage(::GetDlgItem(wnd, IDC_SPIN_JOINT_PER_VER), UDM_GETPOS32, NULL, NULL);
						Interval const se_ticks = instance->max_interface_->GetAnimRange();
						meshml_extractor extractor(instance->max_interface_->GetRootNode(), joint_per_ver,
							instance->max_interface_->GetTime(),
							se_ticks.Start() / GetTicksPerFrame(), se_ticks.End() / GetTicksPerFrame());

						extractor.export_objects(instance->export_nodes_);
						extractor.write_xml(instance->file_name_);

						::EndDialog(wnd, 1);
					}
					break;

				case IDCANCEL:
					::EndDialog(wnd, 0);
					break;
				}
				break;

			case WM_CLOSE:
				::EndDialog(wnd, 0);
				break;

			default:
				return FALSE;
			}
		}

		return TRUE;
	}
}
