/*!
@file RangeDestroyManager.h
@brief RangeDestroyManager
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "MaruAction.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	一定距離を進んだら削除するコンポーネント
	//--------------------------------------------------------------------------------------
	class RangeDestoryManager : public Component
	{
	private:
		float m_maxRange;          //最大距離
		Vec3 m_initializePosition; //初期ポジション

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="maxRange">最大距離</param>
		RangeDestoryManager(const std::shared_ptr<GameObject>& objPtr, const float& maxRange);

		void OnCreate() override;
		void OnUpdate() override;
	};

}

//basecross