
/*!
@file Motion_Idle.h
@brief Motion_Idle�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	namespace Metaball {
		class MotionManager;

		namespace Task {
			struct SeekOffsetMove_Parametor;
		}
	}

	enum class SeekTarget_Type;

	namespace Enemy {

		class EnemyBase;

		namespace Motion {

			//--------------------------------------------------------------------------------------
			/// �Ǐ]�f�[�^
			//--------------------------------------------------------------------------------------
			struct SeekData {
				float speed;
				SeekTarget_Type seekType;

				SeekData();
				SeekData(const float& speed, const SeekTarget_Type& seekType);
			};

			//--------------------------------------------------------------------------------------
			/// �ҋ@�p�����[�^
			//--------------------------------------------------------------------------------------
			struct Idle_Parametor {
				float time;            //����
				float firstSpeed;      //�����X�s�[�h
				SeekData metaballData; //���^�{�[���̒Ǐ]�f�[�^
				SeekData coreData;     //�R�A�̃f�[�^

				Idle_Parametor();
				Idle_Parametor(const float& firstSpeed);
				Idle_Parametor(const float& time, const float& firstSpeed);
				Idle_Parametor(const float& time, const float& firstSpeed, const SeekData& metaballData, const SeekData& coreData);
			};

			//--------------------------------------------------------------------------------------
			/// �ҋ@���[�V����
			//--------------------------------------------------------------------------------------
			class Idle : public maru::MetaballMotionBase<EnemyBase>
			{
			public:
				using Parametor = Idle_Parametor;

			private:
				Parametor m_param;       //�p�����[�^

				float m_radius;          //���a
				vector<Vec3> m_offsets;  //�I�t�Z�b�g�z��

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����N���X</param>
				Idle(const std::shared_ptr<EnemyBase>& owner);

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����N���X</param>
				/// <param name="parametor">�p�����[�^</param>
				Idle(const std::shared_ptr<EnemyBase>& owner, const Parametor& parametor);

			private:
				vector<Metaball::Task::SeekOffsetMove_Parametor> CalculateMotionParametors() override;

			};
		}
	}
}