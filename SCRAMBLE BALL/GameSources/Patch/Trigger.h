#pragma once

namespace itbs
{
	namespace Utility
	{
		class Trigger
		{
			mutable bool flg = false;

		public:
			void Fire()
			{
				flg = true;
			}

			bool Get() const
			{
				auto returnFlg = flg;
				flg = false;
				return returnFlg;
			}
		};
	}
}