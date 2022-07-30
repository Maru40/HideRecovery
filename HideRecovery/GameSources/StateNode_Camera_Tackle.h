/*!
@file StateNode_Camera_Tackle.h
@brief StateNode_Camera_Tackle�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class GameTimer;

	namespace StateNode
	{
		//--------------------------------------------------------------------------------------
		///	�^�b�N�����ɌĂяo���J�����̃X�e�[�g
		//--------------------------------------------------------------------------------------
		class Camera_Tackle : public StateNodeBase<GameObject>
		{
			std::unique_ptr<GameTimer> m_timer = nullptr;  //���ԊǗ��N���X

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			Camera_Tackle(const std::shared_ptr<GameObject>& objPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		};

	}

}

//endbasecross