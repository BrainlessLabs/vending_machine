#pragma once
#include <string>
#include <vector>
#include <set>
#include <atomic>
#include <map>
#include <initializer_list>
#include "reverse_iterator.hpp"

namespace vm {
	/// @class This class represents currency
	/// @brief Currency class is created cause it will help out in adding and removing currency
	///        Currency are tricky to handle as the addition and substraction of currency is
	///        different for different currency. This class is going to help in doing that
	template<typename Coin>
	class CoinManager {
	public:
		using CoinType = Coin;
		using CoinValueType = typename Coin::ValueType;
		using CoinBucketType = std::map<CoinType, std::atomic<std::uint32_t>>;
		using CoinChangeType = std::map<CoinType, std::atomic<std::uint32_t>::value_type>;

	private:
		/// @var _valid_denominations
		/// @brief This stores all the denominations.
		std::set<CoinType> _valid_denominations;
		CoinBucketType _coin_buckets;

	public:
		CoinManager() = default;

		CoinManager(std::initializer_list<CoinType> coins) : _valid_denominations(coins) {}

		/// #fn isValidDenomination
		/// @brief Checks if the denomination is a valid one
		/// @param denom The input denomination 
		/// @return bool Returns true if the denomination is valid among the denominations
		bool isValidDenomination(const CoinValueType denom) const {
			return _valid_denominations.find(denom) != _valid_denominations.end();
		}

		/// @fn addDenomination
		/// @brief Adds a new denomination.
		/// @param denom The denomination to add
		bool addDenomination(const CoinValueType denom) {
			const auto ret = _valid_denominations.emplace(denom);
			return ret.second;
		}

		/// @fn removeDenomination
		/// @brief Remove the said denomination.
		/// @param denom The denomination to remove
		bool removeDenomination(const CoinValueType denom) {
			const auto ret = _valid_denominations.erase(denom);
			return ret.second;
		}

		auto addCoins(CoinType const& coin, const std::uint32_t count) -> typename CoinBucketType::mapped_type::value_type {
			return _coin_buckets[coin] += count;
		}

		CoinValueType addCoins(CoinChangeType const& coins) {
			CoinType coin_value;
			for (const auto coin : coins) {
				coin_value += addCoins(coin.first, coin.second);
			}
			return coin_value.physical_value();
		}

		auto removeCoins(CoinType const& coin, const std::uint32_t count) -> typename CoinBucketType::mapped_type::value_type {
			return _coin_buckets[coin] -= count;
		}

		CoinValueType removeCoins(CoinChangeType const& coins) {
			CoinType coin_value;
			for (const auto coin : coins) {
				coin_value += removeCoins(coin.first, coin.second);
			}
			return coin_value.physical_value();
		}

		auto coinCount(CoinType const& coin) -> typename CoinBucketType::mapped_type::value_type {
			return _coin_buckets[coin].load();
		}

		CoinChangeType renderChange(const CoinValueType change) {
			CoinChangeType ret;

			if (change > 0 && !_coin_buckets.empty()) {
				CoinChangeType temp_coin_bucket;

				for (const auto it : _coin_buckets) {
					temp_coin_bucket[it.first] = it.second.load();
				}

				auto ChangeCount = change;
				for (const auto cur_denomination : reverse_range<decltype(_valid_denominations)>(_valid_denominations)) {
					if (ChangeCount > 0 && temp_coin_bucket[cur_denomination] > 0) {
						while (cur_denomination.digital_value < ChangeCount) {
							ChangeCount -= cur_denomination.digital_value;
							--temp_coin_bucket[cur_denomination];
							++ret[cur_denomination];
						}
					}
				}
			}

			if (0 != ChangeCount) {
				ret.clear();
			}

			return std::move(ret);
		}

		static CoinValueType calculateChange(const CoinValueType submitted_price, const CoinValueType item_price) {
			CoinValueType integral_part(0);
			CoinValueType actual_price = submitted_price;
			if (std::modf(submitted_price, &integral_part) > 0.3) {
				actual_price + 1;
			}

			return actual_price;
		}
	};
}