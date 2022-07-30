/*!
@file MetaballChildrenRender.h
@brief MetaballChildrenRender�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Metaball {

		//--------------------------------------------------------------------------------------
		///	�O���錾
		//--------------------------------------------------------------------------------------
		class ChildrenRender;

		//--------------------------------------------------------------------------------------
		///	���^�{�[���̎q���̕`��p�����[�^
		//--------------------------------------------------------------------------------------
		struct ChildrenRender_Parametor
		{
			float radius;                   //���a
			Col4 color;                     //�F
			ex_weak_ptr<GameObject> parent; //�e�I�u�W�F�N�g
			bool isDrawActive;              //�\�����邩�ǂ���

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			ChildrenRender_Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="radius">���a</param>
			/// <param name="color">�F</param>
			ChildrenRender_Parametor(const float& radius, const Col4& color);
		};

		//--------------------------------------------------------------------------------------
		///	���^�{�[���̎q���̐����N���X
		//--------------------------------------------------------------------------------------
		class Factory_Children 
		{
		private:
			std::weak_ptr<Stage> m_stage;	//��������������X�e�[�W

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="stage">���̃N���X����������X�e�[�W</param>
			Factory_Children(std::shared_ptr<Stage>& stage);

			/// <summary>
			/// ����
			/// </summary>
			/// <param name="parametor">�����p�p�����[�^</param>
			/// <returns>���������I�u�W�F�N�g</returns>
			std::shared_ptr<ChildrenRender> Create(const ChildrenRender_Parametor& parametor);

			/// <summary>
			/// ��������
			/// </summary>
			/// <param name="params">�����p�p�����[�^�z��</param>
			/// <returns>���������I�u�W�F�N�g�̔z��</returns>
			std::vector<std::shared_ptr<ChildrenRender>> Creates(const std::vector<ChildrenRender_Parametor>& params);

			/// <summary>
			/// �X�e�[�W�̎擾
			/// </summary>
			/// <returns>�X�e�[�W</returns>
			std::shared_ptr<Stage> GetStage() const;
		};

		//--------------------------------------------------------------------------------------
		///	���^�{�[���̎q���̕`��
		//--------------------------------------------------------------------------------------
		class ChildrenRender : public Component
		{
		public:
			using Parametor = ChildrenRender_Parametor;

		private:
			Parametor m_param;							//�p�����[�^

			std::function<void()> m_scaleFadeFunction;	//�傫���t�F�[�h�C�x���g

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			ChildrenRender(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="parametor">�p�����[�^</param>
			ChildrenRender(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

			void OnUpdate() override;

		private:
			/// <summary>
			/// �t�F�[�h�X�V(�������Ȃ��ď�����)
			/// </summary>
			/// <param name="speed">�t�F�[�h�X�s�[�h</param>
			void ScaleFadeUpdate(const float& speed);

		public:
			/// <summary>
			/// �t�F�[�h�X�^�[�g(�������Ȃ��ď�����)
			/// </summary>
			/// <param name="speed">�t�F�[�h�X�s�[�h</param>
			void StartScaleFade(const float speed);

			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �p�����[�^�̐ݒ�
			/// </summary>
			/// <param name="parametor">�p�����[�^</param>
			void SetParametor(const Parametor& parametor) noexcept {
				m_param = parametor;
			}
			
			/// <summary>
			/// �p�����[�^�̎擾
			/// </summary>
			/// <returns>�p�����[�^</returns>
			Parametor GetParametor() const noexcept {
				return m_param;
			}

			/// <summary>
			/// ���a�̐ݒ�
			/// </summary>
			/// <param name="radius">���a</param>
			void SetRadius(const float& radius) noexcept {
				m_param.radius = radius;
			}

			/// <summary>
			/// ���a�̎擾
			/// </summary>
			/// <returns>���a</returns>
			float GetRadius() const {
				if (m_param.parent) {
					auto scale = m_param.parent->GetComponent<Transform>()->GetScale();
					return m_param.radius * scale.x;	
				}

				return m_param.radius;
			}

			/// <summary>
			/// ���[�J�����a�̎擾
			/// </summary>
			/// <returns>���[�J�����a</returns>
			float GetLocalRadius() const noexcept {
				return m_param.radius;
			}

			/// <summary>
			/// �F�̐ݒ�
			/// </summary>
			/// <param name="color">�F</param>
			void SetColor(const Col4& color) noexcept {
				m_param.color = color;
			}

			/// <summary>
			/// �F�̎擾
			/// </summary>
			/// <returns>�F</returns>
			Col4 GetColor() const noexcept {
				return m_param.color;
			}

			/// <summary>
			/// �ʒu�̎擾
			/// </summary>
			/// <returns>�ʒu</returns>
			Vec3 GetPosition() const {
				return transform->GetPosition();
			}

			/// <summary>
			/// �X�t�B�A�f�[�^�̎擾
			/// </summary>
			/// <returns>�X�t�B�A�f�[�^</returns>
			Vec4 GetSphereData() const {
				auto center = GetPosition();
				return Vec4(center.x, center.y, center.z, GetRadius());
			}

			/// <summary>
			/// �e�̐ݒ�
			/// </summary>
			/// <param name="parent">�e</param>
			void SetParent(const std::shared_ptr<GameObject>& parent) noexcept {
				m_param.parent = parent;
			}

			/// <summary>
			/// �e�̎擾
			/// </summary>
			/// <returns>�e</returns>
			std::shared_ptr<GameObject> GetParent() const {
				return m_param.parent.GetShard();
			}

			/// <summary>
			/// �`�悵�Ă��邩�ǂ���
			/// </summary>
			/// <returns>�`�悵�Ă���Ȃ�true</returns>
			bool IsDrawActive() const noexcept {
				return m_param.isDrawActive;
			}

			/// <summary>
			/// �`�悷�邩�ǂ���
			/// </summary>
			/// <param name="isDraw">�`�悷�邩�ǂ���</param>
			void SetDrawActive(const bool isDraw) noexcept {
				m_param.isDrawActive = isDraw;
			}

		};

	}
}