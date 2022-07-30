
/*!
@file Motion_KnockBack.h
@brief Motion_KnockBack�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	namespace Metaball {
		class MotionManager;

		namespace Task {
			struct SeekOffsetMove_Parametor;
		}
	}

	namespace Enemy {

		class EnemyBase;

		namespace Motion {

			//--------------------------------------------------------------------------------------
			/// �m�b�N�o�b�N���[�V�����̃p�����[�^
			//--------------------------------------------------------------------------------------
			struct KnockBack_Parametor {
				KnockBackData data;   //�m�b�N�o�b�N�f�[�^
				float range;          //�L����ő吔�B
				float transitionTime; //�J�ڎ���
				float avoidRange;     //���̃��^�{�[���Ƃ̉������
				Vec3 maxRangeVec;     //�L����ő勗���AVec3��

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				KnockBack_Parametor();

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="range">�L����͈�</param>
				KnockBack_Parametor(const float& range);

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="data">�m�b�N�o�b�N�f�[�^</param>
				/// <param name="range">�L����͈�</param>
				KnockBack_Parametor(const KnockBackData& data, const float& range);

				/// <summary>
				/// 
				/// </summary>
				/// <param name="data">�m�b�N�o�b�N�f�[�^</param>
				/// <param name="range">�L����͈�</param>
				/// <param name="transitionTime">�J�ڎ���</param>
				KnockBack_Parametor(const KnockBackData& data, const float& range, const float& transitionTime);
			};

			//--------------------------------------------------------------------------------------
			/// �m�b�N�o�b�N���[�V����
			//--------------------------------------------------------------------------------------
			class KnockBack : public maru::MetaballMotionBase<EnemyBase>
			{
			public:
				using Parametor = KnockBack_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr;   //�p�����[�^
				std::vector<Vec3> m_defaultOffsets;  //�f�t�H���g�̃I�t�Z�b�g�l

			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="owner">���̃N���X�����L����N���X</param>
				/// <param name="paramPtr">�p�����[�^</param>
				KnockBack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<KnockBack::Parametor>& paramPtr);

			private:
				/// <summary>
				/// ���[�V�����p�����[�^���擾����B
				/// </summary>
				/// <returns>���[�V�����p�����[�^</returns>
				vector<Metaball::Task::SeekOffsetMove_Parametor> CalculateMotionParametors();

				/// <summary>
				/// ���S����I�t�Z�b�g�����́A��]���l�����������f�[�^���擾����B
				/// </summary>
				/// <param name="centerPosition">���S�ʒu</param>
				/// <param name="forward">�t�H���[�h</param>
				/// <param name="offset">�I�t�Z�b�g</param>
				/// <returns>���S�����]���l�����������f�[�^</returns>
				Vec3 ConvertRotationCenterToOffset(const Vec3& centerPosition, const Vec3& forward, const Vec3& offset);

				/// <summary>
				/// �݂��̃��^�{�[���̏d�Ȃ���������x�N�g�����v�Z
				/// </summary>
				/// <param name="baseDirect">�x�[�X�ƂȂ����</param>
				/// <param name="currentIndex">���݂̃C���f�b�N�X</param>
				/// <param name="offsets">�I�t�Z�b�g�Q</param>
				/// <returns>�݂��̃��^�{�[���̉������x�N�g��</returns>
				Vec3 CalculateAvoidVec(const Vec3& baseDirect, const int currentIndex, const std::vector<Vec3>& offsets);

				/// <summary>
				/// �ׂ��x�N�g����Ԃ��B
				/// </summary>
				/// <returns>�ׂ��x�N�g��</returns>
				Vec3 CalculatePressedVec();

				/// <summary>
				/// �f�t�H���g�l����ǂꂾ���̋����𓮂������Ԃ��B
				/// </summary>
				/// <returns>�f�t�H���g�l���瓮��������</returns>
				float GetRange() const;

				/// <summary>
				/// �z�����ꂽ�I�u�W�F�N�g�̐����擾
				/// </summary>
				/// <returns>�z�����ꂽ�I�u�W�F�N�g�̐�</returns>
				int GetNumAbsrobed() const;

				/// <summary>
				/// �m�b�N�o�b�N�œ����ő勗�����擾
				/// </summary>
				/// <returns>�m�b�N�o�b�N�œ����ő勗��</returns>
				Vec3 GetMaxRangeVec() const;

				/// <summary>
				/// �z������Ă����Ԃ��ǂ���
				/// </summary>
				/// <returns>�z������Ă�����true</returns>
				bool IsAbsrobed() const;

				/// <summary>
				/// �z������Ă���I�u�W�F�N�g�̃I�t�Z�b�g�Q���擾
				/// </summary>
				/// <returns>�z������Ă���I�u�W�F�N�g�̃I�t�Z�b�g�Q</returns>
				std::vector<Vec3> GetAbsorbedOffsets() const;
			};

		}

	}
}