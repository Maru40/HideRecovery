
/*!
@file EdgeBase.h
@brief EdgeBase
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class NodeBase;

		//--------------------------------------------------------------------------------------
		/// �G�b�W�̃C���^�[�t�F�[�X
		//--------------------------------------------------------------------------------------
		class I_Edge {
			//virtual ~I_Edge() = default;

			virtual void SetFromIndex(const int index) noexcept = 0;

			virtual int GetFromIndex() const noexcept = 0;

			template<class EnumType>
			EnumType GetFromType() const { return static_cast<EnumType>(GetFromIndex()); }

			virtual void SetToIndex(const int index) noexcept = 0;

			virtual int GetToIndex() const noexcept = 0;

			template<class EnumType>
			EnumType GetToType() const { return static_cast<EnumType>(GetToIndex()); }
		};

		//--------------------------------------------------------------------------------------
		/// �G�b�W�̊��N���X(�Œ���̋@�\����������B)i
		//--------------------------------------------------------------------------------------
		class EdgeBase : public I_Edge
		{
			std::weak_ptr<NodeBase> m_fromNode;	//��O�̃m�[�h
			std::weak_ptr<NodeBase> m_toNode;	//��̃m�[�h

		public:
			EdgeBase();
			EdgeBase(const std::shared_ptr<NodeBase>& fromNode, const std::shared_ptr<NodeBase>& toNode);

			virtual ~EdgeBase() = default;

		public:
			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// ��O�̃m�[�h�̐ݒ�
			/// </summary>
			/// <param name="node">��O�̃m�[�h</param>
			void SetFromNode(const std::shared_ptr<NodeBase>& node);

			std::shared_ptr<NodeBase> GetFromNode() const;

			void SetFromIndex(const int index) noexcept override;

			int GetFromIndex() const noexcept override;

			void SetToNode(const std::shared_ptr<NodeBase>& node);

			std::shared_ptr<NodeBase> GetToNode() const;

			void SetToIndex(const int index) noexcept override;

			int GetToIndex() const noexcept override;
		};

	}
}