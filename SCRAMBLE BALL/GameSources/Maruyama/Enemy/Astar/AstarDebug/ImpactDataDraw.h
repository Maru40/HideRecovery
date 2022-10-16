/*!
@file ImpactDataDraw.h
@brief ImpactDataDraw�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class NavGraphNode;
	class NumbersObject;

	//--------------------------------------------------------------------------------------
	///	�e���f�[�^�\���p�N���X
	//--------------------------------------------------------------------------------------
	class ImpactDataDraw : public Component
	{
		//--------------------------------------------------------------------------------------
		///	�\���f�[�^
		//--------------------------------------------------------------------------------------
		struct DrawData {
			ex_weak_ptr<NumbersObject> dangerNumber;	//�댯�x�\���I�u�W�F�N�g
			std::weak_ptr<NumbersObject> areaIndex;		//�G���A�C���f�b�N�X�\��

			DrawData();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="numbersObject">�댯�x�\���I�u�W�F�N�g</param>
			DrawData(const std::shared_ptr<NumbersObject>& dangerNumbers, const std::shared_ptr<NumbersObject>& areaIndex);
		};

		ex_weak_ptr<NavGraphNode> m_node;	//�m�[�h�̃|�C���^
		DrawData m_drawData;				//�\���f�[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="node">�m�[�h�̃|�C���^</param>
		ImpactDataDraw(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<NavGraphNode>& node);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// �댯�x�\���I�u�W�F�N�g�̐���
		/// </summary>
		/// <returns>�댯�x�\���I�u�W�F�N�g</returns>
		std::shared_ptr<NumbersObject> CreateNumbersObject(const Vec3& offset = Vec3(0.0f), const float number = 0.0f);
	};

	//--------------------------------------------------------------------------------------
	///	�e���f�[�^�\���p�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class ImpactDataDrawObject : public GameObject
	{
		ex_weak_ptr<NavGraphNode> m_node;	//�m�[�h�̃|�C���^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		/// <param name="node">�m�[�h�̃|�C���^</param>
		ImpactDataDrawObject(const std::shared_ptr<Stage>& stage, const std::shared_ptr<NavGraphNode>& node);

		void OnCreate() override;
	};
}