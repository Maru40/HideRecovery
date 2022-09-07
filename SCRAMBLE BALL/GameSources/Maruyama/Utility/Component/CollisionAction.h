/*!
@file CollisionAction.h
@brief CollisionAction
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "../MaruAction.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�Փ˃C�x���g
		//--------------------------------------------------------------------------------------
		class CollisionAction : public Component
		{
			/// <summary>
			/// �Փ˃C�x���g�́u�J�n�v�A�u�X�V�v�A�u�I���v���܂Ƃ߂�����
			/// </summary>
			template<class T>
			struct ActionTrio {
				maru::Action<T> enter;   //�ՓˊJ�n�C�x���g
				maru::Action<T> excute;  //�ՓˍX�V�C�x���g
				maru::Action<T> exit;    //�ՓˏI���C�x���g
			};

			ActionTrio<void(const CollisionPair&)> m_collisionAction;  //�R���W�����A�N�V����
			ActionTrio<void(const CollisionPair&)> m_triggerAction;    //�g���K�[�A�N�V����

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			CollisionAction(const std::shared_ptr<GameObject>& objPtr);

			//--------------------------------------------------------------------------------------
			///	�Փ˔���
			//--------------------------------------------------------------------------------------

			void OnCollisionEnter(const CollisionPair& pair) override;
			void OnCollisionExcute(const CollisionPair& pair) override;
			void OnCollisionExit(const CollisionPair& pair) override;

			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------


			//--------------------------------------------------------------------------------------
			///	Collision
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �ՓˊJ�n�C�x���g�̒ǉ�
			/// </summary>
			/// <param name="function">�C�x���g</param>
			void AddCollisionEnter(const std::function<void(const CollisionPair&)>& function) {
				m_collisionAction.enter.AddFunction(function);
			}

			/// <summary>
			/// �ՓˍX�V�C�x���g�̒ǉ�
			/// </summary>
			/// <param name="function">�C�x���g</param>
			void AddCollisionExcute(const std::function<void(const CollisionPair&)>& function) {
				m_collisionAction.excute.AddFunction(function);
			}

			/// <summary>
			/// �ՓˏI���C�x���g�̒ǉ�
			/// </summary>
			/// <param name="function">�C�x���g</param>
			void AddCollisionExit(const std::function<void(const CollisionPair&)>& function) {
				m_collisionAction.exit.AddFunction(function);
			}


			//--------------------------------------------------------------------------------------
			///	Trigger
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// Trigger�J�n�C�x���g�̒ǉ�
			/// </summary>
			/// <param name="function">�C�x���g</param>
			void AddTriggerEnter(const std::function<void(const CollisionPair&)>& function) {
				m_triggerAction.enter.AddFunction(function);
			}

			/// <summary>
			/// Trigger�X�V�C�x���g�̒ǉ�
			/// </summary>
			/// <param name="function">�C�x���g</param>
			void AddTriggerExcute(const std::function<void(const CollisionPair&)>& function) {
				m_triggerAction.excute.AddFunction(function);
			}

			/// <summary>
			/// Trigger�I���C�x���g�̒ǉ�
			/// </summary>
			/// <param name="function">�C�x���g</param>
			void AddTriggerExit(const std::function<void(const CollisionPair&)>& function) {
				m_triggerAction.exit.AddFunction(function);
			}
		};
	}

}