
/*!
@file MaruHitHelper.h
@brief MaruHitHelperクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	衝突データ
		//--------------------------------------------------------------------------------------
		struct HitData
		{
			ex_weak_ptr<GameObject> selfObject = nullptr; //自分自身
			ex_weak_ptr<GameObject> hitObject = nullptr;  //当たった相手
			Vec3 point;    //ヒット場所
			Vec3 normal;   //法線

			/// <summary>
			/// 自分自身の位置を取得
			/// </summary>
			/// <returns>自分自身の位置</returns>
			Vec3 GetSelfPosition();

			/// <summary>
			/// 自分自身からヒット場所に向けたベクトルを取得する
			/// </summary>
			/// <returns>自分自身からヒット場所に向けたベクトル</returns>
			Vec3 GetSelfToHitVec();

			/// <summary>
			/// ヒット場所から自分自身に向けたベクトルを返す
			/// </summary>
			/// <returns>ヒット場所から自分自身に向けたベクトル</returns>
			Vec3 GetHitToSelfVec();
		};

		//--------------------------------------------------------------------------------------
		///	衝突ヘルパー
		//--------------------------------------------------------------------------------------
		class HitHelper
		{

		public:

			/// <summary>
			/// selfPositionから見たOBBの最近接点とそのNormalを得る(selfObjectとhitObjectはnullptr)
			/// </summary>
			/// <param name="selfPosition">自分自身のポジション</param>
			/// <param name="other">相手のOBB</param>
			/// <returns>HitData</returns>
			static HitData ClosestPtPointOBB(const Vec3& selfPosition, const std::shared_ptr<CollisionObb>& other);

			//現在使用禁止
			static HitData ClosestPtPointOBB(const std::shared_ptr<GameObject>& self, const std::shared_ptr<CollisionObb>& other);
		};
	}

}