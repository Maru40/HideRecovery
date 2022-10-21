/*!
@file NodeBase.h
@brief NodeBase
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// ノードの基底クラス(最低限の機能を持たせる。)i
		//--------------------------------------------------------------------------------------
		class NodeBase
		{
			bool m_isActive;	//アクティブ状態
			int m_index;		//インデックス

		public:
			NodeBase();

			NodeBase(const int index);

			virtual ~NodeBase() = default;

		public:
			//--------------------------------------------------------------------------------------
			/// アクセッサ
			//--------------------------------------------------------------------------------------

			void SetIsActive(const bool isActive) noexcept { m_isActive = isActive; }

			bool IsActive() const noexcept { return m_isActive; }

			bool GetIsActive() const noexcept { return IsActive(); }

			void SetIndex(const int index) noexcept { m_index = index; }

			int GetIndex() const noexcept { return m_index; }

			/// <summary>
			/// タイプの設定
			/// </summary>
			template<class EnumType>
			void SetType(const EnumType type) { m_index = static_cast<int>(type); }

			/// <summary>
			/// 何タイプかを取得する
			/// </summary>
			template<class EnumType>
			EnumType GetType() const { return static_cast<EnumType>(m_index); }

		};

	}
}