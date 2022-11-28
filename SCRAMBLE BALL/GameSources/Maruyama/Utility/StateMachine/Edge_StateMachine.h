
/*!
@file Edge_StateMachine.h
@brief Edge_StateMachine
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "EdgeBase.h"

namespace basecross {

	namespace maru {
		//--------------------------------------------------------------------------------------
		/// 前方宣言
		//--------------------------------------------------------------------------------------
		template<class OnwerType>
		class NodeBase_StateMachine;

		class I_StateNode;

		class NodeBase;

		//--------------------------------------------------------------------------------------
		/// ステートマシン用のエッジクラス
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ステートマシン用のエッジクラス
		/// </summary>
		/// <typeparam name="EnumType">使用する列挙体</typeparam>
		/// <typeparam name="TransitionStructMember">遷移条件用の構造体メンバー</typeparam>
		template<class EnumType, class TransitionStructMember>
		class Edge_StateMachine : public EdgeBase
		{
		public:
			using IsTransitionFunction = std::function<bool(const TransitionStructMember& member)>;

		private:
			IsTransitionFunction m_isTransitionFunc = nullptr; //遷移する条件

			int m_priority = 0;             //優先度
			bool m_isEndTransition = false; //終了時に遷移するかどうか

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="from">手前のインデックス</param>
			/// <param name="to">先のインデックス</param>
			Edge_StateMachine(
				const std::shared_ptr<NodeBase>& fromNode,
				const std::shared_ptr<NodeBase>& toNode
			) :
				StateEdgeBase(fromNode, toNode, nullptr)
			{}

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="from">手前のインデックス</param>
			/// <param name="to">先のインデックス</param>
			/// <param name="isTransitionFunc">遷移条件関数</param>
			Edge_StateMachine(
				const std::shared_ptr<NodeBase>& fromNode,
				const std::shared_ptr<NodeBase>& toNode,
				const IsTransitionFunction& isTransitionFunc
			) :
				StateEdgeBase(fromNode, toNode, isTransitionFunc, static_cast<int>(to))
			{}

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="from">手前のインデックス</param>
			/// <param name="to">先のインデックス</param>
			/// <param name="isTransitionFunc">遷移条件関数</param>
			/// <param name="priority">優先度</param>
			Edge_StateMachine(
				const std::shared_ptr<NodeBase>& fromNode,
				const std::shared_ptr<NodeBase>& toNode,
				const IsTransitionFunction& isTransitionFunc,
				const int priority
			) :
				Edge_StateMachine(fromNode, toNode, isTransitionFunc, toNode->GetIndex(), false)
			{}

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="from">手前のインデックス</param>
			/// <param name="to">先のインデックス</param>
			/// <param name="isTransitionFunc">遷移条件関数</param>
			/// <param name="priority">優先度</param>
			/// <param name="isEndTransition">更新処理終了時に判断するかどうか</param>
			Edge_StateMachine(
				const std::shared_ptr<NodeBase>& fromNode,
				const std::shared_ptr<NodeBase>& toNode,
				const IsTransitionFunction& isTransitionFunc,
				const int priority,
				const bool isEndTransition
			) :
				EdgeBase(fromNode, toNode),
				m_isTransitionFunc(isTransitionFunc),
				m_priority(priority),
				m_isEndTransition(isEndTransition)
			{}

			virtual ~Edge_StateMachine() = default;

			//--------------------------------------------------------------------------------------
			///	アクセッサ
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// Toに遷移する条件を設定する。
			/// </summary>
			/// <param name="func">設定する条件</param>
			void SetIsToTransition(const std::function<bool(const TransitionStructMember& member)>& func) {
				m_isTransitionFunc = func;
			}

			/// <summary>
			/// 遷移できるかどうか
			/// </summary>
			/// <param name="member">遷移条件用メンバー</param>
			/// <param name="isEndNodeUpdate">更新終了時に判断するかどうか</param>
			/// <returns></returns>
			bool IsTransition(const TransitionStructMember& member, const bool isEndNodeUpdate = false) {
				//終了時遷移なら
				if (m_isEndTransition) {
					//ノードが終了しているなら監視、そうでないならfalse
					return isEndNodeUpdate ? m_isTransitionFunc(member) : false;
				}

				//終了時遷移でないなら常に監視
				return m_isTransitionFunc(member);
			}

			/// <summary>
			/// 優先度の設定
			/// </summary>
			/// <param name="priority">優先度</param>
			void SetPriority(const int priority) noexcept {
				m_priority = priority;
			}

			/// <summary>
			/// 優先度の取得
			/// </summary>
			/// <returns>優先度</returns>
			int GetPriority() const noexcept {
				return m_priority;
			}

			_NODISCARD EnumType GetFromType() const noexcept {
				return static_cast<EnumType>(EdgeBase::GetFromIndex());
			}

			_NODISCARD EnumType GetToType() const noexcept {
				return static_cast<EnumType>(EdgeBase::GetToIndex());
			}

			/// <summary>
			/// 更新終了時に遷移判断するかどうかを設定
			/// </summary>
			/// <param name="isEndTransition">更新終了時に遷移判断するかどうか</param>
			void SetIsEndTransition(const bool isEndTransition) noexcept {
				m_isEndTransition = true;
			}

			/// <summary>
			/// 更新終了時に遷移判断するかどうかを取得
			/// </summary>
			/// <returns>更新終了時に遷移判断するならtrue</returns>
			bool IsEndTransition() const noexcept {
				return m_isEndTransition;
			}
		};

	}
}