#include "smartInteger.h"
#include <utility>    // for std::move

// 1) Default Constructor
smartInteger::smartInteger()
    : m_data(std::make_unique<int>(0))
{
    // Nothing else needed; unique_ptr manages resource
}

// 2) Parameterized Constructor
smartInteger::smartInteger(int value)
    : m_data(std::make_unique<int>(value))
{
    // Same idea; store the given value in unique_ptr
}

// 3) Copy Constructor
smartInteger::smartInteger(const smartInteger& other)
    : m_data(std::make_unique<int>(*other.m_data))
{
    // Creates a new int with the value contained in 'other'
}

// 4) Move Constructor
smartInteger::smartInteger(smartInteger&& other) noexcept
    : m_data(std::move(other.m_data))
{
    // Transfer ownership from 'other'
    // 'other.m_data' becomes nullptr
}

// 5) Copy Assignment Operator
smartInteger& smartInteger::operator=(const smartInteger& other)
{
    if (this != &other) {
        // Create a new int from the other's value
        m_data = std::make_unique<int>(*other.m_data);
    }
    return *this;
}

// 6) Move Assignment Operator
smartInteger& smartInteger::operator=(smartInteger&& other) noexcept
{
    if (this != &other) {
        // Transfer ownership from 'other'
        m_data = std::move(other.m_data);
    }
    return *this;
}

// Destructor is defaulted in the header, so nothing to do here

// 7) Accessor
int smartInteger::get() const
{
    return *m_data;
}

// 8) Mutator
void smartInteger::set(int newValue)
{
    *m_data = newValue;
}