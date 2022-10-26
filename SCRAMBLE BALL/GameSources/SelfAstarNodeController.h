
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

		/// <summary>
		/// �m�[�h�̍X�V
		/// </summary>
		void UpdateNode();

		/// <summary>
		/// �m�[�h�̍X�V���K�v���ǂ���
		/// </summary>
		/// <returns></returns>
		bool IsUpdateNode();

		/// <summary>
		/// �m�[�h�̏�����
		/// </summary>
		void InitializeNode();

		/// <summary>
		/// �����m�[�h���������邩�ǂ���
		/// </summary>
		/// <returns>��������Ȃ�true</returns>
		bool IsFarNode();

	public:
		std::shared_ptr<NavGraphNode> GetNode() const noexcept;

		void SetNode(const std::shared_ptr<NavGraphNode>& node) noexcept;

		bool HasNode() const noexcept;

		_NODISCARD bool IsInitializeNode() const noexcept { return m_isNodeInitialize; };

	};

}