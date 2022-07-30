
/*!
@file SubAnimatorBase.h
@brief SubAnimatorBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	namespace maru {
		template<class node_type, class StateEnum, class TransitionMember>
		class AnimationMainStateMachine;
	}

	//--------------------------------------------------------------------------------------
	///	サブアニメーターの基底クラス
	//--------------------------------------------------------------------------------------
	template<class node_type, class EnumType, class TransitionStructMember>
	class SubAnimatorBase : public maru::I_AnimationStateNode
	{
	public:
		using NodeType = node_type;
		using StateEnum = EnumType;
		using TransitionMember = TransitionStructMember;
		using StateMachine = maru::AnimationMainStateMachine<NodeType, EnumType, TransitionMember>;

	private:
		ex_weak_ptr<GameObject> m_owner;						//所有者

	protected:
		std::unique_ptr<StateMachine> m_stateMachine = nullptr;	//ステートマシン

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		SubAnimatorBase(const std::shared_ptr<GameObject>& objPtr)
			:m_owner(objPtr), m_stateMachine(new StateMachine())
		{ }
		
		virtual void OnStart() override final {

		}

		virtual bool OnUpdate() override final {
			m_stateMachine->OnUpdate();
			return false;
		}

		virtual void OnExit() override final{
			//ステートマシンの初期化が必要
			m_stateMachine->ResetState();
		}

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
		/// 現在のステートを取得
		/// </summary>
		/// <returns>現在のステート</returns>
		StateEnum GetCurrentState() const {
			return m_stateMachine->GetCurrentState();
		}

		/// <summary>
		/// 現在使っているノードの取得
		/// </summary>
		/// <returns>現在使っているノード</returns>
		auto GetCurrentNode() const {
			return m_stateMachine->GetNowNode();
		}

		std::shared_ptr<maru::AnimationClip> GetAnimationClip() const override {
			auto currentNode = m_stateMachine->GetNowNode();
			return currentNode ? currentNode->GetAnimationClip() : nullptr;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		std::shared_ptr<GameObject> GetGameObject() const {
			return m_owner.GetShard();
		}

	};

}