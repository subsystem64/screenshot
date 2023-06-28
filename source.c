#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

//check if filename is in use
int exist(const char* name){
	struct stat   buffer;
	return (stat(name, &buffer) == 0);
}
//generate random int
int rand_int() {
	int x = rand() % 10000;
	return x;
}
//generates filename
char* get_file_name(char* path) {
	if (NULL) {
		char* buff = malloc(64 * sizeof(char));
		snprintf(buff, 64, "D:\\%i%i.bmp", rand_int(), rand_int());
		return buff;
	}
	char* buff = malloc(64 * sizeof(char));
	snprintf(buff, 64, "%s", path);
	return buff;
	
}

bool SaveBMPFile(char* filename, HBITMAP bitmap, HDC bitmapDC, int width, int height);

bool ScreenCapture(int x, int y, int width, int height, char* filename) {
	// get a DC compat. w/ the screen
	HDC hDc = CreateCompatibleDC(0);

	// make a bmp in memory to store the capture in
	HBITMAP hBmp = CreateCompatibleBitmap(GetDC(0), width, height);

	// join em up
	SelectObject(hDc, hBmp);

	// copy from the screen to my bitmap
	BitBlt(hDc, 0, 0, width, height, GetDC(0), x, y, SRCCOPY);

	// save my bitmap
	bool ret = SaveBMPFile(filename, hBmp, hDc, width, height);

	// free the bitmap memory
	DeleteObject(hBmp);

	return ret;
}

main() {
	//seed rng
	srand((unsigned)time(NULL));

    //Register hotkey
    if (RegisterHotKey(NULL, 1, MOD_ALT | MOD_NOREPEAT, 0x43)) {
        printf("\n[*]Registering hotkey using MOD_NOREPEAT flag ...");
        printf("[-]Press 'ALT + c' to take a screenshot\n");
    }
    else {
        printf("[-]An error occured ...");
		printf("Error Code: %d\n", GetLastError());
    }

    MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0) != 0) {
		//if WM_HOTKEY message is recieved
		if (msg.message == WM_HOTKEY) {

			//generate filename
			char* filename = get_file_name(NULL);

			while (exist(filename)) {//if filename already in use create a new one
				filename = get_file_name(NULL);
			}
			if (ScreenCapture(0, 0, 1920, 1080, filename)) {
				printf("[-]Screenshot Saved to \"%s\"\n",filename);
			}
			else {
				printf("[-]An error occured ...Screenshot failed\n");
			}
			
		}
	}
}
