/********************************************************************************************************************************
OpenCL Wrapper By Punal Manalan
Use this code wherever and whenever you want to!

Information About This Wrapper:
1. This is a LightWeight, Easy to use Simple Wrapper for OpenCL.
2. This Wrapper specializes in Heavy Computation on GPU.
3. From Constrction to Destruction everything is Done automaticaly!
4. No need for manual destruction and releasing cl resources!
5. No need to write Tedious amount of code to counter memory leaks!
6. Upon going out of scope or exiting the program Every resource that is used is safely Released and Deleted.
*********************************************************************************************************************************/

#ifndef OPENCL_WRAPPER_BY_PUNAL
#define OPENCL_WRAPPER_BY_PUNAL
#pragma once
#endif // !OPENCL_WRAPPER_BY_PUNAL

#include <iostream>
#include <fstream>// For file reading

#define __CL_ENABLE_EXCEPTIONS
//#define CL_API_SUFFIX__VERSION_1_2
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL/cl.h>// OpenCl

#ifndef ESSENTIAL_FUNCTIONS_BY_PUNAL
#define ESSENTIAL_FUNCTIONS_BY_PUNAL
#include <chrono>// Mainly For FRAMERATE(FPS) LOCK

namespace Essential
{
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
			std::cout << "\n Error nullptr Passed for 'PointerTo_PointerToArrayOfPointers'. in Malloc_PointerToArrayOfPointers In: Essential!\n";
			return;
		}

		if (NumOfPointerToAdd < 1)
		{
			std::cout << "\n Error NumOfPointerToAdd Is Less than 1. in Malloc_PointerToArrayOfPointers In: Essential!\n";
			return;
		}
		if (SizeOfEachPointer < 1)
		{
			std::cout << "\n Error SizeOfEachPointer Is Less than 1. in Malloc_PointerToArrayOfPointers In: Essential!\n";
			return;
		}

		if (*PointerTo_PointerToArrayOfPointers != nullptr)
		{
			std::cout << "\n Error Not Null : PointerToArrayOfPointers already pointing to some memory, free the memory First. in Malloc_PointerToArrayOfPointers In: Essential!\n";
		}
		else
		{
			*PointerTo_PointerToArrayOfPointers = (void**)calloc(NumOfPointerToAdd, SizeOfEachPointer);// Setting everything to Zero
			if (*PointerTo_PointerToArrayOfPointers == nullptr)
			{
				std::cout << "\n Error Allocating : " << NumOfPointerToAdd * SizeOfEachPointer << " Byes Of Memory for *PointerTo_PointerToArrayOfPointers in Malloc_PointerToArrayOfPointers In: Essential!\n";				
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
		errno_t FileError;

		if ((FileError = fopen_s(&TheFile, Path.c_str(), "r")) != 0)
		{
			char File_ErrorBuffer[256];//[strerrorlen_s(err) + 1]; strerrorlen_s() is undefined... //So using buffer size of 256...
			strerror_s(File_ErrorBuffer, sizeof(File_ErrorBuffer), FileError);
			std::cout << "\nError '" << File_ErrorBuffer << "'\n: Unable to Open File in GetFileContent In: Essential,\n File Path: " << Path << "\n";
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
			std::cout << "\nError : Unable to Open File in GetFileContent In: Essential,\n File Path: " << Path << "\n";
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

namespace OCLW_P//OpenCL Wrapper By Punal Manalan
{
	enum class cl_Memory_Type//NOTE: This is a Enum Based On CL_MEM_READ_ONLY, CL_MEM_WRITE_ONLY, CL_MEM_READ_WRITE for buffer creation in the cl_device
	{
		Uninitialized_cl_Memory = 0,//NULL,	
		CL_READ_ONLY = 1,//CL_MEM_READ_ONLY,
		CL_WRITE_ONLY = 2,//CL_MEM_WRITE_ONLY,
		CL_READ_AND_WRITE = 3,//CL_MEM_READ_WRITE,
		CL_LOCALENUM = 4,//5,// Not To be confused with CL_LOCAL
		CL_PRIVATE = 5//3
	};

	// NOTE: Call Constructor Before Using
	struct cl_PlatformVendorStruct
	{
	private:	
		bool IsPlatformChosen = false;
		cl_uint ChosenPlatform = 0;
		cl_uint TotalNumberOfPlatformVendors = 0;
		std::string** AllAvailablePlatformVendorNames = nullptr;
	
	public:	
		bool IsConstructionSuccesful = false;
		cl_PlatformVendorStruct(bool& IsSuccesful)
		{
			std::cout << "\n Constructing cl_PlatformVendorStruct!";

			IsPlatformChosen = false;
			ChosenPlatform = 0;
			TotalNumberOfPlatformVendors = 0;
			AllAvailablePlatformVendorNames = nullptr;

			IsConstructionSuccesful = false;
			IsSuccesful = false;

			cl_int ClErrorResult = clGetPlatformIDs(0, NULL, &TotalNumberOfPlatformVendors);//clGetPlatformIDs(1, &platform, NULL);// One line code with no checks chooses first platform GPU/CPU(APU if available) in this
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n ClError Code " << ClErrorResult << " : Getting platforms In cl_PlatformVendorStruct!\n";
			}
			else
			{
				Essential::Malloc_PointerToArrayOfPointers((void***)&AllAvailablePlatformVendorNames, TotalNumberOfPlatformVendors, sizeof(std::string*), IsSuccesful);
				if (!IsSuccesful)
				{
					std::cout << "\n Error Allocating :" << TotalNumberOfPlatformVendors * sizeof(std::string*) << " Byes Of Memory for AllAvailablePlatformVendorNames cl_PlatformVendorStruct!\n";
				}
			}

			if (IsSuccesful)// For the safe of readability
			{
				cl_platform_id PlatformID = nullptr;
				for (cl_uint i = 0; i < TotalNumberOfPlatformVendors; ++i)
				{
					ClErrorResult = clGetPlatformIDs(i + 1, &PlatformID, NULL);// for the first platform //Single Line code
					if (ClErrorResult != CL_SUCCESS)
					{
						std::cout << "\n ClError Code " << ClErrorResult << " : Platform Not set In cl_PlatformVendorStruct!\n";
						return;
					}
					else
					{
						char* ReturnValue = NULL;
						size_t SizeOfReturnValue;

						ClErrorResult = clGetPlatformInfo(PlatformID, CL_PLATFORM_VENDOR, NULL, ReturnValue, &SizeOfReturnValue); // Get the size of the ReturnValue
						if (ClErrorResult != CL_SUCCESS)
						{
							std::cout << "\n ClError Code " << ClErrorResult << " : Getting Size of CL_PLATFORM_VENDOR in clGetPlatformInfo In cl_PlatformVendorStruct!\n";
							return;
						}

						ReturnValue = (char*)malloc(SizeOfReturnValue * sizeof(char));
						if (ReturnValue == nullptr)
						{
							std::cout << "\n Error Allocating :" << sizeof(char) * SizeOfReturnValue << " Byes Of Memory for ReturnValue cl_PlatformVendorStruct!\n";
							return;
						}

						ClErrorResult = clGetPlatformInfo(PlatformID, CL_PLATFORM_VENDOR, SizeOfReturnValue, ReturnValue, NULL); // Get the Return Value
						if (ClErrorResult != CL_SUCCESS)
						{
							std::cout << "\n ClError Code " << ClErrorResult << " : Getting Value of CL_PLATFORM_VENDOR in clGetPlatformInfo In cl_PlatformVendorStruct!\n";
							free(ReturnValue);
							return;
						}
						else
						{
							AllAvailablePlatformVendorNames[i] = new std::string(ReturnValue);
							IsSuccesful = AllAvailablePlatformVendorNames[i] != nullptr;
							free(ReturnValue);
							if (!IsSuccesful)
							{
								std::cout << "\n Error Allocating :" << sizeof(std::string*) << " Byes Of Memory for AllAvailablePlatformVendorNames[" << i << "] cl_PlatformVendorStruct!\n";
								for (int j = 0; j < i; ++j)
								{
									delete AllAvailablePlatformVendorNames[j];
								}
								free(AllAvailablePlatformVendorNames);
								return;
							}
						}
					}
				}
				IsConstructionSuccesful = true;
				IsSuccesful = true;
			}
			else
			{
				std::cout << "\n Error Construction Failed cl_PlatformVendorStruct!";
			}
		}

		void GetPlatformVendorName(int PlatformNumber, std::string& PlatformVendorName, bool& IsSuccesful)
		{
			IsSuccesful = false;
			if (IsConstructionSuccesful)
			{
				PlatformVendorName = *(AllAvailablePlatformVendorNames[PlatformNumber]);
				IsSuccesful = true;
			}
			else
			{
				if (!IsConstructionSuccesful)
				{
					std::cout << "\n Error Calling GetTotalNumberOfPlatformVendors Without Constructing the struct In: cl_PlatformVendorStruct!\n";
					return;
				}
			}
		}
		void GetTotalNumberOfPlatformVendors(int& ArgTotalNumberOfPlatformVendors, bool& IsSuccesful)
		{
			IsSuccesful = false;
			if (IsConstructionSuccesful)
			{
				ArgTotalNumberOfPlatformVendors = TotalNumberOfPlatformVendors;
				IsSuccesful = true;
			}
			else
			{
				if (!IsConstructionSuccesful)
				{
					std::cout << "\n Error Calling GetTotalNumberOfPlatformVendors Without Constructing the struct In: cl_PlatformVendorStruct!\n";
					return;
				}
			}
		}
		void GetChosenPlatform(cl_uint& ArgChosenPlatform, bool& IsSuccesful)
		{
			IsSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling GetChosenPlatform Without Constructing the struct In: cl_PlatformVendorStruct!\n";
				return;
			}
			if (!IsPlatformChosen)
			{
				std::cout << "\n Error Platform Is Not Chosen In GetChosenPlatform In: cl_PlatformVendorStruct!\n";
				return;
			}
			ArgChosenPlatform = ChosenPlatform;
			IsSuccesful = true;
		}

		//NOTE: ChosenPlatform is always greater than 1 and less than total number of platforms
		void SetChosenPlatform(cl_uint ArgChosenPlatform, bool& IsSuccesful)
		{
			IsSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetChosenPlatform Without Constructing the struct In: cl_PlatformVendorStruct!\n";
				return;
			}

			if ((ArgChosenPlatform > 0) && (ArgChosenPlatform <= TotalNumberOfPlatformVendors))
			{
				ChosenPlatform = ArgChosenPlatform;
				IsPlatformChosen = true;
				IsSuccesful = true;
			}
		}

		void PrintAllAvailablePlatformVendorNames(bool& IsSuccesful)
		{
			IsSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling PrintAllAvailablePlatformVendorNames Without Constructing the struct In: cl_PlatformVendorStruct!\n";
				return;
			}

			std::string PlatformVendorName;
			for (int i = 0; i < TotalNumberOfPlatformVendors; ++i)
			{				
				GetPlatformVendorName(i, PlatformVendorName, IsSuccesful);
				if (!IsSuccesful)
				{
					std::cout << "\n Error: Function Unsuccesful In: cl_PlatformVendorStruct!\n";
					IsSuccesful = false;
					return;
				}

				std::cout << "\n Platform Vendor Nunber: '" << i + 1 << "', Platform Vendor Name: '" << PlatformVendorName << "'\n";
			}
		}

		~cl_PlatformVendorStruct()
		{
			std::cout << "\n Destructing cl_PlatformVendorStruct!";
			if (IsConstructionSuccesful)
			{
				IsConstructionSuccesful = false;
				for (cl_uint i = 0; i < TotalNumberOfPlatformVendors; ++i)
				{
					delete AllAvailablePlatformVendorNames[i];
				}
				free(AllAvailablePlatformVendorNames);
			}
		}
	};

	// NOTE: Call Constructor Before Using 
	struct cl_KernelFunctionArgumentOrderListStruct
	{			
	public:
		bool IsThisListUsable = false;

		const std::string KernelFunctionName;
		const unsigned int TotalNumberOfArugments;
		unsigned int NumberOfArgumentsSet = 0;
		int NumberOfReads = 0;
		int NumberOfWrites = 0;
		int NumberOfRead_And_Writes = 0;
		int NumberOfLocals = 0;
		int NumberOfPrivates = 0;

		cl_Memory_Type** KernelArgumentsInOrder = nullptr;// Manually Set the enum types or use 'FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode(std::string ProgramKernelCode, std::string FunctionName, cl_KernelFunctionArgumentOrderListStruct& OrderedKernelArgumentList)' Function
	
		bool IsConstructionSuccesful = false;// NOTE: Never Change this Manualy not worth the time...
		cl_KernelFunctionArgumentOrderListStruct(const unsigned int ArgTotalNumberOfArugments, const std::string ArgKernelFunctionName) : TotalNumberOfArugments(ArgTotalNumberOfArugments), KernelFunctionName(ArgKernelFunctionName)
		{
			std::cout << "\n Constructing cl_KernelFunctionArgumentOrderListStruct!";

			IsThisListUsable = false;

			NumberOfArgumentsSet = 0;
			NumberOfReads = 0;
			NumberOfWrites = 0;
			NumberOfRead_And_Writes = 0;
			NumberOfLocals = 0;
			NumberOfPrivates = 0;

			bool IsSuccesful = false;
			IsConstructionSuccesful = false;
			KernelArgumentsInOrder = nullptr;

			Essential::Malloc_PointerToArrayOfPointers((void***)&KernelArgumentsInOrder, TotalNumberOfArugments, sizeof(cl_Memory_Type*), IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Allocating :" << TotalNumberOfArugments * sizeof(cl_Memory_Type*) << " Byes Of Memory for KernelArgumentsInOrder In KernelFunctionArgumentOrderListStruct!\n";
				std::cout << "\n Error Construction Failed cl_KernelFunctionArgumentOrderListStruct!";
				return;
			}
			else
			{
				for (int i = 0; i < TotalNumberOfArugments; ++i)
				{
					KernelArgumentsInOrder[i] = new cl_Memory_Type(cl_Memory_Type::Uninitialized_cl_Memory);
					if (KernelArgumentsInOrder[i] == nullptr)
					{
						std::cout << "\n Error Allocating " << (TotalNumberOfArugments * sizeof(cl_Memory_Type*)) << " Byes Of Memory for KernelArgumentsInOrder[" << i << "] In KernelFunctionArgumentOrderListStruct!\n";
						for (int j = 0; j < i; ++j)
						{
							free(KernelArgumentsInOrder[j]);
						}
						free(KernelArgumentsInOrder);
						std::cout << "\n Error Construction Failed cl_KernelFunctionArgumentOrderListStruct!";
						return;
					}
				}
				IsConstructionSuccesful = true;
			}			
		}

		cl_KernelFunctionArgumentOrderListStruct(const cl_KernelFunctionArgumentOrderListStruct* CopyStruct) : TotalNumberOfArugments(CopyStruct->TotalNumberOfArugments), KernelFunctionName(CopyStruct->KernelFunctionName)
		{
			std::cout << "\n Constructing cl_KernelFunctionArgumentOrderListStruct!";

			IsThisListUsable = false;

			NumberOfArgumentsSet = 0;
			NumberOfReads = 0;
			NumberOfWrites = 0;
			NumberOfRead_And_Writes = 0;
			NumberOfLocals = 0;
			NumberOfPrivates = 0;

			bool IsSuccesful = false;
			IsConstructionSuccesful = false;
			KernelArgumentsInOrder = nullptr;

			if (!CopyStruct->IsConstructionSuccesful)
			{
				std::cout << "\n Error The OrderedListOfArugments Is Not Constructed, So Is Unusable In: cl_MultiDevice_KernelFunctionStruct!\n";
				std::cout << "\n Error Construction Failed cl_KernelFunctionArgumentOrderListStruct!";
				return;
			}
			else
			{
				if (!CopyStruct->IsThisListUsable)
				{
					std::cout << "\n Error The OrderedListOfArugments Is Not Properly Set, So Is Unusable In: cl_MultiDevice_KernelFunctionStruct!\n";
					std::cout << "\n Error Construction Failed cl_KernelFunctionArgumentOrderListStruct!";
					return;
				}
			}

			Essential::Malloc_PointerToArrayOfPointers((void***)&KernelArgumentsInOrder, TotalNumberOfArugments, sizeof(cl_Memory_Type*), IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Allocating :" << TotalNumberOfArugments * sizeof(cl_Memory_Type*) << " Byes Of Memory for KernelArgumentsInOrder In KernelFunctionArgumentOrderListStruct!\n";
				std::cout << "\n Error Construction Failed cl_KernelFunctionArgumentOrderListStruct!";
				return;
			}
			else
			{
				for (int i = 0; i < TotalNumberOfArugments; ++i)
				{
					KernelArgumentsInOrder[i] = new cl_Memory_Type(*((CopyStruct->KernelArgumentsInOrder)[i]));
					if (KernelArgumentsInOrder[i] == nullptr)
					{
						std::cout << "\n Error Allocating " << (TotalNumberOfArugments * sizeof(cl_Memory_Type*)) << " Byes Of Memory for KernelArgumentsInOrder[" << i << "] In KernelFunctionArgumentOrderListStruct!\n";
						for (int j = 0; j < i; ++j)
						{
							free(KernelArgumentsInOrder[j]);
						}
						free(KernelArgumentsInOrder);
						std::cout << "\n Error Construction Failed cl_KernelFunctionArgumentOrderListStruct!";
						return;
					}
				}
				IsThisListUsable = true;
				IsConstructionSuccesful = true;
			}
		}

		//Copy Constructor
		cl_KernelFunctionArgumentOrderListStruct(const cl_KernelFunctionArgumentOrderListStruct& CopyStruct) : TotalNumberOfArugments(CopyStruct.TotalNumberOfArugments), KernelFunctionName(CopyStruct.KernelFunctionName)
		{
			std::cout << "\n Constructing cl_KernelFunctionArgumentOrderListStruct!";

			IsConstructionSuccesful = false;
			bool IsSuccesful = false;

			Essential::Malloc_PointerToArrayOfPointers((void***)&KernelArgumentsInOrder, TotalNumberOfArugments, sizeof(cl_Memory_Type*), IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Allocating :" << TotalNumberOfArugments * sizeof(cl_Memory_Type*) << " Byes Of Memory for KernelArgumentsInOrder In KernelFunctionArgumentOrderListStruct!\n";
				std::cout << "\n Error Construction Failed cl_KernelFunctionArgumentOrderListStruct!";
				return;
			}
			else
			{
				for (int i = 0; i < TotalNumberOfArugments; ++i)
				{
					KernelArgumentsInOrder[i] = new cl_Memory_Type(*((CopyStruct.KernelArgumentsInOrder)[i]));
					if (KernelArgumentsInOrder[i] == nullptr)
					{
						std::cout << "\n Error Allocating " << (TotalNumberOfArugments * sizeof(cl_Memory_Type*)) << " Byes Of Memory for KernelArgumentsInOrder[" << i << "] In KernelFunctionArgumentOrderListStruct!\n";
						for (int j = 0; j < i; ++j)
						{
							free(KernelArgumentsInOrder[j]);
						}
						free(KernelArgumentsInOrder);
						std::cout << "\n Error Construction Failed cl_KernelFunctionArgumentOrderListStruct!";
						return;
					}
				}
				IsConstructionSuccesful = true;

				IsThisListUsable = CopyStruct.IsThisListUsable;
				NumberOfArgumentsSet = CopyStruct.NumberOfArgumentsSet;
				NumberOfReads = CopyStruct.NumberOfReads;
				NumberOfWrites = CopyStruct.NumberOfWrites;
				NumberOfRead_And_Writes = CopyStruct.NumberOfRead_And_Writes;
				NumberOfLocals = CopyStruct.NumberOfLocals;
				NumberOfPrivates = CopyStruct.NumberOfPrivates;
			}
			
		}

		void SetMemoryTypeOfArugment(const unsigned int ArgumentNumber, cl_Memory_Type MemoryType, bool& IsSuccesful)
		{
			IsSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetMemoryTypeOfArugment Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
				return;
			}

			if (MemoryType == cl_Memory_Type::Uninitialized_cl_Memory)
			{
				std::cout << "\n Error Trying to Set The MemoryType of the Argument Number'" << ArgumentNumber << "' to cl_Memory_Type::Uninitialized_cl_Memory In SetMemoryTypeOfArugment In: cl_KernelFunctionArgumentOrderListStruct!\n";
				return;
			}
			if (ArgumentNumber < TotalNumberOfArugments)
			{
				if (ArgumentNumber < 0)
				{
					std::cout << "\n Error Argument Number'" << ArgumentNumber << "' Is Lesser than Zero In SetMemoryTypeOfArugment In: cl_KernelFunctionArgumentOrderListStruct!\n";
					return;
				}
				else
				{
					if (ArgumentNumber >= NumberOfArgumentsSet)
					{
						*(KernelArgumentsInOrder[ArgumentNumber]) = MemoryType;
						NumberOfArgumentsSet = NumberOfArgumentsSet + 1;
					}
					else
					{
						*(KernelArgumentsInOrder[ArgumentNumber]) = MemoryType;
					}
					IsSuccesful = true;
					if (NumberOfArgumentsSet == TotalNumberOfArugments)
					{
						IsThisListUsable = true;
					}
				}			
			}
			else
			{
				std::cout << "\n Error Argument Number + 1' " << ArgumentNumber << "' Is greater than the TotalNumberOfArguments In SetMemoryTypeOfArugment In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}
				
		//Redundant Getter Functions...
		/*void GetIsListUsable(bool& IsUsable)
		{
			IsUsable = IsThisListUsable && IsConstructionSuccesful;
		}
		void GetKernelFunctionName(std::string& ReturnVal, bool& IsSuccesful)
		{
			ReturnVal = "";
			if (IsConstructionSuccesful)
			{
				ReturnVal = KernelFunctionName;
			}
			else
			{
				std::cout << "\n Error Calling GetKernelFunctionName Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}
		void GetTotalNumberOfArugments(int& ReturnVal, bool& IsSuccesful)
		{
			ReturnVal = 0;
			if (IsConstructionSuccesful)
			{
				ReturnVal = TotalNumberOfArugments;
			}
			else
			{
				std::cout << "\n Error Calling GetTotalNumberOfArugments Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}
		void GetNumberOfReads(int& ReturnVal, bool& IsSuccesful)
		{
			ReturnVal = 0;
			if (IsConstructionSuccesful)
			{
				ReturnVal = NumberOfReads;
			}
			else
			{
				std::cout << "\n Error Calling GetNumberOfReads Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}
		void GetNumberOfWrites(int& ReturnVal, bool& IsSuccesful)
		{
			ReturnVal = 0;
			if (IsConstructionSuccesful)
			{
				ReturnVal = NumberOfWrites;
			}
			else
			{
				std::cout << "\n Error Calling GetNumberOfWrites Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}
		void GetNumberOfRead_And_Writes(int& ReturnVal, bool& IsSuccesful)
		{
			ReturnVal = 0;
			if (IsConstructionSuccesful)
			{
				ReturnVal = NumberOfRead_And_Writes;
			}
			else
			{
				std::cout << "\n Error Calling GetNumberOfRead_And_Writes Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}
		void GetNumberOfLocals(int& ReturnVal, bool& IsSuccesful)
		{
			ReturnVal = 0;
			if (IsConstructionSuccesful)
			{
				ReturnVal = NumberOfLocals;
			}
			else
			{
				std::cout << "\n Error Calling GetNumberOfLocals Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}
		void GetNumberOfPrivates(int& ReturnVal, bool& IsSuccesful)
		{
			ReturnVal = 0;
			if (IsConstructionSuccesful)
			{
				ReturnVal = NumberOfPrivates;
			}
			else
			{
				std::cout << "\n Error Calling GetNumberOfPrivates Without Constructing the struct In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}
		void GetMemoryTypeOfArgument(unsigned int ArgumentNumber, cl_Memory_Type& TheMemoryType, bool& IsSuccesful)
		{
			IsSuccesful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling GetMemoryTypeOfArgument, The OrderedListOfArugments Is Not Constructed, So Is Unusable In: cl_KernelFunctionArgumentOrderListStruct!\n";
				return;
			}
			else
			{
				if (!IsThisListUsable)
				{
					std::cout << "\n Error Calling GetMemoryTypeOfArgument,The OrderedListOfArugments Is Not Properly Set, So Is Unusable In: cl_KernelFunctionArgumentOrderListStruct!\n";
					return;
				}
			}
			if (ArgumentNumber < TotalNumberOfArugments)
			{
				TheMemoryType = *((KernelArgumentsInOrder)[ArgumentNumber]);
			}
			else
			{
				std::cout << "\n Error ArgumentNumber Exceeds the total number of Arguments Present! in GetMemoryTypeOfArgument In: cl_KernelFunctionArgumentOrderListStruct!\n";
			}
		}*/

		~cl_KernelFunctionArgumentOrderListStruct()
		{
			std::cout << "\n Destructing cl_KernelFunctionArgumentOrderListStruct!";
			if (IsConstructionSuccesful)
			{
				IsConstructionSuccesful = false;
				for (int i = 0; i < TotalNumberOfArugments; ++i)
				{
					free(KernelArgumentsInOrder[i]);
				}
				free(KernelArgumentsInOrder);
			}
		}
	};

	// NOTE: Call Constructor Before Using
	struct cl_PerDeviceValuesStruct
	{
		const cl_device_id SelectedDevice;
		cl_command_queue DeviceClCommandQueue = NULL;
		size_t MaxComputeUnitPerGPU = 0;
		size_t MaxWorkItemPerGroup = 0;
		size_t MaxGlobalMemoryOfDevice = 0;
		size_t MaxPrivateMemoryBytesPerWorkGroup = 0;
		size_t MaxLocalMemoryBytesPerWorkGroup = 0;
		bool IsConstructionSuccesful = false;// Constructor argument cl_int& ClErrorResult returns our succes value so 'IsConstructionSuccesful' Is only needed when exiting program

		cl_PerDeviceValuesStruct(cl_device_id ArgSelectedDevice, cl_context* TheClContext, cl_int& ClErrorResult) : SelectedDevice(ArgSelectedDevice)
		{
			std::cout << "\n Constructing cl_PerDeviceValuesStruct!";			

			DeviceClCommandQueue = NULL;
			MaxComputeUnitPerGPU = 0;
			MaxWorkItemPerGroup = 0;
			MaxGlobalMemoryOfDevice = 0;
			MaxPrivateMemoryBytesPerWorkGroup = 0;
			MaxLocalMemoryBytesPerWorkGroup = 0;

			IsConstructionSuccesful = false;

			DeviceClCommandQueue = clCreateCommandQueue(*TheClContext, SelectedDevice, 0, &ClErrorResult);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n Error: Cl Command Queue Creation Failed!\n";
				std::cout << "\n Error Construction Failed cl_PerDeviceValuesStruct!";
				return;
			}

			cl_uint Temp1 = 0;
			size_t Temp2 = 0;
			cl_ulong Temp3 = 0;// Calloc Called Below

			ClErrorResult = clGetDeviceInfo(SelectedDevice, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &Temp1, NULL);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n Error Code " << ClErrorResult << " : Device Get CL_DEVICE_MAX_COMPUTE_UNITS Info Failed!\n";
				ClErrorResult = clReleaseCommandQueue(DeviceClCommandQueue);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n CLError " << ClErrorResult << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!\n";
				}
				std::cout << "\n Error Construction Failed cl_PerDeviceValuesStruct!";
				return;
			}
			MaxComputeUnitPerGPU = (int)Temp1;
			std::cout << "\nMaxComputeUnitPerGPU:\n";
			std::cout << MaxComputeUnitPerGPU << "\n";

			ClErrorResult = clGetDeviceInfo(SelectedDevice, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &Temp2, NULL);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n Error Code " << ClErrorResult << " : Device Get CL_DEVICE_MAX_WORK_GROUP_SIZE Info Failed!\n";
				ClErrorResult = clReleaseCommandQueue(DeviceClCommandQueue);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n CLError " << ClErrorResult << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!\n";
				}
				std::cout << "\n Error Construction Failed cl_PerDeviceValuesStruct!";
				return;
			}
			MaxWorkItemPerGroup = (int)Temp2;
			std::cout << "\nMaxWorkItemPerGroup:\n";
			std::cout << MaxWorkItemPerGroup << "\n";

			ClErrorResult = clGetDeviceInfo(SelectedDevice, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &Temp3, NULL);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n Error Code " << ClErrorResult << " : Device Get CL_DEVICE_GLOBAL_MEM_SIZE Info Failed!\n";
				ClErrorResult = clReleaseCommandQueue(DeviceClCommandQueue);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n CLError " << ClErrorResult << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!\n";
				}
				std::cout << "\n Error Construction Failed cl_PerDeviceValuesStruct!";
				return;
			}
			MaxGlobalMemoryOfDevice = Temp3;
			std::cout << "\nMaxGlobalMemoryOfDevice:\n";// Total GPU VRAM 
			std::cout << MaxGlobalMemoryOfDevice << "\n";

			ClErrorResult = clGetDeviceInfo(SelectedDevice, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(cl_ulong), &Temp3, NULL);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n Error Code " << ClErrorResult << " : Device Get CL_DEVICE_GLOBAL_MEM_CACHE_SIZE Info Failed!\n";
				ClErrorResult = clReleaseCommandQueue(DeviceClCommandQueue);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n CLError " << ClErrorResult << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!\n";
				}
				std::cout << "\n Error Construction Failed cl_PerDeviceValuesStruct!";
				return;
			}
			MaxPrivateMemoryBytesPerWorkGroup = (int)Temp3;
			std::cout << "\nMaxPrivateMemoryBytesPerWorkGroup:\n";// Per Work Item
			std::cout << MaxPrivateMemoryBytesPerWorkGroup << "\n";

			ClErrorResult = clGetDeviceInfo(SelectedDevice, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &Temp3, NULL);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n Error Code " << ClErrorResult << " : Device Get CL_DEVICE_LOCAL_MEM_SIZE Info Failed!\n";
				ClErrorResult = clReleaseCommandQueue(DeviceClCommandQueue);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n CLError " << ClErrorResult << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!\n";
				}
				std::cout << "\n Error Construction Failed cl_PerDeviceValuesStruct!";
				return;
			}

			MaxLocalMemoryBytesPerWorkGroup = (int)Temp3;
			std::cout << "\nMaxLocalMemoryBytesPerWorkGroup:\n";// Per Work Group
			std::cout << MaxLocalMemoryBytesPerWorkGroup << "\n\n";
			IsConstructionSuccesful = true;
		}

		~cl_PerDeviceValuesStruct()
		{
			std::cout << "\n Destructing cl_PerDeviceValuesStruct!";
			if (IsConstructionSuccesful)
			{
				IsConstructionSuccesful = false;
				cl_int ClErrorResult = clReleaseCommandQueue(DeviceClCommandQueue);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n CLError " << ClErrorResult << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!\n";
				}
			}
		}
	};

	// NOTE: Call Constructor Before Using
	struct cl_MemoryStruct
	{
	private:
		const cl_kernel* TheKernel;
		const cl_uint KernelArgumentNumber;
		const cl_context* cl_ContextForThisArgument;
		const cl_command_queue* cl_CommandQueueForThisArgument;
		const cl_Memory_Type clMemory_Type_Of_Argument;
		bool DoesBufferAlreadyExist = false;
		cl_mem GlobalMemoryInDevice = nullptr;// Local not needed as it is inaccessible by host..
		void* COPY_OF_PrivateMemoryType = nullptr;
		size_t MemoryInDeviceTotalSizeInBytes = 0;//NOTE: for private pass the sizeof(variable_type)
		size_t MemoryInDevice_Occupied_SizeInBytes = 0;//NOTE: Actually 'MemoryInDeviceTotalSizeInBytes' occupies full space... but by 'MemoryInDevice_Occupied_SizeInBytes' I mean the memory YOU "use". I know this might sound confusing, but here is a simple example; Say you have a box that which can hold 20 pieces of bottles, but we only put in 10 pieces, meaning we used only 10 slot instead fully using 20 slots. 
	
		void FreeBuffer(bool& IsSuccesful)
		{
			IsSuccesful = false;
			if (DoesBufferAlreadyExist)
			{
				if (clMemory_Type_Of_Argument != cl_Memory_Type::Uninitialized_cl_Memory)
				{
					if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_PRIVATE)
					{
						if (COPY_OF_PrivateMemoryType != nullptr)
						{
							free(COPY_OF_PrivateMemoryType);
						}
					}
					else
					{
						cl_uint ClErrorResult = clReleaseMemObject(GlobalMemoryInDevice);// releasing Memory object every time this function is called	
						if (ClErrorResult != CL_SUCCESS)
						{
							std::cout << "\n ClError Code " << ClErrorResult << " : Releasing Memory On device In: cl_MemoryStruct!\n";
							return;
						}
					}

					DoesBufferAlreadyExist = false;
					MemoryInDeviceTotalSizeInBytes = 0;
					MemoryInDevice_Occupied_SizeInBytes = 0;

					IsSuccesful = true;
				}
				else
				{
					std::cout << "\n Error Trying to Releasing Uninitialized_cl_Memory Memory On device In BufferCreation In: cl_MemoryStruct!\n";
					return;
				}
			}
			else
			{
				std::cout << "\n Error Buffer does not exist, unable to free buffer" << ": BufferCreation In: cl_MemoryStruct!\n";
			}
		}

		void BufferCreation(size_t BUFFER_CREATION_ONLY_SizeOfBuffer, bool& IsFunctionSuccesful)
		{
			cl_int ClErrorResult;
			IsFunctionSuccesful = false;

			if (BUFFER_CREATION_ONLY_SizeOfBuffer < 1)
			{
				std::cout << "\n Error Supplied Size of " << BUFFER_CREATION_ONLY_SizeOfBuffer << " Bytes Is Less than 1 Bytes, Pass atleast 1 byte of Data for Buffer Creation" << ": BufferCreation In: cl_MemoryStruct!\n";
				return;
			}
			
			if (DoesBufferAlreadyExist)
			{
				FreeBuffer(IsFunctionSuccesful);
				if(IsFunctionSuccesful)
				{
					IsFunctionSuccesful = false;
				}
				else 
				{
					return;
				}
			}

			switch (clMemory_Type_Of_Argument)
			{
				case cl_Memory_Type::CL_LOCALENUM:
				{
					//No Need for Buffer creation as this is a local memory...
					//if (MemoryInDeviceTotalSizeInBytes != MemoryInDevice_Occupied_SizeInBytes)
					//{
					//	std::cout << "\n Error CL_LOCALENUM MemoryInDeviceTotalSizeInBytes is not equal to MemoryInDevice_Occupied_SizeInBytes In BufferCreation In: cl_MemoryStruct!\n";
					//	std::cout << "NOTE: Local Memory occupies space regardless of MemoryInDevice_Occupied_SizeInBytes, So both variables should have the same value\n";
					//	return;
					//}
					MemoryInDeviceTotalSizeInBytes = BUFFER_CREATION_ONLY_SizeOfBuffer;
					MemoryInDevice_Occupied_SizeInBytes = BUFFER_CREATION_ONLY_SizeOfBuffer;
					IsFunctionSuccesful = true;
					return;
					//break;// Not needed as return is done
				}

				case cl_Memory_Type::CL_PRIVATE:
				{					
					if ((MemoryInDeviceTotalSizeInBytes != 0) || (MemoryInDevice_Occupied_SizeInBytes != 0))
					{
						std::cout << "\n Error Trying to Change the Size of CL_PRIVATE Memory On device In BufferCreation In: cl_MemoryStruct!\n";
						return;
					}

					COPY_OF_PrivateMemoryType = calloc(BUFFER_CREATION_ONLY_SizeOfBuffer, sizeof(char));// malloc works great too, but i prefer to use calloc here, NOTE: Char is 1 Byte so using char
					if (COPY_OF_PrivateMemoryType == nullptr)
					{
						std::cout << "\n Error Allocating" << BUFFER_CREATION_ONLY_SizeOfBuffer * sizeof(char) << "COPY_OF_PrivateMemoryType Variable In: cl_MemoryStruct!\n";
						return;
					}

					MemoryInDeviceTotalSizeInBytes = BUFFER_CREATION_ONLY_SizeOfBuffer;
					MemoryInDevice_Occupied_SizeInBytes = BUFFER_CREATION_ONLY_SizeOfBuffer;
					IsFunctionSuccesful = true;
					return;
					//break;// Not needed as return is done
				}

				case cl_Memory_Type::Uninitialized_cl_Memory:
				{
					std::cout << "\n Error Default 'Uninitialized_cl_Memory' Enum Passed! Please pass any of these Enums CL_PRIVATE, CL_LOCALENUM, CL_READ_ONLY, CL_WRITE_ONLY, CL_READ_AND_WRITE In BufferCreation In: cl_MemoryStruct!\n";
					MemoryInDeviceTotalSizeInBytes = 0;
					MemoryInDevice_Occupied_SizeInBytes = 0;
					return;
					//break;// Not needed as return is done
				}

				default://CL_MEM_READ_ONLY, CL_MEM_WRITE_ONLY And CL_MEM_READ_WRITE
				{
					if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_READ_ONLY)
					{
						GlobalMemoryInDevice = clCreateBuffer(*cl_ContextForThisArgument, CL_MEM_READ_ONLY, BUFFER_CREATION_ONLY_SizeOfBuffer, NULL, &ClErrorResult);
					}
					else
					{
						if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_WRITE_ONLY)
						{
							GlobalMemoryInDevice = clCreateBuffer(*cl_ContextForThisArgument, CL_MEM_WRITE_ONLY, BUFFER_CREATION_ONLY_SizeOfBuffer, NULL, &ClErrorResult);
						}
						else
						{
							if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_READ_AND_WRITE)
							{
								GlobalMemoryInDevice = clCreateBuffer(*cl_ContextForThisArgument, CL_MEM_READ_WRITE, BUFFER_CREATION_ONLY_SizeOfBuffer, NULL, &ClErrorResult);
							}
							else
							{
								std::cout << "\n Error Undefined Enum Passed! Please pass any of these Enums CL_PRIVATE, CL_LOCALENUM, CL_READ_ONLY, CL_WRITE_ONLY, CL_READ_AND_WRITE In BufferCreation In: cl_MemoryStruct!\n";
								return;
							}
						}
					}
					//GlobalMemoryInDevice = clCreateBuffer(*cl_ContextForThisArgument, clMemory_Type_Of_Argument, BUFFER_CREATION_ONLY_SizeOfBuffer, NULL, &ClErrorResult);
					if (ClErrorResult != CL_SUCCESS)
					{
						std::cout << "\n ClError Code " << ClErrorResult << " : Creating Buffer On device In BufferCreation In: cl_MemoryStruct!\n";
						MemoryInDeviceTotalSizeInBytes = 0;
						MemoryInDevice_Occupied_SizeInBytes = 0;
						return;
					}
					MemoryInDeviceTotalSizeInBytes = BUFFER_CREATION_ONLY_SizeOfBuffer;
					MemoryInDevice_Occupied_SizeInBytes = 0;
					IsFunctionSuccesful = true;
					return;
					//break;// Not needed as return is done
				}
			}		
		}

	public:
		bool IsConstructionSuccesful = false;	

		cl_MemoryStruct(const cl_Memory_Type ArgclMemory_Type_Of_Argument, const cl_context* Argcl_ContextForThisArgument, const cl_command_queue* Argcl_CommandQueueForThisArgument, const cl_kernel* ArgTheKernel, const cl_uint ArgKernelArgumentNumber) : clMemory_Type_Of_Argument(ArgclMemory_Type_Of_Argument), cl_ContextForThisArgument(Argcl_ContextForThisArgument), cl_CommandQueueForThisArgument(Argcl_CommandQueueForThisArgument), TheKernel(ArgTheKernel), KernelArgumentNumber(ArgKernelArgumentNumber)
		{
			std::cout << "\n Constructing cl_MemoryStruct!";

			DoesBufferAlreadyExist = false;
			GlobalMemoryInDevice = NULL;
			COPY_OF_PrivateMemoryType = nullptr;
			MemoryInDeviceTotalSizeInBytes = 0;
			MemoryInDevice_Occupied_SizeInBytes = 0;

			IsConstructionSuccesful = false;

			if ((TheKernel == nullptr) || (cl_ContextForThisArgument == nullptr) || (cl_CommandQueueForThisArgument == nullptr))
			{
				std::cout << "\n Error nullptr Passed as value for const variables In: cl_Memory_Struct!";
				std::cout << "\n Error Construction Failed cl_MemoryStruct!";
				return;
			}

			BufferCreation(1, DoesBufferAlreadyExist);
			if(!DoesBufferAlreadyExist)
			{
				std::cout << "\n Error Construction Failed cl_MemoryStruct!";
				return;
			}
			IsConstructionSuccesful = true;			
		}

		void MemoryAllocationOnDevice(void* PointerToMemoryToCopyFrom, size_t SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, size_t BUFFER_CREATION_ONLY_SizeOfBuffer, bool OverWriteMemory, bool& IsFunctionSuccesful)//Note: If MemorySize is 0, Then no memory is written on the device but memory is created, Note: for CreateOnlyOptionalLargerBufferSize to take effect pass a size larger than SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type
		{
			IsFunctionSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling MemoryAllocationOnDevice Without Constructing the struct In: cl_MemoryStruct!\n";
				return;
			}
			else
			{
				cl_int ClErrorResult;

				if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type < 1)
				{
					std::cout << "\n Error :Supplied Memory Size of " << SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type << " Bytes Is Less than 1 Bytes, Pass atleast 1 byte of Data" << ": MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
					return;
				}

				if (OverWriteMemory)
				{
					if (clMemory_Type_Of_Argument != cl_Memory_Type::Uninitialized_cl_Memory)
					{
						//if (MemoryTypeOfThisMemoryInDevice != TheMemoryType)
						//{
						//	std::cout << "\n Error Supplied MemoryType Does not Match with Type Of MemoryInDevice" << ": OverWriting Buffer In: cl_MemoryStruct!\n";
						//	return;
						//}
						////else// This is unlikely to happen see the above two if stement, MemoryTypeOfThisMemoryInDevice != cl_Memory_Type::Uninitialized_cl_Memory, Then MemoryTypeOfThisMemoryInDevice != TheMemoryType
						////{
						////	if (TheMemoryType == cl_Memory_Type::Uninitialized_cl_Memory)
						////	{
						////		std::cout << "\n Error " << ClErrorResult << " : Default 'Uninitialized_cl_Memory' Enum passed! Please pass any of these Enums CL_PRIVATE, CL_LOCALENUM, CL_READ_ONLY, CL_WRITE_ONLY, CL_READ_AND_WRITE\n";
						////	}
						////}
						if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type <= MemoryInDeviceTotalSizeInBytes)
						{
							if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_PRIVATE)
							{
								if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type != MemoryInDeviceTotalSizeInBytes)
								{
									std::cout << "\n Error :Trying to change the Size of Private Variable(NOTE: This is impossible to happen. Because size of variables type does not change...)" << ": OverWriting Buffer In: cl_MemoryStruct!\n";
									return;
								}

								for (int i = 0; i < MemoryInDeviceTotalSizeInBytes; ++i)// Memccpy bad
								{
									((char*)COPY_OF_PrivateMemoryType)[i] = ((char*)PointerToMemoryToCopyFrom)[i];// I could simply convert void* to char*... but i left it as void* for the purpose of 'readability'
								}
							}
							else
							{
								if (clMemory_Type_Of_Argument != cl_Memory_Type::CL_LOCALENUM)// Uninitialized_cl_Memory Is imposible in this case, reason being if IsInitialized == true, then MemoryTypeOfThisMemoryInDevice is definitely not Uninitialized_cl_Memory
								{
									void* TempDataCarryHelper = calloc(SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, sizeof(char));// malloc works great too, but i prefer to use calloc here, NOTE: Char is 1 Byte so using char
									if (TempDataCarryHelper == nullptr)
									{
										std::cout << "\n Error Allocating:" << SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type * sizeof(char) << "TempDataCarryHelper Variable In: cl_MemoryStruct\n";
										return;
									}

									if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type > 0)
									{
										for (int i = 0; i < SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type; ++i)// Memccpy bad
										{
											((char*)TempDataCarryHelper)[i] = ((char*)PointerToMemoryToCopyFrom)[i];// I could simply convert void* to char*... but i left it as void* for the purpose of 'readability'
										}

										ClErrorResult = clEnqueueWriteBuffer(*cl_CommandQueueForThisArgument, GlobalMemoryInDevice, CL_TRUE, 0, SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, TempDataCarryHelper, 0, NULL, NULL);
										free(TempDataCarryHelper);// Free the data

										if (ClErrorResult != CL_SUCCESS)
										{
											std::cout << "\n CL_Error Code " << ClErrorResult << " : OverWriting Buffer In: cl_MemoryStruct!\n";
											return;
										}
									}
									MemoryInDevice_Occupied_SizeInBytes = SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type;
								}
								else
								{
									if (PointerToMemoryToCopyFrom != nullptr)
									{
										std::cout << "\n Error :Trying To OverWrite Local Memory(This type can not be written or overwritten from host...) So Pass nullptr to the parameter 'PointerToMemoryToCopyFrom'" << ": OverWriting Buffer In: cl_MemoryStruct!\n";
									}
									else
									{
										MemoryInDeviceTotalSizeInBytes = SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type;
										MemoryInDevice_Occupied_SizeInBytes = SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type;
									}
								}
							}
						}
						else
						{
							std::cout << "\n Error :Size Passed is larger than the Available buffer Space" << ": OverWriting Buffer In: cl_MemoryStruct!\n";
							return;
						}
					}
					else
					{
						std::cout << "\n Error :Trying To OverWrite Uninitialized Memory" << ": OverWriting Buffer In: cl_MemoryStruct!\n";
						return;
					}
				}
				else
				{
					if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type > BUFFER_CREATION_ONLY_SizeOfBuffer)
					{
						std::cout << "\n Error :Supplied Size Of SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type Excedes the size maximum size specified by BUFFER_CREATION_ONLY_SizeOfBuffer" << " : Creating Buffer In: cl_MemoryStruct!\n";
						return;
					}

					BufferCreation(BUFFER_CREATION_ONLY_SizeOfBuffer, IsFunctionSuccesful);
					if (IsFunctionSuccesful)
					{
						IsFunctionSuccesful = false;// Yes it is set to false So that it can be used for below code
					}
					else
					{
						std::cout << "\n Error :Buffer Creation Unsuccesful In MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
						return;
					}

					if ((clMemory_Type_Of_Argument == cl_Memory_Type::CL_READ_ONLY) || (clMemory_Type_Of_Argument == cl_Memory_Type::CL_WRITE_ONLY) || (clMemory_Type_Of_Argument == cl_Memory_Type::CL_READ_AND_WRITE))
					{
						if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type > 0)
						{
							ClErrorResult = clEnqueueWriteBuffer(*cl_CommandQueueForThisArgument, GlobalMemoryInDevice, CL_TRUE, 0, SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, PointerToMemoryToCopyFrom, 0, NULL, NULL);
							if (ClErrorResult != CL_SUCCESS)
							{
								std::cout << "\n Error Code " << ClErrorResult << " : Writing Buffer In MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
								ClErrorResult = clReleaseMemObject(GlobalMemoryInDevice);
								if (ClErrorResult != CL_SUCCESS)
								{
									std::cout << "\n CL_Error Code " << ClErrorResult << " : Releasing Memory On device In MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
									return;
								}
								MemoryInDeviceTotalSizeInBytes = 0;
								MemoryInDevice_Occupied_SizeInBytes = 0;
								return;
							}
							//Put the Buffer in Specified Device(GPU Or CPU)
							ClErrorResult = clEnqueueMigrateMemObjects(*cl_CommandQueueForThisArgument, 1, &GlobalMemoryInDevice, CL_MIGRATE_MEM_OBJECT_CONTENT_UNDEFINED, 0, NULL, NULL);
							if (ClErrorResult != CL_SUCCESS)
							{
								std::cout << "\n CL_Error Code " << ClErrorResult << " : Migrating Buffer To Specific Deice In MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
								ClErrorResult = clReleaseMemObject(GlobalMemoryInDevice);
								if (ClErrorResult != CL_SUCCESS)
								{
									std::cout << "\n CL_Error Code " << ClErrorResult << " : Releasing Memory On device In MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
									return;
								}
								return;
							}
							//MemoryInDeviceTotalSizeInBytes = BUFFER_CREATION_ONLY_SizeOfBuffer;// Same Value
							MemoryInDevice_Occupied_SizeInBytes = SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type;
						}
					}
					else
					{
						if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_PRIVATE)
						{
							for (int i = 0; i < SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type; ++i)// Memccpy bad
							{
								((char*)COPY_OF_PrivateMemoryType)[i] = ((char*)PointerToMemoryToCopyFrom)[i];// I could simply convert void* to char*... but i left it as void* for the purpose of 'readability'
							}
						}
						//else No need for else, as it is impossible for invalid or other enum type to get past through, unless a glitch occurs which is extremely unlikely...
					}
				}
				IsFunctionSuccesful = true;
			}
		}

		void PassBufferToKernel(bool& IsFunctionSuccesful)
		{
			IsFunctionSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling PassBufferToKernel Without Constructing the struct In: cl_MemoryStruct!\n";
				return;
			}
			else
			{
				cl_int ClErrorResult = 0;
				if ((clMemory_Type_Of_Argument == cl_Memory_Type::CL_READ_ONLY) || (clMemory_Type_Of_Argument == cl_Memory_Type::CL_WRITE_ONLY) || (clMemory_Type_Of_Argument == cl_Memory_Type::CL_READ_AND_WRITE))
				{
					ClErrorResult = clSetKernelArg(*TheKernel, KernelArgumentNumber, sizeof(cl_mem), GlobalMemoryInDevice);
				}
				else
				{
					if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_LOCALENUM)
					{
						ClErrorResult = clSetKernelArg(*TheKernel, KernelArgumentNumber, MemoryInDeviceTotalSizeInBytes, NULL);
					}
					else
					{
						if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_PRIVATE)
						{
							ClErrorResult = clSetKernelArg(*TheKernel, KernelArgumentNumber, MemoryInDeviceTotalSizeInBytes, COPY_OF_PrivateMemoryType);
						}
						//else No need for else, as it is impossible for invalid or other enum type to get past through, unless a glitch occurs which is extremely unlikely...
					}
				}
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n CL_Error Code " << ClErrorResult << " : Passing Buffer To Kernel In PassBufferToKernel In: cl_MemoryStruct!\n";
					return;
				}
				else
				{
					IsFunctionSuccesful = true;
				}
			}
		}

		void AllocateMemoryAndPassToKernel(void* PointerToMemoryToCopyFrom, size_t SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, size_t BUFFER_CREATION_ONLY_SizeOfBuffer, bool OverWriteMemory, bool& IsFunctionSuccesful)
		{
			IsFunctionSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling AllocateMemoryAndPassToKernel Without Constructing the struct In: cl_MemoryStruct!\n";
				return;
			}

			MemoryAllocationOnDevice(PointerToMemoryToCopyFrom, SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, BUFFER_CREATION_ONLY_SizeOfBuffer, OverWriteMemory, IsFunctionSuccesful);
			if (!IsFunctionSuccesful)
			{
				std::cout << "\n Error :Memory Allocation On Device was Unsuccesful In AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
			}
			else
			{
				PassBufferToKernel(IsFunctionSuccesful);
				if (!IsFunctionSuccesful)
				{
					std::cout << "\n Error :Sending buffer to kernel failed In AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
				}
			}
		}

		~cl_MemoryStruct()
		{
			std::cout << "\n Destructing cl_MemoryStruct!";
			if (IsConstructionSuccesful)
			{		
				IsConstructionSuccesful = false;
				if (DoesBufferAlreadyExist)
				{
					FreeBuffer(IsConstructionSuccesful);//Temprarly using IsConstructionSuccesful boolean here
					IsConstructionSuccesful = false;
					if (clMemory_Type_Of_Argument != cl_Memory_Type::Uninitialized_cl_Memory)
					{
						cl_int ClErrorResult;
						ClErrorResult = clReleaseMemObject(GlobalMemoryInDevice);
						if (ClErrorResult != CL_SUCCESS)
						{
							std::cout << "\n CLError " << ClErrorResult << " : Releasing cl_MemoryStruct's Device Memory In: cl_MemoryStruct!";
						}
						if (COPY_OF_PrivateMemoryType != nullptr)
						{
							free(COPY_OF_PrivateMemoryType);
						}
					}
				}
				IsConstructionSuccesful = false;
			}
		}
	};

	// NOTE: Call Constructor Before Using
	struct cl_KernelSingleArgumentStruct
	{
	private:	
		bool IsDataSendCompleted = false;
		bool TrueForCreateFalseForOverWrite = true;//When True Creates The Buffer
		void* COPY_OF_DataFromHost = nullptr;//For Global and Private Only , Pass NULL for Local...
		size_t DataFromHostSize = 0;//For Global ONLY, Reads Data When IsReadMemory is true and IsWriteMemory is true or false And IsLocalMemory is false, Memory Can be Written to the buffer
		size_t CL_MemorySizeToCreate = 0;//For Local And Global //NOTE: NO NEED TO Allocate memory for local from host side... memory allocation from host is only required for global			
		cl_MemoryStruct* BufferOnDevice = nullptr;//Example: Buffer on GPU device	

	public:
		bool IsConstructionSuccesful = false;// NOTE: Do not change this manualy! unless you know what you are doing

		//Contructor
		cl_KernelSingleArgumentStruct(const cl_Memory_Type ArgclMemory_Type_Of_Argument, const cl_context* Argcl_ContextForThisArgument, const cl_command_queue* Argcl_CommandQueueForThisArgument, const cl_kernel* TheKernel,	const cl_uint KernelArgumentNumber)
		{
			std::cout << "\n Constructing cl_KernelSingleArgumentStruct!";

			IsDataSendCompleted = false;
			TrueForCreateFalseForOverWrite = true;
			COPY_OF_DataFromHost = nullptr;
			DataFromHostSize = 0;
			CL_MemorySizeToCreate = 0;
			BufferOnDevice = nullptr;

			IsConstructionSuccesful = false;

			BufferOnDevice = new cl_MemoryStruct(ArgclMemory_Type_Of_Argument, Argcl_ContextForThisArgument, Argcl_CommandQueueForThisArgument, TheKernel, KernelArgumentNumber);
			if (BufferOnDevice == nullptr)
			{			
				std::cout << "\n Error Allocating :" << sizeof(cl_MemoryStruct) << " Byes Of Memory for BufferOnDevice In cl_KernelSingleArgumentStruct!\n";
				return;
			}	
			if (!BufferOnDevice->IsConstructionSuccesful)
			{
				std::cout << "\n Error Construction Failed cl_PerDeviceValuesStruct!";
				delete BufferOnDevice;
				return;
			}
			IsConstructionSuccesful = true;
		}

		//Do not call this without calling PassDataToThisKernelArgument() first
		void PassDataToDeviceBuffer()
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling PassDataToDeviceBuffer Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
				return;
			}

			if (!IsDataSendCompleted)
			{
				BufferOnDevice->MemoryAllocationOnDevice(COPY_OF_DataFromHost, DataFromHostSize, CL_MemorySizeToCreate, !TrueForCreateFalseForOverWrite, IsDataSendCompleted);
				if (!IsDataSendCompleted)
				{
					std::cout << "\n Error :New Data Was not sent to buffer In PassDataToDeviceBuffer In: cl_KernelSingleArgumentStruct!\n";
				}
			}
			else
			{
				std::cout << "\n Error :Same Data Was already sent to buffer In PassDataToDeviceBuffer In: cl_KernelSingleArgumentStruct!\n";
			}
		}
	
		//NOTE: AUTO_AUDJUST_AND_FIX_SIZE_AND_DATA_COMPLETE_ERROR is by default false, 
		void PassDataToThisKernelArgument(void* DataToPass, size_t SizeOfData,const bool If_Data_Empty_PassZeroForSizeOfData_And_SetThisToFalse, size_t BufferSizeToCreate, bool ARGUMENT_TrueForCreateFalseForOverWrite, bool& IsSuccesful, const bool /*AUTO_AUDJUST_AND_FIX_SIZE_AND_DATA_COMPLETE_ERROR*/ = false)
		{
			const bool AUTO_AUDJUST_AND_FIX_SIZE_AND_DATA_COMPLETE_ERROR = false;//NOTE: This parameter is Deprecated, So in place of it a dummy variable is put
			//NOTE: This Deprecated version works perfectly fine but i do not recommend using it, as you MAY make some kind of mistake while using it
			IsSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling PassDataToThisKernelArgument Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
				return;
			}

			if (If_Data_Empty_PassZeroForSizeOfData_And_SetThisToFalse)//Yes, Variable name too long, but it Explains what it does
			{
				if (SizeOfData > 0)
				{
					// Deprecated 
					if (AUTO_AUDJUST_AND_FIX_SIZE_AND_DATA_COMPLETE_ERROR)
					{
						DataToPass = calloc(1, sizeof(char));						
						SizeOfData = 1;
						if (DataToPass == nullptr)
						{
							std::cout << "\n Error Allocating : " << 1 * sizeof(char) << " Byes Of Memory for DataToPass(NOTE: This funtion argument is reused) in PassDataToThisKernelArgument In: cl_KernelSingleArgumentStruct!\n";
							return;
						}
					}
					else// Error Code
					{
						std::cout << "\n Error :Is_Data_Empty is set to true but SizeOfData is Greater than Zero In PassDataToThisKernelArgument In: cl_KernelSingleArgumentStruct!\n";
						return;
					}				
				}
				else
				{
					if (DataToPass != nullptr)
					{
						std::cout << "\n Error :Is_Data_Empty is set to true but DataToPass is not set to nullptr In PassDataToThisKernelArgument In: cl_KernelSingleArgumentStruct!\n";
						return;
					}
					else
					{
						// For Local Memory
						// NOTE: This is a dummy value, it can not be sent to kernel... but can be stored in buffer(Sometimes storing Zero bytes in buffer causes Errors, so storing minimal amount of data which is 1 byte)
						DataToPass = calloc(1, sizeof(char));						
						SizeOfData = 1;
						if (DataToPass == nullptr)
						{
							std::cout << "\n Error Allocating : " << 1 * sizeof(char) << " Byes Of Memory for DataToPass(NOTE: This funtion argument is reused) in PassDataToThisKernelArgument In: cl_KernelSingleArgumentStruct!\n";
							return;
						}
					}				
				}
			}
			else
			{
				if (SizeOfData < 2)
				{
					std::cout << "\n Error :SizeOfData of " << SizeOfData << " bytes Is Less than 2 Bytes In PassDataToThisKernelArgument In: cl_KernelSingleArgumentStruct!\n";
					std::cout << "NOTE: If you are trying to pass Empty/Null Data, Then set the 'DataToPass' to nullptr, 'SizeOfData' to 0 and 'If_Data_Empty_PassZeroForSizeOfData_And_SetThisToFalse' to true\n";
					return;
				}
			}

			if (!ARGUMENT_TrueForCreateFalseForOverWrite)
			{
				if (IsDataSendCompleted)
				{
					if (CL_MemorySizeToCreate < SizeOfData)
					{
						if (AUTO_AUDJUST_AND_FIX_SIZE_AND_DATA_COMPLETE_ERROR)
						{
							ARGUMENT_TrueForCreateFalseForOverWrite = true;
							CL_MemorySizeToCreate = SizeOfData;
						}
						else
						{
							std::cout << "\n Error :SizeOfData Exceeds BufferSize OverWrite Buffer Not Possible In PassDataToThisKernelArgument In: cl_KernelSingleArgumentStruct!\n";
							return;
						}					
					}
				}
				else
				{		
					if (AUTO_AUDJUST_AND_FIX_SIZE_AND_DATA_COMPLETE_ERROR)
					{
						ARGUMENT_TrueForCreateFalseForOverWrite = true;
						DataFromHostSize = SizeOfData;
						CL_MemorySizeToCreate = SizeOfData;
					}
					else
					{
						std::cout << "\n Error :OverWrite Not Possible as IsDataSendCompleted is set to false In PassDataToThisKernelArgument In: cl_KernelSingleArgumentStruct!\n";
						return;
					}				
				}
			}
			else
			{
				if (SizeOfData > BufferSizeToCreate)
				{
					if (AUTO_AUDJUST_AND_FIX_SIZE_AND_DATA_COMPLETE_ERROR)
					{
						CL_MemorySizeToCreate = SizeOfData;
					}
					else
					{
						std::cout << "\n Error :SizeOfData Exceeds BufferSize Create Buffer Not Possible In PassDataToThisKernelArgument cl_KernelSingleArgumentStruct!\n";
						return;
					}				
				}
			}

			void* TempDataCarryHelper = calloc(SizeOfData, sizeof(char));// malloc works great too, but i prefer to use calloc here, NOTE: Char is 1 Byte so using char
			if (TempDataCarryHelper == nullptr)
			{
				std::cout << "\n Error Allocating:" << SizeOfData * sizeof(char) << "TempDataCarryHelper Variable In PassDataToThisKernelArgument In: cl_KernelSingleArgumentStruct!\n";
				return;
			}

			for (int i = 0; i < SizeOfData; ++i)// Memccpy bad
			{
				((char*)TempDataCarryHelper)[i] = ((char*)DataToPass)[i];// I could simply convert void* to char*... but i left it as void* for the purpose of 'readability'				
			}		

			IsDataSendCompleted = false;
			TrueForCreateFalseForOverWrite = ARGUMENT_TrueForCreateFalseForOverWrite;
			if (COPY_OF_DataFromHost != nullptr)
			{
				free(COPY_OF_DataFromHost);
			}
			COPY_OF_DataFromHost = TempDataCarryHelper;
			PassDataToDeviceBuffer();
			IsSuccesful = IsDataSendCompleted;//The Return Value
		}	

		//Destructor
		~cl_KernelSingleArgumentStruct()
		{
			std::cout << "\n Destructing cl_KernelSingleArgumentStruct!";
			if (IsConstructionSuccesful)
			{
				delete BufferOnDevice;
				if (COPY_OF_DataFromHost != nullptr)
				{
					free(COPY_OF_DataFromHost);
				}			
				IsConstructionSuccesful = false;
			}
		}
	};

	// NOTE: Call Constructor Before Using
	struct cl_KernelMultipleArgumentStruct
	{
	private:
		bool IsConstructionSuccesful = false;// NOTE: Memory Leaks bad, so do not change this manualy...
	
		const cl_KernelFunctionArgumentOrderListStruct* OrderedListOfArugments;
		cl_KernelSingleArgumentStruct** SingleKernelFunctionMultiArgumentsArray = nullptr;// Arguments stored here
		cl_kernel ThisKernel = NULL;

		void CreateKernelSingleArgumentStruct(const cl_context* cl_ContextForThisKernel, const cl_command_queue* Argcl_CommandQueueForThisKernel, const cl_kernel* TheKernel, bool& IsSuccesful)
		{
			IsSuccesful = false;

			if (OrderedListOfArugments->IsConstructionSuccesful)
			{
				if (OrderedListOfArugments->IsThisListUsable)
				{
					for (int i = 0; i < OrderedListOfArugments->TotalNumberOfArugments; ++i)
					{
						SingleKernelFunctionMultiArgumentsArray[i] = new cl_KernelSingleArgumentStruct(*(OrderedListOfArugments->KernelArgumentsInOrder[i]), cl_ContextForThisKernel, Argcl_CommandQueueForThisKernel, TheKernel, i);
						if (SingleKernelFunctionMultiArgumentsArray[i] != nullptr)
						{
							if (SingleKernelFunctionMultiArgumentsArray[i]->IsConstructionSuccesful == false)// ! works great but, '== false' make it readable
							{
								std::cout << "\n Error Construction Failed: Of SingleKernelFunctionMultiArgumentsArray[" << i << "] of type CreateKernelSingleArgumentStruct in CreateKernelSingleArgumentStruct In: cl_KernelMultipleArgumentStruct!\n";
								for (int j = 0; j <= i; ++j)// Greater than or equal to i
								{
									delete SingleKernelFunctionMultiArgumentsArray[i];
								}
								free(SingleKernelFunctionMultiArgumentsArray);
								return;
							}
						}
						else
						{
							std::cout << "\n Error Allocating :" << sizeof(cl_KernelSingleArgumentStruct) << " Byes Of Memory for CreateKernelSingleArgumentStruct in CreateKernelSingleArgumentStruct In: cl_KernelMultipleArgumentStruct!\n";
							for (int j = 0; j < i; ++j)// Greater than i
							{
								delete SingleKernelFunctionMultiArgumentsArray[i];
							}
							free(SingleKernelFunctionMultiArgumentsArray);
							return;
						}
					}
					IsSuccesful = true;
				}
				else
				{
					std::cout << "\n Error The OrderedListOfArugments Is Not Properly Set, So Is Unusable in CreateKernelSingleArgumentStruct In: cl_KernelMultipleArgumentStruct!\n";
				}
			}	
			else
			{
				std::cout << "\n Error The OrderedListOfArugments Is Not Constructed, So Is Unusable in CreateKernelSingleArgumentStruct In: cl_KernelMultipleArgumentStruct!\n";
			}
		}

	public:
		cl_KernelMultipleArgumentStruct(const cl_KernelFunctionArgumentOrderListStruct* ArgOrderedListOfArugments,
			const cl_context* Argcl_ContextForThisKernel, const cl_command_queue* Argcl_CommandQueueForThisKernel, cl_program* BuiltClProgramContainingTheSpecifiedFunctions) : OrderedListOfArugments(ArgOrderedListOfArugments)
		{
			std::cout << "\n Constructing cl_KernelMultipleArgumentStruct!";

			SingleKernelFunctionMultiArgumentsArray = nullptr;

			bool IsSuccesful = false;
			IsConstructionSuccesful = false;// Yes this is set to false

			if ((ArgOrderedListOfArugments == nullptr) || (Argcl_ContextForThisKernel == nullptr) || (Argcl_CommandQueueForThisKernel == nullptr) || (BuiltClProgramContainingTheSpecifiedFunctions == nullptr))
			{
				std::cout << "\n Error nullptr Passed as value for const variables In: cl_KernelMultipleArgumentStruct!";
				std::cout << "\n Error Construction Failed cl_KernelMultipleArgumentStruct!";
				return;
			}

			if (OrderedListOfArugments->IsConstructionSuccesful)
			{
				if (OrderedListOfArugments->IsThisListUsable)
				{
					Essential::Malloc_PointerToArrayOfPointers((void***)&SingleKernelFunctionMultiArgumentsArray, OrderedListOfArugments->TotalNumberOfArugments, sizeof(cl_KernelSingleArgumentStruct*), IsSuccesful);
					if (!IsSuccesful)
					{
						std::cout << "\n Error Allocating " << (OrderedListOfArugments->TotalNumberOfArugments * sizeof(cl_KernelSingleArgumentStruct*)) << " Byes Of Memory for SingleKernelFunctionMultiArgumentsArray In cl_KernelMultipleArgumentStruct!\n";
						std::cout << "\n Error Construction Failed cl_KernelMultipleArgumentStruct!";
						return;
					}
				}
				else
				{
					std::cout << "\n Error The OrderedListOfArugments Is Not Properly Set, So Is Unusable In: cl_KernelMultipleArgumentStruct!\n";
					return;
				}
			}
			else
			{
				std::cout << "\n Error The OrderedListOfArugments Is Not Constructed, So Is Unusable In: cl_KernelMultipleArgumentStruct!\n";
				return;
			}
			
			cl_int ClErrorResult = 0;
			ThisKernel = clCreateKernel(*BuiltClProgramContainingTheSpecifiedFunctions, OrderedListOfArugments->KernelFunctionName.c_str(), &ClErrorResult);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n CLError " << ClErrorResult << " : Kernel Creation Failed in cl_KernelMultipleArgumentStruct!\n";
				return;
			}

			CreateKernelSingleArgumentStruct(Argcl_ContextForThisKernel, Argcl_CommandQueueForThisKernel, &ThisKernel, IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Single Kernel Argument Failed In: cl_KernelMultipleArgumentStruct!\n";
				clReleaseKernel(ThisKernel);
				return;
			}			

			IsConstructionSuccesful = true;
		}

		~cl_KernelMultipleArgumentStruct()
		{
			std::cout << "\n Destructing cl_KernelMultipleArgumentStruct!";
			if (IsConstructionSuccesful)
			{
				clReleaseKernel(ThisKernel);
				if (SingleKernelFunctionMultiArgumentsArray != nullptr)
				{
					for (int i = 0; i < OrderedListOfArugments->TotalNumberOfArugments; ++i)
					{
						delete SingleKernelFunctionMultiArgumentsArray[i];
					}
					free(SingleKernelFunctionMultiArgumentsArray);
				}			
				IsConstructionSuccesful = false;
			}
		}
	};

	// NOTE: Call Constructor Before Using
	struct cl_MultiDevice_KernelFunctionStruct
	{
	private:
		const unsigned int NumberOfDevices;
		const std::string KernelFunctionName;
		cl_KernelMultipleArgumentStruct** MultiDeviceKernelArgumentsArray = nullptr;

	public:
		bool IsConstructionSuccesful = false;// Once again i tell you this If you hate memory leaks, Don't change value. If you Love memory leaks change them!
		//NOTE: Kernel Arguments are Ordered from left to right, Read only memory in left, write only in right, Local in middle of them
		//NOTE: any memory which is plain int, float etc etc those which are NOT cl_mem, Will NOT BE INCLUDED HERE instead they will be given as parameters in kernel Host function
		cl_MultiDevice_KernelFunctionStruct(const unsigned int ArgNumberOfDevices, const cl_KernelFunctionArgumentOrderListStruct* ArgOrderedListOfArugments,
			const cl_context* Argcl_ContextForThisKernel, const cl_PerDeviceValuesStruct* Argcl_PerDeviceValueStruct,
			cl_program* BuiltClProgramContainingTheSpecifiedFunctions) : NumberOfDevices(ArgNumberOfDevices), KernelFunctionName(ArgOrderedListOfArugments->KernelFunctionName)
		{
			std::cout << "\n Constructing cl_MultiDevice_KernelFunctionStruct!";

			MultiDeviceKernelArgumentsArray = nullptr;

			IsConstructionSuccesful = false;

			if ((ArgOrderedListOfArugments == nullptr) || (Argcl_ContextForThisKernel == nullptr) || (Argcl_PerDeviceValueStruct == nullptr) || (BuiltClProgramContainingTheSpecifiedFunctions == nullptr))
			{
				std::cout << "\n Error nullptr Passed as value for const variables In: cl_MultiDevice_KernelFunctionStruct!";
				std::cout << "\n Error Construction Failed cl_MultiDevice_KernelFunctionStruct!";
				return;
			}

			if (!ArgOrderedListOfArugments->IsConstructionSuccesful)
			{
					std::cout << "\n Error The OrderedListOfArugments Is Not Constructed, So Is Unusable In: cl_MultiDevice_KernelFunctionStruct!\n";
				return;
			}
			else
			{
				if (!ArgOrderedListOfArugments->IsThisListUsable)
				{
					std::cout << "\n Error The OrderedListOfArugments Is Not Properly Set, So Is Unusable In: cl_MultiDevice_KernelFunctionStruct!\n";
					return;
				}			
			}

			bool IsSuccesful = false;
			if (NumberOfDevices > 0)
			{
				Essential::Malloc_PointerToArrayOfPointers((void***)&MultiDeviceKernelArgumentsArray, NumberOfDevices, sizeof(cl_KernelMultipleArgumentStruct*), IsSuccesful);
				if (!IsSuccesful)
				{
					std::cout << "\n Error Allocating " << (NumberOfDevices * sizeof(cl_KernelMultipleArgumentStruct*)) << " Byes Of Memory for MultiDeviceKernelArgumentsArray In cl_MultiDevice_KernelFunctionStruct!\n";
					return;
				}				
			}
			else
			{
				std::cout << "\n Error The Number Of Devices Is Set to Zero In: cl_MultiDevice_KernelFunctionStruct!\n";
				return;
			}

			cl_int ClErrorResult;
			for (int i = 0; i < NumberOfDevices; ++i)
			{	
				MultiDeviceKernelArgumentsArray[i] = new cl_KernelMultipleArgumentStruct(ArgOrderedListOfArugments, Argcl_ContextForThisKernel, &Argcl_PerDeviceValueStruct->DeviceClCommandQueue, BuiltClProgramContainingTheSpecifiedFunctions);
				if (MultiDeviceKernelArgumentsArray[i] == nullptr)
				{
					std::cout << "\n Error Allocating " << sizeof(cl_KernelMultipleArgumentStruct) << " Byes Of Memory for MultiDeviceKernelArgumentsArray[" << i << "] In cl_MultiDevice_KernelFunctionStruct!\n";
				
					for (int j = 0; j < i; ++j)
					{
						delete MultiDeviceKernelArgumentsArray[j];					
					}
					free(MultiDeviceKernelArgumentsArray);
					return;
				}
			}
			IsConstructionSuccesful = true;
		}

		~cl_MultiDevice_KernelFunctionStruct()
		{
			std::cout << "\n Destructing cl_MultiDevice_KernelFunctionStruct!";
			if (IsConstructionSuccesful)
			{
				for (int i = 0; i < NumberOfDevices; ++i)
				{
					delete MultiDeviceKernelArgumentsArray[i];
				}
				free(MultiDeviceKernelArgumentsArray);
				IsConstructionSuccesful = false;
			}
		}
	};

	// NOTE: Use this to manually add specific functions to the constructor of 'cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct', Also Call Constructor Before Using
	struct cl_KernelFunctionsStruct
	{
	public:
		cl_KernelFunctionArgumentOrderListStruct** OpenCL_KernelFunctions = nullptr;
		const unsigned int TotalNumberOfFuctions;
		bool IsAllFunctionsNameAndTotalArgumentSet = false;
		bool IsAllFunctionsUsable = false;
		unsigned int TotalNumberOfFunctionsNameAndTotalArgumentSet = 0;
		unsigned int TotalNumberOfFunctionsUsable = 0;

	private:
		void InitializeKernelFunctionList(unsigned int NumberOfFunctionsToAdd, bool& IsSuccesful)// This is the initializer for 'cl_KernelFunctionsStruct'
		{
			IsSuccesful = false;
			if (OpenCL_KernelFunctions != nullptr)
			{
				std::cout << "\n Error The value of the variable 'OpenCL_KernelFunctions' Is not nullptr, It should be empty because it is to be initialized, In ManualInitializeKernelFunctionList In: cl_KernelFunctionsStruct!\n";
			}
			else
			{
				Essential::Malloc_PointerToArrayOfPointers((void***)&OpenCL_KernelFunctions, NumberOfFunctionsToAdd, sizeof(cl_KernelFunctionArgumentOrderListStruct*), IsSuccesful);
				if (!IsSuccesful)
				{
					std::cout << "\n Error Allocating " << (NumberOfFunctionsToAdd * sizeof(cl_KernelFunctionArgumentOrderListStruct*)) << " Byes Of Memory for VariableToInitialize In ManualInitializeKernelFunctionList In: cl_KernelFunctionsStruct!\n";
					return;
				}
			}
		}

	public:
		bool IsConstructionSuccesful = false;
		cl_KernelFunctionsStruct(unsigned int NumberOfFunctionsToAdd, bool& IsSuccesful) : TotalNumberOfFuctions(NumberOfFunctionsToAdd)
		{
			std::cout << "\n Constructing cl_KernelFunctionsStruct!";

			OpenCL_KernelFunctions = nullptr;
			IsAllFunctionsNameAndTotalArgumentSet = false;
			IsAllFunctionsUsable = false;
			TotalNumberOfFunctionsNameAndTotalArgumentSet = 0;
			TotalNumberOfFunctionsUsable = 0;

			IsConstructionSuccesful = false;			

			IsAllFunctionsNameAndTotalArgumentSet = false;
			IsAllFunctionsUsable = false;
			TotalNumberOfFunctionsNameAndTotalArgumentSet = 0;
			TotalNumberOfFunctionsUsable = 0;

			InitializeKernelFunctionList(NumberOfFunctionsToAdd, IsConstructionSuccesful);
			IsSuccesful = IsConstructionSuccesful;
		}

		void SetTheNameAndNumberOfArgumentsForNextKernelFunction(std::string FunctionName, unsigned int TotalNumberOfArgumentsForTheFunction, bool& IsSuccesful)
		{
			IsSuccesful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetTheNameAndNumberOfArgumentsForNextKernelFunction Without Constructing the struct In: cl_KernelFunctionsStruct!\n";
				return;
			}

			if (TotalNumberOfFunctionsNameAndTotalArgumentSet < TotalNumberOfFuctions)
			{
				OpenCL_KernelFunctions[TotalNumberOfFunctionsNameAndTotalArgumentSet] = new cl_KernelFunctionArgumentOrderListStruct(TotalNumberOfArgumentsForTheFunction, FunctionName);
				if (OpenCL_KernelFunctions[TotalNumberOfFunctionsNameAndTotalArgumentSet] == nullptr)
				{
					std::cout << "\n Error Allocating " << (sizeof(cl_KernelFunctionArgumentOrderListStruct)) << " Byes Of Memory for OpenCL_KernelFunctions[" << TotalNumberOfFunctionsNameAndTotalArgumentSet << "] in SetTheNameAndNumberOfArgumentsForNextKernelFunction In: cl_KernelFunctionsStruct!\n";
				}
				else
				{
					TotalNumberOfFunctionsNameAndTotalArgumentSet = TotalNumberOfFunctionsNameAndTotalArgumentSet + 1;
					if (TotalNumberOfFunctionsNameAndTotalArgumentSet == TotalNumberOfFuctions)
					{
						IsAllFunctionsNameAndTotalArgumentSet = true;
					}
					IsSuccesful = true;
				}
			}
			else
			{
				std::cout << "\n Error Already Set the Name and Total Number of Arugments of all functions, ";
				if (IsAllFunctionsUsable)
				{
					std::cout << "Also all the functions are Fully Usable! in SetTheNameAndNumberOfArgumentsForNextKernelFunction In: cl_KernelFunctionsStruct!\n";
				}
				else
				{
					if (TotalNumberOfFunctionsUsable == 0)
					{
						std::cout << "But None Of functions Are Usable! in SetTheNameAndNumberOfArgumentsForNextKernelFunction In: cl_KernelFunctionsStruct!\n";
					}
					else
					{
						std::cout << "But only " << TotalNumberOfFunctionsUsable << "Number Of functions Are Usable! in SetTheNameAndNumberOfArgumentsForNextKernelFunction In: cl_KernelFunctionsStruct!\n";
					}
				}
			}
		}

		void SetMemoryTypeOfArugment(const unsigned int FunctionNumber, const unsigned int ArgumentNumber, cl_Memory_Type MemoryType, bool& IsSuccesful)//NOTE: Function Number Starts from 0 To (TotalFunctions - 1), Example: For the first Function Use 0, for the Second use 1 and so on
		{
			IsSuccesful = false;
		
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetMemoryTypeOfArugment Without Constructing the struct In: cl_KernelFunctionsStruct!\n";
				return;
			}

			if (ArgumentNumber < OpenCL_KernelFunctions[FunctionNumber]->TotalNumberOfArugments)
			{
				if (OpenCL_KernelFunctions[FunctionNumber]->IsThisListUsable)
				{
					OpenCL_KernelFunctions[FunctionNumber]->SetMemoryTypeOfArugment(ArgumentNumber, MemoryType, IsSuccesful);
					if (!IsSuccesful)
					{
						std::cout << "\n Error The Function cl_KernelFunctionArgumentOrderListStruct:SetMemoryTypeOfArugment failed in SetMemoryTypeOfArugment In: cl_KernelFunctionsStruct!\n";
					}
				}
				else
				{
					OpenCL_KernelFunctions[FunctionNumber]->SetMemoryTypeOfArugment(ArgumentNumber, MemoryType, IsSuccesful);
					if (IsSuccesful)
					{
						if (!IsAllFunctionsUsable)
						{
							if (OpenCL_KernelFunctions[FunctionNumber]->IsThisListUsable)
							{
								TotalNumberOfFunctionsUsable = TotalNumberOfFunctionsUsable + 1;
								if (TotalNumberOfFunctionsUsable == TotalNumberOfFuctions)
								{
									IsAllFunctionsUsable = true;
								}
							}

						}
					}
					else
					{
						std::cout << "\n Error The Function cl_KernelFunctionArgumentOrderListStruct:SetMemoryTypeOfArugment failed in SetMemoryTypeOfArugment In: cl_KernelFunctionsStruct!\n";
					}
				}

			}
			else
			{
				std::cout << "\n Error Argument Number + 1' " << ArgumentNumber << "' Is greater than the TotalNumberOfArguments, ";
				if (IsAllFunctionsUsable)
				{
					std::cout << "Also all the functions are Fully Usable! In SetMemoryTypeOfArugment In: cl_KernelFunctionsStruct!\n";
				}
				else
				{
					if (TotalNumberOfFunctionsUsable == 0)
					{
						std::cout << "But None Of functions Are Usable! In SetMemoryTypeOfArugment In: cl_KernelFunctionsStruct!\n";
					}
					else
					{
						std::cout << "But only " << TotalNumberOfFunctionsUsable << "Number Of functions Are Usable! In SetMemoryTypeOfArugment In: cl_KernelFunctionsStruct!\n";
					}
				}
			}
		}

		~cl_KernelFunctionsStruct()
		{
			std::cout << "\n Destructing cl_KernelFunctionsStruct!";
			if (IsConstructionSuccesful)
			{
				for (int i = 0; i < TotalNumberOfFunctionsNameAndTotalArgumentSet; ++i)
				{
					delete(OpenCL_KernelFunctions[i]);
				}
				free(OpenCL_KernelFunctions);
				IsConstructionSuccesful = false;
			}
		}
	};

	// IMPORTANT NOTE: This is the Only struct You will ever need, Because this Struct Does everything you want an OpenCL Wrapper to do
	struct cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct
	{
	private:
		const std::string ClSourceFilePath;
		cl_platform_id ChosenPlatform = nullptr;
		cl_program SingleProgram = nullptr;
		cl_context SingleContext = nullptr;
		unsigned int NumberOfDevices = 0;
		cl_device_id* ChosenDevices = nullptr;
		cl_PerDeviceValuesStruct** PerDeviceValueStruct = nullptr;								// Initalized and Constructed in  InitializeOpenCLProgram()
		unsigned int TotalNumberOfKernelFunctions = 0;											// Don't try to manually change this unless you know how to do it properly
		cl_KernelFunctionArgumentOrderListStruct** OrderedKernelArgumentList = nullptr;			// This Contains All the Kernel Functions information
		cl_MultiDevice_KernelFunctionStruct** MultiDevice_And_MultiKernel = nullptr;			// Initialization And Construction functions will take care of it	
	
	public:
		bool IsConstructionSuccesful = false;// Same as before, Manual changes = memory leaks, Automatic(constructor) Only changes will Obliterate the chances of possible memory leaks
		
		//Initialization
		void InitializeOpenCLProgram(bool &IsSuccesful, cl_uint PlatformToUse)//, cl_uint PlatformToUse = 1)
		{
			IsSuccesful = false;
			cl_uint NumOfPlatforms;						//the NO. of platforms
			//ChosenPlatform = nullptr;					////the chosen platform Will only be choosing one platform which will be the first one
			cl_uint	NumberOfGPUs = 0;					//cl_uint	NumberOfGPUorCPU = 0;// We will Only be using GPU here

			bool ReturnResult = true;
			cl_int ClErrorResult = clGetPlatformIDs(0, NULL, &NumOfPlatforms);//clGetPlatformIDs(1, &platform, NULL);// One line code with no checks chooses first platform GPU/CPU(APU if available) in this
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n ClError Code " << ClErrorResult << " : Getting platforms in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}

			ClErrorResult = clGetPlatformIDs(PlatformToUse, &ChosenPlatform, NULL);// for the first platform //Single Line code
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n ClError Code " << ClErrorResult << " : Platform Not set in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}

			ClErrorResult = clGetDeviceIDs(ChosenPlatform, CL_DEVICE_TYPE_GPU, 0, NULL, &NumberOfGPUs);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n ClError Code " << ClErrorResult << " : Getting GPUs in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}
			std::cout << "\n\nTotal Number Of GPUs Available: " << NumberOfGPUs << "\n";

			if (NumberOfGPUs < 1)
			{
				std::cout << "\n : Error NO GPUs Available in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}

			ChosenDevices = (cl_device_id*)malloc(NumberOfGPUs * sizeof(cl_device_id));
			if (ChosenDevices == nullptr)
			{
				std::cout << "\n Error Allocating : " << NumberOfGPUs * sizeof(cl_device_id) << " Byes Of Memory for ChosenDevices in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}

			ClErrorResult = clGetDeviceIDs(ChosenPlatform, CL_DEVICE_TYPE_GPU, NumberOfGPUs, ChosenDevices, NULL);// Same line for code for Multi-GPU
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n ClError Code " << ClErrorResult << " : GPUs Not set in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}

			SingleContext = clCreateContext(NULL, NumberOfGPUs, ChosenDevices, NULL, NULL, &ClErrorResult);
			if (ClErrorResult != CL_SUCCESS)
			{
				free(ChosenDevices);
				std::cout << "\n ClError Code " << ClErrorResult << " : Context Not Created in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}

			Essential::Malloc_PointerToArrayOfPointers((void***)&PerDeviceValueStruct, NumberOfGPUs, sizeof(cl_PerDeviceValuesStruct*), IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Allocating " << (NumberOfGPUs * sizeof(cl_PerDeviceValuesStruct*)) << " Byes Of Memory for PerDeviceValueStruct In InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";				
				free(ChosenDevices);
				ClErrorResult = clReleaseContext(SingleContext);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n CLError " << ClErrorResult << " : Releasing cl_context in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				}
				return;
			}
			else
			{
				for (int i = 0; i < NumberOfGPUs; i++)
				{
					//Create a command queue for every device
					PerDeviceValueStruct[i] = new cl_PerDeviceValuesStruct({ ChosenDevices[i], &SingleContext, ClErrorResult });
					if ((ClErrorResult != CL_SUCCESS) || (PerDeviceValueStruct[i] == nullptr))
					{
						std::cout << "\n Error Allocating " << sizeof(cl_PerDeviceValuesStruct) << " Byes Of Memory for PerDeviceValueStruct[" << i << "] In InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						for (int j = 0; j < i; i++)
						{
							delete(PerDeviceValueStruct[i]);
						}
						free(PerDeviceValueStruct);
						free(ChosenDevices);
						ClErrorResult = clReleaseContext(SingleContext);
						if (ClErrorResult != CL_SUCCESS)
						{
							std::cout << "\n CLError " << ClErrorResult << " : Releasing cl_context in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						}
						return;
					}
				}
			}
			

			//const char* ClSourceFilePath = "D:\\C++ Test Projects\\TestOpenCL4\\PunalManalanOpenCLKernelFunctions.cl";
			std::string ClSourceFileInString;
			Essential::GetFileContent(ClSourceFilePath, ClSourceFileInString, IsSuccesful);
			if (IsSuccesful)
			{
				const char* ClSourceFileInChar = ClSourceFileInString.c_str();
				size_t ClSourceFileInCharSize[] = { strlen(ClSourceFileInChar) };
				SingleProgram = clCreateProgramWithSource(SingleContext, 1, &ClSourceFileInChar, ClSourceFileInCharSize, &ClErrorResult);

				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n ClError Code " << ClErrorResult << " : Cl Program Not Created with Source!\n";
					for (int i = 0; i < NumberOfGPUs; i++)
					{
						delete(PerDeviceValueStruct[i]);
					}
					free(PerDeviceValueStruct);
					free(ChosenDevices);				
					ClErrorResult = clReleaseContext(SingleContext);
					if (ClErrorResult != CL_SUCCESS)
					{
						std::cout << "\n CLError " << ClErrorResult << " : Releasing cl_context in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					}
					return;
				}

				ClErrorResult = clBuildProgram(SingleProgram, 0, NULL, NULL, NULL, NULL);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n ClError Code " << ClErrorResult << " : Cl Program Not Build in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					for (int i = 0; i < NumberOfGPUs; i++)
					{
						delete(PerDeviceValueStruct[i]);
					}
					free(PerDeviceValueStruct);
					free(ChosenDevices);
					ClErrorResult = clReleaseContext(SingleContext);
					if (ClErrorResult != CL_SUCCESS)
					{
						std::cout << "\n CLError " << ClErrorResult << " : Releasing cl_context in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					}
					return;
				}
				NumberOfDevices = NumberOfGPUs;			
				IsSuccesful = true;
				return;
			}
			else
			{
				std::cout << "\n Error OpenCL Program File Was not opened,\n" << ClSourceFilePath << "\n in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
		}	

		void ReleaseAndFreeClData()
		{		
			cl_int ClErrorResult;
			ClErrorResult = clReleaseProgram(SingleProgram);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n CLError " << ClErrorResult << " : Releasing cl_program " << "in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
			ClErrorResult = clReleaseContext(SingleContext);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n CLError " << ClErrorResult << " : Releasing cl_context in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
			for (int i = 0; i < NumberOfDevices; i++)
			{
				delete(PerDeviceValueStruct[i]);
			}
			free(PerDeviceValueStruct);
			free(ChosenDevices);
		}

		void FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode(std::string ProgramKernelCode, cl_KernelFunctionArgumentOrderListStruct*** ArgOrderedKernelArgumentList, unsigned int& ArgTotalNumberOfKernelFunctions, bool& IsSuccesful)
		{
			IsSuccesful = false;		
		
			if (*ArgOrderedKernelArgumentList != nullptr)
			{
				std::cout << "\n Error The value of the variable 'ArgOrderedKernelArgumentList' Is not nullptr, It should be empty, In FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}
			else
			{
				//PENDING
			}

			IsSuccesful = true;
		}

		void FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode(std::string ProgramKernelCode, std::string FunctionName, cl_KernelFunctionArgumentOrderListStruct& ArgOrderedKernelArgumentList, bool& IsSuccesful)
		{
			IsSuccesful = false;

			//PENDING

			IsSuccesful = true;
		}

		void cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper(std::string ClSourceFilePath, cl_KernelFunctionArgumentOrderListStruct** ArgOrderedKernelArgumentList, unsigned int ArgTotalNumberOfKernelFunctions, bool &IsSuccesful)
		{
			IsSuccesful = false;
			Essential::Malloc_PointerToArrayOfPointers((void***)&MultiDevice_And_MultiKernel, TotalNumberOfKernelFunctions, sizeof(cl_MultiDevice_KernelFunctionStruct*), IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Allocating " << (TotalNumberOfKernelFunctions * sizeof(cl_MultiDevice_KernelFunctionStruct*)) << " Byes Of Memory for MultiDevice_And_MultiKernel In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}
			else
			{
				for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
				{
					MultiDevice_And_MultiKernel[i] = new cl_MultiDevice_KernelFunctionStruct(NumberOfDevices, OrderedKernelArgumentList[i], &SingleContext, PerDeviceValueStruct[i], &SingleProgram);

					if (MultiDevice_And_MultiKernel[i] != nullptr)
					{
						if (MultiDevice_And_MultiKernel[i]->IsConstructionSuccesful)
						{
							continue;
						}
						else
						{
							delete MultiDevice_And_MultiKernel[i];
						}
					}
					//else the code below will clean up
					std::cout << "\n Error Allocating " << (sizeof(cl_MultiDevice_KernelFunctionStruct)) << " Byes Of Memory for MultiDevice_And_MultiKernel[i] In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					IsConstructionSuccesful = false;

					for (int j = 0; j < i; ++j)
					{
						delete MultiDevice_And_MultiKernel[j];
					}
					free(MultiDevice_And_MultiKernel);
					return;
				}
				IsSuccesful = true;
			}
		}

		//Manual
		cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct(std::string ClSourceFilePath, cl_KernelFunctionsStruct* KernelFunctionsStruct, unsigned int ArgTotalNumberOfKernelFunctions, cl_PlatformVendorStruct* PlatformToUse, bool& IsSuccesful) : ClSourceFilePath(ClSourceFilePath)
		{
			std::cout << "\n Constructing cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct! Using Manual Constructor";

			ChosenPlatform = nullptr;
			SingleProgram = nullptr;
			SingleContext = nullptr;
			NumberOfDevices = 0;
			ChosenDevices = nullptr;
			PerDeviceValueStruct = nullptr;
			TotalNumberOfKernelFunctions = 0;
			OrderedKernelArgumentList = nullptr;
			MultiDevice_And_MultiKernel = nullptr;

			IsConstructionSuccesful = false;
			IsSuccesful = false;

			if ((KernelFunctionsStruct == nullptr) || (PlatformToUse == nullptr))
			{
				std::cout << "\n Error nullptr Passed as value for const variables In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				std::cout << "\n Error Construction Failed cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}

			cl_uint PlatformNumberToUse = 0;
			PlatformToUse->GetChosenPlatform(PlatformNumberToUse, IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error PlatformToUse->GetChosenPlatform returned false In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}

			// Single Program Is First Initialized
			InitializeOpenCLProgram(IsSuccesful, PlatformNumberToUse);

			if (IsSuccesful && KernelFunctionsStruct->IsConstructionSuccesful)
			{
				IsSuccesful = false;// reseting

				Essential::Malloc_PointerToArrayOfPointers((void***)&OrderedKernelArgumentList, TotalNumberOfKernelFunctions, sizeof(cl_KernelFunctionArgumentOrderListStruct*), IsSuccesful);
				if (!IsSuccesful)
				{
					ReleaseAndFreeClData();
					std::cout << "\n Error Allocating " << (TotalNumberOfKernelFunctions * sizeof(cl_KernelFunctionArgumentOrderListStruct*)) << " Byes Of Memory for OrderedKernelArgumentList In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					return;
				}
				else 
				{
					for (int i = 0; i < ArgTotalNumberOfKernelFunctions; ++i)
					{
						OrderedKernelArgumentList[i] = new cl_KernelFunctionArgumentOrderListStruct(KernelFunctionsStruct->OpenCL_KernelFunctions[i]);
						if (OrderedKernelArgumentList[i] == nullptr)
						{
							std::cout << "\n Error Allocating " << sizeof(cl_KernelFunctionArgumentOrderListStruct) << " Byes Of Memory for OrderedKernelArgumentList[" << i << "] In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
							ReleaseAndFreeClData();
							for (int j = 0; j < i; ++j)
							{
								delete OrderedKernelArgumentList[i];
							}
							free(OrderedKernelArgumentList);
							return;
						}
					}
					TotalNumberOfKernelFunctions = ArgTotalNumberOfKernelFunctions;

					cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper(ClSourceFilePath, OrderedKernelArgumentList, TotalNumberOfKernelFunctions, IsSuccesful);
					if (!IsSuccesful)
					{
						std::cout << "\n Error Construction Of cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						ReleaseAndFreeClData();
						for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
						{
							delete OrderedKernelArgumentList[i];
						}
						return;
					}
					IsConstructionSuccesful = true;
				}				
			}
		}

		//Automatic
		cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct(const std::string ClSourceFilePath, cl_PlatformVendorStruct* PlatformToUse, bool& IsSuccesful) : ClSourceFilePath(ClSourceFilePath)
		{
			std::cout << "\n Constructing cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct! Using Automatic Constructor";
			
			ChosenPlatform = nullptr;
			SingleProgram = nullptr;
			SingleContext = nullptr;
			NumberOfDevices = 0;
			ChosenDevices = nullptr;
			PerDeviceValueStruct = nullptr;
			TotalNumberOfKernelFunctions = 0;
			OrderedKernelArgumentList = nullptr;
			MultiDevice_And_MultiKernel = nullptr;

			IsConstructionSuccesful = false;
			IsSuccesful = false;

			if (PlatformToUse == nullptr)
			{
				std::cout << "\n Error nullptr Passed as value for const variables In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				std::cout << "\n Error Construction Failed cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}

			cl_uint PlatformNumberToUse = 0;
			PlatformToUse->GetChosenPlatform(PlatformNumberToUse, IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error PlatformToUse->GetChosenPlatform returned false In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}

			// Single Program Is First Initialized	
			InitializeOpenCLProgram(IsSuccesful, PlatformNumberToUse);

			if (IsSuccesful)
			{
				std::string ClSourceFileInString;
				Essential::GetFileContent(ClSourceFilePath, ClSourceFileInString, IsSuccesful);
				if(IsSuccesful)
				{				
					FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode(ClSourceFileInString, &OrderedKernelArgumentList, TotalNumberOfKernelFunctions, IsSuccesful);
					if (!IsSuccesful)
					{
						std::cout << "\n Error FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode Failed In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					}
					else
					{
						for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
						{
							if (OrderedKernelArgumentList[i]->IsConstructionSuccesful)
							{
								if (OrderedKernelArgumentList[i]->IsThisListUsable)
								{
									FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode(ClSourceFileInString, OrderedKernelArgumentList[i]->KernelFunctionName, *(OrderedKernelArgumentList[i]), IsSuccesful);
									//IsSuccesful = IsSuccesful;// If the IsSuccesful is false NOTE: the function above returns whether it was Succesful Or not
								}
								else
								{
									std::cout << "\n Error FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode Failed In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
									IsSuccesful = false;
								}
							}
							else
							{
								std::cout << "\n Error FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode Failed In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
								IsSuccesful = false;
							}
							if (!IsSuccesful)
							{
								ReleaseAndFreeClData();
								for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
								{
									delete OrderedKernelArgumentList[i];
								}
								return;
							}
						}
					}								
				}
				else
				{
					IsSuccesful = false;
					std::cout << "\n Error OpenCL Program File Was not opened,\n" << ClSourceFilePath << "\n In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					ReleaseAndFreeClData();
					return;
				}

				cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper(ClSourceFilePath, OrderedKernelArgumentList, TotalNumberOfKernelFunctions, IsSuccesful);
				if (!IsSuccesful)
				{				
					std::cout << "\n Error Construction Of cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					ReleaseAndFreeClData();
					for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
					{
						delete OrderedKernelArgumentList[i];
					}
					return;
				}
			}
			else
			{
				std::cout << "\n Error Initilization Of cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				ReleaseAndFreeClData();
				return;
			}
			IsConstructionSuccesful = true;
		}

		//If Either of DevicesToRunKernelFrom/To is set to -1 Then All the Devices(GPU) Will Run
		void RunKernelFunction(unsigned int KernelToRunNumber, int DevicesToRunKernelFrom, int DevicesToRunKernelTo, bool SplitWorkBetweenDevices, int TotalWorkItems, bool& IsSuccesful, bool ChooseOptimalTotalWorkItemsAndGroupSize = true, int TotalWorkGroups = 0)
		{
			IsSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling RunKernelFunction Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}
			else
			{
				//PENDING
			}
		}

		void RunKernelFunction(std::string NameOfTheKernelToRun, int DevicesToRunKernelFrom, int DevicesToRunKernelTo, bool SplitWorkBetweenDevices, int TotalWorkItems, bool& IsSuccesful, bool ChooseOptimalTotalWorkItemsAndGroupSize = true, int TotalWorkGroups = 0)
		{
			IsSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling RunKernelFunction Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}
			unsigned int KernelNumber = 0;
			GetKernelNumberByKernelName(KernelNumber, NameOfTheKernelToRun, IsSuccesful);
			if (IsSuccesful)
			{
				RunKernelFunction(KernelNumber, DevicesToRunKernelFrom, DevicesToRunKernelTo, SplitWorkBetweenDevices, TotalWorkItems, IsSuccesful, ChooseOptimalTotalWorkItemsAndGroupSize, TotalWorkGroups);
				return;
			}
			else
			{
				//std::cout << "\n OpenCL Kernel by the name " << NameOfTheKernelToRun << " Is Not Found In the Provided Program!\n";
				//std::cout << "\n Please Check for missed letters, symbols or numbers in the Function Name!\n";
			}	
			
		}
		
		unsigned int GetTotalNumberOfDevices()
		{
			if (IsConstructionSuccesful)
			{
				return NumberOfDevices;
			}
			else
			{
				std::cout << "\n Error Calling GetTotalNumberOfDevices Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
			return 0;
		}
		unsigned int GetTotalNumberOfKernelFunctions()
		{
			if (IsConstructionSuccesful)
			{
				return TotalNumberOfKernelFunctions;
			}
			else
			{
				std::cout << "\n Error Calling GetTotalNumberOfKernelFunctions Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
			return 0;
		}
		void GetBinaryInformationOfProgram(size_t& Binary_Size_Of_CLProgram, std::string& Binary_Of_CLProgram, bool& IsSuccesful)
		{
			IsSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling GetBinaryInformationOfProgram Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}

			cl_int ClErrorResult = 0;
			clGetProgramInfo(SingleProgram, CL_PROGRAM_BINARY_SIZES, sizeof(size_t), &Binary_Size_Of_CLProgram, NULL);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n ClError Code " << ClErrorResult << " : clGetProgramInfo to get Binary Size Failed in GetBinaryInformationOfProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}
			char* CharBinary_Of_CLProgram = (char*)malloc(sizeof(char) * Binary_Size_Of_CLProgram);
			if (CharBinary_Of_CLProgram == nullptr)
			{
				std::cout << "\n Error Allocating " << sizeof(char) << " Byes Of Memory for CharBinary_Of_CLProgram In GetBinaryInformationOfProgram In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}
			clGetProgramInfo(SingleProgram, CL_PROGRAM_BINARIES, Binary_Size_Of_CLProgram, &CharBinary_Of_CLProgram, NULL);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n ClError Code " << ClErrorResult << " : clGetProgramInfo to get Binary Information Failed in GetBinaryInformationOfProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				free(CharBinary_Of_CLProgram);
				return;
			}
			Binary_Of_CLProgram = CharBinary_Of_CLProgram;
			free(CharBinary_Of_CLProgram);
			IsSuccesful = true;
		}
		void GetKernelNumberByKernelName(unsigned int& KernelNumber, std::string NameOfTheKernel, bool& IsSuccesful)
		{
			IsSuccesful = false;
			if (IsConstructionSuccesful)
			{
				for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
				{
					if (OrderedKernelArgumentList[i]->KernelFunctionName == NameOfTheKernel)
					{
						IsSuccesful = true;
						KernelNumber = i;
						return;
					}
				}
				std::cout << "\n OpenCL Kernel by the name " << NameOfTheKernel << " Is Not Found In the Provided Program!\n";
				std::cout << "\n Please Check for missed letters, symbols or numbers in the Function Name!\n";
			}
			else
			{
				std::cout << "\n Error Calling GetKernelNumberByKernelName Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
		}
		cl_KernelFunctionArgumentOrderListStruct GetKernelInformation(unsigned int KernelNumber, bool& IsSuccesful)
		{
			IsSuccesful = false;
			if (IsConstructionSuccesful)
			{
				if (KernelNumber < TotalNumberOfKernelFunctions)
				{
					return *(OrderedKernelArgumentList[KernelNumber]);
					IsSuccesful = true;
				}
				else
				{
					std::cout << "\n Error KernelNumber Exceeds the Total Number Of Kernel Functions Present! in GetKernelInformation In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				}
			}
			else
			{
				std::cout << "\n Error Calling GetKernelInformation Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			}
			return cl_KernelFunctionArgumentOrderListStruct(0, "cl_Invalid");
		}
		cl_KernelFunctionArgumentOrderListStruct GetKernelInformation(std::string NameOfTheKernel, bool& IsSuccesful)
		{
			IsSuccesful = false;
			unsigned int KernelNumber = 0;
			GetKernelNumberByKernelName(KernelNumber, NameOfTheKernel, IsSuccesful);
			if (IsSuccesful)
			{
				GetKernelInformation(KernelNumber, IsSuccesful);
			}
		}

		~cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct()
		{
			std::cout << "\n Destructing cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			if (IsConstructionSuccesful)
			{
				IsConstructionSuccesful = false;
				ReleaseAndFreeClData();
				for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
				{
					delete OrderedKernelArgumentList[i];
				}
				for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
				{
					delete MultiDevice_And_MultiKernel[i];
				}
				free(MultiDevice_And_MultiKernel);
			}
			IsConstructionSuccesful = false;
		}
	};

	// NOTE: This Is the shortend Name, Does not matter if you use the struct name directly
	typedef cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct OpenCLWrapper;	
};

int main()
{
	//Testing
	bool IsSuccesful = false;
	OCLW_P::cl_KernelFunctionsStruct Functions_List(1, IsSuccesful);
	if (!IsSuccesful)
	{
		std::cout << "\n Error: Variable construction Unsuccesful";
		return 1;
	}

	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Add_Integers", 3, IsSuccesful);
	if (!IsSuccesful)
	{
		std::cout << "\n Error: Function Unsuccesful";
		return 1;
	}
		
	//NOTE: Checking for failures is a good practice!
	Functions_List.SetMemoryTypeOfArugment(0, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, IsSuccesful);
	/*if (!IsSuccesful)
	{
		std::cout << "\n Error: Function Unsuccesful";
		return 1;
	}*/
	Functions_List.SetMemoryTypeOfArugment(0, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, IsSuccesful);
	/*if (!IsSuccesful)
	{
		std::cout << "\n Error: Function Unsuccesful";
		return 1;
	}*/
	Functions_List.SetMemoryTypeOfArugment(0, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, IsSuccesful);
	/*if (!IsSuccesful)
	{
		std::cout << "\n Error: Function Unsuccesful";
		return 1;
	}*/

	OCLW_P::cl_PlatformVendorStruct AvailablePlatformVendors(IsSuccesful);
	if (!IsSuccesful)
	{
		std::cout << "\n Error: Variable construction Unsuccesful";
		return 1;
	}

	int TotalNumberOfPlatforms;
	AvailablePlatformVendors.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, IsSuccesful);
	/*if (!IsSuccesful)
	{
		std::cout << "\n Error: Function Unsuccesful";
		return 1;
	}*/	
	AvailablePlatformVendors.PrintAllAvailablePlatformVendorNames(IsSuccesful);
	/*if (!IsSuccesful)
	{
		std::cout << "\n Error: Function Unsuccesful";
		return 1;
	}*/
	AvailablePlatformVendors.SetChosenPlatform(1, IsSuccesful);
	/*if (!IsSuccesful)
	{
		std::cout << "\n Error: Function Unsuccesful";
		return 1;
	}*/

	std::string FilePath = "D:\\C++ Projects\\Opencl Punal Wrapper\\OpenCL Wrapper by Punal\\PunalOpenclFunctionsProgram.cl";
	OCLW_P::OpenCLWrapper EntireOpenCLProgram(FilePath, &Functions_List, 1, &AvailablePlatformVendors, IsSuccesful);

	//std::cout << "\n Before Destruction";
	//EntireOpenCLProgram.~cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct();// Doing this just to show that the destructor is working properly
	//std::cout << "\n After  Destruction";

	return 0;
}