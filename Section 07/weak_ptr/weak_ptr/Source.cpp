#include <iostream>
#include <memory>

class Printer {
    // The weak_ptr member that observes but doesn't own the resource
    std::weak_ptr<int> m_pValue{};

public:
    // Overload 1: Takes weak_ptr by const reference
    // - Most efficient for existing weak_ptrs (no parameter copy)
    // - Can accept temporaries (like those created from a shared_ptr)
    // Use this when the caller already has a weak_ptr or when you want
    // to avoid an extra copy of the weak_ptr object.
    void SetObserver(const std::weak_ptr<int>& p) {
        std::cout << "[SetObserver(const weak_ptr&)] Using weak_ptr reference (no param copy)\n";
        m_pValue = p;  // assign from reference
    }

    // Named alternative: Takes weak_ptr by value
    // - Makes a copy of the input weak_ptr
    // - Useful when the callee needs to keep/modify the parameter copy
    // - Demonstrates where by-value may be preferred (explicit local ownership)
    void SetObserverByValue(std::weak_ptr<int> p) {
        std::cout << "[SetObserverByValue(weak_ptr)] Using weak_ptr by-value (param copied)\n";
        m_pValue = std::move(p);  // move into member
    }

    // Overload 2: Takes shared_ptr by const reference
    // - Convenient when caller holds a shared_ptr
    // - No extra temporary weak_ptr needs to be created by the caller
    // - Exact-match overload when a shared_ptr is passed (no ambiguity)
    void SetObserver(const std::shared_ptr<int>& p) {
        std::cout << "[SetObserver(const shared_ptr&)] Converting shared_ptr to weak_ptr\n";
        m_pValue = p;  // implicit conversion to weak_ptr
    }

    // Print the current state of the observed value
    // - Demonstrates proper weak_ptr usage pattern
    // - Shows reference counting information
    // - Handles expired pointers safely
    void Print(const std::string& label = "") const {
        std::cout << "\n=== " << (label.empty() ? "Observer State" : label) << " ===\n";
        
        // Try to lock the weak_ptr to get a shared_ptr
        if (auto sp = m_pValue.lock()) {
            std::cout << "Value: " << *sp << "\n"
                     << "Reference count: " << sp.use_count() << "\n"
                     << "Status: Valid (observable)\n";
        } else {
            std::cout << "Status: Expired (resource no longer available)\n";
        }
    }
};

int main() {
    std::cout << "\n=== Weak Pointer Observer Pattern Demo ===\n";
    
    // Create our resource with a shared_ptr
    auto resource = std::make_shared<int>(42);
    std::cout << "Initial reference count: " << resource.use_count() << "\n";
    
    Printer prn;
    
    // Demonstrate all three observer setting methods
    std::cout << "\n1. Using shared_ptr directly (calls shared_ptr overload):\n";
    prn.SetObserver(resource); // calls SetObserver(const shared_ptr&)
    prn.Print("After SetObserver(shared_ptr)");
    
    std::cout << "\n2. Using existing weak_ptr by reference:\n";
    {
        std::weak_ptr<int> weak = resource;  // Create weak_ptr
        prn.SetObserver(weak); // calls SetObserver(const weak_ptr&)
        prn.Print("After SetObserver(weak_ptr&)");
    }  // weak goes out of scope, but m_pValue still valid
    
    std::cout << "\n3. Using weak_ptr by value:\n";
    {
        std::weak_ptr<int> weak = resource;
        prn.SetObserverByValue(weak); // explicit by-value call
        prn.Print("After SetObserverByValue");
    }
    
    // Demonstrate behavior when resource is destroyed
    std::cout << "\nResetting the shared_ptr (destroying resource):\n";
    resource.reset();
    prn.Print("After resource destruction");
    
    return 0;
}
