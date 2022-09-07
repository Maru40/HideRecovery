
/*!
@file StatorBase.h
@brief StatorBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	template<class node_type, class EnumType, class TransitionType>
	class EnemyMainStateMachine;

	namespace Enemy {
		
		//--------------------------------------------------------------------------------------
		///	EnumTypeを元に定義したいインターフェース
		//--------------------------------------------------------------------------------------
		template<class EnumType>
		class I_Stator_EnumType 
		{
		public:
			/// <summary>
			/// ステートの切り替え
			/// </summary>
			/// <param name="type">ステートのEnumType</param>
			/// <param name="priority"></param>
			virtual void ChangeState(const EnumType& state, const int& priority = 0) = 0;

			/// <summary>
			/// 現在使用中のステートタイプを取得
			/// </summary>
			/// <returns>ステートタイプ</returns>
			virtual EnumType GetCurrentState() const = 0;

			/// <summary>
			/// 現在のステートタイプが指定したタイプかどうかを返す。
			/// </summary>
			/// <param name="type">確認したいステートタイプ</param>
			/// <returns>指定したタイプならtrue</returns>
			virtual bool IsCurrentState(const EnumType& type) const = 0;
		};

		//--------------------------------------------------------------------------------------
		///	TransitiomMemberを元に定義したいインターフェース
		//--------------------------------------------------------------------------------------
		template<class TransitionMember>
		class I_Stator_TransitionType
		{
		public:
			/// <summary>
			/// 遷移条件用のメンバーを取得
			/// </summary>
			/// <returns>遷移条件用のメンバー</returns>
			virtual TransitionMember& GetTransitionMember() const = 0;
		};

		//--------------------------------------------------------------------------------------
		///	ステーターのインターフェース
		//--------------------------------------------------------------------------------------
		template<class EnumType, class TransitionMember>
		class I_Stator : public I_Stator_EnumType<EnumType>, public I_Stator_TransitionType<TransitionMember>
		{

		};

		//--------------------------------------------------------------------------------------
		///	ステーターの基底クラス
		//--------------------------------------------------------------------------------------
		template<class NodeType, class EnumType, class TransitionMember>
		class StatorBase : public Component, public I_Stator<EnumType, TransitionMember>
		{
		public:
			using StateMachine = EnemyMainStateMachine<NodeType, EnumType, TransitionMember>;

		protected:
			std::unique_ptr<StateMachine> m_stateMachine = nullptr;  //ステートマシン

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			StatorBase(const std::shared_ptr<GameObject>& objPtr)
				: StatorBase(objPtr, TransitionMember())
			{}

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
			/// <param name="member">遷移メンバー</param>
			StatorBase(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member)
				: Component(objPtr), m_stateMachine(new StateMachine(member))
			{}

		protected:
			/// <summary>
			/// ノードの生成
			/// </summary>
			virtual void CreateNode() = 0;

			/// <summary>
			/// エッジの生成
			/// </summary>
			virtual void CreateEdge() = 0;

		public:

			void OnCreate() override final{
				CreateNode();
				CreateEdge();
			}

			void OnUpdate() override final{
				m_stateMachine->OnUpdate();
			}

			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// ステートの変更
			/// </summary>
			/// <param name="state">変更したいステート</param>
			/// <param name="priority">優先度</param>
			void ChangeState(const EnumType& state, const int& priority = 0) override final{
				m_stateMachine->ChangeState(state, priority);
			}

			/// <summary>
			/// 現在のステートタイプを取得
			/// </summary>
			/// <returns>現在のステートタイプ</returns>
			EnumType GetCurrentState() const override final{
				return m_stateMachine->GetCurrentNodeType();
			}

			/// <summary>
			/// 指定したステートの状態かどうかを返す。
			/// </summary>
			/// <param name="type">確認したいステート</param>
			/// <returns>指定したステートならtrue</returns>
			bool IsCurrentState(const EnumType& type) const override final {
				return m_stateMachine->IsCurrentNodeType(type);
			}

			/// <summary>
			/// 遷移条件メンバーを取得する。
			/// </summary>
			/// <returns>遷移条件メンバー</returns>
			TransitionMember& GetTransitionMember() const override final{
				return m_stateMachine->GetTransitionStructMember();
			}
		};

	}
}