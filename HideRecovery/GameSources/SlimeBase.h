
/*!
@file SlimeBase.h
@brief SlimeBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "EnemyBase.h"

#include "I_Pressed.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	struct PushData;

	namespace Metaball {
		class ChildrenRender;
		class RenderBase;
	}

	namespace Enemy {

		namespace Motion {
			struct KnockBack_Parametor;
		}

		//--------------------------------------------------------------------------------------
		/// スライムの基底クラス
		//--------------------------------------------------------------------------------------
		class SlimeBase : public EnemyBase, public I_Pressed, public I_PushedWater
		{
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			SlimeBase(const std::shared_ptr<GameObject>& objPtr);

			void OnCreate() override;

		protected:
			/// <summary>
			/// 拡散する速度を計算して返す
			/// </summary>
			/// <param name="selfObject">自分自身のオブジェクト</param>
			/// <param name="childObject">メタボールの一つ</param>
			/// <param name="pressData">潰すデータ</param>
			/// <returns>拡散する速度</returns>
			Vec3 CalculateSpreadVelocity(
				const std::shared_ptr<GameObject>& selfObject,
				const std::shared_ptr<GameObject>& childObject,
				const PressData& pressData);

			/// <summary>
			/// 拡散方向の力をChildに加える。
			/// </summary>
			/// <param name="render">メタボールレンダーコンポーネント</param>
			/// <param name="child">メタボールの全てを管理するコンポーネント</param>
			/// <param name="pressData">潰す力</param>
			void AddSpreadPower(
				const std::shared_ptr<Metaball::RenderBase>& render,
				const std::shared_ptr<Metaball::ChildrenRender>& child,
				const PressData& pressData
			);

		public:
			//--------------------------------------------------------------------------------------
			/// インターフェースの実装
			//--------------------------------------------------------------------------------------

			virtual PressedEnergy Pressed(const PressData& pressData) override;

			virtual void Push(const PushData& data) override;

			virtual void Spread(const PressData& data) override;

			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 潰れる時のアニメーション再生用のデータを取得する。
			/// </summary>
			/// <param name="pressData">潰すデータ</param>
			/// <returns>潰れる時のアニメーション再生用のデータ</returns>
			virtual Motion::KnockBack_Parametor GetPressedMotionData(const PressData& pressData);
		};

	}
}

//endbasecross