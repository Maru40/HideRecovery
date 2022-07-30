/*!
@file LookAtCameraManager.h
@brief LookAtCameraManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	カメラの視点管理クラス
	//--------------------------------------------------------------------------------------
	class LookAtCameraManager : public Component
	{
	public:
		struct Parametor 
		{
			float speed;       //移動スピード
			Vec3 centerOffset; //中心位置のオフセット

			/// <summary>
			/// コンストラクタ
			/// </summary>
			Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="centerOffset">中心位置のオフセット</param>
			Parametor(const Vec3& centerOffset);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="speed">移動スピード</param>
			/// <param name="centerOffset">中心位置のオフセット</param>
			Parametor(const float& speed, const Vec3& centerOffset);
		};

	private:
		Parametor m_param;  //パラメータ

		ex_weak_ptr<GameObject> m_target = nullptr; //ターゲットオブジェクトのポインタ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		LookAtCameraManager(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="target">ターゲットのゲームオブジェクト</param>
		LookAtCameraManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="target">ターゲットのゲームオブジェクト</param>
		/// <param name="param">パラメータ</param>
		LookAtCameraManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& target, const Parametor& param);

		void OnUpdate2() override;

	private:
		/// <summary>
		/// 視点更新処理
		/// </summary>
		void LookAtUpdate();

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ターゲットのセット
		/// </summary>
		/// <param name="target">ターゲットのゲームオブジェクト</param>
		void SetTarget(const std::shared_ptr<GameObject>& target) noexcept { m_target = target; }

		/// <summary>
		/// ターゲットの取得
		/// </summary>
		/// <returns>ターゲットのゲームオブジェクト</returns>
		std::shared_ptr<GameObject> GetTarget() const noexcept { return m_target.GetShard(); }

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
	};

}

//endbasecorss