/*!
@file SpriteObject.h
@brief SpriteObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Utility/SingletonComponent/HitStopManager.h"

#include "Maruyama/Utility/ReactiveBool.h"
#include "Maruyama/UI/BuilderVertexPCT.h"
#include "Maruyama/UI/2D/UILayer.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	namespace Builder {
		struct VertexPCTParametor;
	}

	//--------------------------------------------------------------------------------------
	/// �X�v���C�g�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class SpriteObject : public GameObject
	{
	public:
		using DrawComp = PCTSpriteDraw;
		
	private:
		std::shared_ptr<Builder::VertexPCTParametor> m_paramPtr;  //�X�v���C�g�����p�p�����[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		/// <param name="vertexParam">�X�v���C�g�����p�̃p�����[�^</param>
		SpriteObject(const std::shared_ptr<Stage>& stage, const Builder::VertexPCTParametor& vertexParam);

		void OnCreate() override;

	};

	//--------------------------------------------------------------------------------------
	/// �X�v���C�g�f�[�^
	//--------------------------------------------------------------------------------------
	struct SpriteData
	{
		Vec3 offset;                            //�I�t�Z�b�g�ʒu
		Builder::VertexPCTParametor buildParam; //Sprite�����p�̃p�����[�^

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="offset">�I�t�Z�b�g</param>
		/// <param name="buildParam">Sprite�����p�̃p�����[�^</param>
		SpriteData(
			const Vec2& offset,
			const Builder::VertexPCTParametor& buildParam
		);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="offset">�I�t�Z�b�g</param>
		/// <param name="buildParam">Sprite�����p�̃p�����[�^</param>
		SpriteData(
			const Vec3& offset,
			const Builder::VertexPCTParametor& buildParam
		);
	};

	//--------------------------------------------------------------------------------------------

}