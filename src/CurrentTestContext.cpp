#include <TestExplorer/CurrentTestContext.hpp>
#include <TestExplorer/TestContext.hpp>

#include <stdexcept>

namespace testexplorer
{
    namespace
    {
        thread_local TestContext *currentContext = nullptr;
    }

    void CurrentTestContext::set(TestContext &context)
    {
        currentContext = &context;
    }

    void CurrentTestContext::clear()
    {
        currentContext = nullptr;
    }

    TestContext &CurrentTestContext::get()
    {
        if (currentContext == nullptr)
        {
            throw std::logic_error(
                "No active TestContext");
        }

        return *currentContext;
    }
}

/*
یک نکته مهم هم داریم: بعداً که Exception Handling را اضافه کنیم، clear() را باید طوری مدیریت کنیم که حتی اگر تست exception داد هم current context باقی نماند. فعلاً چون Exception Handling هنوز milestone خودش نیست، وارد آن نمی‌شویم.
*/