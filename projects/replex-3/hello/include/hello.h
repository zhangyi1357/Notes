#pragma once
#include <replex.h>

#include <array>

inline std::array<const char*, 2> g_exports = {"foo", "bar"};

class HelloModule : public ReplexModule<HelloModule, g_exports.size()> {
   public:
    static void Foo() { GetInstance().Execute<void>("foo"); }

    static int GetBar() { return *GetInstance().GetVar<int>("bar"); }

   protected:
    virtual const char* GetPath() const override {
        return "./build/hello/libhello.so";
    }

    virtual std::array<const char*, g_exports.size()>& GetSymbolNames()
        const override {
        return g_exports;
    }
};