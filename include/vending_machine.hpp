#pragma once
#include <string>
#include <vector>
#include <set>
#include <atomic>
#include <map>
#include <initializer_list>
#include <json.hpp>

namespace vm {
	using json = nlohmann::json;

	template<typename CoinManagerT>
	class VendingMachineManager {
	public:
		using CoinType = typename CoinManagerT::CoinType;
		using CoinManagerType = CoinManagerT;
		using CoinValueType = typename CoinType::ValueType;
		using CoinBucketType = typename CoinManagerType::CoinChangeType;

		struct SKU {
			CoinValueType value;
			std::uint32_t count;
			std::uint32_t sku_number;
			std::string description;

			SKU(const CoinValueType coin_value,
				const std::uint32_t count,
				const std::uint32_t sku_number,
				std::string const& description) : value(coin_value), count(count), sku_number(sku_number), description(description){}
		};

	private:
		std::map<std::string, SKU> _skus;
		CoinManagerType _coin_manager;

		std::uint32_t get_next_number() const {
			return static_cast<std::uint32_t>(_skus.size() + 1);
		}

	public:
		VendingMachineManager() = default;

		VendingMachineManager(std::initializer_list<CoinType> coins): _coin_manager(coins){
			// TODO Modify so that later we can have take this from external input
			for (auto coin : _coin_manager.valid_denominations()) {
				_coin_manager.addCoins(coin, 100);
			}
		}

		bool updateCoins(CoinType const& coin, const std::uint32_t count) {
			return true;
		}

		bool addSKU(
			std::string const& sku_name,
			const CoinValueType value,
			const std::uint32_t count,
			std::string const& description = "NA") {
			bool insert_success = false;
			if (!_skus.count(sku_name)) {
				_skus.emplace(sku_name, SKU{value, count, get_next_number(), description});
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

		/// @details Returns a json string {status: "SUCCESS", due: 0}
		std::string canPurchase(std::string const& sku_name,
			const CoinValueType input_value) const {
			json j;
			const auto it = _skus.find(sku_name);
			if (it != _skus.end()) {
				const auto price = it->second.value;
				if (price <= input_value) {
					j["due"] = 0;
					j["status"] = true;
				}
				else {
					j["due"] = price - input_value;
					j["status"] = false;
				}
			}
			 
			return j.dump();
		}

		bool hasSKU(std::string const& sku_name) const {
			const auto it = _skus.find(sku_name);
			return it != _skus.end() && it->second.count > 0;
		}

		/// @details {"status": "SUCCESS", "error_message": "", "change" : [{}]}
		std::string purchase(
			std::string const& sku_name,
			const CoinBucketType& coins) {
			json j;
			CoinBucketType coin_bucket;
			auto it = _skus.find(sku_name);
			if (it != _skus.end() && it->second.count > 0) {
				const auto coins_added = _coin_manager.addCoins(coins);
				if (coins_added > 0) {
					it->second.count--;
					const auto change_amount = _coin_manager.calculateChange(coins_added, it->second.value);
					const auto change = _coin_manager.renderChange(change_amount);
					j["paid_amount"] = coins_added;
					j["change_amount"] = change_amount;
					j["status"] = true;
					j["changes"] = coinBucketTypeToJson(change);
				}
				else {
					j["status"] = false;
					j["error_message"] = "Could not process transaction";
				}
			}
			else {
				j["status"] = false;
				j["error_message"] = "Could not find the item";
			}

			return j.dump();
		}

		static json coinBucketTypeToJson(CoinBucketType const& coins) {
			std::vector<json> v;
			for (const auto coin : coins) {
				json o;
				o["coin_value"] = coin.first.digital_value;
				o["coin_count"] = coin.second;
				v.push_back(o);
			}
			return v;
		}

	};
}