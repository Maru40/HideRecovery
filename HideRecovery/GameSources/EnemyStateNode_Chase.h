
/*!
@file EnemyStateNode_Chase.h
@brief EnemyStateNode_Chaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "EnemyStateNodeBase.h"

#include "SeekTarget.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class TargetManager;
	template<class node_type, class EnumType, class TransitionType>
	class EnemyMainStateMachine;

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			namespace ChaseState {
				struct TargetSeekParametor;
				struct AstarSeekParametor;
			}

			//--------------------------------------------------------------------------------------
			/// スライムの追従ステート
			//--------------------------------------------------------------------------------------
			class Chase : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				/// <summary>
				/// 追従ステートのステートタイプ
				/// </summary>
				enum class StateType {
					TargetSeek,
					AstarSeek,
					Null,
				};

				/// <summary>
				/// 遷移条件メンバー
				/// </summary>
				struct TransitionMember
				{

				};

				/// <summary>
				/// 追従パラメータ
				/// </summary>
				struct Parametor
				{
					std::shared_ptr<ChaseState::TargetSeekParametor> targetSeekParamPtr = nullptr; //ターゲット追従パラメータ
					std::shared_ptr<ChaseState::AstarSeekParametor> astarSeekParamPtr = nullptr;   //Astarを使った追従パラメータ

					Parametor();
				};

				using StateMachine = EnemyMainStateMachine<EnemyBase, StateType, TransitionMember>;

			private:
				ex_weak_ptr<Parametor> m_paramPtr;            //パラメータ

				std::unique_ptr<StateMachine> m_stateMachine; //ステートマシン

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="objPtr">このクラスを所有するクラス</param>
				/// <param name="numCreate">パラメータ</param>
				Chase(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

			private:
				/// <summary>
				/// ノードの生成
				/// </summary>
				void CreateNode();

				/// <summary>
				/// エッジの生成
				/// </summary>
				void CreateEdge();

				//--------------------------------------------------------------------------------------
				/// 遷移条件
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// 追従移動
				/// </summary>
				/// <param name="member">遷移メンバ</param>
				bool ToChaseMove(const TransitionMember& member);

				/// <summary>
				/// Astarを使った追従移動
				/// </summary>
				/// <param name="member">遷移メンバ</param>
				bool ToAstarMove(const TransitionMember& member);
				
			};

		}
	}
}