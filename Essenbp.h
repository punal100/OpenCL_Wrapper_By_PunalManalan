#pragma once

#ifndef ESSENTIAL_FUNCTIONS_BY_PUNAL
#define ESSENTIAL_FUNCTIONS_BY_PUNAL

#include <iostream>
#include <fstream>// For file reading
#include <chrono>// Mainly For FRAMERATE(FPS) LOCK

namespace Essenbp//Essential Functions By Punal
{
	struct UnknownDataAndSize
	{
	private:
		bool IsDummyValue = true;
		void* Data = nullptr;
		size_t SizeOfData = 0;

	public:
		void CopyAndStoreData(void* ArgData, size_t ArgSizeOfData, bool& IsSuccesful, bool DoADummyCopy = false)
		{
			IsSuccesful = false;

			if (!DoADummyCopy)
			{
				if (ArgData == nullptr)
				{
					std::cout << "\n Error nullptr for ArgData in CopyAndStoreData In: UnknownDataAndSize!\n";
					std::cout << "If Dummy Value is to be passed set DoADummyCopy(4th argument) to false, and set ArgData = nullptr!\n";
					return;
				}
				SizeOfData = ArgSizeOfData;
				if (SizeOfData == 0)
				{
					std::cout << "\n Error Size Of SizeOfData is Equal to Zero in CopyAndStoreData In: UnknownDataAndSize!\n";
					return;
				}
				Data = malloc(SizeOfData);
				if (Data == nullptr)
				{
					std::cout << "\n Error Allocating : " << SizeOfData << " Byes Of Memory for Data in CopyAndStoreData In: UnknownDataAndSize!\n";
					return;
				}
				else
				{
					for (int i = 0; i < SizeOfData; ++i)// Memccpy bad
					{
						((char*)Data)[i] = ((char*)ArgData)[i];// I could simply convert void* to char*... but i left it as void* for the purpose of 'readability'
					}
				}
			}
			else
			{
				if (ArgData != nullptr)
				{
					std::cout << "\n Error ArgData is not set to nullptr in CopyAndStoreData In: UnknownDataAndSize!\n";
					std::cout << "If Actual value is to be passed ignore DoADummyCopy(4th argument)!\n";
					return;
				}
				else
				{
					IsDummyValue = true;
					Data = nullptr;
				}
			}
		}

		bool GetIsDataValue() { return IsDummyValue; }
		void* GetData() { return Data; }
		size_t GetDataSize() { return SizeOfData; }

		~UnknownDataAndSize()
		{
			if (Data != nullptr)
			{
				free(Data);
			}
		}
	};

	uint64_t TimeSinceEpochInMilliSecond()
	{
		using namespace std::chrono;
		return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	}

	uint64_t TimeSinceEpochInNanoSecond()
	{
		using namespace std::chrono;
		return duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
	}

	uint64_t ForFunctionStartTime;//Global Variable
	void TimeCalculationInMilliseconds()
	{
		std::cout << '\n';
		std::cout << "      StartTime :" << ForFunctionStartTime << '\n';
		std::cout << "        EndTime :" << TimeSinceEpochInMilliSecond() << '\n';
		std::cout << "Time Difference :" << TimeSinceEpochInMilliSecond() - ForFunctionStartTime << '\n' << '\n';
		ForFunctionStartTime = TimeSinceEpochInMilliSecond();
	}

	void TimeCalculationInNanoSeconds()
	{
		std::cout << '\n';
		std::cout << "      StartTime :" << ForFunctionStartTime << '\n';
		std::cout << "        EndTime :" << TimeSinceEpochInNanoSecond() << '\n';
		std::cout << "Time Difference :" << TimeSinceEpochInNanoSecond() - ForFunctionStartTime << '\n' << '\n';
		ForFunctionStartTime = TimeSinceEpochInNanoSecond();
	}

	int Clamp(double Number, double Min, double Max)
	{
		if (Number > Max)
		{
			return Max;
		}
		if (Number < Min)
		{
			return Min;
		}
		return Number;
	}

	void PrintVector3Dfor(float Pointx, float Pointy, float Pointz, std::string Name, bool InvertXY)
	{
		int SpaceBar = 50;
		SpaceBar = Clamp(SpaceBar - Name.length(), 0, 50);
		std::cout << "\n" << Name << " =";
		for (int i = 0; i < SpaceBar; ++i)
		{
			std::cout << " ";
		}
		std::cout << "(";
		if (InvertXY)
		{
			printf("%f", Pointy);
			printf(", %f", Pointx);
		}
		else
		{
			printf("%f", Pointx);
			printf(", %f", Pointy);
		}

		printf(", %f )", Pointz);
	}

	//NOTE: By PointerToArrayOfPointers it means Example : int** Array, Array[n] == int*, Basicaly an Array of Array of size [1][n]
	//NOTE: Pass first parameter using (void***)&
	void Malloc_PointerToArrayOfPointers(void*** PointerTo_PointerToArrayOfPointers, unsigned int NumOfPointerToAdd, unsigned int SizeOfEachPointer, bool& IsSuccesful)
	{
		IsSuccesful = false;

		if (PointerTo_PointerToArrayOfPointers == nullptr)
		{
			std::cout << "\n Error nullptr Passed for 'PointerTo_PointerToArrayOfPointers'. in Malloc_PointerToArrayOfPointers In: Essenbp!\n";
			return;
		}

		if (NumOfPointerToAdd == 0)
		{
			std::cout << "\n Error NumOfPointerToAdd Is 0. in Malloc_PointerToArrayOfPointers In: Essenbp!\n";
			return;
		}
		if (SizeOfEachPointer == 0)
		{
			std::cout << "\n Error SizeOfEachPointer Is 0. in Malloc_PointerToArrayOfPointers In: Essenbp!\n";
			return;
		}

		if (*PointerTo_PointerToArrayOfPointers != nullptr)
		{
			std::cout << "\n Error Not Null : PointerToArrayOfPointers already pointing to some memory, free the memory First. in Malloc_PointerToArrayOfPointers In: Essenbp!\n";
		}
		else
		{
			*PointerTo_PointerToArrayOfPointers = (void**)calloc(NumOfPointerToAdd, SizeOfEachPointer);// Setting everything to Zero
			if (*PointerTo_PointerToArrayOfPointers == nullptr)
			{
				std::cout << "\n Error Allocating : " << NumOfPointerToAdd * SizeOfEachPointer << " Byes Of Memory for *PointerTo_PointerToArrayOfPointers in Malloc_PointerToArrayOfPointers In: Essenbp!\n";
				return;
			}
			IsSuccesful = true;
		}
	}


#ifdef _WIN32
	void GetFileContent(const std::string Path, std::string& DataStorage, bool& IsSuccesful)
	{
		IsSuccesful = false;
		FILE* TheFile;
		errno_t FileError = fopen_s(&TheFile, Path.c_str(), "r");

		if (FileError != 0)
		{
			char File_ErrorBuffer[256];//[strerrorlen_s(err) + 1]; strerrorlen_s() is undefined... //So using buffer size of 256...
			strerror_s(File_ErrorBuffer, sizeof(File_ErrorBuffer), FileError);
			std::cout << "\nError '" << File_ErrorBuffer << "'\n: Unable to Open File in GetFileContent In: Essenbp,\n File Path: " << Path << "\n";
		}
		else
		{
			std::fseek(TheFile, 0, SEEK_END);
			DataStorage.resize(std::ftell(TheFile));
			std::rewind(TheFile);
			std::fread(&DataStorage[0], 1, DataStorage.size(), TheFile);
			std::fclose(TheFile);
			IsSuccesful = true;
		}
	}
#else
	//Linux
	void GetFileContent(const std::string Path, std::string& DataStorage, bool& IsSuccesful)
	{
		IsSuccesful = false;
		FILE* TheFile = fopen(Path.c_str(), "r");

		if (TheFile != nullptr)
		{
			std::cout << "\nError : Unable to Open File in GetFileContent In: Essenbp,\n File Path: " << Path << "\n";
		}
		else
		{
			std::fseek(TheFile, 0, SEEK_END);
			DataStorage.resize(std::ftell(TheFile));
			std::rewind(TheFile);
			std::fread(&DataStorage[0], 1, DataStorage.size(), TheFile);
			std::fclose(TheFile);
			IsSuccesful = true;
		}
	}
#endif
};

#endif
