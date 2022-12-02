
/*!
@file GraphBase_Ex.cpp
@brief GraphBase_Exのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "GraphBase_Ex.h"

#include "NodeBase.h"
#include "EdgeBase.h"

namespace basecross {

	namespace maru {

		class A {
			int i;

		public:

			A(int i)
				:i(i)
			{}
		};

		class B {
			float f;

		public:

			B(float f) :
				f(f)
			{}
		};

		template<class T>
		class TestClass {

		public:
			template<class... Ts,
				std::enable_if_t<std::is_constructible_v<T, Ts...>, std::nullptr_t> = nullptr>
			void Add(Ts&&... params) {
				auto t = std::make_shared<T>(params...);
			}

		};

		void Test() {
			class EnumT {

			};

			auto test = new TestClass<A>();
			
			//test->Add();
		}

	}
}