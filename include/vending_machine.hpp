#pragma once
#include <string>
#include <vector>
#include <set>
#include <atomic>
#include <map>
#include <initializer_list>

namespace vm {
	template<typename CoinManagerT>
	class VendingMachineManager {
	public:
		using CoinType = typename CoinManagerT::Coin;
		using CoinManagerType = CoinManagerT;
		using CoinValueType = typename CoinType::ValueType;
		using CoinBucketType = typename CoinManagerType::CoinBucketType;

		struct SKU {
			std::string description;
			CoinValueType value;
			std::uint32_t count;
			std::uint32_t sku_number;

			SKU(const CoinValueType coin_value,
				const std::uint32_t count,
				const std::uint32_t sku_number,
				std::string const& description) {}
		};

	private:
		std::map<std::string, SKU> _skus;
		CoinManagerType _coin_manager;

		std::uint32_t get_next_number() const {
			return _skus.size() + 1;
		}

	public:
		VendingMachineManager() = default;

		bool addSKU(
			std::string const& sku_name,
			const CoinValueType value,
			const std::uint32_t count,
			std::string const& description = "NA") {
			bool insert_success = false;
			if (!_skus.count(sku_name)) {
				_skus.emplace({ sku_name, {value, count, get_next_number(), description} });
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

		std::pair<CoinValueType, bool> canPurchase(std::string const& sku_name,
			const CoinValueType input_value) const {
			std::pair<CoinValueType, bool> purchase_success = { 0, false };
			const auto it = _skus.find(sku_name);
			if (it != _skus.end()) {
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

		bool hasSKU(std::string const& sku_name) const {
			const auto it = _skus.find(sku_name);
			return it != _skus.end() && it->second.count > 0;
		}

		auto purchase(
			std::string const& sku_name,
			const CoinBucketType& coins) -> std::pair<CoinBucketType, bool> {
			std::pair<CoinBucketType, bool> success_status;
			auto it = _skus.find(sku_name);
			if (it != _skus.end() && it->second.count > 0) {
				const auto coins_added = _coin_manager.addCoins(coins);
				if (coins_added > 0) {
					it->second.count--;
					const auto change_amount = _coin_manager.calculateChange(coins_added);
					success_status.first = _coin_manager.renderChange(change_amount);
				}
			}

			return std::move(success_status);
		}
	};
}