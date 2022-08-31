/*!
@file EnemyMainStateMachine.h
@brief EnemyMainStateMachine
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "StateNodeBase.h"
#include "StateEdgeBase.h"
#include "GraphBase.h"

#include "DebugObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	template<class node_type, class EnumType, class TransitionStructMember>
	class EnemyMainStateMachine;

	/// <summary>
	/// EnemyStateMachine用のテンプレートクラス。
	/// </summary>
	/// <typeparam name="NodeType">使用するノード</typeparam>
	/// <typeparam name="EnumType">使用する列挙体</typeparam>
	/// <typeparam name="TransitionStructMember">遷移条件用の構造体メンバー</typeparam>
	template<class node_type, class EnumType, class TransitionStructMember>
	class EnemyMainStateMachine : public GraphBase<StateNodeBase<node_type>, EnumType, TransitionStructMember>
	{
		//--------------------------------------------------------------------------------------
		///	遷移先候補のパラメータ
		//--------------------------------------------------------------------------------------
		struct TransitionCandidateParametor
		{
			EnumType type;    //切り替えるタイプ
			int priority;     //優先度

			/// <summary>
			/// 遷移先候補パラメータ
			/// </summary>
			/// <param name="type">遷移タイプ</param>
			TransitionCandidateParametor(const EnumType type)
				:TransitionCandidateParametor(type)
			{}

			/// <summary>
			/// 遷移先候補パラメータ
			/// </summary>
			/// <param name="type">遷移タイプ</param>
			/// <param name="priority">優先度</param>
			TransitionCandidateParametor(const EnumType type, const int priority)
				:type(type), priority(priority)
			{}
		};

	public:
		//外部からEnumを使う時用にEnumTypeをpublicにする。
		using StateEnum = EnumType;

	private:
		EnumType m_currentNodeType = EnumType(0);  //現在のノードタイプ

		TransitionStructMember m_transitionStruct; //遷移条件に利用する構造体

		std::vector<TransitionCandidateParametor> m_transitionCandidates;  //遷移先候補パラメータ群

		/// <summary>
		/// ステートの変更
		/// </summary>
		/// <param name="type">変更したいステートタイプ</param>
		void ChangeState(const EnumType type) {
			std::shared_ptr<NodeType> nowNode = GetNowNode();
			if (nowNode) {
				nowNode->OnExit();
			}

			m_currentNodeType = type;
			std::shared_ptr<NodeType> nextNode = GetNode(m_currentNodeType);
			if (nextNode) {
				nextNode->OnStart();
			}
		}

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		EnemyMainStateMachine() 
			:EnemyMainStateMachine(TransitionStructMember())
		{}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="member">遷移条件用メンバ</param>
		EnemyMainStateMachine(const TransitionStructMember& member)
			:GraphBase(), m_transitionStruct(member)
		{}

		/// <summary>
		/// ノードの追加
		/// </summary>
		/// <param name="type">ノードのEnumType</param>
		/// <param name="node">追加するノード</param>
		void AddNode(const EnumType type, const std::shared_ptr<NodeType>& node) override {
			if (IsEmpty()) {
				m_currentNodeType = type;
				if (node) {
					node->OnStart();
				}
			}

			GraphBase::AddNode(type, node);
		}

		/// <summary>
		/// 現在使用中のノードEnumTypeを取得
		/// </summary>
		/// <returns>ノードのEnumType</returns>
		EnumType GetCurrentNodeType() const noexcept {
			return m_currentNodeType;
		}

		/// <summary>
		/// 現在使用中のノードタイプが引数のタイプと合っているかどうか
		/// </summary>
		/// <param name="type">確認したいノードタイプ</param>
		/// <returns>同じならtrue</returns>
		bool IsCurrentNodeType(const EnumType& type) const {
			return GetCurrentNodeType() == type;
		}

		/// <summary>
		/// 現在使用中のノードを取得
		/// </summary>
		/// <returns>ノードの取得</returns>
		std::shared_ptr<NodeType> GetNowNode() const {
			return GetNode(m_currentNodeType);
		}

		/// <summary>
		/// 現在使用中のノードに設定しているエッジを取得
		/// </summary>
		/// <returns>エッジの取得</returns>
		EdgeVector GetNowNodeEdges() const {
			return GetEdges(m_currentNodeType);
		}

		/// <summary>
		/// 遷移に利用する構造体を取得する。
		/// </summary>
		/// <returns>構造体の参照を渡す</returns>
		TransitionStructMember& GetTransitionStructMember() {
			return m_transitionStruct;
		}

		/// <summary>
		/// ステートの切り替え
		/// </summary>
		/// <param name="type">切り替えたいステート</param>
		/// <param name="priority">優先度</param>
		void ChangeState(const EnumType& type, const int& priority) {
			m_transitionCandidates.push_back(TransitionCandidateParametor(type, priority));
		}

		/// <summary>
		/// 強制的なステートの切り替え。
		/// </summary>
		/// <param name="type">強制的なステートの切り替え</param>
		void ForceChangeState(const EnumType& type) {
			ChangeState(type);
		}

		/// <summary>
		/// 更新処理
		/// </summary>
		void OnUpdate() {
			if (IsEmpty()) {
				return;
			}

			//ノードのUpdate
			bool isEndNodeUpdate = NodeUpdate();

			//エッジの切替判断
			TransitionCheck(isEndNodeUpdate);

			//トリガーのリセット
			TriggerReset();

			//遷移
			Transition();

			//遷移先候補のクリア
			m_transitionCandidates.clear();
		}

	private:
		//プライベート関数----------------------------------------------------------------------------

		/// <summary>
		/// ノードのアップデート
		/// </summary>
		/// <returns>ノードが終了しているかどうか</returns>
		bool NodeUpdate() {
			auto node = GetNowNode();
			if (node) {
				return node->OnUpdate();
			}

			return false;
		}

		/// <summary>
		/// 遷移チェック
		/// </summary>
		void TransitionCheck(const bool isEndNodeUpdate) {
			auto edges = GetNowNodeEdges();
			for (auto& edge : edges) {
				if (edge->IsTransition(m_transitionStruct, isEndNodeUpdate)) {
					const auto type = edge->GetToType();
					const auto priority = edge->GetPriority();
					m_transitionCandidates.push_back(TransitionCandidateParametor(type, priority));
				}
			}
		}

		/// <summary>
		/// トリガーのリセット
		/// </summary>
		void TriggerReset() {
			EdgeVectorMap edgesMap = GetEdgesMap();
			for (std::pair<EnumType, EdgeVector> edgePair : edgesMap) {
				for (auto& edge : edgePair.second) {
					edge->IsTransition(m_transitionStruct);
				}
			}
		}

		/// <summary>
		/// 遷移処理
		/// </summary>
		void Transition() {
			if (m_transitionCandidates.size() == 0) {  //遷移先候補が0なら処理を飛ばす。
				return;
			}

			//遷移先候補のソート
			std::sort(m_transitionCandidates.begin(), m_transitionCandidates.end(),
				[](const TransitionCandidateParametor& left, const TransitionCandidateParametor& right) 
				{ return left.priority > right.priority ? true : false; }); //優先度が高い順から遷移する。
			
			ChangeState(m_transitionCandidates[0].type);
		}
	};

}