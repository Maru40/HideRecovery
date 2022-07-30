/*!
@file MetaballChildrenPressedManager.h
@brief MetaballChildrenPressedManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	struct PressData;

	namespace maru {
		enum class DeltaType;
	}

	namespace Metaball {

		class RenderBase;

		//--------------------------------------------------------------------------------------
		///	潰される管理のパラメータ
		//--------------------------------------------------------------------------------------
		struct ChildrenPressedManager_Parametor 
		{
			float stopCrashPower;       //ヒットストップ時の潰す力
			float slowCrashPower;       //スロー時の潰す力
			float stopAfterDestroyTime; //潰し終わった後、消すまでのインターバル時間

			/// <summary>
			/// コンストラクタ
			/// </summary>
			ChildrenPressedManager_Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="stopCrashPower">ヒットストップ時の潰す力</param>
			/// <param name="slowCrashPower">スロー時の潰す力</param>
			/// <param name="stopAfterDestroyTime">ヒットストップ後に何秒後に消去するか</param>
			ChildrenPressedManager_Parametor(
				const float& stopCrashPower, 
				const float& slowCrashPower, 
				const float& stopAfterDestroyTime
			);
		};

		//--------------------------------------------------------------------------------------
		///	潰される管理
		//--------------------------------------------------------------------------------------
		class ChildrenPressedManager : public Component
		{
		public:
			using Parametor = ChildrenPressedManager_Parametor;

		private:
			Parametor m_param;	//パラメータ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			ChildrenPressedManager(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="parametor">パラメータ</param>
			ChildrenPressedManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

			void OnCreate() override {};

		private:
			/// <summary>
			/// //スロープレス
			/// </summary>
			/// <param name="pressData">潰すデータ</param>
			void SlowTimerPress(const PressData& pressData);

			/// <summary>
			/// ヒットストッププレス
			/// </summary>
			/// <param name="pressData">潰すデータ</param>
			void HitStopPress(const PressData& pressData);   

			/// <summary>
			/// SE再生
			/// </summary>
			/// <param name="pressData">潰すデータ</param>
			/// <param name="pressingTime">潰す時間</param>
			/// <param name="pressedTime">潰される時間</param>
			/// <param name="updateTime">更新速度</param>
			/// <param name="deltaType">更新時間タイプ</param>
			void PlayOneShot(const PressData& pressData,
				const float& pressingTime, const float& pressedTime,
				const float& updateTime, const maru::DeltaType& deltaType);

		public:
			/// <summary>
			/// SlimeChildの追従を解除など、潰された時に呼び出す処理
			/// </summary>
			/// <param name="pressData">潰すデータ</param>
			void PressCrash(const PressData& pressData);

			/// <summary>
			/// 潰される時間の取得
			/// </summary>
			/// <returns>潰される時間</returns>
			float GetPressedTime() const;

			/// <summary>
			/// 潰される力の取得
			/// </summary>
			/// <returns>潰される力</returns>
			float GetPressedPower() const;

		};

	}

}

//endbasecross