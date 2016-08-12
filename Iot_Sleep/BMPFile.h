// _Queue.h: interface for the C_Queue class.
//
// Date : 2000. 3.
// Modify : 2000. 3.
// Programer : Chang-su, Kim
//////////////////////////////////////////////////////////////////////

// Make BMP file information
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);

// Create BMP file
void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, 
		HBITMAP hBMP, HDC hDC);

// Error function
void errhandler(LPTSTR sErr, HWND hwnd);
