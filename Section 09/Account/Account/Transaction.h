#pragma once
#include "Account.h"
/*
Demonstrates runtime polymorphism by operating on a base-class pointer (Account*).
This lets callers pass in any derived type (Checking/Savings) while dispatch stays virtual.
RTTI (Run-Time Type Information) lets us safely discover the dynamic type at runtime.
The paired .cpp uses RTTI via dynamic_cast to downcast so we can access derived-only data.
*/
void Transact(Account *pAccount);
