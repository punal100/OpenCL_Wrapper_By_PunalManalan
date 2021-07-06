#pragma once

#ifndef ESSENTIAL_FUNCTIONS_BY_PUNAL
#define ESSENTIAL_FUNCTIONS_BY_PUNAL

#include <iostream>
#include <fstream>// For file reading
#include <chrono>// Mainly For FRAMERATE(FPS) LOCK

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif


namespace Essenbp//Essential Functions By Punal
{
	std::string GetCurrentDirectory()
	{
		char FilePathBuffer[FILENAME_MAX];
		GetCurrentDir(FilePathBuffer, FILENAME_MAX);
		std::string Working_Dir(FilePathBuffer);
		return Working_Dir;
	}

#ifdef _WIN32
	//Add .txt extension too
	void GetTextFileContent(const std::string Path, std::string& DataStorage, bool& Issuccessful)
	{
		Issuccessful = false;
		FILE* TheFile;
		errno_t FileError = fopen_s(&TheFile, Path.c_str(), "r");

		if (FileError != 0)
		{
			char File_ErrorBuffer[256];//[strerrorlen_s(err) + 1]; strerrorlen_s() is undefined... //So using buffer size of 256...
			strerror_s(File_ErrorBuffer, sizeof(File_ErrorBuffer), FileError);
			//WriteLogToFile("\nError '" + std::string(File_ErrorBuffer) + "'\n: Unable to Open File in GetTextFileContent In: Essenbp,\n File Path: " + Path + "\n");
		}
		else
		{
			std::fseek(TheFile, 0, SEEK_END);
			DataStorage.resize(std::ftell(TheFile));
			std::rewind(TheFile);
			std::fread(&DataStorage[0], 1, DataStorage.size(), TheFile);
			std::fclose(TheFile);
			Issuccessful = true;
		}
	}

	//Just add the path without the .txt extension
	//NOTE: '\0' added at End of the total size of the bytes
	void WriteToTextFile(std::string Path, std::string& DataStorage, bool& Issuccessful)
	{
		Issuccessful = false;
		FILE* TheFile;
		Path = Path + ".txt";
		errno_t FileError = fopen_s(&TheFile, Path.c_str(), "w");

		if (FileError != 0)
		{
			char File_ErrorBuffer[256];//[strerrorlen_s(err) + 1]; strerrorlen_s() is undefined... //So using buffer size of 256...
			strerror_s(File_ErrorBuffer, sizeof(File_ErrorBuffer), FileError);
			//WriteLogToFile("\nError '" + std::string(File_ErrorBuffer) + "'\n: Unable to Open/Create File in WriteToTextFile In: Essenbp,\n File Path: " + Path + "\n");
		}
		else
		{
			std::fseek(TheFile, 0, SEEK_SET);
			std::fwrite(&DataStorage[0], 1, DataStorage.size(), TheFile);
			std::fseek(TheFile, 1, SEEK_CUR);
			const char LastChar = '\0';
			std::fwrite(&LastChar, 1, 1, TheFile);
			std::fclose(TheFile);
			Issuccessful = true;
		}
	}

	void AppendToTextFile(std::string Path, std::string& DataStorage, bool& Issuccessful)
	{
		Issuccessful = false;
		FILE* TheFile;
		Path = Path + ".txt";
		errno_t FileError = fopen_s(&TheFile, Path.c_str(), "a");

		if (FileError != 0)
		{
			char File_ErrorBuffer[256];//[strerrorlen_s(err) + 1]; strerrorlen_s() is undefined... //So using buffer size of 256...
			strerror_s(File_ErrorBuffer, sizeof(File_ErrorBuffer), FileError);
			//WriteLogToFile("\nError '" + std::string(File_ErrorBuffer) + "'\n: Unable to Open/Create File in WriteToTextFile In: Essenbp,\n File Path: " + Path + "\n");
		}
		else
		{
			std::fwrite(&DataStorage[0], 1, DataStorage.size(), TheFile);
			std::fseek(TheFile, 1, SEEK_CUR);
			const char LastChar = '\0';
			std::fwrite(&LastChar, 1, 1, TheFile);
			std::fclose(TheFile);
			Issuccessful = true;
		}
	}

#else
	//Linux
	//Add .txt extension too
	void GetTextFileContent(const std::string Path, std::string& DataStorage, bool& Issuccessful)
	{
		Issuccessful = false;
		FILE* TheFile = fopen(Path.c_str(), "r");

		if (TheFile != nullptr)
		{
			WriteLogToFile("\nError : Unable to Open File in GetTextFileContent In: Essenbp,\n File Path: " + Path + "\n");
		}
		else
		{
			std::fseek(TheFile, 0, SEEK_END);
			DataStorage.resize(std::ftell(TheFile));
			std::rewind(TheFile);
			std::fread(&DataStorage[0], 1, DataStorage.size(), TheFile);
			std::fclose(TheFile);
			Issuccessful = true;
		}
	}

	//Just add the path without the .txt extension
	//NOTE: '\0' added at End of the total size of the bytes
	void WriteToTextFile(std::string Path, std::string& DataStorage, bool& Issuccessful)
	{
		Issuccessful = false;
		FILE* TheFile;
		Path = Path + ".txt";
		FILE* TheFile = fopen(Path.c_str(), "w");

		if (TheFile != nullptr)
		{
			WriteLogToFile("\nError : Unable to Open/Create File in WriteToTextFile In: Essenbp,\n File Path: " + Path + "\n");
		}
		else
		{
			std::fseek(TheFile, 0, SEEK_SET);
			std::fwrite(&DataStorage[0], 1, DataStorage.size(), TheFile);
			std::fseek(TheFile, 1, SEEK_CUR);
			const char LastChar = '\0';
			std::fwrite(&LastChar, 1, 1, TheFile);
			std::fclose(TheFile);
			Issuccessful = true;
		}
	}

	void AppendToTextFile(std::string Path, std::string& DataStorage, bool& Issuccessful)
	{
		Issuccessful = false;
		FILE* TheFile;
		Path = Path + ".txt";
		FILE* TheFile = fopen(Path.c_str(), "a");

		if (TheFile != nullptr)
		{
			WriteLogToFile("\nError : Unable to Open/Create File in WriteToTextFile In: Essenbp,\n File Path: " + Path + "\n");
		}
		else
		{
			std::fwrite(&DataStorage[0], 1, DataStorage.size(), TheFile);
			std::fseek(TheFile, 1, SEEK_CUR);
			const char LastChar = '\0';
			std::fwrite(&LastChar, 1, 1, TheFile);
			std::fclose(TheFile);
			Issuccessful = true;
		}
	}
#endif

	//NOTE: Log is Written To File
	std::string CurrentDirectory = "";
	void WriteLogToFile(std::string WhatToWrite)
	{
		bool TempBool = false;
		if (CurrentDirectory == "")
		{
			CurrentDirectory = GetCurrentDirectory() + "\\Log";
			WriteToTextFile(CurrentDirectory, WhatToWrite, TempBool);
		}
		else
		{
			AppendToTextFile(CurrentDirectory, WhatToWrite, TempBool);
		}
		//std::cout << WhatToWrite;
		/*if(!TempBool)
		{
			WriteLogToFile("\nError '" + std::string(File_ErrorBuffer) + "'\n: Unable to Open/Create File in WriteToTextFile In: Essenbp,\n File Path: " + Path + "\n");
		}*/
	}

	//NOTE:This is Used to Copy Data(Not Address)
	struct UnknownDataAndSizeStruct
	{
	private:
		void* Data = nullptr;
		size_t SizeOfData = 0;

	public:
		UnknownDataAndSizeStruct()
		{
			WriteLogToFile("\n Constructing UnknownDataAndSizeStruct!");
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
		void CopyAndStoreData(void* ArgData, size_t ArgSizeOfData, bool& Issuccessful, bool DoADummyCopy = false, bool AppendData = false)
		{
			Issuccessful = false;

			if (!DoADummyCopy)
			{
				if (ArgData == nullptr)
				{
					WriteLogToFile("\n Error nullptr for ArgData in CopyAndStoreData In: UnknownDataAndSizeStruct!\n");
					WriteLogToFile("If Dummy Value is to be passed set DoADummyCopy(4th argument) to false, and set ArgData = nullptr!\n");
				}
				else
				{
					if (AppendData)
					{
						if (ArgSizeOfData == 0)
						{
							WriteLogToFile("\n Error Size Of ArgSizeOfData is Equal to Zero in CopyAndStoreData In: UnknownDataAndSizeStruct!\n");
							return;
						}
						
						void* AppendDataHelper = malloc((SizeOfData + ArgSizeOfData));// Setting Current
						if (AppendDataHelper == nullptr)
						{
							WriteLogToFile("\n Error Allocating : " + std::to_string(SizeOfData + ArgSizeOfData) + " Byes Of Memory for Data in CopyAndStoreData In: UnknownDataAndSizeStruct!\n");
							return;
						}
						else
						{
							//PENDING CHECK FOR ERROR
							size_t PreviousSize = SizeOfData;
							SizeOfData = SizeOfData + ArgSizeOfData;// Current Size

							for (size_t i = 0; i < PreviousSize; ++i)// Memccpy bad
							{
								((char*)AppendDataHelper)[i] = ((char*)Data)[i];// I could simply convert void* to char*... but i left it as void* for the purpose of 'readability'
							}

							for (size_t i = 0; i < ArgSizeOfData; ++i)// Memccpy bad
							{
								((char*)AppendDataHelper)[(i + PreviousSize)] = ((char*)ArgData)[i];// I could simply convert void* to char*... but i left it as void* for the purpose of 'readability'
							}

							FreeData();
							Data = AppendDataHelper;
							Issuccessful = true;
						}
					}
					else
					{
						SizeOfData = ArgSizeOfData;
						if (SizeOfData == 0)
						{
							WriteLogToFile("\n Error Size Of SizeOfData is Equal to Zero in CopyAndStoreData In: UnknownDataAndSizeStruct!\n");
							return;
						}
						FreeData();
						Data = malloc(SizeOfData);
						if (Data == nullptr)
						{
							SizeOfData = 0;
							WriteLogToFile("\n Error Allocating : " + std::to_string(SizeOfData) + " Byes Of Memory for Data in CopyAndStoreData In: UnknownDataAndSizeStruct!\n");
							return;
						}
						else
						{
							for (size_t i = 0; i < SizeOfData; ++i)// Memccpy bad
							{
								((char*)Data)[i] = ((char*)ArgData)[i];// I could simply convert void* to char*... but i left it as void* for the purpose of 'readability'
							}
							Issuccessful = true;
						}
					}
				}				
			}
			else
			{
				if (ArgData != nullptr)
				{
					WriteLogToFile("\n Error ArgData is not set to nullptr in CopyAndStoreData In: UnknownDataAndSizeStruct!\n");
					WriteLogToFile("If Actual value is to be passed ignore DoADummyCopy(4th argument)!\n");
					return;
				}
				else
				{
					FreeData();
					SizeOfData = ArgSizeOfData;
					Issuccessful = true;
				}
			}
		}

		void* GetData() { return Data; }
		size_t GetDataSize() { return SizeOfData; }

		//NOTE:Use this if Direct access to data is required
		//NOTE: by using this
		void FreeAndResizeData(size_t ArgSizeOfData, bool& Issuccessful)
		{
			Issuccessful = false;
			FreeData();

			SizeOfData = ArgSizeOfData;
			if (SizeOfData == 0)
			{
				WriteLogToFile("\n Error Size Of SizeOfData is Equal to Zero in FreeAndResizeDataAndReturnPointerToDataPointer in UnknownDataAndSizeStruct In: Essenbp!\n");
				return;
			}
			Data = malloc(SizeOfData);
			if (Data == nullptr)
			{
				WriteLogToFile("\n Error Allocating : " + std::to_string(SizeOfData) + " Byes Of Memory for Data in FreeAndResizeDataAndReturnPointerToDataPointer in UnknownDataAndSizeStruct In: Essenbp!\n");
				return;
			}
			Issuccessful = true;
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
				WriteLogToFile("\n Error Argument ReturnData is nullptr in GetDataAndSizeAndSetDataToNull in UnknownDataAndSizeStruct In: Essenbp!\n");
			}
		}

		~UnknownDataAndSizeStruct()
		{
			FreeData();
		}
	};	

	//NOTE: By PointerToArrayOfPointers it means Example : int** Array, Array[n] == int*, Basicaly an Array of Array of size [1][n]
	//NOTE: Pass first parameter using (void***)&
	void Malloc_PointerToArrayOfPointers(void*** PointerTo_PointerToArrayOfPointers, unsigned int NumOfPointerToAdd, unsigned int SizeOfEachPointer, bool& Issuccessful)
	{
		Issuccessful = false;

		if (PointerTo_PointerToArrayOfPointers == nullptr)
		{
			WriteLogToFile("\n Error nullptr Passed for 'PointerTo_PointerToArrayOfPointers'. in Malloc_PointerToArrayOfPointers In: Essenbp!\n");
			return;
		}

		if (NumOfPointerToAdd == 0)
		{
			WriteLogToFile("\n Error NumOfPointerToAdd Is 0. in Malloc_PointerToArrayOfPointers In: Essenbp!\n");
			return;
		}
		if (SizeOfEachPointer == 0)
		{
			WriteLogToFile("\n Error SizeOfEachPointer Is 0. in Malloc_PointerToArrayOfPointers In: Essenbp!\n");
			return;
		}

		if (*PointerTo_PointerToArrayOfPointers != nullptr)
		{
			WriteLogToFile("\n Error Not Null : PointerToArrayOfPointers already pointing to some memory, free the memory First. in Malloc_PointerToArrayOfPointers In: Essenbp!\n");
		}
		else
		{
			*PointerTo_PointerToArrayOfPointers = (void**)calloc(NumOfPointerToAdd, SizeOfEachPointer);// Setting everything to Zero
			if (*PointerTo_PointerToArrayOfPointers == nullptr)
			{
				WriteLogToFile("\n Error Allocating : " + std::to_string(NumOfPointerToAdd * SizeOfEachPointer) + " Byes Of Memory for *PointerTo_PointerToArrayOfPointers in Malloc_PointerToArrayOfPointers In: Essenbp!\n");
				return;
			}
			Issuccessful = true;
		}
	}

	struct ArrayOfUnknownDataAndSize
	{
	private:
		unsigned int TotalNumberOfUnknownData = 0;
		UnknownDataAndSizeStruct** ArrayOfUnknownData = nullptr;

		void ResizeArray(unsigned int TotalNumber, bool& Issuccessful)
		{
			Issuccessful = false;

			UnknownDataAndSizeStruct** TempUnknownData = nullptr;
			Malloc_PointerToArrayOfPointers((void***)&TempUnknownData, TotalNumber, sizeof(UnknownDataAndSizeStruct*), Issuccessful);
			if (!Issuccessful)
			{
				WriteLogToFile("\n Error Malloc_PointerToArrayOfPointers failed in ResizeArray In: ArrayOfUnknownDataAndSize!\n");
			}
			else
			{
				Issuccessful = true;
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
							WriteLogToFile("\n Error Allocating Bytes of Data for UnknownDataAndSizeStruct[" + std::to_string(i) + "] in ResizeArray In: ArrayOfUnknownDataAndSize!\n");
							for (int j = 0; j < i; ++j)
							{
								delete TempUnknownData[j];
							}
							free(TempUnknownData);
							Issuccessful = false;
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

				if (!Issuccessful)
				{
					WriteLogToFile("\n Error ResizeArray failed In: ArrayOfUnknownDataAndSize!\n");
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
			WriteLogToFile("\n Constructing ArrayOfUnknownDataAndSize!");
		}

		void AddElement(bool& Issuccessful)
		{
			Issuccessful = false;
			ResizeArray(TotalNumberOfUnknownData + 1, Issuccessful);
			if(!Issuccessful)
			{
				WriteLogToFile("\n Error ResizeArray failed in AddElement In: ArrayOfUnknownDataAndSize!\n");
			}
		}

		void RemoveElement(unsigned int ElementNumber, bool& Issuccessful)
		{
			Issuccessful = false;
			if (ElementNumber > TotalNumberOfUnknownData)
			{
				WriteLogToFile("\n Error ElementNumber Exceeds the total number of Unknown Data Present! in RemoveElement in AddElement In: ArrayOfUnknownDataAndSize!\n");
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

					ResizeArray(TotalNumberOfUnknownData - 1, Issuccessful);
					if (!Issuccessful)
					{
						WriteLogToFile("\n Error ResizeArray failed in RemoveElement In: ArrayOfUnknownDataAndSize!\n");
					}
				}
				
			}
		}

		unsigned int GetTotalNumberOfUnknownData()
		{
			return TotalNumberOfUnknownData;
		}

		void GetData(unsigned int ElementNumber, UnknownDataAndSizeStruct** ReturnUnknownDataAndSize, bool& Issuccessful)
		{
			if (ElementNumber > TotalNumberOfUnknownData)
			{
				WriteLogToFile("\n Error ElementNumber Exceeds the total number of Unknown Data Present! in GetData in AddElement In: ArrayOfUnknownDataAndSize!\n");
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

	uint64_t TimeSinceEpochInSecond()
	{
		using namespace std::chrono;
		return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
	}

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
	void TimeCalculationInseconds()
	{
		WriteLogToFile("\n");
		WriteLogToFile("      StartTime :" + std::to_string(ForFunctionStartTime) + '\n');
		WriteLogToFile("        EndTime :" + std::to_string(TimeSinceEpochInSecond()) + '\n');
		WriteLogToFile("Time Difference :" + std::to_string(TimeSinceEpochInSecond() - ForFunctionStartTime) + '\n' + '\n');
		ForFunctionStartTime = TimeSinceEpochInSecond();
	}

	void TimeCalculationInMilliseconds()
	{
		WriteLogToFile("\n");
		WriteLogToFile("      StartTime :" + std::to_string(ForFunctionStartTime) + '\n');
		WriteLogToFile("        EndTime :" + std::to_string(TimeSinceEpochInMilliSecond()) + '\n');
		WriteLogToFile("Time Difference :" + std::to_string(TimeSinceEpochInMilliSecond() - ForFunctionStartTime) + '\n' + '\n');
		ForFunctionStartTime = TimeSinceEpochInMilliSecond();
	}

	void TimeCalculationInNanoSeconds()
	{
		WriteLogToFile("\n");
		WriteLogToFile("      StartTime :" + std::to_string(ForFunctionStartTime) + '\n');
		WriteLogToFile("        EndTime :" + std::to_string(TimeSinceEpochInNanoSecond()) + '\n');
		WriteLogToFile("Time Difference :" + std::to_string(TimeSinceEpochInNanoSecond() - ForFunctionStartTime) + '\n' + '\n');
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

	void RemoveCommentsFromCppSource(std::string ProgramSource, std::string& OutPutString)
	{
		int TotalLength = ProgramSource.length();

		OutPutString = "";//Rest

		//These Boolean tells whether specific comment started or not
		bool IsSingeLineStarted = false;//SingleLine
		bool IsMultiLineStarted = false;//MultiLine

		for (int i = 0; i < TotalLength; i++)
		{
			//Checks for End'\n' of Single Line comment If IsSingeLineStarted is set to true
			if (IsSingeLineStarted == true && ProgramSource[i] == '\n')
			{
				IsSingeLineStarted = false;
			}						
			else
			{
				if (IsMultiLineStarted == true && ProgramSource[i] == '*' && ProgramSource[i + 1] == '/')
				{
					IsMultiLineStarted = false, i++;//Checks for End"*/" of Multi Line comment If IsMultiLineStarted is set to true
				}
				else
				{
					if (IsSingeLineStarted || IsMultiLineStarted)
					{
						continue;//Ignore If the Character is inside a comment
					}
					else
					{
						// This checks if the Single Line Comment Started
						if (ProgramSource[i] == '/' && ProgramSource[i + 1] == '/')
						{
							IsSingeLineStarted = true;
							i = i + 1;// Since Single line comment starts with 2 characters, i + 1 is skipped
						}
						else
						{
							// This checks if the Multi Line Comment Started
							if (ProgramSource[i] == '/' && ProgramSource[i + 1] == '*')
							{
								IsMultiLineStarted = true;
								i = i + 1;// Since Multi line comment starts with 2 characters, i + 1 is skipped
							}
							else
							{
								//When everything is false, if Not a comment nor inside a comment
								//Meaning This Non-Comment character is added to the output
								OutPutString = OutPutString + ProgramSource[i];
							}
						}	
					}
				}
			}
		}
	}

	void ReplaceEveryOccuranceWithGivenString(std::string& Source, const std::string From, const std::string To)
	{
		std::string newString;
		newString.reserve(Source.length());  // avoids a few memory allocations

		std::string::size_type lastPos = 0;
		std::string::size_type findPos;

		while (std::string::npos != (findPos = Source.find(From, lastPos)))
		{
			newString.append(Source, lastPos, findPos - lastPos);
			newString += To;
			lastPos = findPos + From.length();
		}

		// Care for the rest after last occurrence
		newString.append(Source, lastPos, Source.length() - lastPos);

		Source.swap(newString);
	}

	void RemoveConsecutiveDulplicateChar(std::string& String, char DuplicateToRemove)
	{
		int n = String.length();

		if (n < 2)
		{
			return;
		}
		int j = 0;

		for (int i = 1; i < n; i++)
		{
			if (DuplicateToRemove == String[j])
			{
				if (String[j] == String[i])
				{
					continue;
				}
			}
			j++;
			String[j] = String[i];
		}
		String.resize(j);
	}

	void FindStartOfSubStringInString(std::string SourceString, std::string SubStringToFind, size_t StartPosition, size_t EndPosition, size_t& ReturnStartOfSubString, bool& Issuccessful)
	{
		Issuccessful = false;
		ReturnStartOfSubString = SourceString.find(SubStringToFind, StartPosition);
		if (ReturnStartOfSubString == std::string::npos)
		{
			ReturnStartOfSubString = EndPosition + 1;// So False
		}

		if (!(ReturnStartOfSubString > EndPosition))
		{
			Issuccessful = true;
			//true
		}
		//else
		//{
		//	//false
		//}
	}

#ifdef _WIN32
	//Extension required for example .bin
	void GetFileContent(std::string Path, std::string ExtensionWithoutTheDot, UnknownDataAndSizeStruct& DataStorage, bool& Issuccessful)
	{
		Issuccessful = false;
		FILE* TheFile;
		Path = Path + "." + ExtensionWithoutTheDot;
		errno_t FileError = fopen_s(&TheFile, Path.c_str(), "r");

		if (FileError != 0)
		{
			char File_ErrorBuffer[256];//[strerrorlen_s(err) + 1]; strerrorlen_s() is undefined... //So using buffer size of 256...
			strerror_s(File_ErrorBuffer, sizeof(File_ErrorBuffer), FileError);
			WriteLogToFile("\nError '" + std::string(File_ErrorBuffer) + "'\n: Unable to Open File in GetFileContent In: Essenbp,\n File Path: " + Path + "\n");
		}
		else
		{
			std::fseek(TheFile, 0, SEEK_END);
			DataStorage.FreeAndResizeData(std::ftell(TheFile), Issuccessful);
			if (Issuccessful)
			{
				std::rewind(TheFile);
				std::fread(DataStorage.GetData(), 1, DataStorage.GetDataSize(), TheFile);
				Issuccessful = true;
			}
			else
			{
				WriteLogToFile("\n Error Essenbp::UnknownDataAndSizeStruct.FreeAndResizeDataAndReturnPointerToDataPointer() Failed In: WriteToFile!\n");
				DataStorage.FreeData();
			}
			std::fclose(TheFile);
		}
	}

	//Put "Path" argument Without the extension
	void WriteBytesToFile(std::string Path, std::string ExtensionWithoutTheDot, UnknownDataAndSizeStruct& DataStorage, bool& Issuccessful)
	{
		Issuccessful = false;
		FILE* TheFile;
		Path = Path + "." + ExtensionWithoutTheDot;
		errno_t FileError = fopen_s(&TheFile, Path.c_str(), "w");// IfFile does not Exist then it creates it

		if (FileError != 0)
		{
			char File_ErrorBuffer[256];//[strerrorlen_s(err) + 1]; strerrorlen_s() is undefined... //So using buffer size of 256...
			strerror_s(File_ErrorBuffer, sizeof(File_ErrorBuffer), FileError);
			WriteLogToFile("\nError '" + std::string(File_ErrorBuffer) + "'\n: Unable to Creat/Open File in WriteBytesToFile In: Essenbp,\n File Path: " + Path + "\n");
		}
		else
		{
			std::fseek(TheFile, 0, SEEK_SET);
			std::fwrite(DataStorage.GetData(), 1, DataStorage.GetDataSize(), TheFile);
			std::fclose(TheFile);
			Issuccessful = true;
		}
	}

#else
	//Linux
	//Extension required for example .bin
	void GetFileContent(std::string Path, std::string ExtensionWithoutTheDot, UnknownDataAndSizeStruct& DataStorage, bool& Issuccessful)
	{
		Issuccessful = false;
		FILE* TheFile;
		Path = Path + "." + ExtensionWithoutTheDot;
		FILE* TheFile = fopen(Path.c_str(), "r");

		if (TheFile != nullptr)
		{
			WriteLogToFile("\nError : Unable to Open File in GetFileContent In: Essenbp,\n File Path: " + Path + "\n");
		}
		else
		{
			std::fseek(TheFile, 0, SEEK_END);
			DataStorage.FreeAndResizeData(std::ftell(TheFile), Issuccessful);
			if (Issuccessful)
			{
				std::rewind(TheFile);
				std::fread(DataStorage.GetData(), 1, DataStorage.GetDataSize(), TheFile);
				Issuccessful = true;
			}
			else
			{
				WriteLogToFile("\n Error Essenbp::UnknownDataAndSizeStruct.FreeAndResizeDataAndReturnPointerToDataPointer() Failed In: WriteToFile!\n");
				DataStorage.FreeData();
			}
			std::fclose(TheFile);
		}
	}

	//Put "Path" argument Without the extension
	void WriteBytesToFile(std::string Path, std::string ExtensionWithoutTheDot, UnknownDataAndSizeStruct& DataStorage, bool& Issuccessful)
	{
		Issuccessful = false;
		FILE* TheFile;
		Path = Path + "." + ExtensionWithoutTheDot;
		FILE* TheFile = fopen(Path.c_str(), "w");

		if (TheFile != nullptr)
		{
			WriteLogToFile("\nError : Unable to Open/Create File in WriteToTextFile In: Essenbp,\n File Path: " + Path + "\n");
		}
		else
		{
			std::fseek(TheFile, 0, SEEK_SET);
			std::fwrite(DataStorage.GetData(), 1, DataStorage.GetDataSize(), TheFile);
			std::fclose(TheFile);
			Issuccessful = true;
		}
	}
#endif
};
#endif
