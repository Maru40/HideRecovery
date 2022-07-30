/*!
@file TargetMarkerManager.h
@brief TargetMarkerManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class TargetMarker;

	//--------------------------------------------------------------------------------------
	/// �^�[�Q�b�g�}�[�J�[�Ǘ�
	//--------------------------------------------------------------------------------------
	class TargetMarkerManager : public Component
	{
	public:
		/// <summary>
		/// �^�[�Q�b�g�f�[�^
		/// </summary>
		struct TargetData;

		/// <summary>
		/// �p�����[�^
		/// </summary>
		struct Parametor;

	private:
		std::unique_ptr<TargetData> m_targetDataPtr;  //�^�[�Q�b�g�f�[�^
		std::unique_ptr<Parametor> m_paramPtr;        //�p�����[�^

		ex_weak_ptr<TargetMarker> m_marker;           //�}�[�J�[

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		TargetMarkerManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// �}�[�J�[�̍X�V
		/// </summary>
		void MarkerUpdate();

		/// <summary>
		/// �T�[�N���}�[�J�[�̍X�V
		/// </summary>
		void CircleMarkerUpdate();

		/// <summary>
		/// �^�[�Q�b�g�}�[�J�[�̍X�V
		/// </summary>
		void TargetMarkerUpdate();

		/// <summary>
		/// �Z�O�����g�}�[�J�[�̍X�V
		/// </summary>
		void SegmentMarkerUpdate();

		/// <summary>
		/// �����}�[�J�[�̍X�V
		/// </summary>
		void SuccessMarkerUpdate();

		/// <summary>
		/// �Z�O�����g�̐ݒ�
		/// </summary>
		/// <param name="segment">�Z�O�����g�̃|�C���g</param>
		/// <param name="direct">���������</param>
		/// <param name="color">�J���[</param>
		void SettingSegment(const std::shared_ptr<GameObject>& segment, Vec3 direct, const Col4& color);

		/// <summary>
		/// ������Ԃ̃Z�O�����g�̃Z�b�e�B���O������
		/// </summary>
		void SettingSuccessSegment();

		/// <summary>
		/// �ׂ�����������p�x�ɒǂ����߂����ǂ���
		/// </summary>
		bool IsSuccess() const; 

		/// <summary>
		/// ������Ԃ̕����̃Z�O�����g��������
		/// </summary>
		/// <returns>������Ԃ̃Z�O�����g</returns>
		std::shared_ptr<GameObject> FindSuccessSegment() const;

	public:

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �}�[�J�[�̃A�N�e�B�u�ݒ�
		/// </summary>
		/// <param name="isActive">�A�N�e�B�u���ǂ���</param>
		void SetMarkerActive(const bool isActive);

		/// <summary>
		/// �^�[�Q�b�g�����݂��邩�ǂ���
		/// </summary>
		/// <returns>�^�[�Q�b�g�����݂���Ȃ�true</returns>
		bool HasTarget() const;

		/// <summary>
		/// �^�[�Q�b�g�̐ݒ�
		/// </summary>
		/// <param name="target">�^�[�Q�b�g</param>
		void SetTarget(const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// �^�[�Q�b�g�f�[�^�̐ݒ�
		/// </summary>
		/// <param name="data">�^�[�Q�b�g�f�[�^</param>
		void SetTargetData(const TargetData& data);

		/// <summary>
		/// �^�[�Q�b�g�̎擾
		/// </summary>
		/// <returns>�^�[�Q�b�g</returns>
		std::shared_ptr<GameObject> GetTarget() const;
	};

}

//endbasecross