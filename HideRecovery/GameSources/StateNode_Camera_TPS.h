/*!
@file StateNode_Camera_TPS.h
@brief StateNode_Camera_TPSなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace StateNode
	{

		//--------------------------------------------------------------------------------------
		///	TPS時に呼び出すカメラのステート
		//--------------------------------------------------------------------------------------
		class Camera_TPS : public StateNodeBase<GameObject>
		{
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			Camera_TPS(const std::shared_ptr<GameObject>& objPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		};

	}

}

//endbasecross