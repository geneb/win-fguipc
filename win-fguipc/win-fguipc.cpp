// win-fguipc.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "win-fguipc.h"

#include "FGSession.h"
#include "FGRead.h"
#include "FGWrite.h"
#include "Common.h"

#pragma comment(lib, "Ws2_32.lib")  // this brings in the winsock code.
#pragma comment(lib, "Mswsock.lib")

#define MAX_LOADSTRING 100
#define FGFS_PORT "5501"      // default port we talk to FlightGear on.

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
WSAData wsaData;                            // used for Winsock work.
SOCKET fgSock = INVALID_SOCKET;             // our link to the running instance of FlightGear.


// Start of FGUIPC specfic globals.
UINT FG_CALL = 0;
static UINT FGModuleCallMessage = WM_USER + 256;
__int32 GlobalOperationType = WaitingT;

// End of FGUIPC specfic globals.


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

/* Winsock routines */
u_long              LookupAddress(const char* pcHost);
SOCKET              EstablishConnection(u_long nRemoteAddr, u_short nPort);

/*******************************************************************************/

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_ LPTSTR    lpCmdLine,
                       _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    MSG msg;
    HACCEL hAccelTable;

    // Initialize global strings
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_WINFGUIPC, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINFGUIPC));

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINFGUIPC));
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WINFGUIPC);
    wcex.lpszClassName	= szWindowClass;  // set to FS98MAIN in the string resource file.
    wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;
    FILE *fHandle;
    struct addrinfo *result = NULL,
        *ptr = NULL,
        hints;

    fopen_s(&fHandle, "fguipc.log", "w");
    hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindow(szWindowClass, TEXT("FS98MAIN"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return FALSE;
    } else {
        FG_CALL = RegisterWindowMessage(TEXT(FG_CALL_CONNECTION));
        if(FG_CALL != 0) {
            fprintf_s(fHandle, "Register call message OK\n");

            XConnectMemBlock = new BYTE[0x10000];

            for (int i = 0; i < 0x10000; i++) {
                XConnectMemBlock[i] = 0;
            }
        } else {
            fprintf_s(fHandle, "Failed to register call message!\n");
        }
        // create the network interface...
        int nCode = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (nCode != 0) {
            // it died.  we die!
            MessageBox(NULL, (LPCWSTR)L"Unable to complete Winsock initialization.",(LPCWSTR)L"Yikes!",MB_OK);
            return FALSE;
        } else {
            // make the connection to FlightGear.
            ZeroMemory(&hints, sizeof(hints));
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;
            int addrResult = getaddrinfo("localhost", FGFS_PORT, &hints, &result);
            if (addrResult != 0) {
                MessageBox(NULL, (LPCWSTR)L"getaddrinfo() failed!",(LPCWSTR)L"Yikes!",MB_OK);
                WSACleanup();
                return FALSE;
            }
            
            for(ptr=result; ptr != NULL; ptr = ptr->ai_next) {
                fgSock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
                if (fgSock == INVALID_SOCKET) {
                    // failed to create the socket, die ugly for now.
                    MessageBox(NULL, (LPCWSTR)L"Unable to create socket!",(LPCWSTR)L"Yikes!",MB_OK);
                    WSACleanup();
                    return FALSE;
                } else {
                    if (connect(fgSock, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
                        closesocket(fgSock);
                        fgSock = INVALID_SOCKET;
                        continue;
                    }
                    break;
                }
            }
            freeaddrinfo(result);

            // we need to put FlightGear into "raw data" mode.
            // TODO: ERROR CHECKING!

            int bytesout = send(fgSock, "data\r\n", strlen("data\r\n"), 0);
            if (bytesout != 6) {
                MessageBox(NULL, (LPCWSTR)L"Didn't send it all!",(LPCWSTR)L"Yikes!",MB_OK);

            }


        }

    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    fclose(fHandle);
    return TRUE;
}



//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    // Let's see if it's an FGUIPC event we need to handle before we check for the standard windows events...

    if((message == FG_CALL) || (message == FGModuleCallMessage)) {
        HANDLE hMap = 0;
        BYTE* pView = NULL;
        BYTE* pNext = NULL;
        FG_ACTION_READ_HDR* pHdrR = NULL;
        FG_ACTION_WRITE_HDR* pHdrW = NULL;
        FG_ACTION_READTOKEN_HDR* pHdrRT = NULL;
        FG_ACTION_WRITETOKEN_HDR* pHdrWT = NULL;
        FG_ACTION_SEARCH_HDR* pHdrS = NULL;
        DWORD* pdw = NULL;
        //DWORD true_offset;
        char szName[MAX_PATH];
        ATOM atom = wParam;
        LRESULT result = FG_RETURN_FAILURE;

        FILE* fHandle;
        errno_t fileError = fopen_s(&fHandle, "fguipc.log", "a+");
        fprintf_s(fHandle, "sizeof(FG_ACTION_READ_HDR)=%d\n", (int)sizeof(FG_ACTION_READ_HDR));
        if (message == FG_CALL) 
            fprintf_s(fHandle, "FG_CALL");

        if (message == FGModuleCallMessage) 
            fprintf_s(fHandle, "FGModuleCallMessage");

        fprintf_s(fHandle, " W=%d L=%p\n", (int)wParam, (void*)lParam);

        if (message == FGModuleCallMessage) {
            hMap = (BYTE*)lParam;
        }
        else {
            GlobalGetAtomName(atom, (LPWSTR)szName, MAX_PATH);
            hMap = OpenFileMapping(FILE_MAP_WRITE, FALSE, (LPWSTR)szName);

            MEMORY_BASIC_INFORMATION info;
            VirtualQuery(hMap, &info, sizeof(info));
            fprintf_s(fHandle, "FileMapping RegionSize=%d\n", (int)info.RegionSize);
        }

        if (hMap) {
            if (message == FGModuleCallMessage)
                pView = (BYTE*)hMap;
            else
                pView = (BYTE*)MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, 0);
            if (pView) {
                result = FG_RETURN_SUCCESS;
                pNext = pView;

                pdw = (DWORD*)pView;
                while ((*pdw) && (result == FG_RETURN_SUCCESS))  {	
                    switch (*pdw) {
                    case FG_ACTION_READ:        /*fprintf_s(fHandle, "  FG_ACTION_READ\n"); */ break;
                    case FG_ACTION_READTRUE:    /*fprintf_s(fHandle, "  FG_ACTION_READTRUE\n");*/ break;
                    case FG_ACTION_WRITE:       /*fprintf_s(fHandle, "  FG_ACTION_WRITE\n");*/ break;
                    case FG_ACTION_WRITETRUE:   /*fprintf_s(fHandle, "  FG_ACTION_WRITETRUE\n");*/ break;
                    case FG_ACTION_READTOKEN:   /*fprintf_s(fHandle, "  FG_ACTION_READTOKEN\n");*/ break;
                    case FG_ACTION_WRITETOKEN:  /*fprintf_s(fHandle, "  FG_ACTION_WRITETOKEN\n");*/ break;
                    case FG_ACTION_SEARCH:      /*fprintf_s(fHandle, "  FG_ACTION_SEARCH\n");*/ break;
                    }
                    switch (*pdw) {
                    case FG_ACTION_READ:
                    case FG_ACTION_READTRUE:
                        {
                            GlobalOperationType = ReadingT;

                            pHdrR = (FG_ACTION_READ_HDR*)pdw;

                            pNext += sizeof(FG_ACTION_READ_HDR);
                            if (pHdrR->size) {
                                GetVariable(pNext, pHdrR->offset, pHdrR->size);

                                //CopyMemory(pNext, (BYTE*)true_offset, pHdrR->size);
                            }
                            pNext += pHdrR->size;
                            pdw = (DWORD*)pNext;
                        }
                        break;
                    case FG_ACTION_WRITE:
                    case FG_ACTION_WRITETRUE:
                        {
                            GlobalOperationType = WritingT;

                            pHdrW = (FG_ACTION_WRITE_HDR*)pdw;

                            pNext += sizeof(FG_ACTION_WRITE_HDR);
                            if (pHdrW->size) {

                                WriteVariable(pNext, pHdrW->offset, pHdrW->size);


                            }
                            pNext += pHdrW->size;
                            pdw = (DWORD*)pNext;
                        }
                        break;

                    default:
                        {
                            *pdw = 0;
                            result = FG_RETURN_FAILURE;
                        }
                        break;
                    }
                }

                pNext = pView;

                if (message == FG_CALL)
                    UnmapViewOfFile((LPVOID)pView);
            }

            if (message == FG_CALL)
                CloseHandle(hMap);
        }

        fclose(fHandle);
        return result;

    }


    switch (message) {
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        // Parse the menu selections:
        switch (wmId) {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            closesocket(fgSock);
            WSACleanup(); // clean up that network nastyness...

            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code here...
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


u_long LookupAddress(const char* pcHost) {
    // cribbed from a simple stream client example I found on the net.
    u_long nRemoteAddr = inet_addr(pcHost);
    if (nRemoteAddr == INADDR_NONE) {
        // pcHost isn't a dotted IP, so resolve it through DNS
        hostent* pHE = gethostbyname(pcHost);
        if (pHE == 0) {
            return INADDR_NONE;
        }
        nRemoteAddr = *((u_long*)pHE->h_addr_list[0]);
    }

    return nRemoteAddr;
}

SOCKET EstablishConnection(u_long nRemoteAddr, u_short nPort) {
    // Create a stream socket
    SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd != INVALID_SOCKET) {
        sockaddr_in sinRemote;
        sinRemote.sin_family = AF_INET;
        sinRemote.sin_addr.s_addr = nRemoteAddr;
        sinRemote.sin_port = nPort;
        if (connect(sd, (sockaddr*)&sinRemote, sizeof(sockaddr_in)) ==
            SOCKET_ERROR) {
                sd = INVALID_SOCKET;
        }
    }

    return sd;
}