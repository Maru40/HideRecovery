/*!
@file MetaballChildrenSeekManager.h
@brief MetaballChildrenSeekManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Metaball {

		//--------------------------------------------------------------------------------------
		///	�O���錾
		//--------------------------------------------------------------------------------------
		class RenderBase;

		//--------------------------------------------------------------------------------------
		///	���^�{�[���̒Ǐ]�p�����[�^
		//--------------------------------------------------------------------------------------
		struct ChildrenSeekManager_Parametor
		{
			vector<SeekTarget::Parametor> seekParams; //�Ǐ]�p�����[�^

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			ChildrenSeekManager_Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="seekParams">�Ǐ]�p�����[�^�z��</param>
			ChildrenSeekManager_Parametor(const vector<SeekTarget::Parametor>& seekParams);
		};

		//--------------------------------------------------------------------------------------
		///	���^�{�[���̒Ǐ]�Ǘ�
		//--------------------------------------------------------------------------------------
		class ChildrenSeekManager : public Component
		{
		public:
			using Parametor = ChildrenSeekManager_Parametor;

		private:
			Parametor m_param;												//�p�����[�^

			std::unique_ptr<SeekTargetOffsetsManager> m_seekOffsetsManager;	//�Ǐ]�I�t�Z�b�g�Ǘ�

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			ChildrenSeekManager(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="parametor">�p�����[�^</param>
			ChildrenSeekManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

			void OnCreate() override;
			void OnUpdate() override;

		private:
			/// <summary>
			/// �Ǐ]�Z�b�e�B���O
			/// </summary>
			void SettingSeek();

			/// <summary>
			/// �Ǐ]�p�����[�^���Z�b�g
			/// </summary>
			/// <param name="seek">�Ǐ]�R���|�[�l���g</param>
			/// <param name="index">���݂̃C���f�b�N�X</param>
			void SetSeekParametor(const std::shared_ptr<SeekTarget>& seek, const int& index);

		public:
			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �ړ��J�n
			/// </summary>
			/// <param name="task">�ړ��^�X�N�^�C�v</param>
			/// <param name="parametor">�ړ��p�����[�^</param>
			void StartMove(const SeekTargetOffsetsManager::TaskEnum& task, const SeekTargetOffsetsManager::Parametor& parametor);

			/// <summary>
			/// Seek��Ԃ��������Ēe��������
			/// </summary>
			/// <param name="velocity">�e���鑬�x</param>
			void Crash(const Vec3& velocity);

			/// <summary>
			/// �^�X�N�̏I�����f
			/// </summary>
			/// <returns>�^�X�N���I�����Ă�����true</returns>
			bool IsEndTask();
			
		};

		//--------------------------------------------------------------------------------------
		///	Utility
		//--------------------------------------------------------------------------------------

		namespace Utility {
			//--------------------------------------------------------------------------------------
			///	�Ǐ]�p�����[�^�̃Z�b�e�B���O���ł���悤�ɂ���N���X
			//--------------------------------------------------------------------------------------
			class ChildrenSeekSetting
			{
			public:
				/// <summary>
				/// �p�����[�^�z��𐶐�
				/// </summary>
				/// <param name="speed">�X�s�[�h</param>
				/// <param name="range">�͈�</param>
				/// <param name="offsets">�I�t�Z�b�g�Q</param>
				/// <param name="seekType">�Ǐ]�^�C�v</param>
				/// <returns>�p�����[�^�z��</returns>
				static vector<SeekTarget::Parametor> CreateSeekTargetParametors(
					const float& speed, 
					const float& range,
					const std::vector<Vec3> offsets, 
					const SeekTarget::SeekType& seekType
				);

			};
		}

	}
}

//endbasecross