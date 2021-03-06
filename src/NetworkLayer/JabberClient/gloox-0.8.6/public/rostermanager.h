/*
  Copyright (c) 2004-2006 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#ifndef ROSTERMANAGER_H__
#define ROSTERMANAGER_H__

#include "subscriptionhandler.h"
#include "rosterlistener.h"
#include "privatexmlhandler.h"
#include "iqhandler.h"
#include "presencehandler.h"

#include <map>
#include <string>
#include <list>

namespace gloox
{

  class ClientBase;
  class Stanza;
  class PrivateXML;

  /**
   * @brief This class implements Jabber/XMPP roster handling in the @b jabber:iq:roster namespace.
   *
   * It takes care of changing presence, subscriptions, etc.
   * You can modify any number of RosterItems within the Roster at any time. These changes must be
   * synchronized with the server by calling @ref synchronize(). Note that incoming Roster pushes
   * initiated by other resources may overwrite changed values.
   * Additionally, JEP-0083 (Nested Roster Groups) is implemented herein.
   *
   * @author Jakob Schroeter <js@camaya.net>
   * @since 0.3
   */
   class GLOOX_API RosterManager : public IqHandler, PresenceHandler, SubscriptionHandler,
                                          PrivateXMLHandler
  {
    public:
      /**
       * Creates a new RosterManager.
       * @param parent The @ref Client which is used for communication.
       * @param self Whether to automatically include a self-contact in the roster. Default: false.
       */
      RosterManager( ClientBase *parent, bool self = false );

      /**
       * Virtual destructor.
       */
      virtual ~RosterManager();

      /**
       * This function does the initial filling of the roster with
       * the current server-side roster.
       */
      void fill();

      /**
       * This function returns the roster.
       * @return Returns a map of JIDs with their current presence.
       */
      RosterListener::Roster* roster();

      /**
       * Use this function to subscribe to a new JID. The contact is added to the roster automatically
       * (by compliant servers, as required by RFC 3921).
       * @param jid The address to subscribe to.
       * @param name The displayed name of the contact.
       * @param groups A list of groups the contact belongs to.
       * @param msg A message sent along with the request.
       */
      void subscribe( const std::string& jid, const std::string& name, StringList& groups,
                      const std::string& msg );

      /**
       * Synchronizes locally modified RosterItems back to the server.
       */
      void synchronize();

      /**
       * Use this function to add a contact to the roster. No subscription request is sent.
       * @note Use @ref unsubscribe() to remove an item from the roster.
       * @param jid The JID to add.
       * @param name The displayed name of the contact.
       * @param groups A list of groups the contact belongs to.
       */
      void add( const std::string& jid, const std::string& name, StringList& groups );

      /**
       * Use this function to unsubscribe from a JID in the roster.
       * @param jid The address to unsubscribe from.
       * @param msg A message to send along with the request.
       * @param remove Whether the contact should also be removed from the roster.
       */
      void unsubscribe( const std::string& jid, const std::string& msg, bool remove );

      /**
       * Use this function to acknowledge a subscription request if you requested asynchronous
       * subscription request handling.
       * @param to The JID to authorize/decline.
       * @param ack Whether to authorize or decline the contact's request.
       */
      void ackSubscriptionRequest( const JID& to, bool ack );

      /**
       * Use this function to retrieve the delimiter of Nested Roster Groups (JEP-0083).
       * @return The group delimiter.
       * @since 0.7
       */
      const std::string& delimiter() const { return m_delimiter; };

      /**
       * Use this function to set the group delimiter (JEP-0083).
       * @param delimiter The group delimiter.
       * @since 0.7
       */
      void setDelimiter( const std::string& delimiter );

      /**
       * Lets you retrieve the RosterItem that belongs to the given JID.
       * @param jid The JID to return the RosterItem for.
       */
      RosterItem* getRosterItem( const JID& jid );

      /**
       * Register @c rl as object that receives updates on roster operations.
       * For GUI applications it may be necessary to display a dialog or whatever to
       * the user without blocking. If you want that, use asynchronous subscription
       * requests. If you want to answer a request right away, make it synchronous.
       * @param rl The object that receives roster updates.
       * @param syncSubscribeReq Indicates whether (Un)SubscriptionRequests shall
       * be handled synchronous (@b true) or asynchronous (@b false). Default: synchronous.
       */
      void registerRosterListener( RosterListener *rl, bool syncSubscribeReq = true );

      /**
       * Complementary function to @ref registerRosterListener. Removes the current RosterListener.
       * Roster events will not be delivered anywhere.
       */
      void removeRosterListener();

      // reimplemented from IqHandler.
      virtual bool handleIq( Stanza *stanza );

      // reimplemented from IqHandler.
      virtual bool handleIqID( Stanza *stanza, int context );

      // reimplemented from PresenceHandler.
      virtual void handlePresence( Stanza *stanza );

      // reimplemented from SubscriptionHandler.
      virtual void handleSubscription( Stanza *stanza );

      // reimplemented from PrivateXMLHandler
      virtual void handlePrivateXML( const std::string& tag, Tag *xml );

      // reimplemented from PrivateXMLHandler
      virtual void handlePrivateXMLResult( const std::string& uid, PrivateXMLResult result );

    private:
      void add( const std::string& jid, const std::string& name,
                StringList& groups, const std::string& sub, bool ask );
      void extractItems( Tag *tag, bool isPush );

      RosterListener *m_rosterListener;
      RosterListener::Roster m_roster;
      ClientBase *m_parent;
      PrivateXML *m_privateXML;

      std::string m_delimiter;
      bool m_syncSubscribeReq;

  };

}

#endif // ROSTER_H__
