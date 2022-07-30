/*!
@file LBRBSpriteObject.h
@brief LBRBSpriteObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class SpriteObject;
	struct SpriteData;

	//--------------------------------------------------------------------------------------
	/// LB,RB�X�v���C�g�^�C�v
	//--------------------------------------------------------------------------------------
	enum class LBRBSpriteObject_Type {
		RB,
		LB
	};

	//--------------------------------------------------------------------------------------
	/// LBRB�X�v���C�g�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class LBRBSpriteObject : public GameObject
	{
	public:
		using Type = LBRBSpriteObject_Type;

	private:
		ex_weak_ptr<SpriteObject> m_back;  //�o�b�N�X�v���C�g
		ex_weak_ptr<SpriteObject> m_main;  //���C���X�v���C�g

		std::vector<ex_weak_ptr<SpriteObject>> m_sprites;  //�S�ẴX�v���C�g�̔z��

		Type m_type = Type::LB;            //�X�v���C�g�^�C�v

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		LBRBSpriteObject(const std::shared_ptr<Stage>& stage);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		/// <param name="type">�X�v���C�g�^�C�v</param>
		LBRBSpriteObject(const std::shared_ptr<Stage>& stage, const Type& type);

		void OnCreate() override;

	private:
		/// <summary>
		/// �X�v���C�g�̕�������
		/// </summary>
		/// <param name="datas">�����p�f�[�^�z��</param>
		void CreateSprites(std::vector<SpriteData>& datas);

		/// <summary>
		/// �X�v���C�g�̃f�[�^�𕡐�����
		/// </summary>
		/// <param name="type">�X�v���C�g�^�C�v</param>
		/// <returns>���������X�v���C�g�̃f�[�^</returns>
		std::vector<SpriteData> CreateSpriteDatas(const LBRBSpriteObject::Type& type);

	};

}