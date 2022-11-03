
/*!
@file TupleSpace.h
@brief TupleSpace�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {
		template<class T>
		class Action;
	}

	namespace Enemy {

		class I_FactionMember;

		namespace Tuple {

			//--------------------------------------------------------------------------------------
			/// �^�v���̃C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_Tuple {
			public:
				virtual ~I_Tuple() = default;

				virtual bool operator== (const I_Tuple& other);
			};

			//--------------------------------------------------------------------------------------
			/// ��]�s���̊��N���X
			//--------------------------------------------------------------------------------------
			class TupleActionBase : public I_Tuple
			{
			public:
				std::weak_ptr<GameObject> acter;		//�s�������
				std::weak_ptr<GameObject> requester;	//�Ώێ�
				float value;							//�]���l

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="acter">�s�������</param>
				/// <param name="requester">�Ώێ�</param>
				/// <param name="value">�]���l</param>
				TupleActionBase(const std::shared_ptr<GameObject>& acter, const std::shared_ptr<GameObject>& requester, const float value);

				virtual ~TupleActionBase() = default;
			};

			//--------------------------------------------------------------------------------------
			/// ���N�G�X�g�̊��N���X
			//--------------------------------------------------------------------------------------
			class TupleRequestBase : public I_Tuple
			{
			public:
				std::weak_ptr<GameObject> requester;	//�v�����o������
				float value;							//�]���l

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="requester">�v�����o������</param>
				/// <param name="value">�]���l</param>
				TupleRequestBase(const std::shared_ptr<GameObject>& requester, const float value);

				virtual ~TupleRequestBase() = default;

				virtual bool operator== (const TupleRequestBase& other);
			};

			//--------------------------------------------------------------------------------------
			/// �^�[�Q�b�g�����������Ƃ�m�点��^�v��
			//--------------------------------------------------------------------------------------
			class FindTarget : public TupleRequestBase
			{
			public:
				std::weak_ptr<GameObject> target;

				FindTarget(const std::shared_ptr<GameObject>& requester, const std::shared_ptr<GameObject>& target, const float value = 1.0f);

				virtual ~FindTarget() = default;

				bool operator== (const FindTarget& other);
			};

			//--------------------------------------------------------------------------------------
			/// �^�[�Q�b�g�ƃo�g�����邱�Ƃ����N�G�X�g����^�v��
			//--------------------------------------------------------------------------------------
			class ButtleTarget : public TupleRequestBase 
			{
				std::weak_ptr<GameObject> target;

			public:
				ButtleTarget(
					const std::shared_ptr<GameObject>& requester,
					const std::shared_ptr<GameObject>& target,
					const float value
				);
			};

			//--------------------------------------------------------------------------------------
			/// �ʒm�p�f�[�^�Ǘ��N���X
			//--------------------------------------------------------------------------------------
			class NotifyController {
			private:
				std::function<void()> func;		//�Ăяo������
				std::function<bool()> isCall;	//�Ăяo������

			public:
				NotifyController(
					const std::function<void()>& func,
					const std::function<bool()>& isCall = []() { return true; }
				);

				void Invoke();
			};

			//--------------------------------------------------------------------------------------
			/// �^�v���X�y�[�X
			//--------------------------------------------------------------------------------------
			class TupleSpace
			{
			public:
				using NotifyControllers = std::vector<std::shared_ptr<NotifyController>>;

			private:
				//�������܂ꂽ���ꗗ
				std::unordered_map<type_index, std::vector<std::shared_ptr<I_Tuple>>> m_tuplesMap;

				//NotipyMap�ʒm��o�^����ꗗ
				std::unordered_map<type_index, NotifyControllers> m_notifysMap;

			public:
				virtual ~TupleSpace() = default;

				//--------------------------------------------------------------------------------------
				/// �A�N�Z�b�T
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// ���b�Z�[�W����������
				/// </summary>
				template<class T, class... Ts,
					std::enable_if_t<
						std::is_base_of_v<I_Tuple, T> &&
						std::is_constructible_v<T, Ts...>,
					std::nullptr_t> = nullptr
				>
				void Write(Ts&&... params) 
				{
					auto typeIndex = type_index(typeid(T));
					auto newTuple = std::make_shared<T>(params...);

					//�������Ȃ珑�����܂Ȃ�
					if (m_tuplesMap.count(typeIndex) != 0) {	//Map�ɑ��݂���(���݂��Ȃ��Ȃ�A�V�K�̂��߁A�������ǂ����̔��f������Ȃ��B)
						for (auto& tuple : m_tuplesMap[typeIndex]) {
							auto tTuple = dynamic_pointer_cast<T>(tuple);
							if (!tTuple) {	//�L���X�g�ł��Ȃ��Ȃ�
								continue;
							}

							//����̏������݂Ȃ�A�������܂Ȃ�
							if (*tTuple.get() == *newTuple.get()) {
								return;
							}
						}
					}

					//�^������B
					m_tuplesMap[typeIndex].push_back(newTuple);

					//�o�^���ꂽ�ʒm���Ăяo���B
					CallNotifys(typeIndex);
				}

				/// <summary>
				/// ���b�Z�[�W��ǂݍ���(���b�Z�[�W�͍폜���Ȃ�)
				/// </summary>
				/// <param name="isFunc">�擾����</param>
				template<class T,
					std::enable_if_t<
						std::is_base_of_v<I_Tuple, T>,
					std::nullptr_t> = nullptr
				>
				const std::shared_ptr<const T> Read(
						const std::function<bool(const std::shared_ptr<T>&)>& isFunc = [](const std::shared_ptr<T>&) { return true; } ) const
				{
					return SearchTuple<T>(isFunc);
				}

				/// <summary>
				/// ���b�Z�[�W����M���āA�폜����B
				/// </summary>
				/// <param name="isFunc">�擾����</param>
				template<class T,
					std::enable_if_t<
						std::is_base_of_v<I_Tuple, T>, 
					std::nullptr_t> = nullptr
				>
				std::shared_ptr<T> Take(
						const std::function<bool(const std::shared_ptr<T>&)>& isFunc = [](const std::shared_ptr<T>&) { return true; }
				) {
					auto tuple = SearchTuple<T>(isFunc);
					if (!tuple) {	//�^�v�������݂��Ȃ��Ȃ�nullptr
						return nullptr;
					}

					//�폜����
					RemoveTuple<T>(tuple);

					return tuple;
				}

				/// <summary>
				/// ���b�Z�[�W�̎擾���m�肵�āA�폜����B
				/// </summary>
				/// <param name="takeTuple">�擾����^�v���̃|�C���^</param>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>
				std::shared_ptr<T> Take(const std::shared_ptr<T>& takeTuple)
				{
					auto typeIndex = type_index(typeid(T));	//�^�C���f�b�N�X�̎擾

					if (m_tuplesMap.count(typeIndex) == 0) {	//Map�ɑ��݂��Ȃ��Ȃ�nullptr��Ԃ��B
						return nullptr;
					}

					auto& tuples = m_tuplesMap.at(typeIndex);	//���̌^�̃^�v�����擾
					for (auto& tuple : tuples) {
						if (tuple == takeTuple) {	//�܂��^�v�������݂���Ȃ�
							//�폜����
							RemoveTuple<T>(takeTuple);

							return takeTuple;
						}
					}

					return nullptr;
				}

				/// <summary>
				/// �e���v���[�g�Ɉ�v���閽�߂���M���ꂽ��A�ʒm���󂯎��B
				/// </summary>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>	//���N���X�̐���
				void Notify(
					const std::function<void()>& func, 
					const std::function<bool()>& isCall = []() { return true; }
				) {
					auto typeIndex = type_index(typeid(T));	//�^�C���f�b�N�X�̎擾

					auto newNotify = std::make_shared<NotifyController>(func, isCall);
					m_notifysMap[typeIndex].push_back(newNotify);	//Notipy�̐���
				}

			private:
				/// <summary>
				/// �o�^���ꂽ�ʒm���Ăяo���B
				/// </summary>
				/// <param name="typeIndex">�^�C�v�C���f�b�N�X</param>
				void CallNotifys(const type_index typeIndex);

				/// <summary>
				/// �o�^���ꂽ�ʒm���Ăяo���B
				/// </summary>
				template<class T,
					std::enable_if_t<
						std::is_base_of_v<I_Tuple, T>,
					std::nullptr_t> = nullptr
				>
				void CallNotifys() {
					auto typeIndex = type_index(typeid(T));
					CallNotifys(typeIndex);
				}

				/// <summary>
				/// �^�v�����������ĕԂ��B
				/// </summary>
				/// <param name="isFunc">�擾����</param>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>
				std::shared_ptr<T> SearchTuple(const std::function<bool(const std::shared_ptr<T>&)>& isFunc) const
				{
					auto tIndex = type_index(typeid(T));	//�^�C���f�b�N�X�̎擾

					if (m_tuplesMap.count(tIndex) == 0) {	//Map�ɑ��݂��Ȃ��Ȃ�nullptr��Ԃ��B
						return nullptr;
					}

					auto& tuples = m_tuplesMap.at(tIndex);	//���̌^�̃^�v���z����擾
					for (auto& i_tupe : tuples) {
						auto tTuple = dynamic_pointer_cast<T>(i_tupe);
						if (!tTuple) {	//�L���X�g�ł��Ȃ������珈�����΂�
							continue;
						}

						////���������ꏏ�Ȃ�B
						if (isFunc(tTuple)) {
							return tTuple;
						}
					}

					return nullptr;
				}

				/// <summary>
				/// �^�v���̍폜
				/// </summary>
				/// <param name="removeTuple">�폜�������^�v��</param>
				template<class T,
					std::enable_if_t<std::is_base_of_v<I_Tuple, T>, std::nullptr_t> = nullptr>
				bool RemoveTuple(const std::shared_ptr<T>& removeTuple) 
				{
					auto tIndex = type_index(typeid(T));	//�^�C���f�b�N�X�̎擾

					if (m_tuplesMap.count(tIndex) == 0) {		//Map�ɑ��݂��Ȃ��Ȃ�false��Ԃ��B
						return false;
					}

					auto& tuples = m_tuplesMap.at(tIndex);

					auto iter = tuples.begin();
					while (iter != tuples.end()) {
						if (*iter == removeTuple) {
							iter = tuples.erase(iter);
							return true;
						}
						iter++;
					}

					return false;
				}

			};

		}
	}
}