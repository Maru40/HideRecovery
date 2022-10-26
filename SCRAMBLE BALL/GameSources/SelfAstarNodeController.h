
/*!
@file SelfAstarNodeController.h
@brief SelfAstarNodeController�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class NavGraphNode;

	//--------------------------------------------------------------------------------------
	///	���������݂ǂ̃m�[�h�ɏ������邩���L�^����B
	//--------------------------------------------------------------------------------------
	class SelfAstarNodeController : public Component
	{
		bool m_isNodeInitialize;

		std::weak_ptr<NavGraphNode> m_node;

	public:
		SelfAstarNodeController(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnLateStart() override;
		void OnUpdate() override;

	private:
		void UpdateProccess();

		void UpdateNode();

		bool IsUpdateNode();

		void InitializeNode();

		bool IsFarNode();

	public:
		std::shared_ptr<NavGraphNode> GetNode() const noexcept;

		void SetNode(const std::shared_ptr<NavGraphNode>& node) noexcept;

		bool HasNode() const noexcept;

		_NODISCARD bool IsInitializeNode() const noexcept { return m_isNodeInitialize; };

	};

}