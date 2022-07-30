/*!
@file StateNode_Camera_TPS.h
@brief StateNode_Camera_TPS�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace StateNode
	{

		//--------------------------------------------------------------------------------------
		///	TPS���ɌĂяo���J�����̃X�e�[�g
		//--------------------------------------------------------------------------------------
		class Camera_TPS : public StateNodeBase<GameObject>
		{
		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			Camera_TPS(const std::shared_ptr<GameObject>& objPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		};

	}

}

//endbasecross