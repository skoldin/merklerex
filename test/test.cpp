#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>
#include "OrderBook.h"
#include "OrderBookEntry.h"
#include "Wallet.h"

class FixtureTests : public CppUnit::TestFixture
{
    public:
        void testHighPrice()
        {
            std::vector<OrderBookEntry> orders;

            orders.push_back(OrderBookEntry{
                100,
                1,
                "2020/03/17 17:01:24.884492",
                "ETH/BTC",
                OrderBookType::ask
            });
            orders.push_back(OrderBookEntry{
                200,
                1,
                "2020/03/17 17:01:24.884492",
                "ETH/BTC",
                OrderBookType::ask
            });
            orders.push_back(OrderBookEntry{
                50,
                1,
                "2020/03/17 17:01:24.884492",
                "ETH/BTC",
                OrderBookType::ask
            });

            CPPUNIT_ASSERT(OrderBook::getHighPrice(orders) == 200);
        }
};

int main()
{
    CppUnit::TextUi::TestRunner runner{};

    runner.addTest(new CppUnit::TestCaller<FixtureTests>{
        "test the percentage calculation",
        &FixtureTests::testHighPrice
    });

    runner.run();

    return 0;
}