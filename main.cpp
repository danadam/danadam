#include "loggerf.h"
#include "loggerqt.h"

#include <list>
#include <map>

#include "itoa.h"
#include "stringutils.h"
#include "emailvalidator.h"
#include "stringenum.h"
#include "daalgorithm.h"
#include "dafunctional.h"

INIT_LOGGER();

void test_loggerf()
{
    TRACE("%1(): --------------------------------").arg(__func__);

    int i = 42;
    char str[] = "meaning of life";

    TRACEF("This is trace");
    INFOF("This is info");
    WARNF("This is warn");
    ERRORF("This is error");

    TRACEF("Plain message, without arguments");
    TRACEF("Message with arguments: %s - %d", str, i);
    TRACEF("  * Let's increase log level to info");
    da::setLogLevel(da::ELogLevel::info);
    TRACEF("!!! This shouldn't be displayed !!!");
    INFOF("  *** This should still be displayed, let's go back to trace level");
    da::setLogLevel(da::ELogLevel::trace);
    TRACEF("***** This should be displayed");

    da::setLogFormat(false, false, false);
    TRACEF("Message with not header");

    da::setLogFormat(true, false, false);
    TRACEF("Message with datetime");

    da::setLogFormat(true, true, false);
    TRACEF("Message with datetime and level");

    da::setLogFormat(true, true, true);
    TRACEF("Message with datetime and level and place");
}

void test_loggerqt()
{
    TRACE("%1(): --------------------------------").arg(__func__);
    int i = 42;
    char str[] = "meaning of life";

    TRACE("This is trace");
    INFO("This is info");
    WARN("This is warn");
    ERROR("This is error");

    TRACE("Plain message, without arguments");
    TRACE("Message with arguments: %1 - %2").arg(str).arg(i);
    TRACE("  * Let's increase log level to info");
    setLogLevel(da::ELogLevel::info);
    TRACE("!!! This shouldn't be displayed !!!");
    INFO("  *** This should still be displayed, let's go back to trace level");
    da::setLogLevel(da::ELogLevel::trace);
    TRACE("***** This should be displayed");

    da::setLogFormat(false, false, false);
    TRACE("Message with not header");

    da::setLogFormat(true, false, false);
    TRACE("Message with datetime");

    da::setLogFormat(true, true, false);
    TRACE("Message with datetime and level");

    da::setLogFormat(true, true, true);
    TRACE("Message with datetime and level and place");
}

void test_itoa()
{
    TRACE("%1(): --------------------------------").arg(__func__);

    int n = 0;

    const int bufsz = 6;
    char buf[bufsz];

    bool ok = false;
    int written = 0;

    n = 123;
    written = da::itoa(n, buf, bufsz, &ok);
    TRACE("n = %1, buf = %2, bufsz = %3, written = %4, truncated? %5").arg(n).arg(buf).arg(bufsz).arg(written).arg(!ok);

    n = 12345;
    written = da::itoa(n, buf, bufsz, &ok);
    TRACE("n = %1, buf = %2, bufsz = %3, written = %4, truncated? %5").arg(n).arg(buf).arg(bufsz).arg(written).arg(!ok);

    n = 123456;
    written = da::itoa(n, buf, bufsz, &ok);
    TRACE("n = %1, buf = %2, bufsz = %3, written = %4, truncated? %5").arg(n).arg(buf).arg(bufsz).arg(written).arg(!ok);

    n = -123;
    written = da::itoa(n, buf, bufsz, &ok);
    TRACE("n = %1, buf = %2, bufsz = %3, written = %4, truncated? %5").arg(n).arg(buf).arg(bufsz).arg(written).arg(!ok);

    n = -1234;
    written = da::itoa(n, buf, bufsz, &ok);
    TRACE("n = %1, buf = %2, bufsz = %3, written = %4, truncated? %5").arg(n).arg(buf).arg(bufsz).arg(written).arg(!ok);

    n = -12345;
    written = da::itoa(n, buf, bufsz, &ok);
    TRACE("n = %1, buf = %2, bufsz = %3, written = %4, truncated? %5").arg(n).arg(buf).arg(bufsz).arg(written).arg(!ok);
}

void test_escapeString_case(
        const std::string & specialCharacters,
        char escapeCharacter,
        const std::string & str,
        const std::string & expected
    )
{
#define TEST_OUTPUT_MSG(id) \
    "%d: IN: |%s| OUT: |%s| REV: |%s| - %s, %s", \
        id, \
        str.c_str(), \
        escaped.c_str(), \
        unescaped.c_str(), \
        escapedOk ? "OK" : "Fail", \
        unescapedOk ? "OK" : "Fail"

    {
        const std::string escaped = da::escapeString(str, specialCharacters, escapeCharacter);
        const std::string unescaped = da::unescapeString(escaped, escapeCharacter);
        const bool escapedOk = escaped == expected;
        const bool unescapedOk = str == unescaped;
        if (escapedOk && unescapedOk)
            TRACEF( TEST_OUTPUT_MSG(1) );
        else
            WARNF( TEST_OUTPUT_MSG(1) );

    }

    {
        std::string specialWithEscape = specialCharacters + escapeCharacter;
        const std::string escaped = da::escapeString(str, specialWithEscape, escapeCharacter);
        const std::string unescaped = da::unescapeString(escaped, escapeCharacter);
        const bool escapedOk = escaped == expected;
        const bool unescapedOk = str == unescaped;

        if (escapedOk && unescapedOk)
            TRACEF( TEST_OUTPUT_MSG(2) );
        else
            WARNF( TEST_OUTPUT_MSG(2) );
    }

#undef TEST_OUTPUT_MSG
}

void test_escapeString()
{
    TRACE("%1(): --------------------------------").arg(__func__);

    test_escapeString_case(":,", '^', "", "");
    test_escapeString_case(":,", '^', "a", "a");
    test_escapeString_case(":,", '^', "abc", "abc");


    test_escapeString_case(":,", '^', ":abc", "^:abc");
    test_escapeString_case(":,", '^', "abc:", "abc^:");
    test_escapeString_case(":,", '^', "abc:def", "abc^:def");
    test_escapeString_case(":,", '^', "abc:def:ghi", "abc^:def^:ghi");

    test_escapeString_case(":,", '^', "abc:def,ghi:jkl,mno", "abc^:def^,ghi^:jkl^,mno");

    test_escapeString_case(":,", '^', "^abc", "^^abc");
    test_escapeString_case(":,", '^', "abc^", "abc^^");
    test_escapeString_case(":,", '^', "abc^def", "abc^^def");

    test_escapeString_case(":,", '^', ":", "^:");
    test_escapeString_case(":,", '^', "::", "^:^:");
    test_escapeString_case(":,", '^', ":,:", "^:^,^:");

    test_escapeString_case(":,", '^', "^", "^^");
    test_escapeString_case(":,", '^', "^^", "^^^^");
    test_escapeString_case(":,", '^', "^^^", "^^^^^^");

    test_escapeString_case(":,", '^', "^:", "^^^:");
    test_escapeString_case(":,", '^', ":^", "^:^^");
    test_escapeString_case(":,", '^', "^::", "^^^:^:");
    test_escapeString_case(":,", '^', "::^", "^:^:^^");
    test_escapeString_case(":,", '^', "^:^,^:^", "^^^:^^^,^^^:^^");
}

void test_emailValidator()
{
    TRACE("%1(): --------------------------------").arg(__func__);

    std::string email("dana<dam@poczta. fm");
    TRACEF("validating: %s", email.c_str());
    da::EmailValidator ev(email);
    TRACEF("valid? %d", ev.isValid());

    std::vector<int> positions = ev.whereInvalid();
    for (int i = 0; i < positions.size(); i++)
    {
        TRACEF("%d '%c'", positions[i], email[positions[i]]);
    }
}

void test_split()
{
    TRACE("%1(): --------------------------------").arg(__func__);

    typedef std::vector<std::string> StringList;
    const bool trim = true;
    {
        StringList result = da::split<StringList>("", " ", !trim);
        if (result.size() != 1 || result[0] != "")
            WARNF("failed");
    }
    {
        StringList result = da::split<StringList>("", " ", trim);
        if (result.size() != 0)
            WARNF("failed");
    }
    {
        StringList result = da::split<StringList>(" ", " ", !trim);
        if (result.size() != 2 || result[0] != "" || result[1] != "")
            WARNF("failed");
    }
    {
        StringList result = da::split<StringList>("", " ", trim);
        if (result.size() != 0)
            WARNF("failed");
    }
    {
        StringList result = da::split<StringList>("abc", " ", !trim);
        if (result.size() != 1 || result[0] != "abc")
            WARNF("failed");
    }
    {
        StringList result = da::split<StringList>("abc", " ", trim);
        if (result.size() != 1 || result[0] != "abc")
            WARNF("failed");
    }
    {
        StringList result = da::split<StringList>(" abc", " ", !trim);
        if (result.size() != 2 || result[0] != "" || result[1] != "abc")
            WARNF("failed");
    }
    {
        StringList result = da::split<StringList>(" abc", " ", trim);
        if (result.size() != 1 || result[0] != "abc")
            WARNF("failed");
    }
    {
        StringList result = da::split<StringList>("abc ", " ", !trim);
        if (result.size() != 2 || result[0] != "abc" || result[1] != "")
            WARNF("failed");
    }
    {
        StringList result = da::split<StringList>("abc ", " ", trim);
        if (result.size() != 1 || result[0] != "abc")
            WARNF("failed");
    }
    {
        StringList result = da::split<StringList>(" abc ", " ", !trim);
        if (result.size() != 3 || result[0] != "" || result[1] != "abc" || result[2] != "")
            WARNF("failed");
    }
    {
        StringList result = da::split<StringList>(" abc ", " ", trim);
        if (result.size() != 1 || result[0] != "abc")
            WARNF("failed");
    }
    {
        StringList result = da::split<StringList>("abc def", " ", !trim);
        if (result.size() != 2 || result[0] != "abc" || result[1] != "def")
            WARNF("failed");
    }
    {
        StringList result = da::split<StringList>("abc def", " ", trim);
        if (result.size() != 2 || result[0] != "abc" || result[1] != "def")
            WARNF("failed");
    }
    {
        StringList result = da::split<StringList>("abc  def", " ", !trim);
        if (result.size() != 3 || result[0] != "abc" || result[1] != "" || result[2] != "def")
            WARNF("failed");
    }
    {
        StringList result = da::split<StringList>("abc  def", " ", trim);
        if (result.size() != 2 || result[0] != "abc" || result[1] != "def")
            WARNF("failed");
    }
}

void test_join()
{
    TRACE("%1(): --------------------------------").arg(__func__);

    std::vector<int> vec{ 1, 2, 3, 4, 5 };
    auto result = da::join(vec, ", ");
    if (result != "1, 2, 3, 4, 5")
        WARNF("failed");
}

void test_stringenum()
{
    TRACE("%1(): --------------------------------").arg(__func__);

    DEFINE_STRING_ENUM(StringEnum, aaa, bbb, ccc);

    TRACEF("---- default constructor");
    {
        StringEnum e;
        TRACEF("e = %s", e.str().c_str());
    }

    TRACEF("---- constructor from enum");
    {
        StringEnum e(StringEnum::bbb);
        TRACEF("e = %s", e.str().c_str());
    }
    {
        StringEnum e = StringEnum::bbb;
        TRACEF("e = %s", e.str().c_str());
    }
    TRACEF("---- assign from enum");
    {
        StringEnum e;
        e = StringEnum::bbb;
        TRACEF("e = %s", e.str().c_str());
    }

    TRACEF("---- copy constructor");
    {
        StringEnum e1 = StringEnum::bbb;
        StringEnum e(e1);
        TRACEF("e = %s", e.str().c_str());
    }
    {
        StringEnum e1 = StringEnum::bbb;
        StringEnum e = e1;
        TRACEF("e = %s", e.str().c_str());
    }
    TRACEF("---- assign operator");
    {
        StringEnum e1 = StringEnum::bbb;
        StringEnum e;
        e = e1;
        TRACEF("e = %s", e.str().c_str());
    }

    TRACEF("---- constructor from int");
    {
//        StringEnum e(0);
//        TRACEF("e = %s", e.str().c_str());
    }
    {
//        StringEnum e = 0;
//        TRACEF("e = %s", e.str().c_str());
    }
    TRACEF("---- assign from int");
    {
//        StringEnum e;
//        e = 0;
//        TRACEF("e = %s", e.str().c_str());
    }

    TRACEF("---- equals operator");
    {
        StringEnum e1 = StringEnum::bbb;
        StringEnum e2 = StringEnum::bbb;
        StringEnum e3 = StringEnum::ccc;
        TRACEF("%s == %s? %d",
                e1.str().c_str(),
                e2.str().c_str(),
                e1 == e2
            );
        TRACEF("%s == %s? %d",
                e1.str().c_str(),
                e3.str().c_str(),
                e1 == e3
            );
    }
    TRACEF("---- not equals operator");
    {
        StringEnum e1 = StringEnum::bbb;
        StringEnum e2 = StringEnum::bbb;
        StringEnum e3 = StringEnum::ccc;
        TRACEF("%s != %s? %d",
                e1.str().c_str(),
                e2.str().c_str(),
                e1 != e2
            );
        TRACEF("%s != %s? %d",
                e1.str().c_str(),
                e3.str().c_str(),
                e1 != e3
            );
    }

    TRACEF("---- equals to enum operator");
    {
        StringEnum e = StringEnum::bbb;
        TRACEF("%s == bbb? %d",
                e.str().c_str(),
                e == StringEnum::bbb
            );
        TRACEF("%s == ccc? %d",
                e.str().c_str(),
                e == StringEnum::ccc
            );
    }
    TRACEF("---- not equals to enum operator");
    {
        StringEnum e = StringEnum::bbb;
        TRACEF("%s != bbb? %d",
                e.str().c_str(),
                e != StringEnum::bbb
            );
        TRACEF("%s != ccc? %d",
                e.str().c_str(),
                e != StringEnum::ccc
            );
    }

    TRACEF("---- switch");
    {
        StringEnum e = StringEnum::bbb;
        switch (e)
        {
            case StringEnum::aaa:
                TRACEF("e is aaa");
                break;
            case StringEnum::bbb:
                TRACEF("e is bbb");
                break;
            case StringEnum::ccc:
                TRACEF("e is ccc");
                break;
        }
    }

    TRACEF("---- fromString");
    {
        StringEnum e;
        bool res = e.fromString("bbb");
        TRACEF("bbb: res? %d, %s",
                res,
                e.str().c_str()
            );
    }
    {
        StringEnum e;
        bool res = e.fromString("zzz");
        TRACEF("zzz: res? %d, %s",
                res,
                e.str().c_str()
            );
    }
}

void test_transform()
{
    TRACE("%1(): --------------------------------").arg(__func__);

    {
        std::vector<int> vec{ 1, 2, 3, 4, 5 };
        auto result = da::transform(vec, [](int elem) { return std::to_string(elem); });

        if (da::join(result, ", ") != "1, 2, 3, 4, 5")
            WARNF("failed");
    }
    {
        std::list<int> list{ 1, 2, 3, 4, 5 };
        auto result = da::transform<std::vector>(list, [](int elem) { return std::to_string(elem); });

        if (da::join(result, ", ") != "1, 2, 3, 4, 5")
            WARNF("failed");
    }
}

void test_call_n_times()
{
    3 * []()
        {
            TRACE("call_n_times");
        };
    3 * [](int i)
        {
            TRACE("call_n_times: %1").arg(i);
        };
}

void test_generate_n()
{
    TRACE("%1(): --------------------------------").arg(__func__);

    {
        auto result = da::generate_n<std::vector>(
                5,
                []() { return 42; }
            );
        if (da::join(result, ", ") != "42, 42, 42, 42, 42")
            WARNF("failed");
    }
    {
        auto result = da::generate_n<std::vector>(
                5,
                [](int i) { return i*10; }
            );
        if (da::join(result, ", ") != "0, 10, 20, 30, 40")
            WARNF("failed");
    }
}

void test_prune()
{
    TRACE("%1(): --------------------------------").arg(__func__);

    {
        const std::vector<int> expected { 1, 3, 5, };

        std::vector<int> v { 1, 2, 3, 4, 5, };
        auto is_even = [](int elem) { return elem % 2 == 0; };

        da::prune(v, is_even);
        if (v != expected)
            WARNF("failed");
    }
    {
        const std::map<int, std::string> expected {
                { 1, "one" },
                { 2, "two" },
            };

        std::map<int, std::string> map {
                { 0, "zero" },
                { 1, "one" },
                { 2, "two" },
                { 3, "three" },
                { 4, "four" },
            };
        auto longer_than_3 = [](const std::pair<int, std::string>& elem) { return elem.second.size() > 3; };

        da::prune(map, longer_than_3);
        if (map != expected)
            WARNF("failed");
    }
}

int main(int argc, char * argv[])
{
    (void)argc;
    (void)argv;

    test_loggerf();
    test_loggerqt();
    test_itoa();
    test_escapeString();
    test_emailValidator();
    test_split();
    test_stringenum();
    test_join();
    test_transform();
    test_call_n_times();
    test_generate_n();
    test_prune();
}
