#include "ground.h"



#define MAX_LOADSTRING 1000
// Globale Variablen:
HINSTANCE hInst;								// Aktuelle Instanz
TCHAR szTitle[MAX_LOADSTRING];					// Titelleistentext
TCHAR szWindowClass[MAX_LOADSTRING];			// Klassenname des Hauptfensters
HFONT editfont;
HWND hMain = NULL;
//-----------------------
static char MainWin[] = "MainWin";

int speed = 4, speed2 = 6;
int px = 0;
int bx = 270, by = 240;
int row, col;
int dirx = speed/2, diry = speed/2;
bool blockDestroyed[3][15];

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void draw_line(HDC DC, int x, int y, int a, int b, int red, int green, int blue, int width);
void draw_block(HDC dc, int nextx, int nexty);
void destroy_block(int row, int col);

HBRUSH  hWinCol = CreateSolidBrush(RGB(180, 180, 180));

HWND g_hwnd=NULL;
//timer:
#define TIMER1 111

// Vorwärtsdeklarationen der in diesem Codemodul enthaltenen Funktionen:
/*ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

*/

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{

	
	hInst = hInstance;												//						save in global variable for further use
	// TODO: Hier Code einfügen.
	MSG msg;
	
	// Globale Zeichenfolgen initialisieren
	LoadString(hInstance, 103, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, 104, szWindowClass, MAX_LOADSTRING);
	//register Window													<<<<<<<<<<			STEP ONE: REGISTER WINDOW						!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	WNDCLASSEX wcex;												//						=> Filling out struct WNDCLASSEX
	BOOL Result = TRUE;
	wcex.cbSize = sizeof(WNDCLASSEX);								//						size of this struct (don't know why
	wcex.style = CS_HREDRAW | CS_VREDRAW;							//						?
	wcex.lpfnWndProc = (WNDPROC)WndProc;							//						The corresponding Proc File -> Message loop switch-case file
	wcex.cbClsExtra = 0;											//
	wcex.cbWndExtra = 0;											//
	wcex.hInstance = hInstance;										//						The number of the program
	wcex.hIcon = LoadIcon(hInstance, NULL);							//
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);						//
	wcex.hbrBackground = hWinCol;									//						Background color
	wcex.lpszMenuName = NULL;										//
	wcex.lpszClassName = MainWin;									//						Name of the window (must the the same as later when opening the window)
	wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);					//
	Result = (RegisterClassEx(&wcex) != 0);							//						Register this struct in the OS

				//													STEP TWO: OPENING THE WINDOW with x,y position and xlen, ylen !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	hMain = CreateWindow(MainWin, "TestWin CST 320", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, hInst, NULL);
	if (!hMain)return 0;
	ShowWindow(hMain, nCmdShow);
	UpdateWindow(hMain);

	//													STEP THREE: Going into the infinity message loop							  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// Hauptmeldungsschleife:
	bool quit = FALSE;
	while (!quit)
	{
		//if (GetMessage(&msg, NULL, 0, 0) > 0)
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == TRUE)
		{
			if (msg.message == WM_QUIT){ quit = TRUE; break; }

			
				TranslateMessage(&msg);								//						IF a meessage occurs, the WinProc will be called!!!!
				DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
///////////////////////////////////////////////////
void redr_win_full(HWND hwnd, bool erase)
	{
	RECT rt;
	GetClientRect(hwnd, &rt);
	InvalidateRect(hwnd, &rt, erase);
	}
void draw_text(HDC DC,char text[], int x, int y, int r, int g, int b)
	{
	COLORREF rgb = RGB(r, g, b);
	SetTextColor(DC, rgb);
	SetBkMode(DC, TRANSPARENT);
	TextOut(DC, x, y, text, strlen(text));
	}
///////////////////////////////////
//		This Function is called every time the Left Mouse Button is down
///////////////////////////////////
void OnLBD(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{

}
///////////////////////////////////
//		This Function is called every time the Right Mouse Button is down
///////////////////////////////////
void OnRBD(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
	{
	
	}
///////////////////////////////////
//		This Function is called every time a character key is pressed
///////////////////////////////////
void OnChar(HWND hwnd, UINT ch, int cRepeat)
{

}
///////////////////////////////////
//		This Function is called every time the Left Mouse Button is up
///////////////////////////////////
void OnLBU(HWND hwnd, int x, int y, UINT keyFlags)
{

	
}
///////////////////////////////////
//		This Function is called every time the Right Mouse Button is up
///////////////////////////////////
void OnRBU(HWND hwnd, int x, int y, UINT keyFlags)
	{
	

	}
///////////////////////////////////
//		This Function is called every time the Mouse Moves
///////////////////////////////////
void OnMM(HWND hwnd, int x, int y, UINT keyFlags)
{
	px = x;
if ((keyFlags & MK_LBUTTON) == MK_LBUTTON)
	{
	}

if ((keyFlags & MK_RBUTTON) == MK_RBUTTON)
	{
	}
}
///////////////////////////////////
//		This Function is called once at the begin of a program
///////////////////////////////////
#define TIMER1 1

BOOL OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct)
{
g_hwnd = hwnd;
	editfont = CreateFont(-10, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, "Arial");	
	if (!SetTimer(hwnd, TIMER1, 1, NULL))
		{
		MessageBox(hwnd, "No Timers Available", "Info", MB_OK);
		return FALSE;
		}
	
	return TRUE;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	HWND hwin;
	
	switch (id)
	{
	default:
		break;
	}

}
//************************************************************************
void OnTimer(HWND hwnd, UINT id)
	{
	// hitting left/right wall
	bx += dirx;
	if (bx > 484)
		bx = 484;
	else if (bx < 0)
		bx = 0;

	if (bx == 484 || bx == 0)
		dirx *= -1;

	// hitting top/bottom wall
	by += diry;
	if (by > 484)
		by = 484;
	else if (by < 0)
		by = 0;

	if (by == 0)
		diry *= -1;


	// hitting paddle
	if (by <= 400 && by > 392) {
		if (bx < px - 24 && bx >= px - 35) {
			diry = -speed;
			dirx = -speed2;
		}
		else if (bx < px - 7 && bx >= px - 24) {
			diry = -speed;
			dirx = -speed;
		}
		else if (bx <= px + 6 && bx >= px - 7) {
			diry = -speed;
			dirx = 0;
		}
		else if (bx <= px + 24 && bx > px + 6) {
			diry = -speed;
			dirx = speed;
		}
		else if (bx <= px + 35 && bx > px + 24) {
			diry = -speed;
			dirx = speed2;
		}
	}

	// hitting blocks
		
	if (by <= 86 && by > 64)
		row = 2;
	if (by <= 64 && by > 42)
		row = 1;
	if (by <= 42 && by >= 20)
		row = 0;

	if (by <= 86 && by >= 20) {
		for (int k = 0; k < 15; k++) {
			if (bx < (35 + 32*k) && bx >= (3 + 32*k))
				col = k;
		}
		destroy_block(row, col);
	}


	redr_win_full(hwnd, FALSE);
	}
//************************************************************************
///////////////////////////////////
//		This Function is called every time the window has to be painted again
///////////////////////////////////


void OnPaint(HWND hwnd)
	{
	PAINTSTRUCT PaintStruct;
	double_buffer_ doublebuffer;
	HDC dc = doublebuffer.begin(hwnd, &PaintStruct, RGB(255, 255, 255));
	
	bool win = true;
	int nexty = 20, nextx = 3;
	
	for (int j = 0; j < 3; j++)
		for (int i = 0; i < 15; i++) {
			if (blockDestroyed[j][i] == false)
				win = false;
	}
	if (win == true)
		draw_text(dc, "Winner", 150, 100, 0, 0, 0);
	if (by == 484 && win == false)
		draw_text(dc, "Game Over", 200, 200, 0, 0, 0);

	for (int j = 0; j < 3; j++) {
		nextx = 3;
		for (int i = 0; i < 15; i++) {
			if (blockDestroyed[j][i] == false)
				draw_block(dc, nextx, nexty);
			nextx += 32;
		}
		nexty += 22;
	}

	//paddle
	draw_line(dc, px-30, 400, px + 30, 400, 100, 100, 100, 10);

	//ball
	draw_line(dc, bx, by, bx, by, 0, 255, 0, 10);

	doublebuffer.end();
	}
//****************************************************************************

void draw_line(HDC DC,int x, int y, int a, int b, int red, int green, int blue, int width){
	HPEN Stift = CreatePen(PS_SOLID, width, RGB(red, green, blue));
	SelectObject(DC, Stift);
	MoveToEx(DC, x, y, NULL);
	LineTo(DC, a, b);
	DeleteObject(Stift);
	}

void draw_block(HDC dc, int nextx, int nexty) {
	for (int yy = nexty; yy < nexty + 20; yy++)
		for (int xx = nextx; xx < nextx + 30; xx++) 
			SetPixel(dc, xx, yy, RGB(0, 200, 0));
}

void destroy_block(int row, int col) {
	if (blockDestroyed[row][col] == false) {
		if (by >= 20 && by <= 22 || by >= 40 && by <= 44 || by >= 62 && by <= 66 || by >= 84 && by <= 86) {
			blockDestroyed[row][col] = true;
			diry *= -1;
		}
		else {
			blockDestroyed[row][col] = true;
			dirx *= -1;
		}
	}
}
//*************************************************************************
void OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
	{

	switch (vk)
		{
			default:break;
			
		}
	}

//*************************************************************************
void OnKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
	{
	switch (vk)
		{
			default:break;
			
		}

	}


//**************************************************************************
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	SCROLLINFO si;


	
	switch (message)
	{
	/*
	#define HANDLE_MSG(hwnd, message, fn)    \
    case (message): return HANDLE_##message((hwnd), (wParam), (lParam), (fn))
	*/
		
		HANDLE_MSG(hwnd, WM_CHAR, OnChar);
		HANDLE_MSG(hwnd, WM_LBUTTONDOWN, OnLBD);
		HANDLE_MSG(hwnd, WM_LBUTTONUP, OnLBU);
		HANDLE_MSG(hwnd, WM_MOUSEMOVE, OnMM);	
		HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hwnd, WM_TIMER, OnTimer);
		HANDLE_MSG(hwnd, WM_KEYDOWN, OnKeyDown);
		HANDLE_MSG(hwnd, WM_KEYUP, OnKeyUp);

	case WM_ERASEBKGND:
		return (LRESULT)1;
	case WM_DESTROY:
		
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
