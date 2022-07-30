
/*!
@file SingletonComponent.h
@brief SingletonComponent�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "DebugObject.h"
#include "MaruUtility.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�O���錾
		//--------------------------------------------------------------------------------------
		template<class T>
		class SingletonComponent;

		//--------------------------------------------------------------------------------------
		///	�V���O���g���R���|�[�l���g
		//--------------------------------------------------------------------------------------
		template<class T>
		class SingletonComponent : public Component
		{ 
			static ex_weak_ptr<T> sm_instance;	//�������g�̃|�C���^

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			SingletonComponent<T>(const std::shared_ptr<GameObject>& objPtr)
				:Component(objPtr)
			{}

			void OnDestroy() override {
				sm_instance = nullptr;
			}

			virtual void OnLateStart() override {
				if (this != GetInstance().get())
				{
					DebugObject::AddString(Util::GetWSTypeName<T>(), false);
					DebugObject::AddString(L"�A�͐�������Ă��邽�߃R���|�[�l���g���폜���܂����B");

					RemoveComponent<T>();
				}
			}

			/// <summary>
			/// �������g�̃X�}�[�g�|�C���^�̎擾
			/// </summary>
			/// <returns>�������g�̃X�}�[�g�|�C���^</returns>
			static std::shared_ptr<T> GetInstance() {
				return GetInstance(maru::Utility::GetStage());
			}

			/// <summary>
			/// �������g�̃X�}�[�g�|�C���^�̎擾
			/// </summary>
			/// <param name="stage">��������X�e�[�W</param>
			/// <returns>�������g�̃X�}�[�g�|�C���^</returns>
			static std::shared_ptr<T> GetInstance(const std::shared_ptr<Stage>& stage) {
				if (sm_instance.GetShard() == nullptr)
				{
					sm_instance = maru::Utility::FindComponent<T>(stage);
					if (sm_instance.GetShard() == nullptr)
					{
						DebugObject::AddString(Util::GetWSTypeName<T>(), false);
						DebugObject::AddString(L"�A��GameScene�ɒǉ�����Ă��܂���");
					}
				}

				return sm_instance.GetShard();
			}
		};

	}
}

//endbasecross