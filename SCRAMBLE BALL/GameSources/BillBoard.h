
/*!
@file BillBoard.h
@brief BillBoardクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ビルボード
	//--------------------------------------------------------------------------------------
	class BillBoard : public Component 
	{
		Vec3 m_rotationOffset;  //向きオフセット
		Vec3 forward;           //正面方向

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		BillBoard(const std::shared_ptr<GameObject>& objPtr):
			BillBoard(objPtr, Vec3(0.0f))
		{}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="rotationOffset">向きオフセット</param>
		BillBoard(const std::shared_ptr<GameObject>& objPtr, const Vec3& rotationOffset):
			Component(objPtr), m_rotationOffset(rotationOffset), forward(Vec3(0.0f))
		{}

		void OnUpdate() override;

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 向きオフセットの設定
		/// </summary>
		/// <param name="offset">向きオフセット</param>
		void SetRotationOffset(const Vec3& offset) noexcept {
			m_rotationOffset = offset;
		}

	};

}

//endbasecross