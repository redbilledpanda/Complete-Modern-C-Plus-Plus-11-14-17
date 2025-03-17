#ifndef SMART_INTEGER_H
#define SMART_INTEGER_H

#include <memory> // for std::unique_ptr

class smartInteger {
public:
    // Default Constructor
    smartInteger();

    // Parameterized Constructor
    explicit smartInteger(int value);

    // Copy Constructor
    smartInteger(const smartInteger& other);

    // Move Constructor
    smartInteger(smartInteger&& other) noexcept;

    // Destructor
    ~smartInteger() = default; // Let unique_ptr handle cleanup automatically

    // Copy Assignment Operator
    smartInteger& operator=(const smartInteger& other);

    // Move Assignment Operator
    smartInteger& operator=(smartInteger&& other) noexcept;

    // Accessor
    int get() const;

    // Mutator
    void set(int newValue);

private:
    // Smart pointer holding our integer data
    std::unique_ptr<int> m_data;
};

#endif // SMART_INTEGER_H
