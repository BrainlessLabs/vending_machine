// vend.cpp : Defines the entry point for the console application.
//

#include "coin.hpp"
#include "coin_manager.hpp"
#include "vending_machine.hpp"
#include "application.hpp"
#include <iostream>
#include <rang.hpp>
#include <stdlib.h>
#define FMT_HEADER_ONLY
#include <fmt/format.h>

namespace vm {
	class ConsoleApplication : ApplicationI {
	public:
		using VendingMachineType = VendingMachineManager<CoinManager<vm::Coin<>>>;
	private:
		VendingMachineType _vending_machine;
		
		void printHeader() const {
			std::cout << rang::style::bold << rang::bg::blue
				<< fmt::format("{:<15}", "Unit No")
				<< fmt::format("{:<15}", "Unit Name")
				<< fmt::format("{:<15}", "Quantity")
				<< fmt::format("{:<15}", "Price")
				<< fmt::format("{:<15}", "Description")
				<< rang::style::reset << std::endl;
		}

		void printSKU(const std::uint32_t unit_no,
			std::string const& unit_name,
			const std::uint32_t quantity,
			const VendingMachineType::CoinValueType price,
			std::string const& description) const {
			std::cout << 
				rang::fg::yellow
				<< fmt::format("{:<15}", unit_no)
				<< rang::style::reset << rang::fg::green
				<< fmt::format("{:<15}", unit_name)
				<< fmt::format("{:<15}", quantity)
				<< fmt::format("{:<15}", price)
				<< fmt::format("{:<15}", description)
				<< rang::style::reset << std::endl;
		}

		void printSKUs(std::string const& skus) {
			const json j = json::parse(skus);
			const auto items = j["items"];
			for (const auto item : items) {
				printSKU(item["number"], item["item"], item["quantity"], item["price"], item["description"]);
			}
		}

		void inputPrompt() const {
			std::cout << rang::fg::red << "Enter q to exit" << rang::style::reset << std::endl;
			std::cout << rang::fg::magenta << "Please enter the Unit No to pruchase"<< rang::fg::red << "->" << rang::style::reset;
		}

		void operation(const std::string& input) {
			try {
				VendingMachineType::CoinBucketType coinBucket;
				const int sku_number = std::stoi(input);
				const std::string item_name = _vending_machine.getSKUNameFromNumber(sku_number);
				if (item_name.empty()) {
					std::cout << fmt::format("Item: {} not found.", input) << std::endl;
					char c;
					std::cin >> c;
				}
				else {
					std::cout << "Please enter " << rang::fg::blue << _vending_machine.getSKUPrice(item_name) << rang::style::reset << std::endl;
					for (const auto& coin : _vending_machine.valid_denominations()) {
						std::cout << "Please enter " << coin.digital_value << ">>";
						std::string value_str;
						std::cin >> value_str;
						const auto value = std::stoi(value_str);
						coinBucket[coin] = value;
					}
					const auto status = _vending_machine.purchase(item_name, coinBucket);
					const json j = json::parse(status);
					auto s = j["change_amount"];
					std::cout << fmt::format("Chante Amount: {}. Press any key to continue", status) << std::endl;
					char c;
					std::cin >> c;
				}
			}
			catch (std::invalid_argument const& e) {
				std::cout << "Invalid Argument. Press any key to continue" << std::endl;
				char c;
				std::cin >> c;
			}
		}
	public:
		ConsoleApplication() : _vending_machine({1, 2, 5, 10}){
			initianize();
		}

		virtual void initianize() {
			_vending_machine.addSKU("Coke", 12.5, 1000, "Coke Bottle");
			_vending_machine.addSKU("Sprite", 11, 1000, "Sprite Bottle");
			_vending_machine.addSKU("ThumbsUp", 20, 1000, "ThumbsUp Bottle");
		}

		virtual void run() {
			//VendingMachineType::CoinBucketType coinBucket = { { 1, 2 },{ 2, 4 },{ 5, 1 },{ 10, 5 } };
			//std::cout << _vending_machine.getSKUs() << std::endl;
			//std::cout << _vending_machine.canPurchase("Coke", 11) << std::endl;
			//std::cout << _vending_machine.purchase("Coke", coinBucket) << std::endl;
			
			while (true) {
				std::system("cls");
				printHeader();
				printSKUs(_vending_machine.getSKUs());
				inputPrompt();
				std::string input;
				std::cin >> input;
				if ("q" == input) {
					break;
				}
				operation(input);
			}
		}
		
		virtual void uninitialize() {

		}
	};
}

int main() {
	vm::CoinManager<vm::Coin<>> cp;
	vm::CoinManager<vm::Coin<>> cp1 = {1, 2, 5, 10};
	vm::ConsoleApplication console_app;
	console_app.run();
    return 0;
}
