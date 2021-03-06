//===========================================================================
// SupportSpace ltd. @{SRCH}
//								IMClient
//
//---------------------------------------------------------------------------
// DESCRIPTION: @{HDES}
// -----------
// IMClient : 
//---------------------------------------------------------------------------
// CHANGES LOG: @{HREV}
// -----------
// Revision: 01.00
// By      : Anatoly Gutnick
// Date    : 10/29/2006 05:21:10 PM
// Comments: First Issue
//===========================================================================
#pragma once

#include "atlbase.h"
#include "stdafx.h"

#include <string>
#include <vector>

#include <Gloox\gloox.h>
#include <Gloox\client.h>
#include <Gloox\connectionlistener.h>
#include <Gloox\presencehandler.h>
#include <Gloox\messagesessionhandler.h>
#include <Gloox\messagesession.h>
#include <Gloox\messagehandler.h>
#include <Gloox\disco.h>
#include <Gloox\rostermanager.h> 

using namespace gloox;
using namespace std;

 /**
   * @brief 
   *
   */
#define	WM_CONNECTED			WM_USER + 101
#define	WM_DISCONNECT			WM_USER + 102
#define	WM_CONNECTING			WM_USER + 103		//  TBD if needed 
#define	WM_CONNECTFAILED		WM_USER + 104		//  TBD if needed
#define	WM_PRESENCEUPDATED		WM_USER + 105		//  

#define	WM_NEWCALL				WM_USER + 110		//  new call with specified CallId will be added to the Inbox
#define	WM_DELETECALL			WM_USER + 111		//  the call with specified CallId will be deleted from the Inbox or InSession
#define	WM_INSESSIONCALL		WM_USER + 112		//  the call with specified CallId will be moved from Inbox to InSession - no call properties update?
#define	WM_UPDATESETTINGS		WM_USER + 113
 /**
   * @brief This class implements a basic Jabber Client.
   * based on the glox library
   *
   *
   * @author Abnatoly Gutnick <anatolyg@gmail.com>
   */
class IMClient : public ConnectionListener, MessageHandler, LogHandler /* RosterListener todo*/
{
public:
	/**
		* Constructs a new Client 
		* @param username The username/local part of the JID.
		* @param resource The resource part of the JID.
		* @param password The password to use for authentication.
		* @param server The Jabber ID'S server part and the host name to connect to. If those are different
		* @param server_addr ipaddress or hostname of IM server
		* @param hWnd handle to the window, notification will besent to
		* @param hWnd handle to the window, notification will besent tobLog
		*/
	IMClient(std::string username,
			 std::string resource, 
			 std::string password, 
			 std::string server, 
			 std::string server_addr, 
			 HWND		 hWnd,
			 bool		 bLog = false);
	/**
		* Destructor
		*/
	~IMClient();

	 /**
       * Initiates the connection to a server. This function blocks as long as a connection is
       * established.
       * You can have the connection block 'til the end of the connection, or you can have it return
       * immediately. If you choose the latter, its your responsibility to call @ref recv() every now
       * and then to actually receive data from the socket and to feed the parser.
       * @param block @b True for blocking, @b false for non-blocking connect. Defaults to @b true.
       * @return @b False if prerequisits are not met (server not set) or if the connection was refused,
       * @b true otherwise.
       */
	bool connect(bool block);

	/**
       * disconnect 
       * @param  
       */
	void disconnect();

     /**
       * Creates a new presence stanza. The created stanza will be a broadcast stanza sent to all
       * contacts in the roster.
       * @param status - the possible 'available presence' types.
       */
	void update_status(Presence  status);
    
	/**
       * Returns the current connection status.
       * @return The status of the connection.
       */
	ConnectionState connection_state()const;

    /**
       * Use this periodically to receive data from the socket and to feed the parser. You need to use
       * this only if you chose to connect in non-blocking mode.
       * @param timeout The timeout in seconds to use for select. Default of -1 means blocking
       * until data was available.
       * @return The state of the connection.
       */
	ConnectionError idle(int timeout);

    /**
       * We reimplement this function of MessageHandler interface to be notified about
       * incoming messages. We parse the Stanza here and notify the Window
       * @param stanza The complete Stanza.
       */
	virtual void handleMessage(Stanza *stanza, MessageSession *session = 0);

	/**
       * handleLog 
       * @param  level
       * @param  area
	   * @param  message
       */
	virtual void handleLog(LogLevel level, LogArea area, const std::string& message);

	/**
       * onConnect 
       * @param  
       */
	virtual void onConnect();

	/**
       * onDisconnect 
       * @param  
       */
	virtual void onDisconnect(ConnectionError e);

	/**
       * onTLSConnect 
       * @param  info
       */
	virtual bool onTLSConnect( const CertInfo& info );

	/**
       * Registers @c cl as object that receives connection notifications.
       * @param cl The object to receive connection notifications.
       */
    void registerConnectionListener( ConnectionListener *cl );

	/**
       * Remove @c cl as object that receives connection notifications.
       * @param cl The object to receive connection notifications.
       */
	void removeConnectionListener( ConnectionListener *cl );

	/**
       * Registers @c lh as object that receives all debug messages of the specified type.
       * Suitable for logging to a file, etc.
       */
      void registerLogHandler( );

    /**
       * Removes the given object from the list of log handlers.
      */
      void removeLogHandler( );


	/**
       * Sends a given msg over an established connection.
       * @param to - the destination id of account
	   * @param body - the message body text 
	   * @param subject - the subject if the message
       */
	void send_msg(std::string &to, std::string &body, std::string &subject);

	 /**
       * Reimplement this function if you want to be notified about new items
       * on the server-side roster (items subject to a so-called Roster Push).
       * This function will be called regardless who added the item, either this
       * resource or another. However, it will not be called for JIDs for which
       * presence is received without them being on the roster.
       * @param jid The new item's full address.
       */
      virtual void itemAdded( const std::string& jid );

    /**
       * Reimplement this function if you want to be notified about items
       * which authorised subscription.
       * @param jid The authorising item's full address.
       */
      virtual void itemSubscribed( const std::string& jid );

      /**
       * Reimplement this function if you want to be notified about items that
       * were removed from the server-side roster (items subject to a so-called Roster Push).
       * This function will be called regardless who deleted the item, either this resource or
       * another.
       * @param jid The removed item's full address.
       */
      virtual void itemRemoved( const std::string& jid );

      /**
       * Reimplement this function if you want to be notified about items that
       * were modified on the server-side roster (items subject to a so-called Roster Push).
       * A roster push is initiated if a second resource of this JID modifies an item stored on the
       * server-side contact list. This can include modifying the item's name, its groups, or the
       * subscription status. These changes are pushed by the server to @b all connected resources.
       * This is why this function will be called if you modify a roster item locally and synchronize
       * it with teh server.
       * @param jid The modified item's full address.
       */
      virtual void itemUpdated( const std::string& jid );

      /**
       * Reimplement this function if you want to be notified about items which
       * removed subscription authorization.
       * @param jid The item's full address.
       */
      virtual void itemUnsubscribed( const std::string& jid );

      /**
       * Reimplement this function if you want to receive the whole server-side roster
       * on the initial roster push. After successful authentication, RosterManager asks the
       * server for the full server-side roster. Invocation of this method announces its arrival.
       * Roster item status is set to 'unavailable' until incoming presence info updates it. A full
       * roster push only happens once per connection.
       * @param roster The full roster.
       */
      virtual void roster( const Roster& roster );

      /**
       * This function is called on every status change of an item in the roster.
       * @note This function is not called for status changes from or to Unavailable.
       * In these cases, @ref itemAvailable() and @ref itemUnavailable() are called,
       * respectively.
       * @param item The roster item.
       * @param status The item's new status.
       * @param msg The status change message.
       */
      virtual void presenceUpdated( const RosterItem& item, int status, const std::string& msg );

      /**
       * This function is called whenever a roster item comes online (is available).
       * However, it will not be called for status changes form Away (or any other
       * status which is not Unavailable) to Available.
       * @param item The changed roster item.
       * @param msg The status change message.
       */
      virtual void itemAvailable( const RosterItem& item, const std::string& msg );

      /**
       * This function is called whenever a roster item goes offline (is unavailable).
       * @param item The roster item.
       * @param msg The status change message.
       */
      virtual void itemUnavailable( const RosterItem& item, const std::string& msg );

      /**
       * This function is called when an entity wishes to subscribe to this entity's presence.
       * If the handler is registered as a asynchronous handler for subscription requests,
       * the return value of this function is ignored. In this case you should use
       * RosterManager::ackSubscriptionRequest() to answer the request.
       * @param jid The requesting item's address.
       * @param msg A message sent along with the request.
       * @return Return @b true to allow subscription and subscribe to the remote entity's
       * presence, @b false to ignore the request.
       */
      virtual bool subscriptionRequest( const std::string& jid, const std::string& msg );

      /**
       * This function is called when an entity unsubscribes from this entity's presence.
       * If the handler is registered as a asynchronous handler for subscription requests,
       * the return value of this function is ignored. In this case you should use
       * RosterManager::unsubscribe() if you want to unsubscribe yourself from the contct's
       * presence and to remove the contact from the roster.
       * @param jid The item's address.
       * @param msg A message sent along with the request.
       * @return Return @b true to unsubscribe from the remote entity, @b false to ignore.
       */
      virtual bool unsubscriptionRequest( const std::string& jid, const std::string& msg );

      /**
       * This function is called whenever presence from an entity is received which is not in
       * the roster.
       * @param jid The entity's full JID.
       */
      virtual void nonrosterPresenceReceived( const JID& jid );

	/**
	  * Use this function to add a contact to the roster. No subscription request is sent.
	  * @note Use @ref unsubscribe() to remove an item from the roster.
	  * @note This function will not be used in first version - roster managemenet will be done on web server side
	  * @param jid The JID to add.
	  * @param name The displayed name of the contact.
	  * @param groups A list of groups the contact belongs to.
	*/
	void	subscribeNewContact(const std::string& jid, const std::string& name, StringList groups);


	void	ping(){ m_client->whitespacePing(); } 

private:
	Client*				m_client; // incapsulate gloox client class
	std::string			m_server; // store server paramaters specified in constructor
	HWND				m_hWnd;   // handle of the window to be notified about callback
};