/*!
@file SelecterSprite.h
@brief SelecterSprite�w�b�_�@
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class SelectSprite;

	//--------------------------------------------------------------------------------------
	/// �X�v���C�g�̑I���Ǘ�
	//--------------------------------------------------------------------------------------
	class SelecterSprite : public Component
	{
		vector<ex_weak_ptr<SelectSprite>> m_targets;
		int m_selectIndex;          //�I�𒆂̃C���f�b�N�X
		int m_maxIndex;             //�ő�C���f�b�N�X

		float m_speed;              //�ړ��X�s�[�h
		float m_baseSize;           //��{�T�C�Y
		float m_maxSize;            //�ő�T�C�Y
		float m_sizeElapsed = 0.0f; //�T�C�Y�̌��ݒl

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		SelecterSprite(const std::shared_ptr<GameObject>& objPtr):
			Component(objPtr),
			m_selectIndex(0),
			m_maxIndex(0),
			m_speed(2.0f),
			m_baseSize(1.0f),
			m_maxSize(0.1f)
		{}

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// �X�v���C�g�G�t�F�N�g
		/// </summary>
		void SpriteEffect();

		/// <summary>
		/// ���͂ɍ��킹�Ăǂ��I�𒆂��𔻒f����
		/// </summary>
		void InputSelect();

		/// <summary>
		/// ������͂�����B
		/// </summary>
		void InputDecision();

		/// <summary>
		/// �I��悪�ύX���ꂽ�����A���ݑI�𒆂�UI�ɓ`����B
		/// </summary>
		void EndSelect();

		/// <summary>
		/// �I����ύX����B
		/// </summary>
		void ChangeSelect();

		/// <summary>
		/// �C���f�b�N�X�͈͓̔����ǂ����𔻒f����B
		/// </summary>
		void CheckSelectIndex();

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �^�[�Q�b�g�z��̐ݒ�
		/// </summary>
		/// <param name="targets">�^�[�Q�b�g�z��</param>
		void SetTargets(const vector<ex_weak_ptr<SelectSprite>>& targets);

		/// <summary>
		/// �^�[�Q�b�g�̒ǉ�
		/// </summary>
		/// <param name="target">�^�[�Q�b�g�̒ǉ�</param>
		void AddTarget(const std::shared_ptr<SelectSprite>& target) {
			m_targets.push_back(target);
		}

		/// <summary>
		/// �I�𒆂̃C���f�b�N�X�̎擾
		/// </summary>
		/// <returns></returns>
		int GetSelectIndex() const {
			return m_selectIndex;
		}

		/// <summary>
		/// �I�𒆂̃X�v���C�g�̎擾
		/// </summary>
		/// <returns></returns>
		std::shared_ptr<SelectSprite> GetSelectObject() const {
			return m_targets[m_selectIndex].GetShard();
		}
	};

}

//endbasecross