
/*!
@file AIPlayerStator.h
@brief AIPlayerStator�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "StatorBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	

	namespace Enemy {

		class EnemyBase;

		//--------------------------------------------------------------------------------------
		///	AIPlayerStator�̃X�e�[�g�^�C�v
		//--------------------------------------------------------------------------------------
		enum class AIPlayerStator_StateType {
			None,
			HidePlacePatrol,	//�B���ꏊ�T���B
			Attack,				//�U��
		};

		//--------------------------------------------------------------------------------------
		///	AIPlayerStator�̑J�ڏ��������o�[
		//--------------------------------------------------------------------------------------
		struct AIPlayerStator_TransitionMember {

		};

		//--------------------------------------------------------------------------------------
		///	AIPlayerStator
		//--------------------------------------------------------------------------------------
		class AIPlayerStator : StatorBase<EnemyBase, AIPlayerStator_StateType, AIPlayerStator_TransitionMember>
		{
		public:
			using StateType = AIPlayerStator_StateType;
			using TransitionMember = AIPlayerStator_TransitionMember;

		private:


		public:
			AIPlayerStator(const std::shared_ptr<GameObject>& objPtr);

			void CreateNode() override;
			void CreateEdge() override;

		};

	}
}