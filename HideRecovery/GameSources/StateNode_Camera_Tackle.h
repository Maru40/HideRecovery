/*!
@file StateNode_Camera_Tackle.h
@brief StateNode_Camera_Tackleなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class GameTimer;

	namespace StateNode
	{
		//--------------------------------------------------------------------------------------
		///	タックル時に呼び出すカメラのステート
		//--------------------------------------------------------------------------------------
		class Camera_Tackle : public StateNodeBase<GameObject>
		{
			std::unique_ptr<GameTimer> m_timer = nullptr;  //時間管理クラス

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			Camera_Tackle(const std::shared_ptr<GameObject>& objPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		};

	}

}

//endbasecross