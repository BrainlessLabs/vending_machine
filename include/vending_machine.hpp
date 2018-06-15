#pragma once
#include <string>
#include <vector>

namespace vm {
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

		Coin physical_value() const {
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

	/// @class This class represents currency
	/// @brief Currency class is created cause it will help out in adding and removing currency
	///        Currency are tricky to handle as the addition and substraction of currency is
	///        different for different currency. This class is going to help in doing that
	template<typename T>
	class CurrencyPolicy {
	private:
		/// @var _locale
		/// @brief This stores the string representation of the currency type.
		std::string _locale;
		/// @var _valid_denominations
		/// @brief This stores all the denominations.
		std::vector<float> _valid_denominations;

	public:
		/// #fn Currency
		/// @brief The constructor
		CurrencyPolicy(std::string const& locale = "INR") :
			_locale(locale),
			_valid_denominations{1, 2, 5, 10}
		{}

		/// #fn locale
		/// @brief The string representation of the currency type
		std::string const& locale() const {
			return _locale;
		}

		/// #fn isValidDenomination
		/// @brief Checks if the denomination is a valid one
		/// @param denom The input denomination 
		/// @return bool Returns true if the denomination is valid among the denominations
		bool isValidDenomination(const float denom) const {
			bool ret = false;
			for (const auto d : _valid_denominations) {
				if (denom == d) {
					ret = true;
					break;
				}
			}
			return ret;
		}

		/// #fn addDenomination
		/// @brief Adds a new denomination. The denominations are always sorted.
		/// @param denom The denomination to add
		void addDenomination(const float denom) {

		}

		/// #fn removeDenomination
		/// @brief Remove the said denomination. The denominations are always sorted.
		/// @param denom The denomination to remove
		void removeDenomination(const float denom) {
			for (auto it = _valid_denominations.begin(); it != _valid_denominations.end(); ++it) {
				if (denom == *it) {
					_valid_denominations.erase(it);
					break;
				}
			}
		}

		float calculateChange(const float submitted_price, const float item_price) {
			float integral_part(0);
			float actual_price = submitted_price;
			if (std::modf(submitted_price, &integral_part) >= 0.3) {
				actual_price + 1;
			}
		}
	};

	class VendingMachineManager {

	};
}