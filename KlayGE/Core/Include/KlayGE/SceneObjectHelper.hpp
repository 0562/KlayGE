// SceneObjectHelper.hpp
// KlayGE һЩ���õĳ������� ͷ�ļ�
// Ver 3.12.0
// ��Ȩ����(C) ������, 2005-2011
// Homepage: http://www.klayge.org
//
// 3.10.0
// SceneObjectSkyBox��SceneObjectHDRSkyBox������Technique() (2010.1.4)
//
// 3.9.0
// ������SceneObjectHDRSkyBox (2009.5.4)
//
// 3.1.0
// ���ν��� (2005.10.31)
//
// �޸ļ�¼
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SCENEOBJECTHELPER_HPP
#define _SCENEOBJECTHELPER_HPP

#pragma once

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/SceneObject.hpp>
#include <KlayGE/Box.hpp>

namespace KlayGE
{
	class KLAYGE_CORE_API SceneObjectHelper : public SceneObject
	{
	public:
		explicit SceneObjectHelper(uint32_t attrib);
		SceneObjectHelper(RenderablePtr const & renderable, uint32_t attrib);
		virtual ~SceneObjectHelper()
		{
		}
	};

	class KLAYGE_CORE_API SceneObjectSkyBox : public SceneObjectHelper
	{
	public:
		SceneObjectSkyBox(uint32_t attrib = 0);
		virtual ~SceneObjectSkyBox()
		{
		}

		void Technique(RenderTechniquePtr const & tech);
		void CubeMap(TexturePtr const & cube);
	};

	class KLAYGE_CORE_API SceneObjectHDRSkyBox : public SceneObjectSkyBox
	{
	public:
		SceneObjectHDRSkyBox(uint32_t attrib = 0);
		virtual ~SceneObjectHDRSkyBox()
		{
		}

		void Technique(RenderTechniquePtr const & tech);
		void CompressedCubeMap(TexturePtr const & y_cube, TexturePtr const & c_cube);
	};

	class KLAYGE_CORE_API SceneObjectLightSourceProxy : public SceneObjectHelper
	{
	public:
		explicit SceneObjectLightSourceProxy(LightSourcePtr const & light, float scale);

		void Update();

		float4x4 const & GetModelMatrix() const;

	protected:
		float4x4 model_;
		float4x4 model_org_;

		LightSourcePtr light_;
	};
}

#endif		// _RENDERABLEHELPER_HPP
