/*!
@file I_Task.h
@brief I_Task�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "I_Node.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�^�X�N�̃C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_Task : public NodeBase {
			public:
				virtual ~I_Task() = default;

				//virtual void OnStart() = 0;
				//virtual bool OnUpdate() = 0;
				//virtual void OnExit() = 0;
			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�^�X�N�̊��N���X
			//--------------------------------------------------------------------------------------
			template<class OwnerType>
			class TaskBase : public I_Task
			{
				std::weak_ptr<OwnerType> m_owner;	//�I�[�i�[

			public:
				TaskBase(const std::shared_ptr<OwnerType>& owner) :
					I_Task(),
					m_owner(owner)
				{}

				virtual ~TaskBase() = default;

				/// <summary>
				/// �I�[�i�[�̎擾
				/// </summary>
				/// <returns>�I�[�i�[</returns>
				std::shared_ptr<OwnerType> GetOwner() const { return m_owner.lock(); }
			};

		}
	}
}