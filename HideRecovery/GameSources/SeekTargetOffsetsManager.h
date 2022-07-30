/*!
@file SeekTargetOffsetsManager.h
@brief SeekTargetOffsetsManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskList;

	class GameTimer;
	class SeekTarget;

	namespace Metaball {
		class ChildrenRender;
	}

	//--------------------------------------------------------------------------------------
	///	追従オフセット管理クラス
	//--------------------------------------------------------------------------------------
	class SeekTargetOffsetsManager 
	{
	public:
		/// <summary>
		/// タスクタイプ
		/// </summary>
		enum class TaskEnum {
			CenterMove,   //中心に移動する処理
			OuterMove,    //外側に移動する処理
			ReturnMove,   //元に戻る処理
			TargetMove,   //ターゲットに向かう処理
		};

		/// <summary>
		/// 追従オフセット管理パラメータ
		/// </summary>
		struct Parametor
		{
			float time;   //どのくらいの時間で移動するか
			float range;  //縮む距離
			Vec3 offset;  //距離のオフセット

			/// <summary>
			/// コンストラクタ
			/// </summary>
			Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="time">どのくらいの時間で移動するか</param>
			/// <param name="range">縮む距離</param>
			/// <param name="offset">距離のオフセット</param>
			Parametor(const float& time, const float& range, const Vec3& offset = Vec3(0.0f));
		};

		/// <summary>
		/// 基本の追従データ
		/// </summary>
		struct DefaultSeekData
		{
			Vec3 offset; //追従オフセット
			float range; //追従距離

			/// <summary>
			/// コンストラクタ
			/// </summary>
			DefaultSeekData();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="offset">追従オフセット</param>
			/// <param name="range">追従距離</param>
			DefaultSeekData(const Vec3& offset, const float& range);
		};

	private:
		ex_weak_ptr<GameObject> m_owner;                //自分自身を所有するゲームオブジェクト

		std::unique_ptr<GameTimer> m_timer;             //時間管理クラス
		std::unique_ptr<TaskList<TaskEnum>> m_taskList; //タスク管理クラス

		Parametor m_centerMoveParam;                    //中心に向かって動くパラメータ
		Parametor m_returnMoveParam;                    //元の位置に戻る時のパラメータ

		vector<DefaultSeekData> m_defaultSeekData;      //追従データのデフォルト値

		vector<ex_weak_ptr<SeekTarget>> m_seekTargets;  //追従パラメータ群

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="owner">このクラスを所有するゲームオブジェクト</param>
		/// <param name="seekTargets">追従コンポーネント群</param>
		SeekTargetOffsetsManager(const std::shared_ptr<GameObject>& owner, const vector<ex_weak_ptr<SeekTarget>>& seekTargets);

		//外部から呼び出し
		void OnUpdate();

	private:
		/// <summary>
		/// タスクの定義
		/// </summary>
		void DefineTask();

		/// <summary>
		/// タスクの選択
		/// </summary>
		/// <param name="task">選択したタスク</param>
		void SelectTask(const TaskEnum& task);

		/// <summary>
		/// 追従のデフォルト値を設定する。
		/// </summary>
		void SetDefaultSeekOffset();

		/// <summary>
		/// 補間された移動場所を計算して返す。
		/// </summary>
		/// <param name="startPosition">開始場所</param>
		/// <param name="endPosition">終了場所</param>
		/// <param name="seek">追従コンポーネント</param>
		/// <param name="parametor">追従パラメータ</param>
		/// <returns>補完された移動場所</returns>
		Vec3 CalculateLerpMovePosition(const Vec3& startPosition, const Vec3& endPosition,
			const std::shared_ptr<SeekTarget>& seek, const Parametor& parametor);

		/// <summary>
		/// 移動場所を計算して返す。
		/// </summary>
		/// <param name="startPosition">開始場所</param>
		/// <param name="endPosition">終了場所</param>
		/// <param name="seek">追従コンポーネント</param>
		/// <param name="parametor">追従パラメータ</param>
		/// <returns>移動場所</returns>
		Vec3 CalculateTransformMovePosition(const Vec3& startPosition, const Vec3& endPosition,
			const std::shared_ptr<SeekTarget>& seek, const Parametor& parametor);

		/// <summary>
		/// 中心方向への移動開始
		/// </summary>
		void StartCenterMove();

		/// <summary>
		/// 中心方向への移動中
		/// </summary>
		/// <returns>移動が完了したらtrue</returns>
		bool UpdateCenterMove();

		/// <summary>
		/// 元の場所に移動開始
		/// </summary>
		void StartReturnMove();

		/// <summary>
		/// 元の場所に移動中
		/// </summary>
		/// <returns>移動が完了したらtrue</returns>
		bool UpdateReturnMove();

		/// <summary>
		/// 元の場所に移動完了後の処理
		/// </summary>
		void ExitReturnMove();

	public:
		/// <summary>
		/// 移動を開始する
		/// </summary>
		/// <param name="task">タスクタイプ</param>
		/// <param name="parametor">追従管理パラメータ</param>
		void StartMove(const TaskEnum& task, const Parametor& parametor);

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// タスクの終了を判断
		/// </summary>
		/// <returns>タスクの終了していたらtrue</returns>
		bool IsEndTask();

		/// <summary>
		///	自分自身を所有するゲームオブジェクトの取得
		/// </summary>
		/// <returns>自分自身を所有するゲームオブジェクト</returns>
		std::shared_ptr<GameObject> GetGameObject();

	};

}

//endbasecross