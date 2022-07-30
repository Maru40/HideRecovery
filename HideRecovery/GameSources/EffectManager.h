
/*!
@file EffectManager.h
@brief EffectManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	namespace Effect {
		struct ParticleData;
	}

	//--------------------------------------------------------------------------------------
	///	�G�t�F�N�g�Ǘ��N���X
	//--------------------------------------------------------------------------------------
	class EffectManager : public maru::SingletonComponent<EffectManager>
	{
	public:
		/// <summary>
		/// �G�t�F�N�gID
		/// </summary>
		enum class ID
		{

		};

	private:
		map<ID, std::function<std::shared_ptr<GameObject>(const Vec3&, const Quat&)>> m_effectMap; //�G�t�F�N�g�}�b�v

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		EffectManager(const std::shared_ptr<GameObject>& objPtr);
		
		void OnCreate() override;

	private:
		/// <summary>
		/// �G�t�F�N�g�}�b�v�̃Z�b�e�B���O
		/// </summary>
		void SettingEffectMap();

	public:
		/// <summary>
		/// �G�t�F�N�g�̐���
		/// </summary>
		/// <param name="id">��������G�t�F�N�gID</param>
		/// <param name="position">��������ʒu</param>
		/// <param name="quat">�����������</param>
		/// <returns>���������G�t�F�N�g�I�u�W�F�N�g</returns>
		std::shared_ptr<GameObject> Play(const ID id, const Vec3& position, const Quat& quat = Quat::Identity());

	private:
		/// <summary>
		/// �p�[�e�B�N���̐���
		/// </summary>
		/// <param name="data">�����p�̃f�[�^</param>
		/// <param name="position">��������ʒu</param>
		/// <param name="quat">�����������</param>
		/// <returns>���������p�[�e�B�N���I�u�W�F�N�g</returns>
		std::shared_ptr<GameObject> CreatePrticle(const Effect::ParticleData& data, const Vec3& position, const Quat& quat = Quat::Identity());

		/// <summary>
		/// �p�[�e�B�N���̕�������
		/// </summary>
		/// <param name="data">�����p�̃f�[�^�z��</param>
		/// <param name="position">��������ʒu</param>
		/// <param name="quat">�����������</param>
		/// <returns>���������p�[�e�B�N���z��</returns>
		std::shared_ptr<GameObject> CreatePrticles(const std::vector<Effect::ParticleData>& data, const Vec3& position, const Quat& quat = Quat::Identity());

		/// <summary>
		/// �G�t�F�N�g�̐���
		/// </summary>
		/// <param name="position">��������ʒu</param>
		/// <param name="quat">�����������</param>
		/// <returns>���������G�t�F�N�g</returns>
		template<class T>
		std::shared_ptr<GameObject> CreateEffect(const Vec3& position, const Quat& quat = Quat::Identity()) {
			auto object = GetStage()->Instantiate<GameObject>(position, quat);
			object->AddComponent<T>();

			return object;
		}

	public:
		//--------------------------------------------------------------------------------------
		///	�p�[�e�B�N���̐����p�f�[�^�擾�֐�
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �o�[�j�A��VLine���g�����f�[�^��Ԃ��B
		/// </summary>
		/// <returns>�o�[�j�A��VLine���g�����f�[�^</returns>
		Effect::ParticleData GetVernierVLineData();

		/// <summary>
		/// �o�[�j�A��Point���g�����f�[�^��Ԃ��B
		/// </summary>
		/// <returns>�o�[�j�A��Point���g�����f�[�^</returns>
		Effect::ParticleData GetVernierPointData();

		/// <summary>
		/// �o�[�j�A�ɕK�v�ȃf�[�^�ꎮ��Ԃ��B
		/// </summary>
		/// <returns>�o�[�j�A�ɕK�v�ȃf�[�^�ꎮ</returns>
		std::vector<Effect::ParticleData> GetVernierData();

	};

}