/*!
@file Task_CameraAtMove.h
@brief Task_CameraAtMoveなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/TaskList/TaskList.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	パン移動タスク
	//--------------------------------------------------------------------------------------
	class Task_CameraAtMove : public TaskNodeBase<Camera>
	{
	public:
		//--------------------------------------------------------------------------------------
		///	移動タイプ
		//--------------------------------------------------------------------------------------
		enum class MoveType 
		{
			Lerp,      //補間
			Transform, //通常トランスフォーム
		};

		//--------------------------------------------------------------------------------------
		///	時間計測タイプ
		//--------------------------------------------------------------------------------------
		enum class DeltaType
		{
			Normal,    //通常
			Real       //RealTime
		};

		//--------------------------------------------------------------------------------------
		///	移動パラメータ
		//--------------------------------------------------------------------------------------
		struct Parametor
		{
			Vec3 startPosition;   //開始位置
			Vec3 endPosition;     //終了位置
			float speed;          //移動速度
			MoveType moveType;    //移動タイプ
			DeltaType deltaType;  //時間計測タイプ

			/// <summary>
			/// コンストラクタ
			/// </summary>
			Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="startPosition">開始位置</param>
			/// <param name="endPosition">終了位置</param>
			/// <param name="speed">移動速度</param>
			/// <param name="moveType">移動タイプ</param>
			/// <param name="deltaType">時間計測タイプ</param>
			Parametor(const Vec3& startPosition, const Vec3& endPosition, const float& speed, 
				const MoveType moveType, const DeltaType& deltaType = DeltaType::Normal);

			/// <summary>
			/// 終了位置の方向を取得
			/// </summary>
			/// <returns>終了位置の方向</returns>
			Vec3 GetToEndDirection() const {
				return endPosition - startPosition;
			}
		};

	private:
		std::vector<Parametor> m_params;	//移動パラメータ配列
		int m_currentIndex = 0;				//現在のインデックス

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="camera">カメラのポインタ</param>
		Task_CameraAtMove(const std::shared_ptr<Camera> camera);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="camera">カメラのポインタ</param>
		/// <param name="parametor">移動パラメータ</param>
		Task_CameraAtMove(const std::shared_ptr<Camera> camera, const Parametor& parametor);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="camera">カメラのポインタ</param>
		/// <param name="parametors">移動パラメータの配列</param>
		Task_CameraAtMove(const std::shared_ptr<Camera> camera, const vector<Parametor>& parametors);

		void OnStart() override;
		bool OnUpdate() override;
		void OnExit() override;

	private:
		/// <summary>
		/// 移動更新処理
		/// </summary>
		void MoveUpdate();

		/// <summary>
		/// 補完移動処理
		/// </summary>
		/// <param name="param">移動パラメータ</param>
		void LerpMove(const Parametor& param);

		/// <summary>
		/// Transform移動処理
		/// </summary>
		/// <param name="param">移動パラメータ</param>
		void TransformMove(const Parametor& param);

		/// <summary>
		/// 次の目的地に移行するかどうか
		/// </summary>
		/// <param name="param">移動パラメータ</param>
		/// <returns>次の目的地に移行するならtrue</returns>
		bool IsNext(const Parametor& param);

		/// <summary>
		/// 計測時間の取得
		/// </summary>
		/// <param name="param">移動パラメータ</param>
		/// <returns>計測時間</returns>
		float GetElapsedTime(const Parametor& param);

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 終了判断
		/// </summary>
		/// <returns>全ての移動が完了したらtrue</returns>
		bool IsEnd();

		/// <summary>
		/// 移動パラメータの追加
		/// </summary>
		/// <param name="parametor">移動パラメータ</param>
		void AddParametor(const Parametor& parametor);

		/// <summary>
		/// 現在の移動パラメータの取得
		/// </summary>
		/// <returns>現在の移動パラメータ</returns>
		Parametor GetNowParametor() const;

		/// <summary>
		/// 移動パラメータ配列の設定
		/// </summary>
		/// <param name="parametors">移動パラメータの配列</param>
		void SetParametors(const vector<Parametor>& parametors);

		/// <summary>
		/// 移動パラメータの配列の取得
		/// </summary>
		/// <returns>移動パラメータの配列</returns>
		vector<Parametor> GetParametors() const;
	};

}

//endbasecross