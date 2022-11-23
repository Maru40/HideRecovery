/*!
@file CameraShake.h
@brief CameraShakeなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class GameTimer;

	namespace maru{
		enum class DeltaType;
	}

	//--------------------------------------------------------------------------------------
	///	カメラシェイク管理クラス
	//--------------------------------------------------------------------------------------
	class CameraShake : public Component
	{
	public:
		/// <summary>
		/// カメラシェイクのパラメータ
		/// </summary>
		struct Parametor
		{
			float time;				   //揺らす時間
			Vec3 powerVec;			   //揺らす力の方向
			float powerf;              //揺らす力
			float updateTimeSpeed;     //計測時間のスピード
			maru::DeltaType deltaType; //計測時間タイプ
			Vec3 centerPosition;       //中心の場所
			std::function<void()> exitFunction = nullptr;	//終了時に呼び出したい処理

			/// <summary>
			/// コンストラクタ
			/// </summary>
			Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="time">揺らす時間</param>
			/// <param name="powerVec">揺らす力の方向</param>
			/// <param name="powerf">揺らす力</param>
			/// <param name="updateTimeSpeed">計測時間のスピード</param>
			/// <param name="deltaType">計測時間タイプ</param>
			Parametor(const float& time,
				const Vec3& powerVec,
				const float& powerf = 0.25f,
				const float& updateTimeSpeed = 1.0f,
				const maru::DeltaType& deltaType = maru::DeltaType(0)
			);
		};

	private:
		Parametor m_param;             //パラメータ

		ex_weak_ptr<Camera> m_camera;  //カメラのポインタ

		unique_ptr<GameTimer> m_timer; //時間計測クラス

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		CameraShake(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate2() override;

	private:
		/// <summary>
		/// 揺らす更新処理
		/// </summary>
		void ShakeUpdate();

	public:
		/// <summary>
		/// カメラ揺らし開始
		/// </summary>
		/// <param name="time">揺らす時間</param>
		void StartShake(const float& time);

		/// <summary>
		/// カメラ揺らし開始
		/// </summary>
		/// <param name="time">揺らす時間</param>
		/// <param name="powerVec">揺らす方向</param>
		void StartShake(const float& time, const Vec3& powerVec);

		/// <summary>
		/// カメラ揺らし開始
		/// </summary>
		/// <param name="time">揺らす時間</param>
		/// <param name="powerVec">揺らす方向</param>
		/// <param name="powerf">揺らす力</param>
		void StartShake(const float& time, const Vec3& powerVec, const float& powerf);

		/// <summary>
		/// カメラ揺らし開始
		/// </summary>
		/// <param name="time">揺らす時間</param>
		/// <param name="powerVec">揺らす方向</param>
		/// <param name="powerf">揺らす力</param>
		/// <param name="updateTimeSpeed">時間計測スピード</param>
		void StartShake(const float& time, const Vec3& powerVec, const float& powerf, const float& updateTimeSpeed);

		/// <summary>
		/// カメラ揺らし開始
		/// </summary>
		/// <param name="time">揺らす時間</param>
		/// <param name="powerVec">揺らす方向</param>
		/// <param name="powerf">揺らす力</param>
		/// <param name="updateTimeSpeed">時間計測スピード</param>
		/// <param name="deltaType">時間計測タイプ</param>
		void StartShake(const float& time, const Vec3& powerVec, const float& powerf, const float& updateTimeSpeed, const maru::DeltaType& deltaType);

		/// <summary>
		/// カメラ揺らし開始
		/// </summary>
		/// <param name="time">揺らす時間</param>
		/// <param name="deltaType">時間計測タイプ</param>
		void StartShake(const float& time, const maru::DeltaType& deltaType);

		/// <summary>
		/// カメラ揺らし開始
		/// </summary>
		/// <param name="parametor">カメラ揺らしパラメータ</param>
		void StartShake(const Parametor& parametor);

		/// <summary>
		/// 終了時に呼び出したい処理を設定
		/// </summary>
		/// <param name="exitFunction"></param>
		void SetExitFunction(const std::function<void()>& exitFunction) { m_param.exitFunction = exitFunction; }

	};

}

//endbasecross