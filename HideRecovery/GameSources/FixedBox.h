/*!
@file FixedBox.h
@brief FixedBox�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "StageObjectBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�Œ�{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedBox : public StageObjectBase
	{
	protected:
		vector<VertexPositionNormalTexture> m_vertices;  //���_�z��
		vector<uint16_t> m_indices;                      //�C���f�B�Z�X

		Vec3 m_meshCreateScale = Vec3(0.0f);			 //���b�V�������X�P�[��
		
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		/// <param name="name">�I�u�W�F�N�g��</param>
		/// <param name="texture">�e�N�X�`����</param>
		FixedBox(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& texture)
			:StageObjectBase(stage, name, texture)
		{}

		void OnCreate() override;
		void OnUpdate() override;

		/// <summary>
		/// ���b�V���̍X�V
		/// </summary>
		void UpdateMesh();
	};

}

//endbasecross