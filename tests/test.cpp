#include <Transaction.h>
#include <gtest/gtest.h>

TEST(Transaction, Banking){
	const int base_A = 5000, base_B = 5000, base_fee = 100;
	//тестовые объекты
	Account x(0,base_A), y(1,base_B);
	Transaction tran;
	//конструкторы и т п
	ASSERT_EQ(tran.fee(), 1);
	tran.set_fee(base_fee);
	ASSERT_EQ(tran.fee(), base_fee);
	//ошибка транзакции
	ASSERT_THROW(tran.Make(x, x, 1000), std::logic_error);
	ASSERT_THROW(tran.Make(x, y, -50), std::invalid_argument);
	ASSERT_THROW(tran.Make(x, y, 50), std::logic_error);
	if (tran.fee()*2-1 >= 100) {
		ASSERT_EQ(tran.Make(x, y, tran.fee()*2-1), false);
	}
	//что все верно лочится
	x.Lock();
	ASSERT_THROW(tran.Make(x, y, 1000), std::runtime_error);
	x.Unlock();
	//транзакция проходит
	ASSERT_EQ(tran.Make(x, y, 1000), true);
	ASSERT_EQ(y.GetBalance(), base_B+1000);
	ASSERT_EQ(x.GetBalance(), base_A-1000-base_fee);
	//недостаточно денег
	ASSERT_EQ(tran.Make(x, y, 3900), false);
	ASSERT_EQ(y.GetBalance(), base_B+1000);
	ASSERT_EQ(x.GetBalance(), base_A-1000-base_fee);
}
