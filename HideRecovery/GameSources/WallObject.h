
/*!
@file WallObject.h
@brief WallObject�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "StageObjectBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�ǃI�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class WallObject : public StageObjectBase
	{
		/// <summary>
		/// �`��ݒ�
		/// </summary>
		void SettingDraw();

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		WallObject(const std::shared_ptr<Stage>& stage)
			:WallObject(stage, L"WallObject", L"")
		{}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		/// <param name="name">�I�u�W�F�N�g��</param>
		/// <param name="texture">�e�N�X�`����</param>
		WallObject(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& texture)
			:StageObjectBase(stage, name, texture)
		{}

		void OnCreate() override;
	};

}

//endbasecross