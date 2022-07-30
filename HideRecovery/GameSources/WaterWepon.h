/*!
@file WaterWepon.h
@brief WaterWeponなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 水圧ジェット装備
	//--------------------------------------------------------------------------------------
	class WaterWepon : public WeponBase
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		WaterWepon(const std::shared_ptr<GameObject>& objPtr);

	protected:
		virtual void HoldShotUpdate() override;
		virtual void HoldShotEnd() override;

		/// <summary>
		/// 飛ばす方向の取得
		/// </summary>
		/// <returns>飛ばす方向</returns>
		Vec3 GetDirect() const;
	};

}