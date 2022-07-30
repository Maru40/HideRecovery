/*!
@file StateNode_Camera_Look.h
@brief StateNode_Camera_Look�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace StateNode
	{

		//--------------------------------------------------------------------------------------
		///	�^�[�Q�b�g�����b�N���ɌĂяo���J�����̃X�e�[�g
		//--------------------------------------------------------------------------------------
		class Camera_Look : public StateNodeBase<GameObject>
		{
		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			Camera_Look(const std::shared_ptr<GameObject>& objPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// ���͊Ǘ�
			/// </summary>
			void InputManager();

		};

	}

}

//endbasecross