#pragma once
#include <array>

#include "replex.h"

inline std::array<const char*, 2> g_exports = {"foo", "bar"};

class HelloModule : public ReplexModule<HelloModule, g_exports.size()> {
   public:
    static void Foo() {
        // Execute might throw, but we don't bother catching the exception here
        // for brevity
        GetInstance().Execute<void>("foo");
    }

    static int GetBar() {
        // decltype is a relatively new operator. decltype(bar) resolves to int
        // Note that this function does not protect against retrieving nullptr
        return *GetInstance().GetVar<int>("bar");
    }

   protected:
    virtual const char* GetPath() const override { return "libhello.so"; }

    virtual std::array<const char*, g_exports.size()>& GetSymbolNames()
        const override {
        return g_exports;
    }
};