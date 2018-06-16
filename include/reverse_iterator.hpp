#pragma once

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
}