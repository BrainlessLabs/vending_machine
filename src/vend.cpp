// vend.cpp : Defines the entry point for the console application.
//

#include "vending_machine.hpp"

int main() {
	vm::CoinManager<vm::Coin<>> cp;
	vm::CoinManager<vm::Coin<>> cp1 = {1, 2, 5};
    return 0;
}
