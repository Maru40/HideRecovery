/*!
@file ShareClassesManager.h
@brief ShareClassManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "SingletonComponent.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ���ݎg�p�s��
	//--------------------------------------------------------------------------------------
	class I_ShareClass
	{
	public:
			
	};

	//--------------------------------------------------------------------------------------
	/// ���ݎg�p�s��
	//--------------------------------------------------------------------------------------
	template<class T>
	class ShareCalss : public I_ShareClass
	{
		std::vector<std::weak_ptr<T>> m_shareClasses;	//�V�F�A�N���X�̔z��
	public:
		void AddShareClass(const std::shared_ptr<T>& shareClass) noexcept { m_shareClasses.push_back(shareClass); }

		std::vector<std::weak_ptr<T>> GetShareClasses() const noexcept { return m_shareClasses; }
	};

	//--------------------------------------------------------------------------------------
	/// �N���X�̃V�F�A���Ǘ�����N���X
	//--------------------------------------------------------------------------------------
	class ShareClassesManager : public maru::SingletonComponent<ShareClassesManager>
	{
	public:
		using ShareClassVector = std::vector<std::weak_ptr<ObjectInterface>>;
		using ShareClassesMap = std::unordered_map<type_index, ShareClassVector>;

	private:
		ShareClassesMap m_shareClassesMap;	//�V�F�A����N���X�̃}�b�v

	public:
		ShareClassesManager(std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// �e���v���[�g�N���X�̃^�C�vID���擾
		/// </summary>
		template<class T>
		type_index GetTypeIndex() const noexcept {
			return type_index(typeid(T));
		}

		/// <summary>
		/// �w�肵���C���f�b�N�X�����łɓo�^����Ă��邩�m�F����B
		/// </summary>
		/// <param name="index">�N���X�̃^�C�v�C���f�b�N�X</param>
		/// <returns>�o�^����Ă�����true</returns>
		bool HasShareType(const type_index index) const noexcept {
			return m_shareClassesMap.count(index) != 0;
		}

		/// <summary>
		/// �e���v���[�g�Ŏw�肵���N���X�����łɓo�^����Ă��邩�m�F����B
		/// </summary>
		template<class T>
		bool HasShareType() const noexcept {
			auto index = type_index(typeid(T));
			return HasShareType(index);
		}

		/// <summary>
		/// �V�F�A����N���X���擾����B
		/// </summary>
		template<class T>
		bool AddShareClass(const std::shared_ptr<ObjectInterface>& objPtr) {
			auto index = type_index(typeid(T));
			m_shareClassesMap[index].push_back(objPtr);
			return true;
		}

		/// <summary>
		/// �V�F�A�N���X�̎擾
		/// </summary>
		template<class T>
		ShareClassVector GetShareClasses() const noexcept {
			auto index = type_index(typeid(T));
			return HasShareType(index) ? m_shareClassesMap.at(index) : ShareClassVector();
		}

		/// <summary>
		/// �V�F�A�N���X���e���v���[�g�̌^�ɃL���X�g���Ď擾����B
		/// </summary>
		template<class T>
		std::vector<std::weak_ptr<T>> GetCastShareClasses() const {
			std::vector<std::weak_ptr<T>> result;
			auto index = type_index(typeid(T));

			if (!HasShareType(index)) {	//���݂��Ȃ��Ȃ�A��̔z���Ԃ��B
				return result;
			}

			//�L���X�g���Ĕz��ɓ����B
			for (auto& shareClass : m_shareClassesMap.at(index)) {
				auto tPtr = std::dynamic_pointer_cast<T>(shareClass.lock());
				if (tPtr) {	//�L���X�g�ɐ���������z���pushback
					result.push_back(tPtr);
				}
			}

			return result;
		}

	};

}