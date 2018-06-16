#pragma once
#include <string>
#include <vector>
#include <set>
#include <atomic>
#include <map>
#include <initializer_list>

namespace vm {
	template <typename T>
	class reverse_range {
		T& x;

	public:
		reverse_range(T& x) : x(x) {}

		auto begin() const -> decltype(this->x.rbegin()) {
			return x.rbegin();
		}

		auto end() const -> decltype(this->x.rend()) {
			return x.rend();
		}
	};

	template <typename T>
	auto reverse_iterate(T& x) -> reverse_range<T> {
		return reverse_range<T>(x);
	}

	template<int T = 'INR'>
	struct Coin {
		using ValueType = float;
		static const int digital_type = T;

		ValueType digital_value;
		Coin() : value(0) {}
		Coin(const ValueType value) : digital_value(value) {}
		Coin(Coin const& coin) {
			digital_value = coin.digital_value;
		}

		Coin& operator=(ValueType const value) {
			digital_value = value;
			return *this;
		}

		bool operator==(Coin const& coin) const {
			return digital_value == coin.digital_value;
		}

		bool operator!=(Coin const& coin) const {
			return !(coin == !this);
		}

		bool operator>(Coin const& coin) {
			return digital_value > coin.digital_value;
		}

		bool operator>=(Coin const& coin) {
			return digital_value >= coin.digital_value;
		}

		bool operator<(Coin const& coin) {
			return digital_value < coin.digital_value;
		}

		Coin operator-(Coin const& coin) const {
			ValueType value = 0;
			if (digital_value >= coin.digital_value) {
				value = digital_value - coin.digital_value;
			}

			return Coin(value);
		}

		Coin operator+(Coin const& coin) const {
			return Coin(digital_value  + coin.digital_value);
		}

		explicit operator ValueType() const {
			return digital_value;
		}

		ValueType physical_value() const {
			ValueType integral_part = 0;
			const auto point_value = std::modf(digital_value, &integral_part);
			if (point_value > 0.0) {
				if (point_value < 0.5) {
					integral_part += 0.5;
				}
				else if (integral_part > 0.5) {
					integral_part += 1.0;
				}
			}
			return integral_part;
		}
	};

	bool operator<(Coin<'INR'> const& lhs, Coin<'INR'> const& rhs) {
		return lhs.digital_value < rhs.digital_value;
	}

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

		CoinManager(std::initializer_list<CoinType> coins): _valid_denominations(coins){}

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

	template<typename CoinT, typename CoinManagerT>
	class VendingMachineManager {
	public:
		using CoinType = CoinT;
		using CoinManagerType = CoinManagerT<CoinType>;
		using CoinValueType = typename CoinType::ValueType;
		using CoinBucketType = typename CoinManagerType::CoinBucketType;

		struct SKU {
			std::string description;
			CoinValueType value;
			std::uint32_t count;

			SKU(const CoinValueType coin_value,
				const std::uint32_t count,
				std::string const& description) {}
		};

	private:
		std::map<std::string, SKU> _skus;
		CoinManagerType _coin_manager;

	public:
		VendingMachineManager() = default;

		bool addSKU(
			std::string const& sku_name,
			const CoinValueType value,
			const std::uint32_t count,
			std::string const& description = "NA") {
			bool insert_success = false;
			if (!_skus.count(sku_name)) {
				_skus.emplace({ sku_name, {value, count, description} });
				insert_success = true;
			}
			return insert_success;
		}

		bool updateSKUCount(
			std::string const& sku_name,
			const std::uint32_t count) {
			bool update_success = false;
			auto it = _skus.find(sku_name);
			if(it != _skus.end()){
				it->second.count = count;
				update_success = true;
			}
			return update_success;
		}

		bool updateSKUValue(std::string const& sku_name, const CoinValueType value) {
			bool update_success = false;
			auto it = _skus.find(sku_name);
			if (it != _skus.end()) {
				it->second.value = value;
				update_success = true;
			}
			return update_success;
		}

		bool updateSKUDescription(std::string const& sku_name, std::string const& description) {
			bool update_success = false;
			auto it = _skus.find(sku_name);
			if (it != _skus.end()) {
				it->second.description = description;
				update_success = true;
			}
			return update_success;
		}

		auto canPurchase(std::string const& sku_name,
			const CoinValueType input_value) -> std::pair<CoinValueType, bool> {
			std::pair<CoinValueType, bool> purchase_success = { 0, false };
			auto it = _skus.find(sku_name);
			if (it !_skus.end()) {
				const auto price = it->second.value;
				if (price <= input_value) {
					purchase_success.second = true;
				}
				else {
					purchase_success.first = price - input_value;
					purchase_success.second = false;
				}
			}

			return purchase_success;
		}

		auto purchase(
			std::string const& sku_name,
			const CoinValueType value) -> std::pair<CoinValueType, bool> {

		}
	};
}