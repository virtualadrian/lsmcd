/*****************************************************************************
*    Open LiteSpeed is an open source HTTP server.                           *
*    Copyright (C) 2013 - 2015  LiteSpeed Technologies, Inc.                 *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program. If not, see http://www.gnu.org/licenses/.      *
*****************************************************************************/
#ifndef LSSHMSASL_H
#define LSSHMSASL_H

// define to include sasl interface
//#define USE_SASL

// define to verify plaintext password
#define ENABLE_SASL_PWDB

// define to search for sasl configuration file
#define HAVE_SASL_CB_GETCONF

#include <stdlib.h>
#include <stdint.h>
#ifdef USE_SASL
// If this fails, may need to install Cyrus SASL
// As of Oct 16, 2015 this link works:
// http://www.linuxfromscratch.org/blfs/view/svn/postlfs/cyrus-sasl.html
#include <sasl/sasl.h>
#endif

#define SASLMECH_MAXLEN     32  // max string length of sasl mechanism

class LsMcSasl
{
#ifdef USE_SASL
public:
    LsMcSasl()
        : m_pSaslConn(NULL)
        , m_authenticated(false)
    {}
    ~LsMcSasl()
    {   clrSaslConn();   }

    static int  initSasl();
    int  listMechs(const char **pResult);
    int  chkAuth(char *pBuf, unsigned int mechLen, unsigned int valLen,
                 const char **pResult, unsigned int *pLen);
    int  chkAuthStep(char *pBuf, unsigned int valLen,
                 const char **pResult, unsigned int *pLen);

    bool isAuthenticated()
    {   return m_authenticated;  }

    static const char  *s_pAppName;
    static char        *s_pSaslPwdb;
    static uint8_t      verbose;

private:
    LsMcSasl(const LsMcSasl &other);
    LsMcSasl &operator=(const LsMcSasl &other);

    sasl_conn_t *getSaslConn()
    {
        m_authenticated = false;
        return ((m_pSaslConn != NULL)
            || (sasl_server_new(s_pAppName,
                NULL, NULL, NULL, NULL, NULL, 0, &m_pSaslConn) == SASL_OK)) ?
            m_pSaslConn : NULL;
    }

    void clrSaslConn()
    {
        if (m_pSaslConn != NULL)
        {
            sasl_dispose(&m_pSaslConn);
            m_pSaslConn = NULL;
        }
    }

private:
    sasl_conn_t        *m_pSaslConn;
    bool                m_authenticated;
#endif  // USE_SASL
};

#endif // LSSHMSASL_H

