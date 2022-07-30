
#pragma once
/*!
@file StartCamera.h
@brief StartCameraクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class GameStageBase;

	//--------------------------------------------------------------------------------------
	///	カメラの移動管理パラメータ
	//--------------------------------------------------------------------------------------
	struct CameraMoveParametor {
		Vec3 startPosition;   //開始位置
		Vec3 endPosition;     //終了位置
		float speed;          //移動スピード

		/// <summary>
		/// コストラクタ
		/// </summary>
		CameraMoveParametor();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="startPosition">開始位置</param>
		/// <param name="speed">移動スピード</param>
		CameraMoveParametor(
			const Vec3& startPosition,
			const float& speed = 1.0f
		);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="startPosition">開始位置</param>
		/// <param name="endPosition">終了位置</param>
		/// <param name="speed">移動スピード</param>
		CameraMoveParametor(
			const Vec3& startPosition,
			const Vec3& endPosition,
			const float& speed = 1.0f
		);
	};

	//--------------------------------------------------------------------------------------
	///	開始演習用のカメラ
	//--------------------------------------------------------------------------------------
	class StartCamera : public Camera
	{
	public:

		/// <summary>
		/// 開始カメラ演出用のパラメータ
		/// </summary>
		struct ShowParametor
		{
			CameraMoveParametor eye; //カメラ位置の移動パラメータ
			CameraMoveParametor at;  //カメラの視点移動パラメータ
			float lookTime;          //どの位の時間を見るか

			/// <summary>
			/// コンストラクタ
			/// </summary>
			ShowParametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="eye">カメラ位置の移動パラメータ</param>
			/// <param name="at">カメラの視点移動パラメータ</param>
			/// <param name="lookTime">どのくらいの時間を見るか</param>
			ShowParametor(const CameraMoveParametor& eye,
				const CameraMoveParametor& at,
				const float lookTime = 3.0f
			);
		};

	private:
		std::vector<ShowParametor> m_params; //演出移動用のパラメータ群
		int m_index;                         //現在のインデックス

		float m_timeElapsed;                 //計測時間

	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="params">開始カメラ演出のパラメータ</param>
		StartCamera(const vector<ShowParametor>& params) :
			Camera(),m_params(params),m_index(0),m_timeElapsed(0.0f)
		{}

		void OnCreate() override;
		void OnUpdate() override;

	private:

		/// <summary>
		/// ゲームステージの取得
		/// </summary>
		/// <returns>ゲームステージ</returns>
		std::shared_ptr<GameStageBase> GetGameStage();

		/// <summary>
		/// カメラ位置移動
		/// </summary>
		void EyeMove();

		/// <summary>
		/// 視点移動
		/// </summary>
		void AtMove();

	public:

		/// <summary>
		/// 演出スキップ
		/// </summary>
		void Skip();

		/// <summary>
		/// カメラの変更
		/// </summary>
		void ChangeCamera();

		/// <summary>
		/// 演出が終了しているか判断
		/// </summary>
		/// <returns>終了演出</returns>
		bool IsEnd() {  return m_params.size() <= m_index; }

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 開始演出のパラメータ群のセット
		/// </summary>
		/// <param name="params">パラメータ群</param>
		void SetShowParametor(const std::vector<ShowParametor>& params) noexcept { m_params = params; }
	};

}

//endbasecross