#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

using eosio::asset;
using eosio::name;
using eosio::symbol;
using eosio::symbol_code;
using eosio::check;
using eosio::permission_level;
using eosio::current_time_point;
using eosio::action;
using eosio::same_payer;
using std::string;
using std::vector;
using std::make_tuple;

class [[eosio::contract]] reward : public eosio::contract
{
  public:
    using contract::contract;

    [[eosio::action]] void claim();
    [[eosio::on_notify("eosio.token::transfer")]] void transfer(const name &from, const name &to, const asset &quantity, const string &memo);

private:
    struct [[eosio::table]] stats_t {
        uint32_t last_claimed;
        asset quantity;
        uint64_t primary_key() const {
            return 0;
        }
    };

    typedef eosio::multi_index<name("stats"), stats_t> stats;

    static constexpr name DEV_ACCOUNT = "kesaritooooo"_n;
    static constexpr name COMPANY_ACCOUNT = "kesaritooooo"_n;
    static constexpr uint32_t PERCENTAGE_DEV = 90;
    static constexpr uint32_t TIME_TO_CLAIM = 86400; // 1 day
    static constexpr name EOS_CONTRACT = "eosio.token"_n;
    static constexpr symbol EOS_SYMBOL = symbol(symbol_code("EOS"), 4);
};
