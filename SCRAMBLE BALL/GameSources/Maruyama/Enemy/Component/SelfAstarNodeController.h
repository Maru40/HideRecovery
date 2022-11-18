
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
		bool m_isNodeInitialize;	//�����I�ɂ͂Ȃ��������B

		bool m_isMover;				//�����҂Ȃ�X�V��������B

		std::mutex m_mtx;			//�~���[�e�b�N�X

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

		//���߂̃G���A�ɂȂ������ꍇ�ɍċN���ăm�[�h����������B
		std::shared_ptr<NavGraphNode> SearchNode(const int firstAreaIndex, const int currentAreaIndex, const int direction = -1);

		/// <summary>
		/// �����m�[�h���������邩�ǂ���
		/// </summary>
		/// <returns>��������Ȃ�true</returns>
		bool IsFarNode();

	public:
		/// <summary>
		/// �蓮�Ō��݃m�[�h���X�V���������ɗ��p����B
		/// </summary>
		/// <returns>���݂̃m�[�h</returns>
		std::shared_ptr<NavGraphNode> CalculateNode();

		void SetNode(const std::shared_ptr<NavGraphNode>& node) noexcept;

		std::shared_ptr<NavGraphNode> GetNode() const noexcept;

		bool HasNode() const noexcept;

		_NODISCARD bool IsInitializeNode() const noexcept { return m_isNodeInitialize; };

		void SetIsMover(const bool isMover) noexcept { m_isMover = isMover; }

		_NODISCARD bool IsMover() const noexcept { return m_isMover; }

	};

}