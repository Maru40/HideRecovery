
/*!
@file AstarEdge.cpp
@brief AstarEdge�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "AstarEdge_Ex.h"
#include "AstarNode.h"

#include "Maruyama/DebugClass/Debug_DrawController.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// AstarEdge�{��
		//--------------------------------------------------------------------------------------

		AstarEdge::AstarEdge():
			AstarEdge(nullptr, nullptr)
		{}

		AstarEdge::AstarEdge(
			const std::shared_ptr<AstarNode>& fromNode,
			const std::shared_ptr<AstarNode>& toNode
		):
			EdgeBase(fromNode, toNode)
		{}

		std::shared_ptr<AstarNode> AstarEdge::GetFromAstarNode() { return std::dynamic_pointer_cast<AstarNode>(EdgeBase::GetFromNode()); }

		std::shared_ptr<AstarNode> AstarEdge::GetToAstarNode() { return std::dynamic_pointer_cast<AstarNode>(EdgeBase::GetToNode()); }

		//--------------------------------------------------------------------------------------
		/// �f�o�b�O
		//--------------------------------------------------------------------------------------

		void AstarEdge::OnDebugDraw() {
			if (m_debugDrawController == nullptr) {
				const std::wstring Texture = L"Arrow_Detph_TX";
				constexpr float Width = 1.0f;
				constexpr float Depth = 1.0f;
				auto data = Debug_DrawData(maru::Rect(Vec3(0.0f), Width, Depth), Texture);
				m_debugDrawController = std::make_shared<Debug_DrawController>(data);
			}

			auto toNodeVec = GetToAstarNode()->GetPosition() - GetFromAstarNode()->GetPosition();
			auto position = (GetFromAstarNode()->GetPosition() + GetToAstarNode()->GetPosition()) / 2.0f;	//���ꂼ��̃m�[�h�̒��Ԓn�_���擾

			//�X�P�[���ݒ�
			constexpr float ScaleWidth = 1.0f;
			constexpr float ScaleUp = 1.0f;
			constexpr float ScaleAdjust = 0.95f;
			const float Length = toNodeVec.length();
			const Vec3 Scale = Vec3(ScaleWidth, ScaleUp, Length * ScaleAdjust);

			//�\��
			m_debugDrawController->OnDraw(position, toNodeVec.GetNormalized(), Scale);
		}
	}

}