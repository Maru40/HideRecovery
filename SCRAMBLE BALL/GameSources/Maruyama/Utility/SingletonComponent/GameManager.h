
/*!
@file GameManager.h
@brief GameManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "SingletonComponent.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class GeneratorBase;

	//--------------------------------------------------------------------------------------
	/// �Q�[���}�l�[�W���[
	//--------------------------------------------------------------------------------------
	class GameManager : public maru::SingletonComponent<GameManager>
	{
	public:
		/// <summary>
		/// �Q�[���}�l�[�W���[�̃X�e�[�g�^�C�v
		/// </summary>
		enum class State 
		{
			Start,     //�X�^�[�g
			Game,      //�Q�[����
			Clear,     //�N���A
			Result,    //result
			GameOver,  //�Q�[���I�[�o�[
		};

		/// <summary>
		/// �p�����[�^
		/// </summary>
		struct Parametor;

	private:
		std::unique_ptr<Parametor> m_paramPtr = nullptr;  //�p�����[�^

		vector<ex_weak_ptr<GeneratorBase>> m_generators;  //�S�ẴW�F�l���[�^��ۑ�

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		GameManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// �Q�[���N���A���邩���f
		/// </summary>
		/// <returns>�N���A�����𖞂�������true</returns>
		bool IsGameClear();

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �X�e�[�g�̕ύX
		/// </summary>
		/// <param name="state">�ύX�������X�e�[�g</param>
		void ChangeState(const State state);

		/// <summary>
		/// ���݂̃X�e�[�g�������œn�����X�e�[�g���ǂ����𔻒f
		/// </summary>
		/// <param name="state">�X�e�[�g</param>
		/// <returns>�����œn�����X�e�[�g�Ȃ�true</returns>
		bool IsCurrentState(const State state) const;

		/// <summary>
		/// ���݂̃X�e�[�g���擾
		/// </summary>
		/// <returns>���݂̃X�e�[�g</returns>
		State GetCurrentState() const;

		/// <summary>
		/// �Q�[���I�����邩���f����
		/// </summary>
		void CheckClear();

	};

}

//endbasecross