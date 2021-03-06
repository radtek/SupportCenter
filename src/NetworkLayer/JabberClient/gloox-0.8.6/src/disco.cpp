/*
  Copyright (c) 2004-2006 by Jakob Schroeter <js@camaya.net>
  This file is part of the gloox library. http://camaya.net/gloox

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/


#include "disco.h"
#include "discohandler.h"
#include "clientbase.h"
#include "disconodehandler.h"


namespace gloox
{

  Disco::Disco( ClientBase *parent )
    : m_parent( parent )
  {
    addFeature( XMLNS_VERSION );
    addFeature( XMLNS_DISCO_INFO );
    addFeature( XMLNS_DISCO_ITEMS );
    if( m_parent )
    {
      m_parent->registerIqHandler( this, XMLNS_DISCO_INFO );
      m_parent->registerIqHandler( this, XMLNS_DISCO_ITEMS );
      m_parent->registerIqHandler( this, XMLNS_VERSION );
    }
  }

  Disco::~Disco()
  {
    if( m_parent )
    {
      m_parent->removeIqHandler( XMLNS_DISCO_INFO );
      m_parent->removeIqHandler( XMLNS_DISCO_ITEMS );
      m_parent->removeIqHandler( XMLNS_VERSION );
    }
  }

  bool Disco::handleIq( Stanza *stanza )
  {
    switch( stanza->subtype() )
    {
      case StanzaIqGet:
        if( stanza->xmlns() == XMLNS_VERSION )
        {
          Tag *iq = new Tag( "iq" );
          iq->addAttribute( "id", stanza->id() );
          iq->addAttribute( "from", m_parent->jid().full() );
          iq->addAttribute( "to", stanza->from().full() );
          iq->addAttribute( "type", "result" );
          Tag *query = new Tag( iq, "query" );
          query->addAttribute( "xmlns", XMLNS_VERSION );
          new Tag( query, "name", m_versionName );
          new Tag( query, "version", m_versionVersion );
          new Tag( query, "os", m_versionOs );

          m_parent->send( iq );
        }
        else if( stanza->xmlns() == XMLNS_DISCO_INFO )
        {
          Tag *iq = new Tag( "iq" );
          iq->addAttribute( "id", stanza->id() );
          iq->addAttribute( "from", m_parent->jid().full() );
          iq->addAttribute( "to", stanza->from().full() );
          iq->addAttribute( "type", "result" );
          Tag *query = new Tag( iq, "query" );
          query->addAttribute( "xmlns", XMLNS_DISCO_INFO );

          Tag *q = stanza->findChild( "query" );
          const std::string node = q->findAttribute( "node" );
          if( !node.empty() )
          {
            DiscoNodeHandlerMap::const_iterator it = m_nodeHandlers.find( node );
            if( it != m_nodeHandlers.end() )
            {
              std::string name;
              StringMap identities =
                  (*it).second->handleDiscoNodeIdentities( node, name );
              StringMap::const_iterator im = identities.begin();
              for( ; im != identities.end(); ++im )
              {
                Tag *i = new Tag( query, "identity" );
                i->addAttribute( "category", (*im).first );
                i->addAttribute( "type", (*im).second );
                i->addAttribute( "name", name );
              }
              StringList features = (*it).second->handleDiscoNodeFeatures( node );
              StringList::const_iterator fi = features.begin();
              for( ; fi != features.end(); ++fi )
              {
                Tag *f = new Tag( query, "feature" );
                f->addAttribute( "var", (*fi) );
              }
            }
          }
          else
          {
            Tag *i = new Tag( query, "identity" );
            i->addAttribute( "category", m_identityCategory );
            i->addAttribute( "type", m_identityType );
            i->addAttribute( "name", m_versionName );

            StringList::const_iterator it = m_features.begin();
            for( ; it != m_features.end(); ++it )
            {
              Tag *f = new Tag( query, "feature" );
              f->addAttribute( "var", (*it).c_str() );
            }
          }

          m_parent->send( iq );
        }
        else if( stanza->xmlns() == XMLNS_DISCO_ITEMS )
        {
          Tag *iq = new Tag( "iq" );
          iq->addAttribute( "id", stanza->id() );
          iq->addAttribute( "to", stanza->from().full() );
          iq->addAttribute( "from", m_parent->jid().full() );
          iq->addAttribute( "type", "result" );
          Tag *query = new Tag( iq, "query" );
          query->addAttribute( "xmlns", XMLNS_DISCO_ITEMS );

          StringMap items;
          DiscoNodeHandlerMap::const_iterator it;
          Tag *q = stanza->findChild( "query" );
          const std::string node = q->findAttribute( "node" );
          if( !node.empty() )
          {
            it = m_nodeHandlers.find( node );
            if( it != m_nodeHandlers.end() )
            {
              items = (*it).second->handleDiscoNodeItems( node );
            }
          }
          else
          {
            it = m_nodeHandlers.begin();
            for( ; it != m_nodeHandlers.end(); ++it )
            {
              items = (*it).second->handleDiscoNodeItems( "" );
            }
          }

          if( items.size() )
          {
            StringMap::const_iterator it = items.begin();
            for( ; it != items.end(); ++it )
            {
              if( !(*it).first.empty() && !(*it).second.empty() )
              {
                Tag *i = new Tag( query, "item" );
                i->addAttribute( "jid",  m_parent->jid().full() );
                i->addAttribute( "node", (*it).first );
                i->addAttribute( "name", (*it).second );
              }
            }
          }

          m_parent->send( iq );
        }
        return true;
        break;

      case StanzaIqSet:
      {
        bool res = false;
        DiscoHandlerList::const_iterator it = m_discoHandlers.begin();
        for( ; it != m_discoHandlers.end(); ++it )
        {
          if( (*it)->handleDiscoSet( stanza ) )
            res = true;
        }
        return res;
        break;
      }

      default:
        break;
    }
    return false;
  }

  bool Disco::handleIqID( Stanza *stanza, int context )
  {
    DiscoHandlerMap::const_iterator it = m_track.find( stanza->id() );
    if( it != m_track.end() )
    {
      switch( stanza->subtype() )
      {
        case StanzaIqResult:
          switch( context )
          {
            case GET_DISCO_INFO:
              (*it).second.dh->handleDiscoInfoResult( stanza, (*it).second.context );
              break;
            case GET_DISCO_ITEMS:
              (*it).second.dh->handleDiscoItemsResult( stanza, (*it).second.context );
              break;
           }
        break;

        case StanzaIqError:
          (*it).second.dh->handleDiscoError( stanza, (*it).second.context );
          break;

        default:
          break;
      }
    }

    return false;
  }

  void Disco::addFeature( const std::string& feature )
  {
    m_features.push_back( feature );
  }

  void Disco::getDiscoInfo( const std::string& to, const std::string& node, DiscoHandler *dh, int context )
  {
    std::string id = m_parent->getID();

    Tag *iq = new Tag( "iq" );
    iq->addAttribute( "id", id );
    iq->addAttribute( "to", to );
    iq->addAttribute( "from", m_parent->jid().full() );
    iq->addAttribute( "type", "get" );
    Tag *q = new Tag( iq, "query" );
    q->addAttribute( "xmlns", XMLNS_DISCO_INFO );
    if( !node.empty() )
      q->addAttribute( "node", node );

    DiscoHandlerContext ct;
    ct.dh = dh;
    ct.context = context;
    m_track[id] = ct;
    m_parent->trackID( this, id, GET_DISCO_INFO );
    m_parent->send( iq );
  }

  void Disco::getDiscoItems( const std::string& to, const std::string& node, DiscoHandler *dh, int context )
  {
    std::string id = m_parent->getID();

    Tag *iq = new Tag( "iq" );
    iq->addAttribute( "id", id );
    iq->addAttribute( "to", to );
    iq->addAttribute( "from", m_parent->jid().full() );
    iq->addAttribute( "type", "get" );
    Tag *q = new Tag( iq, "query" );
    q->addAttribute( "xmlns", XMLNS_DISCO_ITEMS );
    if( !node.empty() )
      q->addAttribute( "node", node );

    DiscoHandlerContext ct;
    ct.dh = dh;
    ct.context = context;
    m_track[id] = ct;
    m_parent->trackID( this, id, GET_DISCO_ITEMS );
    m_parent->send( iq );
  }

  void Disco::setVersion( const std::string& name, const std::string& version, const std::string& os )
  {
    m_versionName = name;
    m_versionVersion = version;
    m_versionOs = os;
  }

  void Disco::setIdentity( const std::string& category, const std::string& type )
  {
    m_identityCategory = category;
    m_identityType = type;
  }

  void Disco::registerDiscoHandler( DiscoHandler *dh )
  {
    m_discoHandlers.push_back( dh );
  }

  void Disco::removeDiscoHandler( DiscoHandler *dh )
  {
    m_discoHandlers.remove( dh );
  }

  void Disco::registerNodeHandler( DiscoNodeHandler *nh, const std::string& node )
  {
    m_nodeHandlers[node] = nh;
  }

  void Disco::removeNodeHandler( const std::string& node )
  {
    m_nodeHandlers.erase( node );
  }

}
