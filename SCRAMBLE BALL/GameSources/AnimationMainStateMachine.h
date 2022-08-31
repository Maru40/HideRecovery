
/*!
@file AnimationMainStateMachine.h
@brief AnimationMainStateMachineなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "MaruAnimationHelper.h"
#include "GraphBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	template<class node_type, class EnumType, class TransitionStructMember>
	class GraphBase;

	template<class EnumType, class TransitionStructMember>
	class StateEdgeBase;

	template<class NodeType>
	class StateNodeBase;

	namespace maru {

		class AnimationClip;
		class I_Motion;

		template<class node_type, class EnumType, class TransitionStructMember>
		class AnimationMainStateMachine;

		//--------------------------------------------------------------------------------------
		///	アニメーション用ステートマシン
		//--------------------------------------------------------------------------------------
		template<class node_type, class EnumType, class TransitionStructMember>
		class AnimationMainStateMachine : public GraphBase<I_AnimationStateNode, EnumType, TransitionStructMember>
		{
			/// <summary>
			/// 遷移先候補のパラメータ
			/// </summary>
			struct TransitionCandidateParametor
			{
				EnumType type;  //ステートタイプ
				int priority;   //優先度

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="type">ステートタイプ</param>
				TransitionCandidateParametor(const EnumType type)
					:TransitionCandidateParametor(type)
				{}

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="type">ステートタイプ</param>
				/// <param name="priority">優先度</param>
				TransitionCandidateParametor(const EnumType type, const int priority)
					:type(type), priority(priority)
				{ }
			};

		public:
			//外部からEnumを使う時用にEnumTypeをpublicにする。
			using StateEnum = EnumType;

		private:
			EnumType m_currentNodeType;                                        //現在のノードタイプ

			TransitionStructMember m_transitionStruct;                         //遷移条件に利用する構造体

			std::vector<TransitionCandidateParametor> m_transitionCandidates;  //遷移先候補パラメータ群

			/// <summary>
			/// ステートの変更
			/// </summary>
			/// <param name="type">変更するステートタイプ</param>
			void ChangeState(const EnumType type) {
				auto nowNode = GetNowNode();
				if (nowNode) {
					nowNode->OnExit();
				}

				m_currentNodeType = type;
				auto nextNode = GetNode(m_currentNodeType);
				if (nextNode) {
					nextNode->OnStart();
				}
			}

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			AnimationMainStateMachine()
				:AnimationMainStateMachine(TransitionStructMember())
			{}

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="member">遷移条件メンバー</param>
			AnimationMainStateMachine(const TransitionStructMember& member)
				:GraphBase(), m_transitionStruct(member), m_currentNodeType(EnumType(0))
			{}

			/// <summary>
			/// ノードの追加
			/// </summary>
			/// <param name="type">追加したいノードタイプ</param>
			/// <param name="node">追加したいノード</param>
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
			/// 現在のステートを取得
			/// </summary>
			/// <returns>現在のステート</returns>
			EnumType GetCurrentState() const {
				return m_currentNodeType;
			}

			/// <summary>
			/// 指定したステートかどうかの判断
			/// </summary>
			/// <param name="state">確認したいステート</param>
			/// <returns>指定したステートならtrue</returns>
			bool IsCurrentState(const EnumType& state) const {
				return m_currentNodeType == state;
			}

			/// <summary>
			/// 現在使っているノードの取得
			/// </summary>
			/// <returns>現在使っているノード</returns>
			std::shared_ptr<NodeType> GetNowNode() const {
				return GetNode(m_currentNodeType);
			}

			/// <summary>
			/// 現在使っているエッジの取得
			/// </summary>
			/// <returns>現在使っているエッジ</returns>
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
			void ChangeState(const EnumType& type, const int& priority)
			{
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

			/// <summary>
			/// ステートのリセット
			/// </summary>
			void ResetState() {
				ForceChangeState(EnumType(0));
			}

		private:

			/// <summary>
			/// ノードの更新処理
			/// </summary>
			/// <returns>更新中ならtrue</returns>
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
			/// <param name="isEndNodeUpdate">更新処理の終了を確認するかどうか</param>
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
			/// 遷移
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
}

//endbasecross