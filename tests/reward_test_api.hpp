#pragma once
#include <eosio/testing/tester.hpp>

using namespace eosio::testing;
using namespace eosio;
using namespace eosio::chain;

using mvo = fc::mutable_variant_object;
using action_result = base_tester::action_result;

class reward_test_api {
public:
  name contract;

  reward_test_api(name acnt, tester* tester) {
    contract = acnt;
    _tester = tester;

	_tester->create_accounts({contract});
	_tester->set_code(contract, contracts::reward_wasm());
	_tester->set_abi(contract, contracts::reward_abi().data());

	const auto &accnt = _tester->control->db().get<account_object, by_name>(contract);

	abi_def abi;
	BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
	abi_ser.set_abi(abi, abi_serializer_max_time);

	auto auth = authority(
		1,
		{
			key_weight{_tester->get_public_key(contract, "active"), 1}
		},
		{
			permission_level_weight{{contract, config::eosio_code_name}, 1}
		}
	);

	_tester->set_authority(contract, config::active_name, auth);
  }

    fc::variant get_stats() {
      vector<char> data = _tester->get_row_by_account(contract, 0, N(stats), 0);
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant("stats_t", data, abi_serializer_max_time);
    }

    action_result claim() {
      return push_action(contract, contract, N(claim), mvo()
      );
    }

private:
    action_result push_action(const name &signer,
							const name &cnt,
							const action_name &name,
							const variant_object &data) {
	string action_type_name = abi_ser.get_action_type(name);
	action act;
	act.account = cnt;
	act.name = name;
	act.data = abi_ser.variant_to_binary(action_type_name, data, abi_serializer_max_time);

	return _tester->push_action(std::move(act), uint64_t(signer));
  }

  abi_serializer abi_ser;
  tester* _tester;
  fc::microseconds abi_serializer_max_time{1000*1000};
};