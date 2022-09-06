
/*!
@file ChaseState_TargetSeek.h
@brief ChaseState_TargetSeek�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Utility/Component/SeekTarget.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	template<class T>
	class EnemyStateNodeBase;

	namespace Enemy {

		class EnemyBase;

		namespace StateNode
		{
			namespace ChaseState {

				//--------------------------------------------------------------------------------------
				/// �^�[�Q�b�g�Ǐ]�p�����[�^
				//--------------------------------------------------------------------------------------
				struct TargetSeekParametor
				{
					std::shared_ptr<SeekTarget::Parametor> seekParamPtr = nullptr;  //�Ǐ]�p�����[�^

					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					TargetSeekParametor();

					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="seekParamPtr">�Ǐ]�p�����[�^</param>
					TargetSeekParametor(const std::shared_ptr<SeekTarget::Parametor>& seekParamPtr);
				};

				//--------------------------------------------------------------------------------------
				/// �^�[�Q�b�g�Ǐ]�X�e�[�g
				//--------------------------------------------------------------------------------------
				class ChaseState_TargetSeek : public EnemyStateNodeBase<EnemyBase>
				{
				private:
					ex_weak_ptr<TargetSeekParametor> m_paramPtr = nullptr;  //�p�����[�^

				public:
					/// <summary>
					/// �R���X�g���N�^
					/// </summary>
					/// <param name="owner">���̃N���X�����L����N���X</param>
					/// <param name="parametor">�Ǐ]�p�����[�^</param>
					ChaseState_TargetSeek(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<TargetSeekParametor>& parametor);

					void OnStart() override;
					bool OnUpdate() override;
					void OnExit() override;

				private:
					/// <summary>
					/// �����̍X�V
					/// </summary>
					void RotationUpdate();

				};
			}
		}
	}
}

//endbasecross