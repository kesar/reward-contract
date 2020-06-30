# Reward.Contracts 1.0.0

## Dependencies:
* [eosio v1.8.5](https://github.com/eosio/eos/releases/tag/v1.8.5)
* [eosio.cdt v1.7.0-rc1](https://github.com/EOSIO/eosio.cdt/releases/tag/v1.7.0-rc1)

To build the contracts and the unit tests:
* First, ensure that your __eosio__ is compiled to the core symbol for the EOSIO blockchain that intend to deploy to.
* Second, make sure that you have ```sudo make install```ed __eosio__.

After build:
* The unit tests executable is placed in the _build/tests_ and is named __unit_test__.
* The contracts are built into a _bin/\<contract name\>_ folder in their respective directories.
* Finally, simply use __cleos__ to _set contract_ by pointing to the previously mentioned directory.

Build & Run tests:
```
./build.sh -y && ./build/tests/unit_test
```

From docker:
```
docker pull orbiterco/eosio
docker run -t -i -v $(pwd):/reward orbiterco/eosio -c "export BOOST_ROOT=/root/eosio/1.8/src/boost_1_70_0 && cd /reward && /bin/bash /reward/build.sh -y"
```
