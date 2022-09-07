/*!
@file Task_ToTargetMove.h
@brief Task_ToTargetMove
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"

namespace basecross {
	namespace Task {
		//--------------------------------------------------------------------------------------
		///	�ړ��^�C�v
		//--------------------------------------------------------------------------------------
		enum class ToTargetMove_MoveType {
			Lerp,            //���
			Transform,       //�ʏ�g�����X�t�H�[��
			SeekVelocity,    //�Ǐ]���x
			ArriveVelocity,  //�������x
		};

		//--------------------------------------------------------------------------------------
		///	���Ԍv���^�C�v
		//--------------------------------------------------------------------------------------
		enum class ToTargetMove_DeltaType {
			Normal, //�ʏ�
			Real,   //���A���^�C��
		};

		//--------------------------------------------------------------------------------------
		///	�w��ӏ��܂ňړ�����^�X�N�̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct ToTargetMove_Parametor
		{
			using MoveType = ToTargetMove_MoveType;	
			using DeltaType = ToTargetMove_DeltaType;

			Vec3 startPosition;       //�J�n�ʒu
			Vec3 endPosition;         //�I���ʒu
			float speed;              //�ړ����x
			float targetNearRange;    //�ړI�n�ɂ��ǂ蒅�����Ɣ��f���鋗��
			MoveType moveType;        //�ړ��^�C�v
			DeltaType deltaType;      //���Ԍv���^�C�v

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			ToTargetMove_Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="speed">�ړ����x</param>
			/// <param name="taretNearRange">�ړI�n�ɂ��ǂ蒅�����Ɣ��f���鋗��</param>
			/// <param name="moveType">�ړ��^�C�v</param>
			/// <param name="deltaType">���Ԍv���^�C�v</param>
			ToTargetMove_Parametor(const float& speed, const float& taretNearRange,
				const MoveType moveType = MoveType::Lerp, const DeltaType& deltaType = DeltaType::Normal);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="startPosition">�J�n�ʒu</param>
			/// <param name="endPosition">�I���ʒu</param>
			/// <param name="speed">�ړ����x</param>
			/// <param name="taretNearRange">�ړI�n�ɂ��ǂ蒅�����Ɣ��f���鋗��</param>
			/// <param name="moveType">�ړ��^�C�v</param>
			/// <param name="deltaType">���Ԍv���^�C�v</param>
			ToTargetMove_Parametor(const Vec3& startPosition, const Vec3& endPosition,
				const float& speed, const float& taretNearRange,
				const MoveType moveType = MoveType::Lerp, const DeltaType& deltaType = DeltaType::Normal);
		};

		//--------------------------------------------------------------------------------------
		///	�w��ӏ��܂ňړ�����^�X�N
		//--------------------------------------------------------------------------------------
		class ToTargetMove : public TaskNodeBase<GameObject>
		{
		public:
			using Parametor = ToTargetMove_Parametor;
			using MoveType = ToTargetMove_MoveType;
			using DeltaType = ToTargetMove_DeltaType;

		private:
			ex_weak_ptr<Parametor> m_paramPtr;	//�p�����[�^

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="owner">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="paramPtr">�p�����[�^</param>
			ToTargetMove(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// ���̃N���X�����L����Q�[���I�u�W�F�N�g�̈ʒu�ݒ�
			/// </summary>
			/// <param name="position">�ʒu</param>
			void SetOwnerPosition(const Vec3& position);

			/// <summary>
			/// ���̃N���X�����L����Q�[���I�u�W�F�N�g�̈ʒu�̎擾
			/// </summary>
			/// <returns>���̃N���X�����L����Q�[���I�u�W�F�N�g�̈ʒu</returns>
			Vec3 GetOwnerPosition();

			/// <summary>
			/// �ړ��X�V
			/// </summary>
			void MoveUpdate();

			/// <summary>
			/// �⊮�ړ�����
			/// </summary>
			void LerpMove();

			/// <summary>
			/// �ʏ�ړ�����
			/// </summary>
			void TransformMove();

			/// <summary>
			/// ���x�ړ�
			/// </summary>
			void VelocityMove();

			/// <summary>
			/// �^�C�v�̍��킹�������͂�Ԃ��B
			/// </summary>
			/// <returns>�^�C�v�ɍ��킹��������</returns>
			Vec3 CalculateVelocityForce();

			/// <summary>
			/// �o�ߎ��Ԃ̎擾
			/// </summary>
			/// <returns>�o�ߎ���</returns>
			float GetElapsedTime();

			/// <summary>
			/// �I�����f
			/// </summary>
			/// <returns>�ړI�n�܂ňړ�������������true</returns>
			bool IsEnd();

		public:
			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �p�����[�^�̐ݒ�
			/// </summary>
			/// <param name="parametor">�p�����[�^</param>
			void SetParametor(const std::shared_ptr<ToTargetMove::Parametor>& parametor) noexcept;

			/// <summary>
			/// �p�����[�^�̎擾
			/// </summary>
			/// <returns>�p�����[�^</returns>
			std::shared_ptr<ToTargetMove::Parametor> GetParametor() const;

		};

	}
}

//endbasecross