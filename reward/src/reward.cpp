#include <reward/reward.hpp>

void reward::claim()
{
    stats stat(_self, 0);
    const auto& record = stat.get(0);
    auto time = current_time_point().sec_since_epoch();
    check(time > record.last_claimed + TIME_TO_CLAIM, "need more time to claim");

    if (record.quantity.amount > 0) {
        auto dev_amount = record.quantity * PERCENTAGE_DEV / 100;
        action(permission_level{_self, "active"_n}, EOS_CONTRACT, "transfer"_n, make_tuple(_self, DEV_ACCOUNT, dev_amount, string("claim reward"))).send();
        action(permission_level{_self, "active"_n}, EOS_CONTRACT, "transfer"_n, make_tuple(_self, COMPANY_ACCOUNT, (record.quantity - dev_amount), string("claim reward"))).send();
    }

    stat.modify(record, same_payer, [time](auto &a) {
        a.last_claimed = time;
        a.quantity = asset(0, EOS_SYMBOL);
    });
}

// EOS
void reward::transfer(const name &from, const name &to, const asset &quantity, const string &memo)
{
    require_auth(from);

    if (from == _self) {
        return;
    }
    check(EOS_CONTRACT == get_first_receiver(), "invalid contract");
    check(to == _self, "contract is not involved in this transfer");
    check(quantity.symbol.is_valid(), "invalid quantity");
    check(quantity.amount > 0, "only positive quantity allowed");
    check(quantity.symbol == EOS_SYMBOL, "only EOS tokens allowed");

    stats stat(_self, 0);
    auto record = stat.find(0);

    if (record == stat.end()) {
        stat.emplace(_self, [&](auto &a) {
            a.last_claimed = 0;
            a.quantity = quantity;
        });
    }
    else {
        stat.modify(record, same_payer, [quantity](auto &a) {
            a.quantity += quantity;
        });
    }
}
