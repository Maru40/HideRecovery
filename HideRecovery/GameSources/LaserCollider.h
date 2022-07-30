
/*!
@file LaserCollider.h
@brief LaserColliderなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// レーザー用のコライダー
	//--------------------------------------------------------------------------------------
	class LaserCollider : public Component
	{
		ex_weak_ptr<GameObject> m_collisionObject;  //コライダーオブジェクト

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスが所有するゲームオブジェクト</param>
		LaserCollider(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

		/// <summary>
		/// コライダーを描画するか設定
		/// </summary>
		/// <param name="isDraw">描画するかどうか</param>
		void SetColliderDrawActive(const bool isDraw);

	private:
		/// <summary>
		/// 高さ更新
		/// </summary>
		void HeightUpdate();
	};

}