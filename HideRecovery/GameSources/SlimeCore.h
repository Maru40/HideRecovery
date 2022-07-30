
/*!
@file SlimeCore.h
@brief SlimeCore�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	struct SeekTarget_Parametor;
	class SeekTarget;

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �ڂ̃I�u�W�F�N�g
		//--------------------------------------------------------------------------------------
		class EyeObject : public GameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="stage">���̃N���X����������X�e�[�W</param>
			EyeObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;

			/// <summary>
			/// �f�B�t���[�Y�J���[�̐ݒ�
			/// </summary>
			/// <param name="color">�f�B�t���[�Y�J���[</param>
			void SetDiffuse(const Col4& color);
		};

		//--------------------------------------------------------------------------------------
		/// �X���C���̃R�A
		//--------------------------------------------------------------------------------------
		class SlimeCore : public Component
		{
			ex_weak_ptr<GameObject> m_parentObject; //�ڋʂ̐e�I�u�W�F�N�g
			ex_weak_ptr<EyeObject> m_inObject;      //�ڋʂ̍�������
			ex_weak_ptr<EyeObject> m_outObject;     //�ڋʂ̔�������

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			SlimeCore(const std::shared_ptr<GameObject>& objPtr);

			void OnCreate() override;
			void OnUpdate() override;
			void OnDestroy() override;
			void OnDraw() override;

			void OnDrawFalse() override;

		private:
			/// <summary>
			/// �R�A�̐���
			/// </summary>
			void CreateCore();

			/// <summary>
			/// �R�A�̍��ڂ𐶐�
			/// </summary>
			void CreateInObject();

			/// <summary>
			/// �R�A�̊ዅ�𐶐�
			/// </summary>
			void CreateOutObject();

		public:
			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// ���ڂ̃f�B�t���[�Y�̐ݒ�
			/// </summary>
			/// <param name="color">�f�B�t���[�Y�J���[</param>
			void SetEyeDiffuse(const Col4& color);

			/// <summary>
			/// �ዅ�̃f�B�t���[�Y�̐ݒ�
			/// </summary>
			/// <param name="color">�f�B�t���[�Y�J���[</param>
			void SetEyeBallDiffuse(const Col4& color);

			/// <summary>
			/// ParentObject��SeekTarget���擾����B
			/// </summary>
			/// <returns>ParentObject��SeekTarget</returns>
			std::shared_ptr<SeekTarget> GetSeekTargetPtr() const;
			
			/// <summary>
			/// �Ǐ]�p�����[�^�̐ݒ�
			/// </summary>
			/// <param name="seekParam">�Ǐ]�p�����[�^</param>
			void SetSeekParametor(const SeekTarget_Parametor& seekParam);

			/// <summary>
			/// �Ǐ]�p�����[�^���擾
			/// </summary>
			/// <returns>�Ǐ]�p�����[�^</returns>
			SeekTarget_Parametor GetSeekParametor() const;
		}; 

	}
}
