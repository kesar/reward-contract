#include <boost/test/unit_test.hpp>
#include <eosio/testing/tester.hpp>
#include "contracts.hpp"
#include "reward_test_api.hpp"
#include "token_test_api.hpp"

using namespace eosio::testing;
using namespace eosio;
using namespace eosio::chain;
using namespace fc;
using namespace std;

using mvo = fc::mutable_variant_object;

#define SUCCESS(call) BOOST_REQUIRE_EQUAL(success(), call)
#define ERROR(msg, call) BOOST_REQUIRE_EQUAL(wasm_assert_msg(msg), call)

class reward_tester: public tester
{
protected:
  reward_test_api api;
  token_test_api eos;
public:
    reward_tester(): api(N(reward), this), eos(N(eosio.token), this)
	{
		create_accounts({ N(kesaritooooo), N(bob) });
	  	produce_blocks(2);
        init_token(N(kesaritooooo));
	}

private:
	void init_token(const name& to)
	{
	  SUCCESS(eos.push_action(eos.contract, eos.contract, N(create), mvo()
		  ("issuer", eos.contract)
		  ("maximum_supply", asset::from_string("100000.0000 EOS"))
	  ));

	  SUCCESS(eos.push_action(eos.contract, eos.contract, N(issue), mvo()
		  ("to", eos.contract)
		  ("quantity", asset::from_string("300.0000 EOS"))
		  ("memo", "")
	  ));

	  SUCCESS(eos.transfer(eos.contract, N(kesaritooooo), asset::from_string("100.0000 EOS")));
	  SUCCESS(eos.transfer(eos.contract, N(bob), asset::from_string("100.0000 EOS")));
	}
};

BOOST_AUTO_TEST_SUITE(reward_tests)

    BOOST_FIXTURE_TEST_CASE(right_transfer, reward_tester) try {
        SUCCESS(eos.transfer(N(kesaritooooo), api.contract,  asset::from_string("5.0000 EOS"), ""));
        SUCCESS(eos.transfer(N(kesaritooooo), api.contract,  asset::from_string("5.0000 EOS"), ""));
        auto balances_row = api.get_stats();
        BOOST_REQUIRE_EQUAL(balances_row["quantity"].as_string(), "10.0000 EOS");
        SUCCESS(api.claim());
        balances_row = api.get_stats();
        BOOST_REQUIRE_EQUAL(balances_row["quantity"].as_string(), "0.0000 EOS");
        ERROR("need more time to claim", api.claim());
    }
    FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
