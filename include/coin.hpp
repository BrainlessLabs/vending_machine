#pragma once
#include <string>
#include <vector>
#include <set>
#include <atomic>
#include <map>
#include <initializer_list>

namespace vm {
	template<int T = 'INR'>
	struct Coin {
		using ValueType = float;
		static const int digital_type = T;

		ValueType digital_value;

		Coin() : digital_value(0) {}
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
			return Coin(digital_value + coin.digital_value);
		}

		Coin& operator+=(Coin const& coin) {
			digital_value += coin.digital_value;
			return *this;
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
}