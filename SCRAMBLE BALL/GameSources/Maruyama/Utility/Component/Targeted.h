/*!
@file Targeted.h
@brief Targeted�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	namespace TargetedPriority {
		constexpr float OPEN_BOX = 10.0f;	//�{�[���������Ă���\�������锠
		constexpr float PLAYER = 1.0f;		//player
		constexpr float GOAL = 0.75f;		//�S�[��
		constexpr float BALL = 0.5f;		//�{�[��
	}

	//--------------------------------------------------------------------------------------
	/// �^�[�Q�b�g�ɂ����Ώۂ̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct Targeted_Parametor
	{
		Vec3 offset;
		float priority;	//�D��x

		Targeted_Parametor();

		Targeted_Parametor(const float priority);
	};

	//--------------------------------------------------------------------------------------
	/// �^�[�Q�b�g�ɂ����Ώ�
	//--------------------------------------------------------------------------------------
	class Targeted : public Component
	{
	public:
		using Parametor = Targeted_Parametor;

	private:
		Parametor m_param; //�p�����[�^

		//�^�[�Q�b�g�w��ł����Ԃǂ����𔻒f����bool�֐��z��
		std::vector<std::function<bool()>> m_canTargetFunctions;	

	public:

		Targeted(const std::shared_ptr<GameObject>& objPtr);

		Targeted(
			const std::shared_ptr<GameObject>& objPtr,
			const Parametor& param
		);
		
	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �I�t�Z�b�g���l�������^�[�Q�b�g�ʒu���擾
		/// </summary>
		/// <returns>�I�t�Z�b�g���l�������^�[�Q�b�g�̈ʒu</returns>
		Vec3 GetPosition() const;

		/// <summary>
		/// �^�[�Q�b�g�ʒu�̃I�t�Z�b�g��ݒ�
		/// </summary>
		/// <param name="offset">�^�[�Q�b�g�ʒu�̃I�t�Z�b�g</param>
		void SetOffset(const Vec3& offset);

		/// <summary>
		/// �^�[�Q�b�g�ʒu�̃I�t�Z�b�g���擾
		/// </summary>
		/// <returns>�^�[�Q�b�g�ʒu�̃I�t�Z�b�g</returns>
		Vec3 GetOffset() const;

		/// <summary>
		/// �D��x�̐ݒ�
		/// </summary>
		/// <param name="priority">�D��x</param>
		void SetPriority(const float priority) { m_param.priority = priority; }

		/// <summary>
		/// �D��x�̎擾
		/// </summary>
		/// <returns></returns>
		_NODISCARD float GetPriority() const noexcept { return m_param.priority; }

		/// <summary>
		/// �^�[�Q�b�g�ɂ��邱�Ƃ��ł��邩���f
		/// </summary>
		/// <returns>�^�[�Q�b�g�ɂł���Ȃ�true</returns>
		bool CanTarget() const;

		/// <summary>
		/// �^�[�Q�b�g�w��ł��邩�ǂ����̏�������ǉ�
		/// </summary>
		/// <param name="canTargetFunction">�ǉ�������������</param>
		void AddCanTargetFunction(const std::function<bool()>& canTargetFunction);

	};

}