
/*!
@file AnimatorBase.h
@brief AnimatorBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	namespace maru {
		template<class node_type, class EnumType, class TransitionStructMember>
		class AnimationMainStateMachine;
	}

	//--------------------------------------------------------------------------------------
	///	アニメーターの基底クラス
	//--------------------------------------------------------------------------------------
	template<class NodeType, class StateEnum, class TransitionStructMember>
	class AnimatorBase : public Component
	{
	public:
		using StateMachine = maru::AnimationMainStateMachine<NodeType, StateEnum, TransitionStructMember>;

	protected:
		std::unique_ptr<StateMachine> m_stateMachine = nullptr; //ステートマシン

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		AnimatorBase(const std::shared_ptr<GameObject>& objPtr)
			: Component(objPtr), m_stateMachine(new StateMachine(TransitionStructMember()))
		{}

		void OnCreate() override final {
			CreateNode();
			CreateEdge();
		}

		void OnUpdate() override final {
			m_stateMachine->OnUpdate();
		}

	private:
		/// <summary>
		/// ノードの生成
		/// </summary>
		virtual void CreateNode() = 0;

		/// <summary>
		/// エッジの生成
		/// </summary>
		virtual void CreateEdge() = 0;

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ステートの変更
		/// </summary>
		/// <param name="state">変更したいステート</param>
		/// <param name="priority">優先度</param>
		void ChangeState(const StateEnum state, const int& priority = 0) {
			m_stateMachine->ChangeState(state, priority);
		}

		/// <summary>
		/// 強制ステート変更
		/// </summary>
		/// <param name="state">変更したいステート</param>
		/// <param name="priority">優先度</param>
		void ForceChangeState(const StateEnum& state, const int& priority = 0) {
			m_stateMachine->ForceChangeState(state);
		}

		/// <summary>
		/// 現在のステートタイプを取得
		/// </summary>
		/// <returns>現在のステートタイプ</returns>
	    StateEnum GetCurrentState() const {
			return m_stateMachine->GetCurrentState();
		}

		/// <summary>
		/// 現在使用中のノードを取得
		/// </summary>
		/// <returns>現在使用中のノード</returns>
		auto GetCurrentNode() const {
			return m_stateMachine->GetNowNode();
		}

	};

}