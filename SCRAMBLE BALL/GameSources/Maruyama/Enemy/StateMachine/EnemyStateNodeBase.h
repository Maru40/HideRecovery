
/*!
@file EnemyStateNodeBase.h
@brief EnemyStateNodeBase
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "StateNodeBase.h"

namespace basecross {

	/// <summary>
	/// �G�l�~�[�p�X�e�[�g�m�[�h�̊��N���X
	/// </summary>
	/// <typeparam name="EnemyType">�G�l�~�[�^�C�v</typeparam>
	template<class EnemyType>
	class EnemyStateNodeBase : public StateNodeBase<EnemyType>
	{
		/// <summary>
		/// �R���|�[�l���g�̕ύX�֌W�̏����܂Ƃ߂��\����
		/// </summary>
		struct ChangeComponentParametor 
		{
			std::shared_ptr<Component> component; //�R���|�[�l���g�̃|�C���^
			bool isStart;                         //�J�n���ɂǂ����ɂ��邩�H
			bool isExit;                          //�I�����ɂǂ����ɂ��邩�H

			ChangeComponentParametor(
				const std::shared_ptr<Component>& component,
				const bool isStart,
				const bool isExit
			) :
				component(component),
				isStart(isStart),
				isExit(isExit)
			{}
		};

		//�R���|�[�l���g�ύX�p�̃p�����[�^�z��
		std::vector<ChangeComponentParametor> m_changeParams;

	protected:

		/// <summary>
		/// �J�n�ƏI�����ɐ؂�ւ���R���|�[�l���g�̒ǉ�
		/// </summary>
		/// <param name="component">�؂�ւ���R���|�[�l���g�̃|�C���^</param>
		/// <param name="isStart">�X�^�[�g���ɂǂ����ɐ؂�ւ���</param>
		/// <param name="isExit">�I�����ɂǂ����ɐ؂�ւ��邩</param>
		void AddChangeComponent(
			const std::shared_ptr<Component>& component,
			const bool isStart, 
			const bool isExit)
		{
			if (component == nullptr) {  //nullptr�Ȃ�ǉ����Ȃ�
				return;
			}

			ChangeComponentParametor param(component, isStart, isExit);
			m_changeParams.push_back(param);
		}

		/// <summary>
		/// �J�n�ƏI�����ɐ؂�ւ���R���|�[�l���g�̒ǉ�
		/// </summary>
		/// <param name="param">�؂�ւ�������܂Ƃ߂��p�����[�^</param>
		void AddChangeComponent(const ChangeComponentParametor& param) {
			if (param.component == nullptr) {
				return;
			}
			m_changeParams.push_back(param);
		}

		/// <summary>
		/// �o�^���ꂽ�R���|�[�l���g�̐؂�ւ����s��(�X�^�[�g��)
		/// </summary>
		void StartChangeComponents() {
			for (auto& param : m_changeParams) {
				param.component->SetUpdateActive(param.isStart);
			}
		}

		/// <summary>
		/// �o�^���ꂽ�R���|�[�l���g�̐؂�ւ����s��(�I����)
		/// </summary>
		void ExitChangeComponents() {
			for (auto& param : m_changeParams) {
				param.component->SetUpdateActive(param.isExit);
			}
		}

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="owner">���̃N���X�����L����N���X</param>
		EnemyStateNodeBase(const std::shared_ptr<EnemyType>& owner)
			:StateNodeBase<EnemyType>(owner)
		{}

		virtual ~EnemyStateNodeBase() {}

	};

}

//endbasecross