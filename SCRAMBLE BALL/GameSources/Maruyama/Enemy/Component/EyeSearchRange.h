/*!
@file EyeSearchRangeParam.h
@brief EyeSearchRangeParam�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//------------------------------------------------------------------------------------------------
	/// ���E�^�[�Q�b�g�Ǘ��p�̃p�����[�^
	//------------------------------------------------------------------------------------------------
	struct EyeTargetParametor {
		std::shared_ptr<GameObject> target;  //�^�[�Q�b�g�̃|�C���^
		mutable bool isFind;                 //��������Ԃ��ǂ�����Ԃ�

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="target">�^�[�Q�b�g�̃|�C���^</param>
		EyeTargetParametor(const std::shared_ptr<GameObject>& target):
			target(target),
			isFind(false)
		{}
	};

	//------------------------------------------------------------------------------------------------
	/// ���E�͈̓p�����[�^
	//------------------------------------------------------------------------------------------------
	struct EyeSearchRangeParametor {
		float length; //���G�͈�(�~�`)
		float height; //���G�͈�(����)
		float rad;    //���G�͈�(�p�x)

		EyeSearchRangeParametor()
			:EyeSearchRangeParametor(20.0f)
		{}

		EyeSearchRangeParametor(const float& length) :
			EyeSearchRangeParametor(length, 3.0f, XMConvertToRadians(30.0f))
		{}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="length">���G�͈�(�~�`)</param>
		/// <param name="height">���G�͈�(����)</param>
		/// <param name="rad">���G�͈�(�p�x)</param>
		EyeSearchRangeParametor(const float& length,
			const float& height,
			const float& rad
		):
			length(length),
			height(height),
			rad(rad)
		{}
	};

	//------------------------------------------------------------------------------------------------
	/// ���E�͈͂��R���g���[������R���|�[�l���g
	//------------------------------------------------------------------------------------------------
	class EyeSearchRange : public Component
	{
	public:
		using Parametor = EyeSearchRangeParametor;

	private:
		std::vector<EyeTargetParametor> m_targetParams; //�͈͂ɓ����Ă��邩�̑ΏۂɂȂ�I�u�W�F�N�g

		EyeSearchRangeParametor m_param;                //���E�͈̓p�����[�^

	public:

		/// <summary>
		/// �^�[�Q�b�g�������𒆐S�ɋ��̏�͈͓̔��ɂ��邩�ǂ������f
		/// </summary>
		/// <param name="target">���G�^�[�Q�b�g</param>
		/// <returns>�͈͓��Ȃ�true</returns>
		bool IsRange(const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// �^�[�Q�b�g�������𒆐S�ɋ��̏�͈͓̔��ɂ��邩�ǂ������f
		/// </summary>
		/// <param name="targetPosition">���G�^�[�Q�b�g�̈ʒu</param>
		/// <returns>�͈͓��Ȃ�true</returns>
		bool IsRange(const Vec3& targetPosition);

		/// <summary>
		/// �^�[�Q�b�g�������̍��G�͈͓��́u�����v�ɂ��邩�ǂ����𔻒f
		/// </summary>
		/// <param name="target">���G�^�[�Q�b�g</param>
		/// <returns>�͈͓��Ȃ�true</returns>
		bool IsHeight(const std::shared_ptr<GameObject>& target);

		bool IsHeight(const Vec3& targetPosition);

		/// <summary>
		/// �^�[�Q�b�g�������̍��G�͈͓��̊p�x�ɂ��邩���f
		/// </summary>
		/// <param name="target">���G�^�[�Q�b�g</param>
		/// <returns>�͈͓��Ȃ�true</returns>
		bool IsRad(const std::shared_ptr<GameObject>& target);

		bool IsRad(const Vec3& targetPosition);

		/// <summary>
		/// Ray���΂����Ƃ��̃`�F�b�N
		/// </summary>
		/// <param name="targetParam">���G�^�[�Q�b�g</param>
		/// <returns>�͈͓��Ȃ�true</returns>
		bool IsRay(const std::shared_ptr<GameObject>& target);

		bool IsRay(const Vec3& targetPosition);

		/// <summary>
		/// �^�[�Q�b�g�����G�͈͓��ɂ���Ƃ��ɌĂяo�����֐�
		/// </summary>
		void Hit(const EyeTargetParametor& targetParam);

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����I�u�W�F�N�g</param>
		EyeSearchRange(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����I�u�W�F�N�g</param>
		/// <param name="param">���E�͈̓p�����[�^</param>
		EyeSearchRange(const std::shared_ptr<GameObject>& objPtr,
			const EyeSearchRangeParametor& param);

		void OnUpdate() override;

		/// <summary>
		/// ���E���ɂ���Ȃ�true��Ԃ�
		/// </summary>
		/// <param name="target">�^�[�Q�b�g</param>
		/// <returns>���E�̒��ɂ���Ȃ�true</returns>
		bool IsInEyeRange(std::shared_ptr<GameObject>& target);

		bool IsInEyeRange(const Vec3& position);

		/// <summary>
		/// ���E���ɂ���Ȃ�true��Ԃ�
		/// </summary>
		/// <param name="target">�^�[�Q�b�g</param>
		/// <returns>���E�̒��ɂ���Ȃ�true</returns>
		bool IsInEyeRange(std::shared_ptr<GameObject>& target, const float& length);

		/// <summary>
		/// ���E���ɂ���Ȃ�true��Ԃ�
		/// </summary>
		/// <param name="target">�^�[�Q�b�g</param>
		/// <returns>���E�̒��ɂ���Ȃ�true</returns>
		bool IsInEyeRange(std::shared_ptr<GameObject>& target, const Parametor& param);

		//------------------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//------------------------------------------------------------------------------------------------

		/// <summary>
		/// �^�[�Q�b�g�̒ǉ�
		/// </summary>
		/// <param name="obj">�^�[�Q�b�g�̃Q�[���I�u�W�F�N�g</param>
		void AddTarget(const std::shared_ptr<GameObject>& target) {
			m_targetParams.push_back(EyeTargetParametor(target));
		}

		/// <summary>
		/// ���E�͈̓p�����[�^�̐ݒ�
		/// </summary>
		/// <param name="param">���E�͈̓p�����[�^</param>
		void SetParametor(const EyeSearchRangeParametor& param) noexcept {
			m_param = param;
		}

		/// <summary>
		/// ���E�͈̓p�����[�^�̎擾
		/// </summary>
		/// <returns>���E�͈̓p�����[�^</returns>
		EyeSearchRangeParametor GetParametor() const noexcept {
			return m_param;
		}

		/// <summary>
		/// ���E�͈�(�~�`)�̐ݒ�
		/// </summary>
		/// <param name="length">���E�͈�(�~�`)</param>
		void SetEyeLength(const float& length) noexcept {
			m_param.length = length;
		}

		/// <summary>
		/// ���E�͈�(�~�`)�̎擾
		/// </summary>
		/// <returns>���E�͈�(�~�`)</returns>
		float GetEyeLength() const noexcept {
			return m_param.length;
		}

		/// <summary>
		/// ���E�͈�(����)�̎擾
		/// </summary>
		/// <param name="height">���E�͈�(����)</param>
		void SetEyeHeight(const float& height) noexcept {
			m_param.height = height;
		}

		/// <summary>
		/// ���E�͈�(����)�̎擾
		/// </summary>
		/// <returns>���E�͈�(����)</returns>
		float GetEyeHeight() const noexcept {
			return m_param.height;
		}

		/// <summary>
		/// ���E�͈�(�f�O���[�p)�̐ݒ�
		/// </summary>
		/// <param name="degree">���E�͈�(�f�O���[�p)</param>
		void SetEyeDegree(const float& degree) {
			m_param.rad = XMConvertToRadians(degree);
		}

		/// <summary>
		/// ���E�͈�(�f�O���[�p)�̎擾
		/// </summary>
		/// <returns>���E�͈�(�f�O���[�p)</returns>
		float GetEyeDegree() const {
			return XMConvertToDegrees(m_param.rad);
		}

		/// <summary>
		/// ���E�͈�(���W�A���p)�̐ݒ�
		/// </summary>
		/// <param name="radian">���E�͈�(���W�A���p)</param>
		void SetEyeRadian(const float& radian) noexcept {
			m_param.rad = radian;
		}

		/// <summary>
		/// ���E�͈�(���W�A���p)�̎擾
		/// </summary>
		/// <returns>���E�͈�(���W�A���p)</returns>
		float GetEyeRadian() const noexcept {
			return m_param.rad;
		}

		/// <summary>
		/// �����Ă����Ԃ��ǂ����𔻒f����
		/// </summary>
		/// <param name="target">�^�[�Q�b�g</param>
		/// <returns>���E�͈̔͂Ȃ�true</returns>
		bool IsLookTarget(const std::shared_ptr<GameObject>& target) {
			for (auto param : m_targetParams) {
				if (target == param.target) {  //������ނ�������
					return param.isFind;
				}
			}

			return false;
		}


		bool IsRad(const Vec3& selfForward, const Vec3& selfPosition, const Vec3& targetPosition, const float radian) {
			auto forward = selfForward;
			forward.y = 0.0f;
			auto toVec = targetPosition - selfPosition;
			toVec.y = 0.0f;

			auto newDot = dot(forward.GetNormalized(), toVec.GetNormalized());
			auto newRad = acosf(newDot);
			//���G�͈͂ɓ����Ă�����B
			return newRad <= radian ? true : false;
		}

	};

}

//endbasecross