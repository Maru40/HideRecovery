#pragma once
#include"stdafx.h"

namespace basecross
{
	//�t�@���N�V����Interface�N���X
	class I_Function
	{
	public:

		virtual bool operator()() const = 0;
	};


	//�I�u�W�F�N�g�̊֐��|�C���^�𐶐�����p�̃N���X
	template<class T>
	class C_Function : public I_Function
	{
	public:

		std::weak_ptr<T> m_objPtr;  //���̃I�u�W�F�N�g
		void(T::*m_fn)();           //�e���v���[�g�^�̊֐��|�C���^��������ϐ�

		void nullFunction() {}

	public:

		C_Function(std::weak_ptr<T> obj, void(T::*fn)()) :
			m_objPtr(obj),
			m_fn(fn)
		{

		}

		virtual bool operator()() const override   //�I�y���[�^�̃I�[�o�[���C�h
		{
			auto obj = m_objPtr.lock();
			if (!obj)
			{
				return false;
			}

			auto* ptr = obj.get();   //�I�u�W�F�N�g�̐��|�C���^���擾
			(ptr->*m_fn)();          //�I�u�W�F�N�g�̎擾���Ă����֐����Ăяo���B
			return true;
		}
	};


	//�f���Q�[�h�N���X
	class MyDelegate
	{
		std::vector<std::shared_ptr<I_Function>> m_functions;    //I_Function�����x�N�^�[�z��

	public:

		//MyDelegate() {};

		template<class T>
		void AddFunction(std::shared_ptr<T> thisPtr, void(T::*fn)())   //�֐��̒ǉ�
		{
			std::shared_ptr<I_Function> shard = std::make_shared<C_Function<T>>(thisPtr, fn);  //�t�@���N�V�����̐���
			m_functions.push_back(shard);   //�z��ɒǉ�
		}

		void RemoveAll()   //�S�Ẵt�@���N�V�����̍폜
		{
			m_functions.clear();
		}

		void Invoke() //invoke�֐�
		{
			if (m_functions.empty())  //�z�񂪋󂾂����珈�����s��Ȃ��B
			{
				return;
			}

			auto iter = m_functions.begin();   //�C�e���[�^�̎擾

			while (iter != m_functions.end())  //�C�e���[�^���Ō�ɂȂ�܂�
			{

				//�֐��|�C���^���g�������Ƃ��͐��|�C���^���g���K�v������B
				bool b = (*(iter->get()))();  //���|�C���^���擾���A���̃A�h���X��̃I�[�o�[���C�h�����֐����Ăяo���B
				
				if (!b)  //����false���������Ă�����objPtr�̎擾�Ɏ��s
				{
					iter = m_functions.erase(iter);  //���̂��ߔz�񂩂�폜
					continue;
				}

				iter++;
			}
		}

		bool empty()  //�G���v�e�B�[
		{
			return m_functions.empty();
		}
	};

}