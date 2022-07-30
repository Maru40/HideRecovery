
/*!
@file Motion_Idle.h
@brief Motion_Idleなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	namespace Metaball {
		class MotionManager;

		namespace Task {
			struct SeekOffsetMove_Parametor;
		}
	}

	enum class SeekTarget_Type;

	namespace Enemy {

		class EnemyBase;

		namespace Motion {

			//--------------------------------------------------------------------------------------
			/// 追従データ
			//--------------------------------------------------------------------------------------
			struct SeekData {
				float speed;
				SeekTarget_Type seekType;

				SeekData();
				SeekData(const float& speed, const SeekTarget_Type& seekType);
			};

			//--------------------------------------------------------------------------------------
			/// 待機パラメータ
			//--------------------------------------------------------------------------------------
			struct Idle_Parametor {
				float time;            //時間
				float firstSpeed;      //初期スピード
				SeekData metaballData; //メタボールの追従データ
				SeekData coreData;     //コアのデータ

				Idle_Parametor();
				Idle_Parametor(const float& firstSpeed);
				Idle_Parametor(const float& time, const float& firstSpeed);
				Idle_Parametor(const float& time, const float& firstSpeed, const SeekData& metaballData, const SeekData& coreData);
			};

			//--------------------------------------------------------------------------------------
			/// 待機モーション
			//--------------------------------------------------------------------------------------
			class Idle : public maru::MetaballMotionBase<EnemyBase>
			{
			public:
				using Parametor = Idle_Parametor;

			private:
				Parametor m_param;       //パラメータ

				float m_radius;          //半径
				vector<Vec3> m_offsets;  //オフセット配列

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するクラス</param>
				Idle(const std::shared_ptr<EnemyBase>& owner);

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するクラス</param>
				/// <param name="parametor">パラメータ</param>
				Idle(const std::shared_ptr<EnemyBase>& owner, const Parametor& parametor);

			private:
				vector<Metaball::Task::SeekOffsetMove_Parametor> CalculateMotionParametors() override;

			};
		}
	}
}