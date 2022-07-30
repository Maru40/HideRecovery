/*!
@file SegmentMarkerObject.h
@brief SegmentMarkerObject�Ȃ�
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
	/// �Z�O�����g�}�[�J�[�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class SegmentMarkerObject : public BoardObject
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		SegmentMarkerObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

	};

}