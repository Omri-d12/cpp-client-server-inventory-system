#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <stdexcept>

class Item {
private:
    int id;
    std::string name;
    bool isBorrowed;
    std::string borrowedBy;

public:
    Item(int id, const std::string& name);

    int getId() const;
    const std::string& getName() const;
    bool isAvailable() const;
    const std::string& getBorrower() const;

    void borrow(const std::string& username);
    void returnBack(const std::string& username);


    std::string toString() const;
};

#endif