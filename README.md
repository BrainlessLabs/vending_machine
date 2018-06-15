# Description
This gives a simple vending machine solution in ```C++```.

# Problem Statement
Design a vending machine with the following characteristics.
* Display a list of drinks on the screen
* Allow the user to either quit or pick a drink
* If the user picks a drink, he or she will then asked to enter the amount of money to be inserted into the machine
* The change will be rendered and the drink will be displayed
* If the user does not enter enough money then the needed amount should be displayed
* The list of drinks will be displayed again

# Assumptions
* Accepts coins of following denominations only (`Rupee INR`)
* * 1
  * 2
  * 5
  * 10
* The difference is only given out in the obove denomination, so the price of the product is adjusted to the same as following
  * If the price is less than **X.3** then it is converted to X else converted to **X + 1**

# Libraries Used
* [SQLite](https://www.sqlite.org/index.html) : This is the library for database
* [rang](https://agauniyal.github.io/rang/) : Library for colorful console applications that is cross platform
* [CppConsoleTable](https://github.com/Oradle/CppConsoleTable) : Creating tables in the console
* [sqlite_modern_cpp](https://github.com/SqliteModernCpp/sqlite_modern_cpp) : A modern C++14 wrapper around SQLite
