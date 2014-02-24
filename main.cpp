#include "loggerf.h"
#include "loggerqt.h"

#include "itoa.h"

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

int main(int argc, char * argv[])
{
    (void)argc;
    (void)argv;

    test_loggerf();
    test_loggerqt();
    test_itoa();
}
