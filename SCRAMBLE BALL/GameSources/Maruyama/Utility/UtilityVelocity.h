
/*!
@file UtilityVelocity.h
@brief UtilityVelocityクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	速度計算に必要な便利関数まとめ
	//--------------------------------------------------------------------------------------
	class UtilityVelocity 
	{
	public:

		/// <summary>
		/// 最大速度制限
		/// </summary>
		/// <param name="velocity">制限したいベクトル</param>
		/// <param name="maxSpeed">制限速度</param>
		/// <returns>制限された範囲のベクトルを返す。</returns>
		static Vec3 MaxSpeedVecCheck(const Vec3 velocity ,const float& maxSpeed);

		/// <summary>
		/// 直線的に追いかけるためのベクトルを計算して返す関数
		/// </summary>
		/// <param name="velocity">現在の速度</param>
		/// <param name="toVec">ターゲット方向のベクトル</param>
		/// <param name="maxSpeed">最大速度</param>
		/// <param name="forceMultipier">力の追従度(大きければ旋回しやすい)</param>
		/// <returns>「ターゲットの方向のベクトル」- 「現在の速度」</returns>
		static Vec3 CalucSeekVec(const Vec3& velocity, const Vec3& toVec,const float& maxSpeed, const float forceMultipier = 0.025f);

		/// <summary>
		/// 到着ベクトルを返す(近づくと小さくなるベクトル)
		/// </summary>
		/// <param name="velocity">現在の速度</param>
		/// <param name="toVec">ターゲット方向のベクトル</param>
		/// <param name="maxSpeed">最大速度</param>
		/// <param name="forceMultipier">力の追従度(大きければ旋回しやすい)</param>
		/// <param name="decl"></param>
		/// <returns>到着ベクトルを返す(近づくと小さくなるベクトル)を返す</returns>
		static Vec3 CalucArriveVec(const Vec3& velocity, const Vec3& toVec,const float& maxSpeed, const float forceMultipier = 0.025f, const float& decl = 3.0f);

		/// <summary>
		/// 近くにいるときはArriveで,遠くにいるときはSeekで追いかける関数
		/// </summary>
		/// <param name="velocity">現在の速度</param>
		/// <param name="toVec">ターゲット方向のベクトル</param>
		/// <param name="maxSpeed">最大速度</param>
		/// <param name="nearRange">計算を切り替える距離</param>
		/// <param name="decl"></param>
		/// <returns>計算されたForce</returns>
		static Vec3 CalucNearArriveFarSeek(const Vec3& velocity, const Vec3& toVec,
			const float& maxSpeed, const float& nearRange, const float& decl = 3.0f);

		/// <summary>
		/// 当たった壁に対して反射ベクトルを求める。
		/// </summary>
		/// <param name="direct">現在の進行方向</param>
		/// <param name="normal">あたったコリジョンのNomarl</param>
		/// <returns>反射ベクトル</returns>
		static Vec3 Reflection(const Vec3& moveDirect, const Vec3& normal);
	};

}

//endbasecross