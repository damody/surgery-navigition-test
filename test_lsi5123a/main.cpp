#include <iostream>
#include <cstdio>
#include "LSI5123.h"
#include <windows.h>
#include <cstdio>
#include <conio.h>
#include <string>

class FileMappingDataSetter
{
public:
	int	m_BufferSize;
	HANDLE m_hMapFile;
	char* m_pBuf;
	FileMappingDataSetter():m_BufferSize(1024){}
	~FileMappingDataSetter()
	{
		UnmapViewOfFile(m_pBuf);
		CloseHandle(m_hMapFile);
	}
	bool OpenShareMemory(std::wstring name)
	{
		m_hMapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,    // use paging file
			NULL,                    // default security
			PAGE_READWRITE,          // read/write access
			0,                       // maximum object size (high-order DWORD)
			m_BufferSize,                // maximum object size (low-order DWORD)
			name.c_str());                 // name of mapping object
		if (m_hMapFile == NULL)
		{
			return false;
		}
		m_pBuf = (char*) MapViewOfFile(m_hMapFile, // handle to map object
			FILE_MAP_ALL_ACCESS,  // read/write permission
			0,
			0,
			m_BufferSize);

		if (m_pBuf == NULL)
		{
			CloseHandle(m_hMapFile);

			return 0;
		}
	}
	void SetData(int* input)
	{
		int* data = (int*)m_pBuf;
		for (int i=0;i<6;i++)
		{
			data[i] = input[i];
		}
	}
};

int main()
{
	int res = lsi5123_initial();

	int data[6];
	FileMappingDataSetter fmr;
	fmr.OpenShareMemory(L"Global\\Lsi5123a");
	lsi5123_load_counter(0, AXIS_X, 0);
	lsi5123_load_counter(0, AXIS_Y, 0);
	lsi5123_load_counter(0, AXIS_Z, 0);
	lsi5123_load_counter(1, AXIS_X, 0);
	lsi5123_load_counter(1, AXIS_Y, 0);
	lsi5123_load_counter(1, AXIS_Z, 0);
	for (;;)
	{
		long lngCounter;
		lsi5123_read_counter(0, AXIS_X, &lngCounter);
		printf("x1: %d, ", lngCounter);
		data[0] = lngCounter;
		lsi5123_read_counter(0, AXIS_Y, &lngCounter);
		printf("y1: %d, ", lngCounter);
		data[1] = lngCounter;
		lsi5123_read_counter(0, AXIS_Z, &lngCounter);
		printf("z1: %d, ", lngCounter);
		data[2] = lngCounter;
		lsi5123_read_counter(1, AXIS_X, &lngCounter);
		printf("x2: %d, ", lngCounter);
		data[3] = lngCounter;
		lsi5123_read_counter(1, AXIS_Y, &lngCounter);
		printf("y2: %d, ", lngCounter);
		data[4] = lngCounter;
		lsi5123_read_counter(1, AXIS_Z, &lngCounter);
		printf("z2: %d, ", lngCounter);
		data[5] = lngCounter;
		fmr.SetData(data);
		printf("\n");
	}
	lsi5123_close();
}
