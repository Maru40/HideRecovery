#pragma once
/*!
@file MetaballRenderBase.h
@brief MetaballRenderBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class BcPNTSlimeDraw;
	class RayMarchingFilterObject;

	namespace Metaball {

		class ChildrenRender;
		struct ChildrenRender_Parametor;

		//--------------------------------------------------------------------------------------
		///	���^�{�[���`��̊��N���X
		//--------------------------------------------------------------------------------------
		class RenderBase : public Component
		{
		private:
			int m_numCreate;											//�������鐔

			vector<ex_weak_ptr<ChildrenRender>> m_children;				//�Ǘ����郁�^�{�[���̔z��

			ex_weak_ptr<RayMarchingFilterObject> m_rayMarchingFilter;	//���C�}�[�`���O�p�̃t�B���^�[�𐶐�
			ex_weak_ptr<BcPNTSlimeDraw> m_draw;							//�h���\�R���|�[�l���g

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			RenderBase(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="numCreate">�������鐔</param>
			RenderBase(const std::shared_ptr<GameObject>& objPtr, const int& numCreate);

			void OnCreate() override;
			void OnDestroy() override;

		private:
			/// <summary>
			/// ���^�{�[���𐶐�
			/// </summary>
			void CreateChildren();

		public:
			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �f�B�t���[�Y�J���[�̐ݒ�
			/// </summary>
			/// <param name="color">�f�B�t���[�Y�J���[</param>
			void SetDiffuse(const Col4& color);

			/// <summary>
			/// ���^�{�[���̔��a�̐ݒ�
			/// </summary>
			/// <param name="radius">���a</param>
			void SetChildrenRadius(const float& radius);

			/// <summary>
			/// ���^�{�[���̐e�̐ݒ�
			/// </summary>
			/// <param name="parent">�e</param>
			void SetChildrenParent(const std::shared_ptr<GameObject>& parent);

			/// <summary>
			/// ���^�{�[���̔z���Ԃ�
			/// </summary>
			/// <returns>���^�{�[���̔z��</returns>
			vector<ex_weak_ptr<ChildrenRender>> GetChildren() const;

			/// <summary>
			/// ���^�{�[���̃`���h�����ɐe�ݒ肪���邩�ǂ���
			/// </summary>
			/// <returns>�e�����݂���Ȃ�true</returns>
			bool IsChildrenParent() const;

			/// <summary>
			/// ���^�{�[���̃`���h�����ɐe�ݒ���擾
			/// </summary>
			/// <returns>���^�{�[���̐e</returns>
			std::shared_ptr<GameObject> GetChildrenParent() const;

			/// <summary>
			/// �q�I�u�W�F�N�g�̒ǉ�
			/// </summary>
			/// <param name="child">�q�I�u�W�F�N�g</param>
			void AddChild(const std::shared_ptr<ChildrenRender>& child);

			/// <summary>
			/// �q�I�u�W�F�N�g�̕����ǉ�
			/// </summary>
			/// <param name="children">�q�I�u�W�F�N�g�̔z��</param>
			void AddChildren(const std::vector<std::shared_ptr<ChildrenRender>>& children);

			/// <summary>
			/// �q�I�u�W�F�N�g�̍폜
			/// </summary>
			/// <param name="child">�폜����q�I�u�W�F�N�g</param>
			void RemoveChild(const ex_weak_ptr<ChildrenRender>& child);

			/// <summary>
			/// �S�Ă̎q�I�u�W�F�N�g�̉��
			/// </summary>
			void AllReleaseChildren();

			/// <summary>
			/// �w�肵���R���|�[�l���g��z��ɂ��ĕԂ��B
			/// </summary>
			/// <returns>�R���|�[�l���g�̔z��</returns>
			template<class T>
			vector<ex_weak_ptr<T>> GetChildrenComponents() {
				vector<ex_weak_ptr<T>> resultVec;
				for (const auto& child : m_children) {
					if (!child) {
						continue;
					}

					resultVec.push_back(child->GetGameObject()->GetComponent<T>(false));
				}

				return resultVec;
			}

			/// <summary>
			/// �S�Ẵ��^�{�[���̒Ǐ]�I�t�Z�b�g��z��Ŏ擾
			/// </summary>
			/// <returns>�S�Ẵ��^�{�[���̃I�t�Z�b�g</returns>
			vector<Vec3> CalculateChildrenSeekOffsets();

			void OnDrawActive() override;
			void OnDrawFalse() override;

		};

	}
}

//endbasecross