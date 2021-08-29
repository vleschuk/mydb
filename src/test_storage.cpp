#include <iostream>
#include "storage.h"

int main() {
  {
    my::Storage s("test.db");
    std::cout << "Adding key1:value1" << std::endl;
    auto res = s.addRecord("key1", "value1");
    std::cout << "Result: " << my::strError(res) << std::endl;
    std::cout << "Adding key2:value2" << std::endl;
    res = s.addRecord("key2", "value2");
    std::cout << "Result: " << my::strError(res) << std::endl;
    std::cout << "Adding key2:value2" << std::endl;
    res = s.addRecord("key2", "value2");
    std::cout << "Result: " << my::strError(res) << std::endl;

    std::string val;
    std::cout << "Getting value by key1" << std::endl;
    res = s.getValue("key1", val);
    std::cout << "Result: " << my::strError(res) << " Value: " << val << std::endl;

    val.clear();
    std::cout << "Getting value by key3" << std::endl;
    res = s.getValue("key3", val);
    std::cout << "Result: " << my::strError(res) << " Value: " << val << std::endl;
  }
  {
    my::Storage s("test.db");

    std::string val;
    std::cout << "Getting value by key1" << std::endl;
    auto res = s.getValue("key1", val);
    std::cout << "Result: " << my::strError(res) << " Value: " << val << std::endl;

    val.clear();
    std::cout << "Getting value by key2" << std::endl;
    res = s.getValue("key2", val);
    std::cout << "Result: " << my::strError(res) << " Value: " << val << std::endl;

    std::cout << "Updating value by key2" << std::endl;
    res = s.updateRecord("key2", "updated_value2");
    std::cout << "Result: " << my::strError(res) << std::endl;
    val.clear();
    std::cout << "Getting value by key2" << std::endl;
    res = s.getValue("key2", val);
    std::cout << "Result: " << my::strError(res) << " Value: " << val << std::endl;

    std::cout << "Deleting value by key1" << std::endl;
    res = s.removeRecord("key1");
    std::cout << "Result: " << my::strError(res) << std::endl;
    std::cout << "Deleting value by key2" << std::endl;
    res = s.removeRecord("key2");
    std::cout << "Result: " << my::strError(res) << std::endl;
  }

}
