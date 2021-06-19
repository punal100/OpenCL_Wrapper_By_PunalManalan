#pragma once

#ifndef ESSENTIAL_FUNCTIONS_BY_PUNAL
#define ESSENTIAL_FUNCTIONS_BY_PUNAL

#include <iostream>
#include <fstream>// For file reading
#include <chrono>// Mainly For FRAMERATE(FPS) LOCK

namespace Essenbp//Essential Functions By Punal
{
	struct UnknownDataAndSizeStruct
	{
	private:
		void* Data = nullptr;
		size_t SizeOfData = 0;

	public:
		UnknownDataAndSizeStruct()
		{
			std::cout << "\n Constructing UnknownDataAndSizeStruct!";
		}

		void FreeData()
		{
			if (Data != nullptr)
			{
				free(Data);//Free Previous Data
				Data = nullptr;
				SizeOfData = 0;
			}			
		}

		//NOTE DoADummyCopy stores only the Size but not the data
		void CopyAndStoreData(void* ArgData, size_t ArgSizeOfData, bool& IsSuccesful, bool DoADummyCopy = false)
		{
			IsSuccesful = false;

			if (!DoADummyCopy)
			{
				if (ArgData == nullptr)
				{
					std::cout << "\n Error nullptr for ArgData in CopyAndStoreData In: UnknownDataAndSizeStruct!\n";
					std::cout << "If Dummy Value is to be passed set DoADummyCopy(4th argument) to false, and set ArgData = nullptr!\n";
					return;
				}
				SizeOfData = ArgSizeOfData;
				if (SizeOfData == 0)
				{
					std::cout << "\n Error Size Of SizeOfData is Equal to Zero in CopyAndStoreData In: UnknownDataAndSizeStruct!\n";
					return;
				}
				FreeData();
				Data = malloc(SizeOfData);
				if (Data == nullptr)
				{
					std::cout << "\n Error Allocating : " << SizeOfData << " Byes Of Memory for Data in CopyAndStoreData In: UnknownDataAndSizeStruct!\n";
					return;
				}
				else
				{
					for (int i = 0; i < SizeOfData; ++i)// Memccpy bad
					{
						((char*)Data)[i] = ((char*)ArgData)[i];// I could simply convert void* to char*... but i left it as void* for the purpose of 'readability'
					}
					IsSuccesful = true;
				}
			}
			else
			{
				if (ArgData != nullptr)
				{
					std::cout << "\n Error ArgData is not set to nullptr in CopyAndStoreData In: UnknownDataAndSizeStruct!\n";
					std::cout << "If Actual value is to be passed ignore DoADummyCopy(4th argument)!\n";
					return;
				}
				else
				{
					FreeData();
					SizeOfData = ArgSizeOfData;
					IsSuccesful = true;
				}
			}
		}

		void* GetData() { return Data; }
		size_t GetDataSize() { return SizeOfData; }

		//NOTE:Use this if Direct access to data is required
		//NOTE: by using this
		void FreeAndResizeData(size_t ArgSizeOfData, bool& IsSuccesful)
		{
			IsSuccesful = false;
			FreeData();

			SizeOfData = ArgSizeOfData;
			if (SizeOfData == 0)
			{
				std::cout << "\n Error Size Of SizeOfData is Equal to Zero in FreeAndResizeDataAndReturnPointerToDataPointer in UnknownDataAndSizeStruct In: Essenbp!\n";
				return;
			}
			Data = malloc(SizeOfData);
			if (Data == nullptr)
			{
				std::cout << "\n Error Allocating : " << SizeOfData << " Byes Of Memory for Data in FreeAndResizeDataAndReturnPointerToDataPointer in UnknownDataAndSizeStruct In: Essenbp!\n";
				return;
			}
			IsSuccesful = true;
		}
		
		//NOTE: The Data Is Returned But, not released, SO USE THIS WITH CAUTION, if being careless then there is a HUGE chance of memory leak
		void GetDataAndSizeAndSetDataToNull(void** ReturnData, size_t& ReturnDataSize)
		{
			if (ReturnData != nullptr)
			{
				*ReturnData = Data;
				ReturnDataSize= SizeOfData;
				Data = nullptr;
				SizeOfData = 0;
			}
			else
			{
				std::cout << "\n Error Argument ReturnData is nullptr in GetDataAndSizeAndSetDataToNull in UnknownDataAndSizeStruct In: Essenbp!\n";
			}
		}

		~UnknownDataAndSizeStruct()
		{
			FreeData();
		}
	};

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

	struct ArrayOfUnknownDataAndSize
	{
	private:
		unsigned int TotalNumberOfUnknownData = 0;
		UnknownDataAndSizeStruct** ArrayOfUnknownData = nullptr;

		void ResizeArray(unsigned int TotalNumber, bool& IsSuccesful)
		{
			IsSuccesful = false;

			UnknownDataAndSizeStruct** TempUnknownData = nullptr;
			Malloc_PointerToArrayOfPointers((void***)&TempUnknownData, TotalNumber, sizeof(UnknownDataAndSizeStruct*), IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Malloc_PointerToArrayOfPointers failed in ResizeArray In: ArrayOfUnknownDataAndSize!\n";
			}
			else
			{
				IsSuccesful = true;
				int MaxLimit = (TotalNumber < TotalNumberOfUnknownData) ? TotalNumber : TotalNumberOfUnknownData;

				for (int i = 0; i < MaxLimit; ++i)
				{
					TempUnknownData[i] = ArrayOfUnknownData[i];
				}
				if (TotalNumber > TotalNumberOfUnknownData)
				{
					for (int i = TotalNumberOfUnknownData; i < TotalNumber; ++i)
					{
						TempUnknownData[i] = new UnknownDataAndSizeStruct();
						if (TempUnknownData[i] == nullptr)
						{
							std::cout << "\n Error Allocating Bytes of Data for UnknownDataAndSizeStruct[" << i << "] in ResizeArray In: ArrayOfUnknownDataAndSize!\n";
							for (int j = 0; j < i; ++j)
							{
								delete TempUnknownData[j];
							}
							free(TempUnknownData);
							IsSuccesful = false;
							break;
						}
					}
				}
				else
				{
					for (int j = TotalNumber; j < TotalNumberOfUnknownData; ++j)
					{
						delete ArrayOfUnknownData[j];
					}
				}

				if (!IsSuccesful)
				{
					std::cout << "\n Error ResizeArray failed In: ArrayOfUnknownDataAndSize!\n";
				}
				else
				{
					ArrayOfUnknownData = TempUnknownData;
					TotalNumberOfUnknownData = TotalNumber;
				}
			}
		}

	public:
		ArrayOfUnknownDataAndSize()
		{
			std::cout << "\n Constructing ArrayOfUnknownDataAndSize!";
		}

		void AddElement(bool& IsSuccesful)
		{
			IsSuccesful = false;
			ResizeArray(TotalNumberOfUnknownData + 1, IsSuccesful);
			if(!IsSuccesful)
			{
				std::cout << "\n Error ResizeArray failed in AddElement In: ArrayOfUnknownDataAndSize!\n";
			}
		}

		void RemoveElement(unsigned int ElementNumber, bool& IsSuccesful)
		{
			IsSuccesful = false;
			if (ElementNumber > TotalNumberOfUnknownData)
			{
				std::cout << "\n Error ElementNumber Exceeds the total number of Unknown Data Present! in RemoveElement in AddElement In: ArrayOfUnknownDataAndSize!\n";
			}
			else
			{
				if (TotalNumberOfUnknownData == 1)
				{
					delete ArrayOfUnknownData[0];
				}
				else
				{
					//The Gap is filled by next element, and the next element's gap is filled by the element next to it, and so on until last element
					//Example 1,2,3,4,5
					//Remove 3
					//1,2, gap ,4,5
					//4 fills 3rd gap
					//1,2,4, gap ,5
					//5 fills 4rd gap
					//1,2,4,5, gap
					//1,2,4,5 (gap is deleted)
					UnknownDataAndSizeStruct* TempChangeptr = ArrayOfUnknownData[ElementNumber];

					for (int i = TotalNumberOfUnknownData - 1; i > ElementNumber; --i)
					{
						ArrayOfUnknownData[(i - 1)] = ArrayOfUnknownData[i];
					}
					ArrayOfUnknownData[TotalNumberOfUnknownData - 1] = TempChangeptr;

					ResizeArray(TotalNumberOfUnknownData - 1, IsSuccesful);
					if (!IsSuccesful)
					{
						std::cout << "\n Error ResizeArray failed in RemoveElement In: ArrayOfUnknownDataAndSize!\n";
					}
				}
				
			}
		}

		unsigned int GetTotalNumberOfUnknownData()
		{
			return TotalNumberOfUnknownData;
		}

		void GetUnknownData(unsigned int ElementNumber, UnknownDataAndSizeStruct** ReturnUnknownDataAndSize, bool& IsSuccesful)
		{
			if (ElementNumber > TotalNumberOfUnknownData)
			{
				std::cout << "\n Error ElementNumber Exceeds the total number of Unknown Data Present! in GetUnknownData in AddElement In: ArrayOfUnknownDataAndSize!\n";
			}
			else
			{
				*ReturnUnknownDataAndSize = ArrayOfUnknownData[ElementNumber];
			}
		}

		~ArrayOfUnknownDataAndSize()
		{
			for (int j = 0; j < TotalNumberOfUnknownData; ++j)
			{
				delete ArrayOfUnknownData[j];
			}
			free(ArrayOfUnknownData);
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

#ifdef _WIN32
	void GetTextFileContent(const std::string Path, std::string& DataStorage, bool& IsSuccesful)
	{
		IsSuccesful = false;
		FILE* TheFile;
		errno_t FileError = fopen_s(&TheFile, Path.c_str(), "r");

		if (FileError != 0)
		{
			char File_ErrorBuffer[256];//[strerrorlen_s(err) + 1]; strerrorlen_s() is undefined... //So using buffer size of 256...
			strerror_s(File_ErrorBuffer, sizeof(File_ErrorBuffer), FileError);
			std::cout << "\nError '" << File_ErrorBuffer << "'\n: Unable to Open File in GetTextFileContent In: Essenbp,\n File Path: " << Path << "\n";
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

	void WriteToTextFile(const std::string Path, std::string& DataStorage, bool& IsSuccesful)
	{
		IsSuccesful = false;
		FILE* TheFile;
		errno_t FileError = fopen_s(&TheFile, Path.c_str(), "r");

		if (FileError != 0)
		{
			char File_ErrorBuffer[256];//[strerrorlen_s(err) + 1]; strerrorlen_s() is undefined... //So using buffer size of 256...
			strerror_s(File_ErrorBuffer, sizeof(File_ErrorBuffer), FileError);
			std::cout << "\nError '" << File_ErrorBuffer << "'\n: Unable to Open File in GetTextFileContent In: Essenbp,\n File Path: " << Path << "\n";
		}
		else
		{
			std::fseek(TheFile, 0, SEEK_SET);
			std::fwrite(&DataStorage[0], 1, DataStorage.size(), TheFile);
			std::fclose(TheFile);
			IsSuccesful = true;
		}
	}
	
	void GetFileContent(const std::string Path, UnknownDataAndSizeStruct& DataStorage, bool& IsSuccesful)
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
			DataStorage.FreeAndResizeData(std::ftell(TheFile), IsSuccesful);
			if (IsSuccesful)
			{
				std::rewind(TheFile);
				std::fread(DataStorage.GetData(), 1, DataStorage.GetDataSize(), TheFile);
				IsSuccesful = true;
			}
			else
			{
				std::cout << "\n Error Essenbp::UnknownDataAndSizeStruct.FreeAndResizeDataAndReturnPointerToDataPointer() Failed In: WriteToFile!\n";
				DataStorage.FreeData();
			}		
			std::fclose(TheFile);
		}
	}

	void WriteBytesToFile(const std::string Path, UnknownDataAndSizeStruct& DataStorage, bool& IsSuccesful)
	{
		IsSuccesful = false;
		FILE* TheFile;
		errno_t FileError = fopen_s(&TheFile, Path.c_str(), "w");// IfFile does not Exist then it creates it

		if (FileError != 0)
		{
			char File_ErrorBuffer[256];//[strerrorlen_s(err) + 1]; strerrorlen_s() is undefined... //So using buffer size of 256...
			strerror_s(File_ErrorBuffer, sizeof(File_ErrorBuffer), FileError);
			std::cout << "\nError '" << File_ErrorBuffer << "'\n: Unable to Creat/Open File in WriteBytesToFile In: Essenbp,\n File Path: " << Path << "\n";
		}
		else
		{
			std::fseek(TheFile, 0, SEEK_SET);
			std::fwrite(DataStorage.GetData(), 1, DataStorage.GetDataSize(), TheFile);
			std::fclose(TheFile);
			IsSuccesful = true;
		}
	}
#else
	//Linux
	void GetTextFileContent(const std::string Path, std::string& DataStorage, bool& IsSuccesful)
	{
		IsSuccesful = false;
		FILE* TheFile = fopen(Path.c_str(), "r");

		if (TheFile != nullptr)
		{
			std::cout << "\nError : Unable to Open File in GetTextFileContent In: Essenbp,\n File Path: " << Path << "\n";
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

	void GetFileContent(const std::string Path, UnknownDataAndSizeStruct& DataStorage, bool& IsSuccesful)
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
			DataStorage.FreeAndResizeData(std::ftell(TheFile), IsSuccesful);
			if (IsSuccesful)
			{
				std::rewind(TheFile);
				std::fread(DataStorage.GetData(), 1, DataStorage.GetDataSize(), TheFile);
				IsSuccesful = true;
			}
			else
			{
				std::cout << "\n Error Essenbp::UnknownDataAndSizeStruct.FreeAndResizeDataAndReturnPointerToDataPointer() Failed In: GetFileContent!\n";
				DataStorage.FreeData();
			}		
			std::fclose(TheFile);
		}
	}

	void WriteToFile(const std::string Path, std::string& DataStorage, bool& IsSuccesful)
	{
		IsSuccesful = false;
		FILE* TheFile = fopen(Path.c_str(), "w");

		if (TheFile != nullptr)
		{
			std::cout << "\nError : Unable to Create/Open File in WriteToFile In: Essenbp,\n File Path: " << Path << "\n";
		}
		else
		{
			std::fseek(TheFile, 0, SEEK_SET);
			std::fwrite(&DataStorage[0], 1, DataStorage.size(), TheFile);
			std::fclose(TheFile);	
			IsSuccesful = true;
		}
	}

	void WriteBytesToFile(const std::string Path, UnknownDataAndSizeStruct& DataStorage, bool& IsSuccesful)
	{
		IsSuccesful = false;
		FILE* TheFile;
		FILE* TheFile = fopen(Path.c_str(), "w");;// IfFile does not Exist then it creates it

		if (TheFile != nullptr)
		{
			std::cout << "\nError : Unable to Create/Open File in WriteBytesToFile In: Essenbp,\n File Path: " << Path << "\n";
		}
		else
		{
			std::fseek(TheFile, 0, SEEK_SET);
			std::fwrite(DataStorage.GetData(), 1, DataStorage.GetDataSize(), TheFile);
			std::fclose(TheFile);
			IsSuccesful = true;
		}
	}
#endif
};

#endif
