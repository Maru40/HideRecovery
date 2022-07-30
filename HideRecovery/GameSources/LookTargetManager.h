
/*!
@file LookTargetManager.h
@brief LookTargetManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	注視する敵を管理するクラス
	//--------------------------------------------------------------------------------------
	class LookTargetManager : public Component
	{
	public:
		struct Parametor
		{
			float inputChangeRange;  //スティック入力によるカメラ切り替えの範囲
			float inputChangeDegree; //スティック入力によるカメラ切り替えの角度

			Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="inputChangeRange">入力切り替え範囲</param>
			/// <param name="inputChangeDegree">入力切り替え範囲のデグリー角</param>
			Parametor(const float& inputChangeRange, const float& inputChangeDegree);
		};

	private:
		Parametor m_param;  //パラメータ

		ex_weak_ptr<GameObject> m_target; //ターゲットのゲームオブジェクトのポインタ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		LookTargetManager(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="parametor">パラメータ</param>
		LookTargetManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnUpdate() override;

	private:
		/// <summary>
		/// インプットに合わせたUpdateイベント。
		/// </summary>
		void ChangeTargetManager();

		/// <summary>
		/// ターゲットの近くのターゲットを取得する。
		/// </summary>
		/// <param name="range">探索範囲</param>
		/// <param name="target">探索するターゲット</param>
		/// <returns>一番近くにいるターゲット</returns>
		vector<std::shared_ptr<GameObject>> SearchTargetNearTargets(
			const float& range, const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// ターゲット群から入力方向のターゲットを取得
		/// </summary>
		/// <param name="currentTarget">現在のターゲット</param>
		/// <param name="targets">探索するターゲット群</param>
		/// <returns>探索したターゲットを返す</returns>
		std::shared_ptr<GameObject> FindDirectTarget(
			const std::shared_ptr<GameObject>& currentTarget, const std::vector<std::shared_ptr<GameObject>>& targets);
	
	public:
		/// <summary>
		/// 近くのターゲットを取得する
		/// </summary>
		/// <returns>一番近くのターゲットを取得する</returns>
		std::shared_ptr<GameObject> SearchTarget();

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// パラメータのセット
		/// </summary>
		/// <param name="parametor">パラメータ</param>
		void SetParametor(const Parametor& parametor) noexcept { m_param = parametor; }

		/// <summary>
		/// パラメータの取得
		/// </summary>
		/// <returns>パラメータ</returns>
		Parametor GetParametor() const noexcept { return m_param; }

		/// <summary>
		/// ターゲットのセット
		/// </summary>
		/// <param name="target">ターゲットのゲームオブジェクト</param>
		void SetTarget(const std::shared_ptr<GameObject>& target) { m_target = target; }

		/// <summary>
		/// ターゲットの取得
		/// </summary>
		/// <returns>ターゲットのゲームオブジェクト</returns>
		std::shared_ptr<GameObject> GetTarget() const noexcept { return m_target.GetShard(); }

		/// <summary>
		/// ターゲットが存在するかどうか
		/// </summary>
		/// <returns>ターゲットが存在するならtrue</returns>
		bool HasTarget() const noexcept { return m_target.GetShard() ? true : false; }

	};

}

//endbasecross