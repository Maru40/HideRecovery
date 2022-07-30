/*!
@file ClearUIObject.h
@brief ClearUIObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class SpriteObject;

	namespace Builder {
		struct VertexPCTParametor;
	}

	//--------------------------------------------------------------------------------------
	///	�N���AUI�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class ClearUIObject : public GameObject
	{
	private:
		ex_weak_ptr<SpriteObject> m_back;					 //back��UI
		ex_weak_ptr<SpriteObject> m_clear;					 //clearUI

		std::vector<ex_weak_ptr<SpriteObject>> m_allObjects; //�Ǘ�����S�Ă�UI�I�u�W�F�N�g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		ClearUIObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

	private:
		/// <summary>
		/// �o�b�NUI�̐���
		/// </summary>
		void CreateBackUI();

		/// <summary>
		/// �N���AUI�̐���
		/// </summary>
		void CreateClearUI();

		/// <summary>
		/// UI�̐���
		/// </summary>
		/// <param name="vertexParam">UI�����p�p�����[�^</param>
		/// <param name="position">�����ʒu</param>
		/// <returns>��������UI</returns>
		std::shared_ptr<SpriteObject> CreateUI(const Builder::VertexPCTParametor& vertexParam, const Vec3& position);

	public:
		/// <summary>
		/// �o�b�NUI�̎擾
		/// </summary>
		/// <returns>�o�b�NUI</returns>
		std::shared_ptr<SpriteObject> GetBackUI() const;

		/// <summary>
		/// �N���AUI�̎擾
		/// </summary>
		/// <returns>�N���AUI</returns>
		std::shared_ptr<SpriteObject> GetClearUI() const;

	};

}