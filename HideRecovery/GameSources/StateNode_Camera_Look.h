/*!
@file StateNode_Camera_Look.h
@brief StateNode_Camera_Lookなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace StateNode
	{

		//--------------------------------------------------------------------------------------
		///	ターゲットをロック時に呼び出すカメラのステート
		//--------------------------------------------------------------------------------------
		class Camera_Look : public StateNodeBase<GameObject>
		{
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			Camera_Look(const std::shared_ptr<GameObject>& objPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// 入力管理
			/// </summary>
			void InputManager();

		};

	}

}

//endbasecross