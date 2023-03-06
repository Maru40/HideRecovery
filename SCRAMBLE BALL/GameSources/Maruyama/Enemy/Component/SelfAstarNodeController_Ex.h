
/*!
@file SelfAstarNodeController_Ex.h
@brief SelfAstarNodeController_Ex�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Thread/ThreadPool.h"

namespace basecross {

	namespace maru {
		//--------------------------------------------------------------------------------------
		///	�O���錾
		//--------------------------------------------------------------------------------------
		class AstarNode;
	}

	//--------------------------------------------------------------------------------------
	///	�������g��AstarNode�������˗����o���C���^�[�t�F�[�X
	//--------------------------------------------------------------------------------------
	class I_SelfAstarNodeRequester : public I_ThreadRequester {
	public:
		virtual ~I_SelfAstarNodeRequester() = default;

		/// <summary>
		/// �������g�̃m�[�h����������B
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<maru::AstarNode> Search_SelfAstarNode() = 0;
	};

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// �������g����������m�[�h�̍X�V
		//--------------------------------------------------------------------------------------
		class SelfAstarNodeController_Ex : public Component, public I_SelfAstarNodeRequester
		{
			std::weak_ptr<AstarNode> m_currentNode;				//���݂̃m�[�h

			std::future<std::shared_ptr<AstarNode>> m_future;	//�t���[�`���[�f�[�^

		public:
			SelfAstarNodeController_Ex(const std::shared_ptr<GameObject>& owner);

			virtual ~SelfAstarNodeController_Ex() = default;

			void OnUpdate() override;

		private:

		public:
			//�����̈ʒu�m�[�h����������	
			std::shared_ptr<AstarNode> Search_SelfAstarNode() override;

			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// ���݂̃m�[�h�����݂��邩�ǂ����𔻒f����B
			/// </summary>
			/// <returns>���݂̃m�[�h</returns>
			bool HasCurrentNode() const { return !m_currentNode.expired(); }

			/// <summary>
			/// ���݂̃m�[�h���擾����B
			/// </summary>
			/// <returns>���݂̃m�[�h</returns>
			std::shared_ptr<maru::AstarNode> GetCurrentNode() const;

		private:
			void SetCurrentNode(const std::shared_ptr<AstarNode>& node);

		};
	}
}