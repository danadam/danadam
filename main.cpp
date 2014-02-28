#include "loggerf.h"
#include "loggerqt.h"

#include "itoa.h"
#include "stringutils.h"
#include <vector>

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

class EmailValidator
{
public:
    explicit EmailValidator(const std::string & email)
        : m_valid(false)
    {
        validate(email);
    }

    bool isValid() const { return m_valid; }
    std::vector<int> whereInvalid() const { return m_positions; }

private:
    void validate(const std::string & email)
    {
//        static const std::string invalidInLocalPart("@()[]\\:;\",<>");
        static const std::string invalidInLocalPart(" []\\:;,<>");
        static const std::string invalidInDomain(" ");

        typedef std::vector<std::string> StringVector;
        const StringVector tokens = da::split<StringVector>(email, "@", true);
        if (tokens.size() != 2)
        {
            m_valid = false;
            return;
        }

        std::string::size_type pos, lastPos = 0;

        while (true)
        {
            pos = tokens[0].find_first_of(invalidInLocalPart, lastPos);
            if (pos == std::string::npos)
            {
                break;
            }
            else
            {
                m_positions.push_back(pos);
            }

            lastPos = pos + 1;
        }
        lastPos = 0;
        while (true)
        {
            pos = tokens[1].find_first_of(invalidInDomain, lastPos);
            if (pos == std::string::npos)
            {
                break;
            }
            else
            {
                m_positions.push_back(pos + tokens[0].length() + 1);
            }

            lastPos = pos + 1;
        }

        m_valid = m_positions.empty();
    }

    bool m_valid;
    std::vector<int> m_positions;
};

int main(int argc, char * argv[])
{
    (void)argc;
    (void)argv;

    test_loggerf();
    test_loggerqt();
    test_itoa();

    std::string email("dana<dam@poczta. fm");
    EmailValidator ev(email);
    TRACEF("valid? %d", ev.isValid());

    std::vector<int> positions = ev.whereInvalid();
    for (int i = 0; i < positions.size(); i++)
    {
        TRACEF("%d '%c'", positions[i], email[positions[i]]);
    }
}
