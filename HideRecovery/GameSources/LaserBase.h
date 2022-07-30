/*!
@file LaserBase.h
@brief LaserBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	enum class ScaleChangeType {
		Normal,
		Reverse,
	};

	//--------------------------------------------------------------------------------------
	/// ���[�U�[�̕`��p�����[�^
	//--------------------------------------------------------------------------------------
	struct LaserBaseDraw_Parametor
	{
		float width = 0.25f;		 //����
		float maxWidth = 3.0f;		 //�ő�̉���
		float length = 1.0f;		 //����
		float maxLength = 5.0f;		 //�ő�̒���
									 
		float longerSpeed = 1.0f;	 //�L�т�X�s�[�h
		float smallerSpeed = 1.0f;	 //�k�܂�X�s�[�h
									 
		Vec3 forward;				 //���ʕ���
		Col4 color;					 //�F
		wstring texture;			 //�e�N�X�`����
		ScaleChangeType longerType;  //�L�т�^�C�v
		ScaleChangeType smallerType; //�k�ރ^�C�v
		bool isSmaller = false;	     //�k�܂��Ă����Ԃ��ǂ���

		/// <summary>
		///	�R���X�g���N�^
		/// </summary>
		LaserBaseDraw_Parametor();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="width">����</param>
		/// <param name="maxWidth">�ő�̉���</param>
		/// <param name="length">����</param>
		/// <param name="maxLength">�ő�̒���</param>
		/// <param name="longerSpeed">�L�т�X�s�[�h</param>
		/// <param name="smallerSpeed">�k�܂�X�s�[�h</param>
		/// <param name="forward">���ʕ���</param>
		/// <param name="color">�F</param>
		/// <param name="texture">�e�N�X�`����</param>
		LaserBaseDraw_Parametor(
			const float& width, const float& maxWidth, 
			const float& length, const float& maxLength,
			const float& longerSpeed, const float& smallerSpeed,
			const Vec3& forward, const Col4& color,
			const wstring& texture);
	};

	class LaserBaseDraw : public Component
	{
	public:
		using Parametor = LaserBaseDraw_Parametor;
		using DrawComp = BcPCTStaticDraw;

	private:
		Parametor m_param;                                  //�p�����[�^

		std::vector<VertexPositionColorTexture> m_vertices; //���_�f�[�^�z��

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		LaserBaseDraw(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="param">�p�����[�^</param>
		LaserBaseDraw(const std::shared_ptr<GameObject>& objPtr, const Parametor& param);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// �L�т�X�V����
		/// </summary>
		void LongerUpdate();

		/// <summary>
		/// �k�ލX�V����
		/// </summary>
		void SmallerUpdate();

		/// <summary>
		/// ���_�̍X�V
		/// </summary>
		void VerticesUpdate();

		/// <summary>
		/// �X�V�����I�����ɌĂԏ���
		/// </summary>
		void EndProcess();

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �k��ł����Ԃ��ǂ���
		/// </summary>
		/// <returns>�k��ł���Ȃ�true</returns>
		bool IsSmaller() const {
			return m_param.isSmaller;
		}

		/// <summary>
		/// �k��ł����Ԃ̐ݒ�
		/// </summary>
		/// <param name="isSmaller">�k��ł����Ԃ��ǂ���</param>
		void SetIsSmaller(const bool isSmaller) {
			m_param.isSmaller = isSmaller;
			if (isSmaller) {
				transform->SetPosition(transform->GetPosition() + (GetLongerForward() * GetLength()));
			}
		}

		/// <summary>
		/// ���ʕ����̎擾
		/// </summary>
		/// <returns>���ʕ���</returns>
		Vec3 GetForward() const {
			return m_param.forward;
		}

		/// <summary>
		/// ���ʕ����̐ݒ�
		/// </summary>
		/// <param name="forward">���ʕ���</param>
		void SetForward(const Vec3& forward) noexcept {
			m_param.forward = forward;
		}

		/// <summary>
		/// �L�т鐳�ʕ���
		/// </summary>
		/// <returns>�L�т鐳�ʕ���</returns>
		Vec3 GetLongerForward() const {
			ScaleChangeType type = m_param.isSmaller ? m_param.smallerType : m_param.longerType;

			switch (type)
			{
			case ScaleChangeType::Normal:
				return m_param.forward;
				break;

			case ScaleChangeType::Reverse:
				return -m_param.forward;
				break;
			}

			return m_param.forward;
		}

		/// <summary>
		/// �����̎擾
		/// </summary>
		/// <returns>����</returns>
		float GetLength() const {
			return m_param.length;
		}

		/// <summary>
		/// �����̎擾
		/// </summary>
		/// <returns>����</returns>
		float GetWidth() const {
			return m_param.width;
		}

		/// <summary>
		/// �F�̎擾
		/// </summary>
		/// <returns>�F</returns>
		Col4 GetColor() const {
		 	return m_param.color;
		}

		/// <summary>
		/// �F�̐ݒ�
		/// </summary>
		/// <param name="color">�F</param>
		void SetColor(const Col4& color) {
			m_param.color = color;
		}

		/// <summary>
		/// �L�т�^�C�v�̎擾
		/// </summary>
		/// <returns>�L�т�^�C�v</returns>
		ScaleChangeType GetLongerType() const {
			return m_param.longerType;
		}

		/// <summary>
		/// �L�т�^�C�v�̐ݒ�
		/// </summary>
		/// <param name="type">�L�т�^�C�v</param>
		void SetLongerType(const ScaleChangeType& type) {
			m_param.longerType = type;
		}

		/// <summary>
		/// �e�N�X�`���̐ݒ�
		/// </summary>
		/// <param name="texture">�e�N�X�`����</param>
		void SetTexture(const wstring& texture) {
			GetGameObject()->GetComponent<DrawComp>()->SetTextureResource(texture);
		}
	};

}