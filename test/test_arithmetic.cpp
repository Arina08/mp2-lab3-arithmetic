// тесты для вычисления арифметических выражений

#include <gtest.h>
#include <../include/arithmetic.h>

#define EPS 1e-5

TEST(arithmetic, can_create_arithmetic)
{
	ASSERT_ANY_THROW(Arithmetic a); // пустое выражение
	ASSERT_NO_THROW(Arithmetic b("3"));
}

TEST(arithmetic, simple_sum_1)
{
	string s = "12+2";
	Arithmetic a(s);
	EXPECT_EQ(14.0, a.calculate());
}
TEST(arithmetic, simple_sum_2)
{
	string s = "12+8+9-9";
	Arithmetic a(s);
	EXPECT_EQ(20.0, a.calculate());
}
TEST(arithmetic, all_op)
{
	string s = "25.6*2.9-9.8+5.0/2.0";
	Arithmetic a(s);
	EXPECT_NEAR(66.94, a.calculate(), EPS);
}
TEST(arithmetic, simple_brackets_sequence)
{
	string s = "4.5*(1.+2.)";
	Arithmetic a(s);
	EXPECT_EQ(13.5, a.calculate());
}
TEST(arithmetic, unary_minus_without_brackets)
{
	string s = "2+-2";
	Arithmetic a(s);
	EXPECT_EQ(0, a.calculate());
}
TEST(arithmetic, unary_minus_with_brackets_1)
{
	string s = "2+(-2)";
	Arithmetic a(s);
	EXPECT_EQ(0, a.calculate());
}
TEST(arithmetic, unary_minus_with_brackets_2)
{
	string s = "-12.5+(-12.5)";
	Arithmetic a(s);
	EXPECT_EQ(-25.0, a.calculate());
}
TEST(arithmetic, unary_minus_with_division)
{
	string s = "2/-2";
	Arithmetic a(s);
	EXPECT_EQ(-1.0, a.calculate());
}
TEST(arithmetic, unary_minus_with_division_with_brackets_1)
{
	string s = "2/(-2)";
	Arithmetic a(s);
	EXPECT_EQ(-1.0, a.calculate());
}
TEST(arithmetic, unary_minus_with_division_with_brackets_2)
{
	string s = "1/-(4+6)";
	Arithmetic a(s);
	EXPECT_EQ(-0.1, a.calculate());
}
TEST(arithmetic, unary_minus_with_brackets_more_complex)
{
	string s = "1+(-2+3/4-2)";
	Arithmetic a(s);
	EXPECT_EQ(-2.25, a.calculate());
}

TEST(arithmetic, lots_of_unary_minus)
{
	string s = "2-------2";
	Arithmetic a(s);
	EXPECT_EQ(0.0, a.calculate());
}

TEST(arithmetic, unary_minus_before_brackets)
{
	string s = "-(2+1)";
	Arithmetic a(s);
	EXPECT_EQ(-3.0, a.calculate());
}
TEST(arithmetic, spaces_check_1)
{
	string s = "8.345 + 4.-9.125 +   9.125";
	Arithmetic a(s);
	EXPECT_NEAR(12.345, a.calculate(), EPS);
}
TEST(arithmetic, spaces_check_2)
{
	string s = "8.345 / 8.345 + ( 4.-9.125 ) +   9.125";
	Arithmetic a(s);
	EXPECT_NEAR(5.0, a.calculate(), EPS);
}
TEST(arithmetic, throw_when_first_character_is_closing_bracket)
{
	string s = ")4*3";

	ASSERT_ANY_THROW(Arithmetic a(s));
}
TEST(arithmetic, throw_when_first_character_is_operation)
{
	string s = "/4*3";
	ASSERT_ANY_THROW(Arithmetic(s));
}
TEST(arithmetic, throw_when_opening_bracket_goes_after_number)
{
	string s = "8(34+3)";
	ASSERT_ANY_THROW(Arithmetic(s));
}
TEST(arithmetic, throw_wrong_amounts_of_brackets)
{
	string s = "(8.0))";
	ASSERT_ANY_THROW(Arithmetic(s));
}
TEST(arithmetic, throw_operation_after_opening_bracket)
{
	string s = "3+1*(/4)";
	ASSERT_ANY_THROW(Arithmetic(s));
}
TEST(arithmetic, no_throw_unary_minus_after_opening_bracket)
{
	string s = "3+1*(-4)";
	ASSERT_NO_THROW(Arithmetic a(s));
}
TEST(arithmetic, throw_last_character_is_operation)
{
	string s = "3+1*";
	ASSERT_ANY_THROW(Arithmetic(s));
}
TEST(arithmetic, throw_last_character_is_opening_bracket)
{
	string s = "3+1*(";
	ASSERT_ANY_THROW(Arithmetic(s));
}
TEST(arithmetic, division_by_zero)
{
	string s = "5.1234/0.0 + 6*(-9+3)";
	ASSERT_ANY_THROW(Arithmetic a(s); a.calculate());
}
TEST(arithmetic, too_many_dots)
{
	string s = "6 / 3.2.1 + 4.98";
	ASSERT_ANY_THROW(Arithmetic(s));
}
TEST(arithmetic, two_binary_ops_in_a_row) {
	string s = "5++5";
	ASSERT_ANY_THROW(Arithmetic(s));
}
