/*
  Copyright (c) 2005-2006 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#ifndef REGISTRATION_H__
#define REGISTRATION_H__

#include "iqhandler.h"
#include "registrationhandler.h"
#include "dataform.h"

#include <string>
#include <map>

namespace gloox
{

  class ClientBase;
  class Stanza;

  /**
   * @brief This class is an implementation of JEP-0077 (In-Band Registration).
   *
   * Derive your object from @ref RegistrationHandler and implement the
   * virtual functions offered by that interface. Then use it like this:
   * @code
   * void MyClass::myFunc()
   * {
   *   m_client = new Client( "example.org" );
   *   m_client->disableRoster(); // a roster is not necessary for registration
   *   m_client->disableDisco(); // the disco client is not necessary for registration
   *   m_client->registerConnectionListener( this );
   *
   *   m_reg = new Registration( c );
   *   m_reg->registerRegistrationHandler( this );
   *
   *   m_client->connect();
   * }
   *
   * void MyClass::onConnect()
   * {
   *   m_reg->fetchRegistrationFields();
   * }
   * @endcode
   *
   * In RegistrationHandler::handleRegistrationFields() you should check which information the server
   * requires to open a new account. You might not always get away with just username and password.
   * Then call createAccount() with a filled-in fieldStruct and an @c int representing the bit-wise
   * ORed fields you want to have included in the registration attempt. For your convenience you can
   * use the 'fields' argument of handleRegistrationFields(). ;) It's your responsibility to make
   * sure at least those fields the server requested are filled in.
   *
   * Check @c tests/register_test.cpp for an example.
   *
   * @author Jakob Schroeter <js@camaya.net>
   * @since 0.2
   */
  class GLOOX_API Registration : public IqHandler
  {
    public:

      /**
       * Holds all the possible fields a server may require for registration according
       * to Section 14.1, JEP-0077.
       */
      struct fieldStruct
      {
        std::string username;
        std::string nick;
        std::string password;
        std::string name;
        std::string first;
        std::string last;
        std::string email;
        std::string address;
        std::string city;
        std::string state;
        std::string zip;
        std::string phone;
        std::string url;
        std::string date;
        std::string misc;
        std::string text;
      };

      /**
       * The possible fields of a JEP-0077 account registration.
       */
      enum fieldEnum
      {
        FIELD_USERNAME =     1,
        FIELD_NICK     =     2,
        FIELD_PASSWORD =     4,
        FIELD_NAME     =     8,
        FIELD_FIRST    =    16,
        FIELD_LAST     =    32,
        FIELD_EMAIL    =    64,
        FIELD_ADDRESS  =   128,
        FIELD_CITY     =   256,
        FIELD_STATE    =   512,
        FIELD_ZIP      =  1024,
        FIELD_PHONE    =  2048,
        FIELD_URL      =  4096,
        FIELD_DATE     =  8192,
        FIELD_MISC     = 16384,
        FIELD_TEXT     = 32768
      };

      /**
       * Constructor.
       * @param parent The ClientBase which is used for establishing a connection.
       */
      Registration( ClientBase *parent );

      /**
       * Virtual destructor.
       */
      virtual ~Registration();

      /**
       * Use this function to request the registration fields the server requires.
       * The required fields are returned asynchronously to the object registered as
       * @ref RegistrationHandler by calling @ref RegistrationHandler::handleRegistrationFields().
       */
      void fetchRegistrationFields();

      /**
       * Attempts to register an account with the given credentials. Only the fields OR'ed in
       * @c fields will be sent. This can only be called with an unauthenticated parent (@ref Client).
       * @note It is recommended to use @ref fetchRegistrationFields to find out which fields the
       * server requires.
       * @param fields The fields to use to generate the registration request. OR'ed @ref fieldEnum values.
       * @param values The struct contains the values which shall be used for the registration.
       */
      void createAccount( int fields, const fieldStruct& values );

      /**
       * Attempts to register an account with the given credentials. This can only be called with an
       * unauthenticated parent (@ref Client).
       * @note According to JEP-0077, if the server sends both old-style fields and data form,
       * implementations SHOULD prefer data forms.
       * @param form The DataForm containing the registration credentials.
       */
      void createAccount( const DataForm& form );

      /**
       * Tells the server to remove the currently authenticated account from the server.
       */
      void removeAccount();

      /**
       * Tells the server to change the password for the current account.
       * @param password The new password.
       */
      void changePassword( const std::string& password );

      /**
       * Registers the given @c rh as RegistrationHandler. Only one handler is possible at a time.
       * @param rh The RegistrationHandler to register.
       */
      void registerRegistrationHandler( RegistrationHandler *rh );

      /**
       * Un-registers the current RegistrationHandler.
       */
      void removeRegistrationHandler();

      // reimplemented from IqHandler
      virtual bool handleIq( Stanza *stanza );

      // reimplemented from IqHandler
      virtual bool handleIqID( Stanza *stanza, int context );

    private:
      enum IdType
      {
        FETCH_REGISTRATION_FIELDS,
        CREATE_ACCOUNT,
        REMOVE_ACCOUNT,
        CHANGE_PASSWORD
      };

      ClientBase *m_parent;
      RegistrationHandler *m_registrationHandler;
  };

}

#endif // REGISTRATION_H__
