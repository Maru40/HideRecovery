
/*!
@file AroundEyeCheck.h
@brief AroundEyeCheckなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskNodeBase;

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// 周囲を確認するパラメータ
		//--------------------------------------------------------------------------------------
		struct AroundEyeCheck_Parametor
		{
			float speed;         //目線の移動スピード
			float aroundRadian;  //周囲を確認する角度

			/// <summary>
			/// コンストラクタ
			/// </summary>
			AroundEyeCheck_Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="speed">目線の移動スピード</param>
			/// <param name="aroundDegree">周辺を確認する角度</param>
			AroundEyeCheck_Parametor(const float& speed, const float& aroundDegree);
		};

		//--------------------------------------------------------------------------------------
		/// 周囲を確認するタスク
		//--------------------------------------------------------------------------------------
		class AroundEyeCheck : public TaskNodeBase<GameObject>
		{
		public:
			using Parametor = AroundEyeCheck_Parametor;

		private:
			std::weak_ptr<Parametor> m_paramPtr; //パラメータ
			float m_defaultSpeed = 0.0f;       //初期スピード

			uint16_t m_index = 0;              //見る方向配列のインデックス
			std::vector<Vec3> m_directs;       //確認する方向の配列

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="owner">このクラスを所有するゲームオブジェクト</param>
			/// <param name="parametor">パラメータ</param>
			AroundEyeCheck(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// 向きたい方向群のセッティング
			/// </summary>
			void SettingDirects();

			/// <summary>
			/// 次に向きたい方向のセット
			/// </summary>
			void NextDirect();

		public:
			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// 終了判断
			/// </summary>
			/// <returns>更新が終了したならtrue</returns>
			bool IsEnd();
		};

	}

}

//endbasecross