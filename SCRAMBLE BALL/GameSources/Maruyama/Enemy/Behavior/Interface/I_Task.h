/*!
@file I_Task.h
@brief I_Taskなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "I_Node.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			/// ビヘイビアタスクのインターフェース
			//--------------------------------------------------------------------------------------
			class I_Task : public NodeBase {
			public:
				virtual ~I_Task() = default;

				virtual void OnStart() = 0;
				virtual bool OnUpdate() = 0;
				virtual void OnExit() = 0;
			};

			//--------------------------------------------------------------------------------------
			/// ビヘイビアタスクの基底クラス
			//--------------------------------------------------------------------------------------
			template<class OwnerType>
			class TaskBase : public I_Task
			{
				std::weak_ptr<OwnerType> m_owner;	//オーナー

			public:
				TaskBase(const std::shared_ptr<OwnerType>& owner) :
					m_owner(owner)
				{}

				virtual ~TaskBase() = default;

				/// <summary>
				/// オーナーの取得
				/// </summary>
				/// <returns>オーナー</returns>
				std::shared_ptr<OwnerType> GetOwner() const { return m_owner.lock(); }
			};

		}
	}
}