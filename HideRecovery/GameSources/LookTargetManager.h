
/*!
@file LookTargetManager.h
@brief LookTargetManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	��������G���Ǘ�����N���X
	//--------------------------------------------------------------------------------------
	class LookTargetManager : public Component
	{
	public:
		struct Parametor
		{
			float inputChangeRange;  //�X�e�B�b�N���͂ɂ��J�����؂�ւ��͈̔�
			float inputChangeDegree; //�X�e�B�b�N���͂ɂ��J�����؂�ւ��̊p�x

			Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="inputChangeRange">���͐؂�ւ��͈�</param>
			/// <param name="inputChangeDegree">���͐؂�ւ��͈͂̃f�O���[�p</param>
			Parametor(const float& inputChangeRange, const float& inputChangeDegree);
		};

	private:
		Parametor m_param;  //�p�����[�^

		ex_weak_ptr<GameObject> m_target; //�^�[�Q�b�g�̃Q�[���I�u�W�F�N�g�̃|�C���^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		LookTargetManager(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="parametor">�p�����[�^</param>
		LookTargetManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnUpdate() override;

	private:
		/// <summary>
		/// �C���v�b�g�ɍ��킹��Update�C�x���g�B
		/// </summary>
		void ChangeTargetManager();

		/// <summary>
		/// �^�[�Q�b�g�̋߂��̃^�[�Q�b�g���擾����B
		/// </summary>
		/// <param name="range">�T���͈�</param>
		/// <param name="target">�T������^�[�Q�b�g</param>
		/// <returns>��ԋ߂��ɂ���^�[�Q�b�g</returns>
		vector<std::shared_ptr<GameObject>> SearchTargetNearTargets(
			const float& range, const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// �^�[�Q�b�g�Q������͕����̃^�[�Q�b�g���擾
		/// </summary>
		/// <param name="currentTarget">���݂̃^�[�Q�b�g</param>
		/// <param name="targets">�T������^�[�Q�b�g�Q</param>
		/// <returns>�T�������^�[�Q�b�g��Ԃ�</returns>
		std::shared_ptr<GameObject> FindDirectTarget(
			const std::shared_ptr<GameObject>& currentTarget, const std::vector<std::shared_ptr<GameObject>>& targets);
	
	public:
		/// <summary>
		/// �߂��̃^�[�Q�b�g���擾����
		/// </summary>
		/// <returns>��ԋ߂��̃^�[�Q�b�g���擾����</returns>
		std::shared_ptr<GameObject> SearchTarget();

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �p�����[�^�̃Z�b�g
		/// </summary>
		/// <param name="parametor">�p�����[�^</param>
		void SetParametor(const Parametor& parametor) noexcept { m_param = parametor; }

		/// <summary>
		/// �p�����[�^�̎擾
		/// </summary>
		/// <returns>�p�����[�^</returns>
		Parametor GetParametor() const noexcept { return m_param; }

		/// <summary>
		/// �^�[�Q�b�g�̃Z�b�g
		/// </summary>
		/// <param name="target">�^�[�Q�b�g�̃Q�[���I�u�W�F�N�g</param>
		void SetTarget(const std::shared_ptr<GameObject>& target) { m_target = target; }

		/// <summary>
		/// �^�[�Q�b�g�̎擾
		/// </summary>
		/// <returns>�^�[�Q�b�g�̃Q�[���I�u�W�F�N�g</returns>
		std::shared_ptr<GameObject> GetTarget() const noexcept { return m_target.GetShard(); }

		/// <summary>
		/// �^�[�Q�b�g�����݂��邩�ǂ���
		/// </summary>
		/// <returns>�^�[�Q�b�g�����݂���Ȃ�true</returns>
		bool HasTarget() const noexcept { return m_target.GetShard() ? true : false; }

	};

}

//endbasecross