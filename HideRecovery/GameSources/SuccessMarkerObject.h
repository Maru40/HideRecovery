/*!
@file SuccessMarkerObject.h
@brief SuccessMarkerObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "BoardObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	namespace Builder {
		struct VertexPCTParametor;
	}

	//--------------------------------------------------------------------------------------
	/// �������}�[�J�[�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class SuccessMarkerObject : public BoardObject
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		SuccessMarkerObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

	};

}