/*!
@file CircleMarkerObject.h
@brief CircleMarkerObject�Ȃ�
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
	/// �T�[�N���}�[�J�[�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class CircleMarkerObject : public BoardObject
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		CircleMarkerObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

	};

}

//endbasecross