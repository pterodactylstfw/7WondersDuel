#include <gtest/gtest.h>
#include <QCoreApplication>

int main(int argc, char* argv[])
{
	// Ini?ializ?m Qt Core (f?r? grafic?) pentru ca clasele care folosesc QString s? nu crape
	QCoreApplication app(argc, argv);

	// Ini?ializ?m Google Test
	::testing::InitGoogleTest(&argc, argv);

	// Rul?m toate testele
	return RUN_ALL_TESTS();
}