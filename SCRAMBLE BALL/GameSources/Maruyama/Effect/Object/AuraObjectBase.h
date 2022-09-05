/*!
@file AuraObjectBase.h
@brief AuraObjectBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Effect {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		struct Aura_Parametor;

		//--------------------------------------------------------------------------------------
		/// オーラオブジェクトの基底クラス
		//--------------------------------------------------------------------------------------
		class AuraObjectBase : public GameObject
		{
			std::unique_ptr<Aura_Parametor> m_auraParamPtr;  //オーラパラメータ

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="stage">このクラスが所属するステージ</param>
			/// <param name="parametor">オーラパラメータ</param>
			AuraObjectBase(const std::shared_ptr<Stage>& stage, const Aura_Parametor& parametor);

			void OnCreate() override;
		};

	}
}