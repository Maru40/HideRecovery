/*!
@file Slime_Absorbed.h
@brief Slime_Absorbedなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "EnemyStateNodeBase.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// 吸収されたステートのパラメータ
			//--------------------------------------------------------------------------------------
			struct Slime_Absorbed_Parametor 
			{
				std::weak_ptr<GameObject> parent;  //自分を吸収した相手。
			};

			//--------------------------------------------------------------------------------------
			/// 吸収されたステート
			//--------------------------------------------------------------------------------------
			class Slime_Absorbed : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = Slime_Absorbed_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr; //パラメータ

			public:
				/// <summary>
				/// 吸収されたステート
				/// </summary>
				/// <param name="objPtr">このクラスを所有するクラス</param>
				/// <param name="numCreate">パラメータ</param>
				Slime_Absorbed(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

				//--------------------------------------------------------------------------------------
				/// アクセッサ
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// 自分を吸収した相手を取得する。
				/// </summary>
				/// <returns>自分を吸収した相手</returns>
				std::shared_ptr<GameObject> GetParent() const;
			};

		}
	}
}