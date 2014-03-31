#include "loggerf.h"
#include "loggerqt.h"

#include "itoa.h"
#include "stringutils.h"

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

int main(int argc, char * argv[])
{
    (void)argc;
    (void)argv;

    test_loggerf();
    test_loggerqt();
    test_itoa();
    test_escapeString();
}
