/////////////////////////////////////////////////////////////////////////
///
///  Archer Software.
///
///  CSettings.h
///
///  Declares CSettings class, responsible for settings
///
///  @author Dmitry Netrebenko @date 07.06.2007
///
////////////////////////////////////////////////////////////////////////

#pragma once

#include <windows.h>
#include <AidLib/Strings/tstring.h>
#include <AidLib/Loki/Singleton.h>

#define DEFAULT_PORT		3000
#define DEFAULT_TIMEOUT		30000
#define DEFAULT_ADDR		_T("127.0.0.1")
#define DEFAULT_COLS		8
#define DEFAULT_ROWS		8
#define DEFAULT_TEST_TIME	60000

///  CSettings class, responsible for settings
///  Access through singleton
class CSettings
{
private:
///  Prevents making copies of CSettings objects.
	CSettings( const CSettings& );
	CSettings& operator=( const CSettings& );

public:
///  Constructor
	CSettings();
///  Destructor
	~CSettings();

private:
///  Handle of frame rate test window
	HWND			m_windowHandle;
///  DC of frame rate test window
	HDC				m_windowDC;
///  Window width
	int				m_windowWidth;
///  Window height
	int				m_windowHeight;
///  Count of colunms in the grid
	int				m_colCount;
///  Count of rows in the grid
	int				m_rowCount;
///  Local port for CSocketStream
	unsigned int	m_localPort;
///  Connect timeout
	unsigned int	m_timeout;
///  Remote address
	tstring			m_remoteAddr;
///  Remote port
	unsigned int	m_remotePort;
///  Starting viewer at application starting
	bool			m_clientAutoStart;
///  Client display mode
	int				m_displayMode;
///  Test time
	DWORD			m_testTime;
///  Using garbage thread
	bool			m_useGarbageThread;
///  Is garbage will be sent
	bool			m_sendGarbage;

public:
///  Sets window's handle and gets window's properties
///  @param hwnd - new window handle
	void SetWindow( HWND hwnd );

///  Returns handle of the window
	HWND GetWindow() const;

///  Returns DC of test window
	HDC GetDC() const;

///  Sets new window width
///  @param width - new window width
	void SetWindowWidth( const int width );

///  Returns width of the window
	int GetWindowWidth() const;

///  Sets new window height
///  @param height - new window height
	void SetWindowHeight( const int height );

///  Returns height of the window
	int GetWindowHeight() const;

///  Returns count of columns in the grid
	int GetColCount() const;

///  Returns count of rows in the grid
	int GetRowCount() const;

///  Sets new local port
///  @param port - local port
	void SetLocalPort( const unsigned int port );

///  Returns local port
	unsigned int GetLocalPort() const;

///  Sets new connection timeout
///  @param timeout - connect timeout
	void SetTimeout( const unsigned int timeout );

///  Returns connection timeout
	unsigned int GetTimeout() const;

///  Sets remote address
///  @param addr - remote address
	void SetRemoteAddr( const tstring& addr );

///  Returns remote address
	tstring GetRemoteAddr() const;

///  Sets remote port
///  @param port - new remote port
	void SetRemotePort( const unsigned int port );

///  Returns remote port
	unsigned int GetRemotePort() const;

///  Set "auto start" mode for client
	void SetClientAutoStart( const bool autoStart );

///  Get client's "auto start mode"
	bool GetClientAutoStart() const;

///  Sets new display mode for client
///  @param mode - new display mode
	void SetDisplayMode( const int mode );

///  Returns current display mode
	int GetDisplayMode() const;

///  Sets test time
///  @param testTime - new test time
	void SetTestTime( const DWORD testTime );

///  Rerurns test time
	DWORD GetTestTime() const;

///  Set usage garbage thread
	void SetUseGarbageThread( const bool useThread );

///  Get usage garbage thread
	bool GetUseGarbageThread() const;

///  Sending garbage mode
	void SetSendGarbage( const bool sendGarbage );

///  Get sending garbage mode
	bool GetSendGarbage() const;
};

/// Should be used to CSettings as single instance
#define SETTINGS_INSTANCE Loki::SingletonHolder<CSettings, Loki::CreateUsingNew, Loki::DefaultLifetime, Loki::ClassLevelLockable>::Instance()
