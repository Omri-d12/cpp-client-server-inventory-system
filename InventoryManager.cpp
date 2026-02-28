#include "InventoryManager.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

InventoryManager::InventoryManager() {
    items.emplace_back(1, "Camera");
    items.emplace_back(2, "Tripod");
    items.emplace_back(3, "Laptop");
    items.emplace_back(4, "Projector");
    items.emplace_back(5, "Microphone");
    items.emplace_back(6, "Speaker");
    items.emplace_back(7, "HDMI_Cable");
    items.emplace_back(8, "Ethernet_Cable");
    items.emplace_back(9, "Keyboard");
    items.emplace_back(10, "Mouse");
    items.emplace_back(11, "Monitor");
    items.emplace_back(12, "USB_Hub");
    items.emplace_back(13, "Power_Bank");
    items.emplace_back(14, "Router");
    items.emplace_back(15, "VR_Headset");

}

std::string InventoryManager::listItems() {
    std::unique_lock<std::mutex> lock(mtx);
    std::ostringstream ss;
    ss << "OK LIST " << items.size() << "\n";

    for (const Item &item : items) {
        ss << item.toString()<< "\n";
    }
    return ss.str();
}

Item& InventoryManager::getItemById(int itemid) {
    for (Item& item : items) {
        if (item.getId() == itemid) {
            return item;
        }

    }
    throw std::runtime_error("NOT_FOUND item");
}

void InventoryManager::borrowItem(int itemId, const std::string &username) {
    std::unique_lock<std::mutex> lock(mtx);

    Item& item = getItemById(itemId);
    item.borrow(username);

}
void InventoryManager::returnItem(int itemId, const std::string &username) {
    std::unique_lock<std::mutex> lock(mtx);
    Item& item = getItemById(itemId);
    item.returnBack(username);

    itemCvs[itemId].notify_all();
    }


void InventoryManager::waitUntilAvailable(int itemId, const std::string& username) {
    std::unique_lock<std::mutex> lock(mtx);

    Item& item = getItemById(itemId);

    if (item.getBorrower()==username) {
        throw std::runtime_error("DEADLOCK item");
    }

    if (item.isAvailable()) {
        return;
    }
    auto& cv = itemCvs[itemId];
    cv.wait(lock, [&] {
        return item.isAvailable();
    });




}
