
/*!
@file AIDirector_Ex.h
@brief AIDirector_Ex�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class ThreadPool;
	class AstarThreadController;

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class I_AIRequester;
		class I_AIRequestEvent;
		struct AIRequestData;

		//--------------------------------------------------------------------------------------
		/// AIDirector�̉��ǔ�
		//--------------------------------------------------------------------------------------
		class AIDirector_Ex : public maru::SingletonComponent<AIDirector_Ex>
		{
		public:

		private:
			std::unique_ptr<AstarThreadController> m_astarThread;	//AstarThreadController

		public:
			AIDirector_Ex(const std::shared_ptr<GameObject>& owner);

			virtual ~AIDirector_Ex() = default;

		private:

		public:

			/// <summary>
			/// Astar�X���b�h�̎擾
			/// </summary>
			/// <returns></returns>
			_NODISCARD const std::unique_ptr<AstarThreadController>& GetAstarThreadController() const;

		};

	}
}