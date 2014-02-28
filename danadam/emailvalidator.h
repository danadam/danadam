#ifndef DANADAM_EMAIL_VALIDATOR_H_GUARD
#define DANADAM_EMAIL_VALIDATOR_H_GUARD

#include <string>
#include <vector>

#include "loggerf.h"

namespace da
{

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

        const std::string::size_type atSignPos = email.find_last_of("@");
        if (atSignPos == std::string::npos)
        {
            m_valid = false;
            return;
        }

        const std::string localPart = email.substr(0, atSignPos);
        const std::string domain = email.substr(atSignPos + 1);

        TRACEF("local: %s", localPart.c_str());
        TRACEF("domain: %s", domain.c_str());

        std::string::size_type pos, lastPos = 0;
        while (true)
        {
            pos = localPart.find_first_of(invalidInLocalPart, lastPos);
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
            pos = domain.find_first_of(invalidInDomain, lastPos);
            if (pos == std::string::npos)
            {
                break;
            }
            else
            {
                m_positions.push_back(pos + localPart.length() + 1);
            }

            lastPos = pos + 1;
        }

        m_valid = m_positions.empty();
    }

    bool m_valid;
    std::vector<int> m_positions;
};

} // namespace

#endif

