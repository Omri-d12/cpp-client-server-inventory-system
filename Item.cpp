#include "Item.h"

#include <sstream>
#include <stdexcept>

Item::Item(int id, const std::string& name)
    : id(id),
      name(name),
      isBorrowed(false),
      borrowedBy("")
{
}

int Item::getId() const {
    return id;
}

const std::string& Item::getName() const {
    return name;
}

bool Item::isAvailable() const {
    return !isBorrowed;
}

const std::string& Item::getBorrower() const {
    return borrowedBy;
}

void Item::borrow(const std::string& username) {
    if (isBorrowed) {
        throw std::runtime_error("borrowed_by="+borrowedBy);
    }
    if (username.empty() or username.find(' ') != std::string::npos) {
        throw std::invalid_argument("invalid username");
    }
    borrowedBy = username;
    isBorrowed = true;
}

void Item::returnBack(const std::string& username) {
    if (!isBorrowed) {
        throw std::runtime_error("item is not borrowed");
    }
    if (borrowedBy != username) {
        throw std::runtime_error("PERMISSION not_owner");
    }
    isBorrowed = false;
    borrowedBy = "";
}


std::string Item::toString() const {
    if (isBorrowed) {
        return std::to_string(id) + " " + name + " BORROWED by=" + borrowedBy;
    } else {
        return std::to_string(id) + " " + name + " FREE";
    }
}