/*!
@file TargetMarker.h
@brief TargetMarker�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �^�[�Q�b�g�}�[�J�[
	//--------------------------------------------------------------------------------------
	class TargetMarker : public Component
	{
	public:
		/// <summary>
		/// �p�����[�^
		/// </summary>
		struct Parametor;

	private:
		std::unique_ptr<Parametor> m_param; //�p�����[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		TargetMarker(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

	private:
		/// <summary>
		/// �T�[�N���I�u�W�F�N�g�̐���
		/// </summary>
		void CreateCircleObject();

		/// <summary>
		/// �Z�O�����gUI�̐���
		/// </summary>
		/// <returns>����UI</returns>
		std::shared_ptr<GameObject> AddSegmentObject();

		/// <summary>
		/// �g��Ȃ��Z�O�����g�̔�\��
		/// </summary>
		/// <param name="useNum">�g��Ȃ��Z�O�����g�̐�</param>
		void NotUseSegmentManager(const int& useNum);

	public:
		/// <summary>
		/// �g�p����}�[�J�[�̐����󂯎��
		/// </summary>
		/// <param name="useNum">�g�p����}�[�J�[�̐�</param>
		void AcceptUseSegmentMarker(const int& useNum);

		/// <summary>
		/// Success��Ԃɂ���B
		/// </summary>
		/// <param name="segment">Success��Ԃɂ���Z�O�����g</param>
		void StartSuccess(const std::shared_ptr<GameObject>& segment);

		/// <summary>
		/// Success��ԏI������
		/// </summary>
		void EndSuccess(); 

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �T�[�N���}�[�J�[���擾
		/// </summary>
		/// <returns>�T�[�N���}�[�J�[�̎擾</returns>
		std::shared_ptr<GameObject> GetCircleMarker() const;

		/// <summary>
		/// �^�[�Q�b�g�p�̃Z�O�����g�}�[�J�[���擾
		/// </summary>
		/// <returns>�^�[�Q�b�g�p�̃Z�O�����g�}�[�J�[</returns>
		std::shared_ptr<GameObject> GetTargetSegmentMarker() const;

		/// <summary>
		/// Success���̃Z�O�����g�}�[�J�[���擾
		/// </summary>
		/// <returns>Success���̃Z�O�����g�}�[�J�[��/returns>
		std::shared_ptr<GameObject> GetSuccessSegmentMarker() const;

		/// <summary>
		/// �w�肵�����̃Z�O�����g�}�[�J�[���擾
		/// </summary>
		/// <param name="getNum">�擾�������Z�O�����g�}�[�J�[��</param>
		/// <returns>�w�肵�����̃Z�O�����g�}�[�J�[��/returns>
		vector<std::shared_ptr<GameObject>> GetSegmentMarkers(const int& getNum);

		/// <summary>
		/// �S�ẴZ�O�����g�}�[�J�[���擾
		/// </summary>
		/// <returns>���ׂẴZ�O�����g�}�[�J�[��/returns>
		vector<ex_weak_ptr<GameObject>> GetAllSegmentMarkers() const;

		/// <summary>
		/// �Z�O�����g�}�[�J�[���g�p���邩�ǂ�����ݒ�
		/// </summary>
		/// <param name="isUse">�Z�O�����g�}�[�J�[���g�p���邩�ǂ���</param>
		void SetIsUseMarker(const bool isUse);

		/// <summary>
		/// ���ݎg�p���̃}�[�J�[�̐����擾
		/// </summary>
		/// <returns>���ݎg�p���̃}�[�J�[</returns>
		int GetUseNum() const;
	};

}

//endbasecross