#pragma once
#include "Item.h"
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <queue>
#include <unordered_map>

class InventoryManager {
private:
        std::vector<Item> items;
        std::mutex mtx;
        std::condition_variable cv;
        std::unordered_map<int, std::condition_variable> itemCvs;
        Item& getItemById(int itemId);


public:
    InventoryManager();
    std::string listItems() ;
    void borrowItem(int itemId, const std::string& username);
    void returnItem(int itemId, const std::string& username);
    void waitUntilAvailable(int itemId, const std::string& username);

};