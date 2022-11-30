/*!
@file BehaviorTesterObject.h
@brief BehaviorTesterObjectなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			///	前方宣言
			//--------------------------------------------------------------------------------------
			template<class EnumType>
			class BehaviorTree;

			//--------------------------------------------------------------------------------------
			///	テスト用のビヘイビアオブジェクト
			//--------------------------------------------------------------------------------------
			class BehaviorTesterObject : public GameObject
			{
				

			public:
				BehaviorTesterObject(const std::shared_ptr<Stage>& stage);

				void OnCreate() override;
			};

		}
	}
}