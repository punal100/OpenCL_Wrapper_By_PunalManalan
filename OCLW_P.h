/********************************************************************************************************************************
OpenCL Wrapper By Punal Manalan
Use this code wherever and whenever you want to!

Information About This Wrapper:
1. LightWeight, Easy to use **Header-Only** Wrapper for OpenCL.
2. 'OCLW_P::OpenCLWrapper' This Single line of code does Everything(Including Adding Functions) Automaticaly.
3. Written Using only "CL/cl.h" and has backward compatblity upto OpenCL 1.2!
3. No need to maually Add kernel function or figure out argument types.
4. The Entire OpenCL Program File is Read and automaticaly Assignes Kernel Functions Ready for use!
5. Information about each kernel function and it's argument types can be Accesed with **getter**functions().
6. Compiles and Runs on Windows, Linux and Mac.
7. This Wrapper specializes in Heavy Computation on GPU.
8. From Constrction to Destruction everything is Done automaticaly!
9. No need for manual destruction and releasing cl resources!
10. No need to write Tedious amount of code to counter memory leaks!
11. Upon going out of scope or exiting the program Every resource that is used is safely Released and Deleted.
*********************************************************************************************************************************/

#ifndef OPENCL_WRAPPER_BY_PUNAL
#define OPENCL_WRAPPER_BY_PUNAL
#pragma once

#include <iostream>
#include <fstream>// For file reading

#define __CL_ENABLE_EXCEPTIONS
#define CL_API_SUFFIX__VERSION_1_2
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
//#define CL_TARGET_OPENCL_VERSION 120
#include <CL/cl.h>// OpenCl

#ifndef ESSENTIAL_FUNCTIONS_BY_PUNAL
#define ESSENTIAL_FUNCTIONS_BY_PUNAL
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
				Essenbp::Malloc_PointerToArrayOfPointers((void***)&AllAvailablePlatformVendorNames, TotalNumberOfPlatformVendors, sizeof(std::string*), IsSuccesful);
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

		//NOTE: ChosenPlatform is always greater than 1 and less than Number Of Platform + 1
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
				std::cout << "\n ClError Code " << ClErrorResult << " : cl_CommandQueue Creation In: ccl_PerDeviceValuesStruct!\n";
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
	struct cl_NDRangeStruct//Maximum Dimensions is 3
	{
	private:
		cl_uint Dimensions = 0;//By default it is 1
		size_t GlobalWorkSize[3];	// Total Number Of Work Items
		size_t LocalWorkSize[3];	// Work Items per Work Group
		size_t WorkSizeOffset[3];	// WorkSize Offset

	public:
		bool IsConstructionSuccesful = false;
		cl_NDRangeStruct()
		{
			Dimensions = 0;//By default it is 1
			GlobalWorkSize[0] = 0;
			GlobalWorkSize[1] = 0;
			GlobalWorkSize[2] = 0;

			LocalWorkSize[0] = 0;
			LocalWorkSize[1] = 0;
			LocalWorkSize[2] = 0;

			WorkSizeOffset[0] = 0;
			WorkSizeOffset[1] = 0;
			WorkSizeOffset[2] = 0;

			IsConstructionSuccesful = true;
		}

		// Deprecated NOTE: No purpose
		/*
		//NOTE: This sets everything to 0 which means enqueuendrangekernel should be passed with null for both global and local work sizes
		void SetNDRange(cl_uint ArgDimensions)
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				return;
			}

			Dimensions = ArgDimensions;//By default it is 1
			GlobalWorkSize[0] = 0;
			GlobalWorkSize[1] = 0;
			GlobalWorkSize[2] = 0;

			LocalWorkSize[0] = 0;
			LocalWorkSize[1] = 0;
			LocalWorkSize[2] = 0;

			WorkSizeOffset[0] = 0;
			WorkSizeOffset[1] = 0;
			WorkSizeOffset[2] = 0;
		}*/

		//NOTE: 1 Dimension
		void SetNDRange(size_t GlobalSize1D, size_t LocalSize1D, size_t WorkSizeOffset1D)
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				return;
			}

			Dimensions = 1;//By default it is 1
			GlobalWorkSize[0] = GlobalSize1D;
			GlobalWorkSize[1] = 0;
			GlobalWorkSize[2] = 0;

			LocalWorkSize[0] = LocalSize1D;
			LocalWorkSize[1] = 0;
			LocalWorkSize[2] = 0;

			WorkSizeOffset[0] = WorkSizeOffset1D;
			WorkSizeOffset[1] = 0;
			WorkSizeOffset[2] = 0;
		}
		
		//NOTE: 2 Dimension
		void SetNDRange(size_t GlobalSize1D, size_t LocalSize1D, size_t WorkSizeOffset1D, size_t GlobalSize2D, size_t LocalSize2D, size_t WorkSizeOffset2D)
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				return;
			}

			Dimensions = 2;//By default it is 1
			GlobalWorkSize[0] = GlobalSize1D;
			GlobalWorkSize[1] = GlobalSize2D;
			GlobalWorkSize[2] = 0;

			LocalWorkSize[0] = LocalSize1D;
			LocalWorkSize[1] = LocalSize2D;
			LocalWorkSize[2] = 0;

			WorkSizeOffset[0] = WorkSizeOffset1D;
			WorkSizeOffset[1] = WorkSizeOffset2D;
			WorkSizeOffset[2] = 0;
		}
		
		//NOTE: 3 Dimension
		void SetNDRange(size_t GlobalSize1D, size_t LocalSize1D, size_t WorkSizeOffset1D, size_t GlobalSize2D, size_t LocalSize2D, size_t WorkSizeOffset2D, size_t GlobalSize3D, size_t LocalSize3D, size_t WorkSizeOffset3D)
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				return;
			}

			Dimensions = 3;//By default it is 1
			GlobalWorkSize[0] = GlobalSize1D;
			GlobalWorkSize[1] = GlobalSize2D;
			GlobalWorkSize[2] = GlobalSize3D;

			LocalWorkSize[0] =  LocalSize1D;
			LocalWorkSize[1] =  LocalSize2D;
			LocalWorkSize[2] =  LocalSize3D;

			WorkSizeOffset[0] = WorkSizeOffset1D;
			WorkSizeOffset[1] = WorkSizeOffset2D;
			WorkSizeOffset[2] = WorkSizeOffset3D;
		}

		//NOTE: 1 Dimension With Offset 0
		void SetNDRangeWithoutLocal(size_t GlobalSize1D, size_t WorkSizeOffset1D)
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				return;
			}

			Dimensions = 1;//By default it is 1
			GlobalWorkSize[0] = GlobalSize1D;
			GlobalWorkSize[1] = 0;
			GlobalWorkSize[2] = 0;

			LocalWorkSize[0] = 0;
			LocalWorkSize[1] = 0;
			LocalWorkSize[2] = 0;

			WorkSizeOffset[0] = WorkSizeOffset1D;
			WorkSizeOffset[1] = 0;
			WorkSizeOffset[2] = 0;
		}

		//NOTE: 2 Dimension With Offset 0
		void SetNDRangeWithoutLocal(size_t GlobalSize1D, size_t WorkSizeOffset1D, size_t GlobalSize2D, size_t WorkSizeOffset2D)
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				return;
			}

			Dimensions = 2;//By default it is 1
			GlobalWorkSize[0] = GlobalSize1D;
			GlobalWorkSize[1] = GlobalSize2D;
			GlobalWorkSize[2] = 0;

			LocalWorkSize[0] = 0;
			LocalWorkSize[1] = 0;
			LocalWorkSize[2] = 0;

			WorkSizeOffset[0] = WorkSizeOffset1D;
			WorkSizeOffset[1] = WorkSizeOffset2D;
			WorkSizeOffset[2] = 0;
		}	

		//NOTE: 3 Dimension With Offset 0
		void SetNDRangeWithoutLocal(size_t GlobalSize1D, size_t WorkSizeOffset1D, size_t GlobalSize2D, size_t WorkSizeOffset2D, size_t GlobalSize3D, size_t WorkSizeOffset3D)
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				return;
			}

			Dimensions = 3;//By default it is 1
			GlobalWorkSize[0] = GlobalSize1D;
			GlobalWorkSize[1] = GlobalSize2D;
			GlobalWorkSize[2] = GlobalSize3D;

			LocalWorkSize[0] = 0;
			LocalWorkSize[1] = 0;
			LocalWorkSize[2] = 0;
			
			WorkSizeOffset[0] = WorkSizeOffset1D;
			WorkSizeOffset[1] = WorkSizeOffset2D;
			WorkSizeOffset[2] = WorkSizeOffset3D;
		}

		void SetNDRange(cl_NDRangeStruct* ArgNDRange, bool& IsSuccesful)
		{
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				return;
			}

			cl_uint ArgDimensions = 0;//By default it is 1
			size_t* ArgGlobalSize;	// Total Number Of Work Items
			size_t* ArgLocalSize;	// Work Items per Work Group
			size_t* ArgWorkSizeOffset;	// WorkSize Offset

			GetNDRange(&ArgDimensions, &ArgGlobalSize, &ArgLocalSize, &ArgWorkSizeOffset, IsSuccesful);
			Dimensions = ArgDimensions;//By default it is 1
			GlobalWorkSize[0] = ArgGlobalSize[0];
			GlobalWorkSize[1] = ArgGlobalSize[1];
			GlobalWorkSize[2] = ArgGlobalSize[2];

			LocalWorkSize[0] = ArgLocalSize[0];
			LocalWorkSize[1] = ArgLocalSize[1];
			LocalWorkSize[2] = ArgLocalSize[2];

			WorkSizeOffset[0] = ArgWorkSizeOffset[0];
			WorkSizeOffset[1] = ArgWorkSizeOffset[1];
			WorkSizeOffset[2] = ArgWorkSizeOffset[2];
		}

		void GetNDRange(cl_uint* ArgDimensions, size_t** ArgGlobalSize, size_t** ArgLocalSize, size_t** ArgWorkSizeOffset, bool& IsSuccesful)
		{
			IsSuccesful = false;

			*ArgDimensions = 0;
			*ArgGlobalSize = nullptr;
			*ArgLocalSize = nullptr;
			*ArgWorkSizeOffset = nullptr;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling GetNDRange Without Constructing the struct In: cl_NDRangeStruct!\n";
				return;
			}
			if (Dimensions == 0)
			{
				std::cout << "\n Error Calling GetNDRange Without SettingNDRange First using SetNDRange() In: cl_NDRangeStruct!\n";
				return;
			}

			// Temporarly using this variable for different purpose
			IsSuccesful =  (LocalWorkSize[0] == 0) || (LocalWorkSize[1] == 0) || (LocalWorkSize[2] == 0);

			if (IsSuccesful)
			{
				*ArgDimensions = Dimensions;
				*ArgGlobalSize = nullptr;
				*ArgLocalSize = nullptr;
				*ArgWorkSizeOffset = nullptr;
			}
			else
			{
				*ArgDimensions = Dimensions;
				*ArgGlobalSize = GlobalWorkSize;
				*ArgLocalSize = LocalWorkSize;
				*ArgWorkSizeOffset = WorkSizeOffset;
			}
			
			IsSuccesful = true;
		}		

		~cl_NDRangeStruct()
		{
			std::cout << "\n Destructing cl_NDRangeStruct!";			
			IsConstructionSuccesful = false;
		}
	};

	// NOTE: Call Constructor Before Using
	struct cl_MultiDevice_NDRangeStruct
	{
	private:
		const unsigned int NumberOfDevices;
		cl_NDRangeStruct** MultiNDRange = nullptr; //PENDING

	public:
		bool IsConstructionSuccesful = false;
		cl_MultiDevice_NDRangeStruct(unsigned int ArgTotalNumberOfDevices, bool& IsSuccesful) : NumberOfDevices(ArgTotalNumberOfDevices)
		{
			IsConstructionSuccesful = false;
			IsSuccesful = false;

			if (NumberOfDevices == 0)
			{
				std::cout << "\n Error : ArgTotalNumberOfDevices is 0, should be atleast In: cl_MultiDevice_NDRangeStruct!\n";
			}

			Essenbp::Malloc_PointerToArrayOfPointers((void***)MultiNDRange, NumberOfDevices, sizeof(cl_NDRangeStruct*), IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Allocating :" << NumberOfDevices * sizeof(cl_NDRangeStruct*) << " Byes Of Memory for MultiNDRange In: cl_MultiDevice_NDRangeStruct!\n";
				return;
			}
			else
			{
				for (int i = 0; i < NumberOfDevices; ++i)
				{
					MultiNDRange[i] = new cl_NDRangeStruct();
					if (MultiNDRange[i] == nullptr)
					{
						std::cout << "\n Error Allocating :" << NumberOfDevices * sizeof(cl_NDRangeStruct) << " Byes Of Memory for MultiNDRange["<<i<<"] In: cl_MultiDevice_NDRangeStruct!\n";
						for (int j = 0; j < i; ++j)
						{
							delete MultiNDRange[j];
						}
						free(MultiNDRange);
						std::cout << "\n Error Construction Failed cl_MultiDevice_NDRangeStruct!";
						return;
					}
				}
			}
			IsConstructionSuccesful = true;
		}

		unsigned int GetNumberOfDevices() { return NumberOfDevices; }
		cl_NDRangeStruct* GetNDRangeOfDevice(unsigned int DeviceNumber, bool& IsSuccesful) 
		{
			IsSuccesful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling GetNDRangeOfDevice Without Constructing the struct In: cl_MultiDevice_NDRangeStruct!\n";
			}
			else
			{
				if (DeviceNumber < NumberOfDevices)
				{
					std::cout << "\n Error Argument Number + 1' " << DeviceNumber + 1 << "' Is greater than the NumberOfDevices in GetNDRangeOfDevice In: cl_NDRangeStruct!\n";
				}
				else
				{
					IsSuccesful = true;
					return MultiNDRange[DeviceNumber];
				}
				return nullptr;
			}
		}

		void SetNDRangeOfDevice(unsigned int DeviceNumber, bool& IsSuccesful)
		{
			IsSuccesful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNDRangeOfDevice Without Constructing the struct In: cl_MultiDevice_NDRangeStruct!\n";
			}
			else
			{
				if (DeviceNumber < NumberOfDevices)
				{
					std::cout << "\n Error Argument Number + 1' " << DeviceNumber + 1 << "' Is greater than the NumberOfDevices in SetNDRangeOfDevice In: cl_NDRangeStruct!\n";
				}
				else
				{
					IsSuccesful = true;
					return MultiNDRange[DeviceNumber];
				}
				return nullptr;
			}
		}

		~cl_MultiDevice_NDRangeStruct()
		{
			std::cout << "\n Destructing cl_MultiDevice_NDRangeStruct!";
			if (IsConstructionSuccesful)
			{
				for (int i = 0; i < NumberOfDevices; ++i)
				{
					delete MultiNDRange[i];
				}
				free(MultiNDRange);
				IsConstructionSuccesful = false;
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

			Essenbp::Malloc_PointerToArrayOfPointers((void***)&KernelArgumentsInOrder, TotalNumberOfArugments, sizeof(cl_Memory_Type*), IsSuccesful);
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
							delete KernelArgumentsInOrder[j];
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
				std::cout << "\n Error The OrderedListOfArugments Is Not Constructed, So Is Unusable In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
				std::cout << "\n Error Construction Failed cl_KernelFunctionArgumentOrderListStruct!";
				return;
			}
			else
			{
				if (!CopyStruct->IsThisListUsable)
				{
					std::cout << "\n Error The OrderedListOfArugments Is Not Properly Set, So Is Unusable In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
					std::cout << "\n Error Construction Failed cl_KernelFunctionArgumentOrderListStruct!";
					return;
				}
			}

			Essenbp::Malloc_PointerToArrayOfPointers((void***)&KernelArgumentsInOrder, TotalNumberOfArugments, sizeof(cl_Memory_Type*), IsSuccesful);
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
							delete KernelArgumentsInOrder[j];
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

			Essenbp::Malloc_PointerToArrayOfPointers((void***)&KernelArgumentsInOrder, TotalNumberOfArugments, sizeof(cl_Memory_Type*), IsSuccesful);
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
							delete KernelArgumentsInOrder[j];
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
				std::cout << "\n Error Argument Number + 1' " << ArgumentNumber + 1<< "' Is greater than the TotalNumberOfArguments In SetMemoryTypeOfArugment In: cl_KernelFunctionArgumentOrderListStruct!\n";
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
					delete KernelArgumentsInOrder[i];
				}
				free(KernelArgumentsInOrder);
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
						if (clMemory_Type_Of_Argument != cl_Memory_Type::CL_LOCALENUM)
						{
							cl_uint ClErrorResult = clReleaseMemObject(GlobalMemoryInDevice);// releasing Memory object every time this function is called	
							if (ClErrorResult != CL_SUCCESS)
							{
								std::cout << "\n ClError Code " << ClErrorResult << " : Releasing Memory On device In: cl_MemoryStruct!\n";
								return;
							}
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
				if (IsFunctionSuccesful)
				{
					IsFunctionSuccesful = false;
				}
				else
				{
					std::cout << "\n Error FreeBuffer failed in BufferCreation In: cl_MemoryStruct!\n";
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
					DoesBufferAlreadyExist = true;
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
					DoesBufferAlreadyExist = true;
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
					DoesBufferAlreadyExist = true;
					return;
					//break;// Not needed as return is done
				}
			}
		}

	public:
		bool IsConstructionSuccesful = false;

		cl_MemoryStruct(const cl_Memory_Type ArgclMemory_Type_Of_Argument, const cl_context* Argcl_ContextForThisArgument, const cl_command_queue* Argcl_CommandQueueForThisArgument, const cl_kernel* ArgTheKernel, const cl_uint ArgKernelArgumentNumber, bool& IsSuccesful) : clMemory_Type_Of_Argument(ArgclMemory_Type_Of_Argument), cl_ContextForThisArgument(Argcl_ContextForThisArgument), cl_CommandQueueForThisArgument(Argcl_CommandQueueForThisArgument), TheKernel(ArgTheKernel), KernelArgumentNumber(ArgKernelArgumentNumber)
		{
			std::cout << "\n Constructing cl_MemoryStruct!";

			DoesBufferAlreadyExist = false;
			GlobalMemoryInDevice = NULL;
			COPY_OF_PrivateMemoryType = nullptr;
			MemoryInDeviceTotalSizeInBytes = 0;
			MemoryInDevice_Occupied_SizeInBytes = 0;

			IsConstructionSuccesful = false;
			IsSuccesful = true;

			if ((TheKernel == nullptr) || (cl_ContextForThisArgument == nullptr) || (cl_CommandQueueForThisArgument == nullptr))
			{
				std::cout << "\n Error nullptr Passed as value for const variables In: cl_Memory_Struct!";
				std::cout << "\n Error Construction Failed cl_MemoryStruct!";
				IsSuccesful = false;
				return;
			}

			BufferCreation(1, DoesBufferAlreadyExist);// No Need to use Is Succesful Here
			if (!DoesBufferAlreadyExist)
			{
				std::cout << "\n Error Construction Failed cl_MemoryStruct!";
				IsSuccesful = false;
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
					if (BUFFER_CREATION_ONLY_SizeOfBuffer > 0)
					{
						std::cout << "\n Error :Trying To OverWrite Memory but the 'BUFFER_CREATION_ONLY_SizeOfBuffer' argument is not set to equal to 0" << ": OverWriting Buffer In: cl_MemoryStruct!\n";
						return;
					}
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
										free(TempDataCarryHelper);// Free the Data

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
						}//Local can get pass through here but no code is required for it...
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

		void AllocateMemoryAndPassToKernel(void* PointerToMemoryToCopyFrom, size_t SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, size_t BUFFER_CREATION_ONLY_SizeOfBuffer, bool OverWriteMemory, bool UsePreviouslyAllocatedMemoryOnBuffer, bool& IsFunctionSuccesful)
		{
			IsFunctionSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling AllocateMemoryAndPassToKernel Without Constructing the struct In: cl_MemoryStruct!\n";
				return;
			}

			if (UsePreviouslyAllocatedMemoryOnBuffer)
			{
				if (!DoesBufferAlreadyExist)
				{
					std::cout << "\n Error :Kernel Argument Number of " << KernelArgumentNumber << " Does not have buffer created. in AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
					IsFunctionSuccesful = false;
				}
				if (PointerToMemoryToCopyFrom != nullptr)
				{
					std::cout << "\n Error :UsePreviouslyAllocatedMemoryOnBuffer is set to true but PointerToMemoryToCopyFrom is not nullptr in AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
					IsFunctionSuccesful = false;
				}
				if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type != 0)
				{
					std::cout << "\n Error :UsePreviouslyAllocatedMemoryOnBuffer is set to true but SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type is set not to 0 in AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
					IsFunctionSuccesful = false;
				}
				if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type != 0)
				{
					std::cout << "\n Error :UsePreviouslyAllocatedMemoryOnBuffer is set to true but BUFFER_CREATION_ONLY_SizeOfBuffer is set not to 0 in AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
					IsFunctionSuccesful = false;
				}
				//Continue to PassBufferToKernel
			}
			else
			{
				MemoryAllocationOnDevice(PointerToMemoryToCopyFrom, SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, BUFFER_CREATION_ONLY_SizeOfBuffer, OverWriteMemory, IsFunctionSuccesful);
				if (!IsFunctionSuccesful)
				{
					std::cout << "\n Error :Memory Allocation On Device was Unsuccesful In AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
				}
			}
			if (IsFunctionSuccesful)
			{
				PassBufferToKernel(IsFunctionSuccesful);
				if (!IsFunctionSuccesful)
				{
					std::cout << "\n Error :Sending buffer to kernel failed In AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
				}
			}			
		}

		bool GetDoesBufferAlreadyExist() { return DoesBufferAlreadyExist; }

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
		//bool IsDataSendCompleted = false;
		//bool TrueForCreateFalseForOverWrite = true;//When True Creates The Buffer
		//void* COPY_OF_DataFromHost = nullptr;//For Global and Private Only , Pass NULL for Local...
		//size_t DataFromHostSize = 0;//For Global ONLY, Reads Data When IsReadMemory is true and IsWriteMemory is true or false And IsLocalMemory is false, Memory Can be Written to the buffer
		//size_t CL_MemorySizeToCreate = 0;//For Local And Global //NOTE: NO NEED TO Allocate memory for local from host side... memory allocation from host is only required for global			
		cl_MemoryStruct* BufferOnDevice = nullptr;//Example: Buffer on GPU device	

	public:
		bool IsConstructionSuccesful = false;// NOTE: Do not change this manualy! unless you know what you are doing

		//Contructor
		cl_KernelSingleArgumentStruct(const cl_Memory_Type ArgclMemory_Type_Of_Argument, const cl_context* Argcl_ContextForThisArgument, const cl_command_queue* Argcl_CommandQueueForThisArgument, const cl_kernel* TheKernel, const cl_uint KernelArgumentNumber, bool& IsSuccesful)
		{
			std::cout << "\n Constructing cl_KernelSingleArgumentStruct!";

			BufferOnDevice = nullptr;

			IsConstructionSuccesful = false;
			IsSuccesful = false;

			BufferOnDevice = new cl_MemoryStruct(ArgclMemory_Type_Of_Argument, Argcl_ContextForThisArgument, Argcl_CommandQueueForThisArgument, TheKernel, KernelArgumentNumber, IsSuccesful);
			if (BufferOnDevice == nullptr)
			{
				std::cout << "\n Error Allocating :" << sizeof(cl_MemoryStruct) << " Byes Of Memory for BufferOnDevice In cl_KernelSingleArgumentStruct!\n";
				return;
			}
			if (!IsSuccesful)
			{
				std::cout << "\n Error Construction Failed cl_PerDeviceValuesStruct!";
				delete BufferOnDevice;
				return;
			}
			
			IsConstructionSuccesful = true;
			IsSuccesful = true;
		}
				
		void AllocateMemoryAndPassToKernel(void* PointerToMemoryToCopyFrom, size_t SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, size_t BUFFER_CREATION_ONLY_SizeOfBuffer, bool OverWriteMemory, bool UsePreviouslyAllocatedMemoryOnBuffer, bool& IsFunctionSuccesful)
		{
			IsFunctionSuccesful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling AllocateMemoryAndPassToKernel Without Constructing the struct In: cl_KernelSingleArgumentStruct!\n";
				return;
			}
			
			BufferOnDevice->AllocateMemoryAndPassToKernel(PointerToMemoryToCopyFrom, SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, BUFFER_CREATION_ONLY_SizeOfBuffer, OverWriteMemory, UsePreviouslyAllocatedMemoryOnBuffer, IsFunctionSuccesful);
			if (!IsFunctionSuccesful)
			{
				std::cout << "\n Error :BufferOnDevice->AllocateMemoryAndPassToKernel Failed in AllocateMemoryAndPassToKernel In: cl_KernelSingleArgumentStruct!\n";
				return;
			}
		}

		bool IsBufferCreatedOnDevice() { return BufferOnDevice->GetDoesBufferAlreadyExist(); }

		//Destructor
		~cl_KernelSingleArgumentStruct()
		{
			std::cout << "\n Destructing cl_KernelSingleArgumentStruct!";
			if (IsConstructionSuccesful)
			{
				delete BufferOnDevice;
				IsConstructionSuccesful = false;
			}
		}
	};

	// NOTE: Call Constructor Before Using
	struct cl_KernelMultipleArgumentStruct
	{
	private:
		cl_KernelSingleArgumentStruct** SingleKernelFunctionMultiArgumentsArray = nullptr;// Arguments stored here
		const cl_KernelFunctionArgumentOrderListStruct* OrderedListOfArugments;
		const cl_command_queue* cl_CommandQueueForThisKernel;
		cl_kernel ThisKernel = NULL;

		void CreateKernelSingleArgumentStruct(const cl_context* cl_ContextForThisKernel, const cl_command_queue* cl_CommandQueueForThisKernel, const cl_kernel* TheKernel, bool& IsSuccesful)
		{
			IsSuccesful = false;

			if (OrderedListOfArugments->IsConstructionSuccesful)
			{
				if (OrderedListOfArugments->IsThisListUsable)
				{
					for (int i = 0; i < OrderedListOfArugments->TotalNumberOfArugments; ++i)
					{
						SingleKernelFunctionMultiArgumentsArray[i] = new cl_KernelSingleArgumentStruct(*(OrderedListOfArugments->KernelArgumentsInOrder[i]), cl_ContextForThisKernel, cl_CommandQueueForThisKernel, TheKernel, i, IsSuccesful);
						if (SingleKernelFunctionMultiArgumentsArray[i] != nullptr)
						{
							if (IsSuccesful == false)// ! works great but, '== false' make it readable
							{
								std::cout << "\n Error Construction Failed: Of SingleKernelFunctionMultiArgumentsArray[" << i << "] of type CreateKernelSingleArgumentStruct in CreateKernelSingleArgumentStruct In: cl_KernelMultipleArgumentStruct!\n";
								for (int j = 0; j <= i; ++j)// Greater than or equal to i
								{
									delete SingleKernelFunctionMultiArgumentsArray[j];
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
								delete SingleKernelFunctionMultiArgumentsArray[j];
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
		bool IsConstructionSuccesful = false;// NOTE: Memory Leaks bad, so do not change this manualy...
		cl_KernelMultipleArgumentStruct(const cl_KernelFunctionArgumentOrderListStruct* ArgOrderedListOfArugments,
			const cl_context* cl_ContextForThisKernel, const cl_command_queue* Argcl_CommandQueueForThisKernel, cl_program* BuiltClProgramContainingTheSpecifiedFunctions, bool& IsSuccesful) : OrderedListOfArugments(ArgOrderedListOfArugments), cl_CommandQueueForThisKernel(Argcl_CommandQueueForThisKernel)
		{
			std::cout << "\n Constructing cl_KernelMultipleArgumentStruct!";

			SingleKernelFunctionMultiArgumentsArray = nullptr;

			IsSuccesful = true;
			IsConstructionSuccesful = false;// Yes this is set to false

			if ((ArgOrderedListOfArugments == nullptr) || (cl_ContextForThisKernel == nullptr) || (cl_CommandQueueForThisKernel == nullptr) || (BuiltClProgramContainingTheSpecifiedFunctions == nullptr))
			{
				std::cout << "\n Error nullptr Passed as value for const variables In: cl_KernelMultipleArgumentStruct!";
				std::cout << "\n Error Construction Failed cl_KernelMultipleArgumentStruct!";
				IsSuccesful = false;
				return;
			}

			if (OrderedListOfArugments->IsConstructionSuccesful)
			{
				if (OrderedListOfArugments->IsThisListUsable)
				{
					Essenbp::Malloc_PointerToArrayOfPointers((void***)&SingleKernelFunctionMultiArgumentsArray, OrderedListOfArugments->TotalNumberOfArugments, sizeof(cl_KernelSingleArgumentStruct*), IsSuccesful);
					if (!IsSuccesful)
					{
						std::cout << "\n Error Allocating " << (OrderedListOfArugments->TotalNumberOfArugments * sizeof(cl_KernelSingleArgumentStruct*)) << " Byes Of Memory for SingleKernelFunctionMultiArgumentsArray In cl_KernelMultipleArgumentStruct!\n";
						IsSuccesful = false;
					}
				}
				else
				{
					std::cout << "\n Error The OrderedListOfArugments Is Not Properly Set, So Is Unusable In: cl_KernelMultipleArgumentStruct!\n";
					IsSuccesful = false;
				}
			}
			else
			{
				std::cout << "\n Error The OrderedListOfArugments Is Not Constructed, So Is Unusable In: cl_KernelMultipleArgumentStruct!\n";
				IsSuccesful = false;
			}

			if (!IsSuccesful)
			{
				std::cout << "\n Error Construction Failed cl_KernelMultipleArgumentStruct!";
				return;
			}
			else
			{
				IsSuccesful = true;

				cl_int ClErrorResult = 0;
				ThisKernel = clCreateKernel(*BuiltClProgramContainingTheSpecifiedFunctions, OrderedListOfArugments->KernelFunctionName.c_str(), &ClErrorResult);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n CLError " << ClErrorResult << " : Kernel Creation Failed in cl_KernelMultipleArgumentStruct!\n";
					std::cout << "\n Error Construction Failed cl_KernelMultipleArgumentStruct!";
					return;
				}

				CreateKernelSingleArgumentStruct(cl_ContextForThisKernel, cl_CommandQueueForThisKernel, &ThisKernel, IsSuccesful);
				if (!IsSuccesful)
				{
					std::cout << "\n Error Single Kernel Argument Failed In: cl_KernelMultipleArgumentStruct!\n";
					clReleaseKernel(ThisKernel);
					std::cout << "\n Error Construction Failed cl_KernelMultipleArgumentStruct!";
					return;
				}

				IsConstructionSuccesful = true;
			}
		}

		void AllocateMemoryAndPassToKernel(unsigned int const KernelArgumentNumber, void* PointerToMemoryToCopyFrom, size_t SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, size_t BUFFER_CREATION_ONLY_SizeOfBuffer, bool OverWriteMemory, bool UsePreviouslyAllocatedMemoryOnBuffer, bool& IsFunctionSuccesful)
		{
			IsFunctionSuccesful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling AllocateMemoryAndPassToKernel Without Constructing the struct In: cl_KernelMultipleArgumentStruct!\n";
				return;
			}

			if (KernelArgumentNumber >= OrderedListOfArugments->TotalNumberOfArugments)
			{
				std::cout << "\n Error KernelArgumentNumber + 1 Exceeds the Total Number Of Arugments Present! in AllocateMemoryAndPassToKernel In: cl_KernelMultipleArgumentStruct!\n";
				return;
			}
			else
			{
				SingleKernelFunctionMultiArgumentsArray[KernelArgumentNumber]->AllocateMemoryAndPassToKernel(PointerToMemoryToCopyFrom, SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, BUFFER_CREATION_ONLY_SizeOfBuffer, OverWriteMemory, UsePreviouslyAllocatedMemoryOnBuffer, IsFunctionSuccesful);
				if (!IsFunctionSuccesful)
				{
					std::cout << "\n Error :SingleKernelFunctionMultiArgumentsArray[" << KernelArgumentNumber << "] Failed in AllocateMemoryAndPassToKernel In: cl_KernelMultipleArgumentStruct!\n";
					return;
				}
			}
		}

		void RunKernel(cl_NDRangeStruct* NDRange , bool& IsSuccesful)
		{
			IsSuccesful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling RunKernel Without Constructing the struct In: cl_KernelMultipleArgumentStruct!\n";
				return;
			}

			for (int i = 0; i < OrderedListOfArugments->TotalNumberOfArugments; ++i)
			{
				if (!(SingleKernelFunctionMultiArgumentsArray[i]->IsBufferCreatedOnDevice()))
				{
					std::cout << "\n Error :Kernel Is Not ready for use, Pass AllocateMemoryAndPassToKernel() buffer of Kernel Argument " << i << "Since it Does not Exist. in RunKernel In: cl_KernelMultipleArgumentStruct!\n";
					return;
				}
			}

			if (NDRange == nullptr)
			{
				std::cout << "\n Error :The Argument NDRange is nullptr in RunKernel In: cl_KernelMultipleArgumentStruct!\n";
				IsSuccesful = false;
			}
			else
			{
				cl_uint Dimensions = 0;//By default it is 1
				size_t* GlobalWorkSize;	// Total Number Of Work Items
				size_t* LocalWorkSize;	// Work Items per Work Group
				size_t* WorkSizeOffset;	// WorkSize Offset

				NDRange->GetNDRange(&Dimensions, &GlobalWorkSize, &LocalWorkSize, &WorkSizeOffset, IsSuccesful);
				if (!IsSuccesful)
				{
					std::cout << "\n Error :NDRange->GetNDRange() Failed in RunKernel In: cl_KernelMultipleArgumentStruct!\n";
					return;
				}
				else//Run the Kernel
				{
					cl_int ClErrorResult = 0;
					ClErrorResult = clEnqueueNDRangeKernel(*cl_CommandQueueForThisKernel, ThisKernel, Dimensions, WorkSizeOffset, GlobalWorkSize, LocalWorkSize, 0, NULL, NULL);
					if (ClErrorResult == CL_SUCCESS)
					{
						IsSuccesful = true;
					}
					else
					{
						std::cout << "\n CL_Error Code " << ClErrorResult << " : clEnqueueNDRangeKernel in RunKernel In: MemoryAllocationOnDevice In: cl_KernelMultipleArgumentStruct!\n";
						IsSuccesful = false;
					}
				}
			}
			if (!IsSuccesful)
			{
				std::cout << "\n Error :RunKernel Failed In: cl_KernelMultipleArgumentStruct!\n";
			}
					
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
	struct cl_MultiKernelFunction_MultiDeviceStruct
	{
		const unsigned int NumberOfDevices;
		const std::string KernelFunctionName;
		cl_KernelMultipleArgumentStruct** MultiDeviceKernelArgumentsArray = nullptr;
		const cl_context* cl_ContextForThisKernel;
		const cl_PerDeviceValuesStruct* DeviceValuesForOneDevice;

		bool IsConstructionSuccesful = false;// Once again i tell you this If you hate memory leaks, Don't change value. If you Love memory leaks change them!
		
		//NOTE: Kernel Arguments are Ordered from left to right, Read only memory in left, write only in right, Local in middle of them
		//NOTE: any memory which is plain int, float etc etc those which are NOT cl_mem, Will NOT BE INCLUDED HERE instead they will be given as parameters in kernel Host function
		cl_MultiKernelFunction_MultiDeviceStruct(const unsigned int ArgNumberOfDevices, const cl_KernelFunctionArgumentOrderListStruct* ArgOrderedListOfArugments,
			const cl_context* Argcl_ContextForThisKernel, const cl_PerDeviceValuesStruct* ArgDeviceValuesForOneDevice,
			cl_program* BuiltClProgramContainingTheSpecifiedFunctions, bool& IsSuccesful) : NumberOfDevices(ArgNumberOfDevices), KernelFunctionName(ArgOrderedListOfArugments->KernelFunctionName), cl_ContextForThisKernel(Argcl_ContextForThisKernel), DeviceValuesForOneDevice(ArgDeviceValuesForOneDevice)
		{
			std::cout << "\n Constructing cl_MultiKernelFunction_MultiDeviceStruct!";

			MultiDeviceKernelArgumentsArray = nullptr;

			IsConstructionSuccesful = false;
			IsSuccesful = true;

			if ((ArgOrderedListOfArugments == nullptr) || (cl_ContextForThisKernel == nullptr) || (DeviceValuesForOneDevice == nullptr) || (BuiltClProgramContainingTheSpecifiedFunctions == nullptr))
			{
				std::cout << "\n Error nullptr Passed as value for const variables In: cl_MultiKernelFunction_MultiDeviceStruct!";
				std::cout << "\n Error Construction Failed cl_MultiKernelFunction_MultiDeviceStruct!";
				IsSuccesful = false;
				return;
			}

			if (!ArgOrderedListOfArugments->IsConstructionSuccesful)
			{
				std::cout << "\n Error The OrderedListOfArugments Is Not Constructed, So Is Unusable In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
				IsSuccesful = false;
			}
			else
			{
				if (!ArgOrderedListOfArugments->IsThisListUsable)
				{
					std::cout << "\n Error The OrderedListOfArugments Is Not Properly Set, So Is Unusable In: cl_MultiKernelFunction_MultiDeviceStruct!\n";		
					IsSuccesful = false;
				}
			}

			if (!IsSuccesful)
			{
				std::cout << "\n Error Construction Failed cl_MultiKernelFunction_MultiDeviceStruct!";
				return;
			}
			else
			{
				IsSuccesful = false;

				if (NumberOfDevices > 0)
				{
					Essenbp::Malloc_PointerToArrayOfPointers((void***)&MultiDeviceKernelArgumentsArray, NumberOfDevices, sizeof(cl_KernelMultipleArgumentStruct*), IsSuccesful);
					if (!IsSuccesful)
					{
						std::cout << "\n Error Allocating " << (NumberOfDevices * sizeof(cl_KernelMultipleArgumentStruct*)) << " Byes Of Memory for MultiDeviceKernelArgumentsArray In cl_MultiKernelFunction_MultiDeviceStruct!\n";
					}
				}
				else
				{
					std::cout << "\n Error The Number Of Devices Is Set to Zero In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
				}

				if (!IsSuccesful)
				{
					std::cout << "\n Error Construction Failed cl_MultiKernelFunction_MultiDeviceStruct!";
					return;
				}

				for (int i = 0; i < NumberOfDevices; ++i)
				{
					MultiDeviceKernelArgumentsArray[i] = new cl_KernelMultipleArgumentStruct(ArgOrderedListOfArugments, cl_ContextForThisKernel, &DeviceValuesForOneDevice->DeviceClCommandQueue, BuiltClProgramContainingTheSpecifiedFunctions, IsSuccesful);
					if (MultiDeviceKernelArgumentsArray[i] == nullptr)
					{
						std::cout << "\n Error Allocating " << sizeof(cl_KernelMultipleArgumentStruct) << " Byes Of Memory for MultiDeviceKernelArgumentsArray[" << i << "] In cl_MultiKernelFunction_MultiDeviceStruct!\n";

						for (int j = 0; j < i; ++j)
						{
							delete MultiDeviceKernelArgumentsArray[j];
						}
						free(MultiDeviceKernelArgumentsArray);
						std::cout << "\n Error Construction Failed cl_MultiKernelFunction_MultiDeviceStruct!";
						return;
					}
					if (!IsSuccesful)
					{
						std::cout << "\n Error Construction Failed cl_MultiKernelFunction_MultiDeviceStructt!";
						delete MultiDeviceKernelArgumentsArray[i];
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
		}

		void RunKernel(unsigned int ArgDeviceNumber, cl_NDRangeStruct* NDRange, bool& IsSuccesful)
		{
			IsSuccesful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling RunKernel Without Constructing the struct In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
				return;
			}

			if (ArgDeviceNumber < NumberOfDevices)
			{
				if (NDRange == nullptr)
				{
					std::cout << "\n Error :The Argument NDRange is nullptr in RunKernel In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
					IsSuccesful = false;
				}
				else
				{
					MultiDeviceKernelArgumentsArray[ArgDeviceNumber]->RunKernel(NDRange, IsSuccesful);					
				}
				if (!IsSuccesful)
				{
					std::cout << "\n Error :cl_KernelMultipleArgumentStruct::RunKernel Failed in RunKernel In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
				}
				return;
			}
			else
			{
				std::cout << "\n Error :ArgDeviceNumber Exceds the total number of Devices in RunKernel In: cl_MultiKernelFunction_MultiDeviceStruct!\n";
				return;
			}
		}

		~cl_MultiKernelFunction_MultiDeviceStruct()
		{
			std::cout << "\n Destructing cl_MultiKernelFunction_MultiDeviceStruct!";
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
				Essenbp::Malloc_PointerToArrayOfPointers((void***)&OpenCL_KernelFunctions, NumberOfFunctionsToAdd, sizeof(cl_KernelFunctionArgumentOrderListStruct*), IsSuccesful);
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
				std::cout << "\n Error Argument Number + 1' " << ArgumentNumber + 1 << "' Is greater than the TotalNumberOfArguments, ";
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

	// NOTE: Call Constructor Before Using, This is a Helper Struct for sending data to kernel(buffer), Also each of this is for 1 type of kernel only
	struct cl_KernelArgumentSendStruct
	{
	private:
		const cl_KernelFunctionArgumentOrderListStruct* ArugmentList;
		bool** ArrayOfOverWriteDataSetBoolean = nullptr;
		bool** ArrayOfIsArgumentAlreadySetBoolean = nullptr;
		bool** ArrayOfUsePreviouslyAllocatedMemoryBoolean = nullptr;
		Essenbp::UnknownDataAndSize** ArrayOfArgumentData = nullptr;

	public:
		bool IsConstructionSuccesful = false;
		bool DataPassedToKernel = false;

		cl_KernelArgumentSendStruct(cl_KernelFunctionArgumentOrderListStruct* ArgArugmentList, bool& IsSuccesful) : ArugmentList(ArgArugmentList)
		{
			std::cout << "\n Constructing cl_KernelArgumentSendStruct";

			IsConstructionSuccesful = false;
			DataPassedToKernel = false;
			IsSuccesful = false;

			if (!ArugmentList->IsConstructionSuccesful)
			{
				std::cout << "\n Error The ArugmentList Is Not Constructed, So Is Unusable In: cl_KernelArgumentSendStruct!\n";
				std::cout << "\n Error Construction Failed cl_KernelArgumentSendStruct!";
				return;
			}
			else
			{
				if (!ArugmentList->IsThisListUsable)
				{
					std::cout << "\n Error ArugmentList Is Not Properly Set, So Is Unusable In: cl_KernelArgumentSendStruct!\n";
					std::cout << "\n Error Construction Failed cl_KernelArgumentSendStruct!";
					return;
				}
			}

			Essenbp::Malloc_PointerToArrayOfPointers((void***)&ArrayOfOverWriteDataSetBoolean, ArugmentList->TotalNumberOfArugments, sizeof(bool*), IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(bool*) << " Byes Of Memory for ArrayOfOverWriteDataSetBoolean In: cl_KernelArgumentSendStruct!\n";
				std::cout << "\n Error Construction Failed cl_KernelArgumentSendStruct!";
				return;
			}

			Essenbp::Malloc_PointerToArrayOfPointers((void***)&ArrayOfIsArgumentAlreadySetBoolean, ArugmentList->TotalNumberOfArugments, sizeof(bool*), IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(bool*) << " Byes Of Memory for ArrayOfIsArgumentAlreadySetBoolean In: cl_KernelArgumentSendStruct!\n";
				std::cout << "\n Error Construction Failed cl_KernelArgumentSendStruct!";
				free(ArrayOfOverWriteDataSetBoolean);
				return;
			}

			Essenbp::Malloc_PointerToArrayOfPointers((void***)&ArrayOfUsePreviouslyAllocatedMemoryBoolean, ArugmentList->TotalNumberOfArugments, sizeof(bool*), IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(bool*) << " Byes Of Memory for ArrayOfUsePreviouslyAllocatedMemoryBoolean In: cl_KernelArgumentSendStruct!\n";
				std::cout << "\n Error Construction Failed cl_KernelArgumentSendStruct!";
				free(ArrayOfOverWriteDataSetBoolean);
				free(ArrayOfIsArgumentAlreadySetBoolean);
				return;
			}


			Essenbp::Malloc_PointerToArrayOfPointers((void***)&ArrayOfArgumentData, ArugmentList->TotalNumberOfArugments, sizeof(Essenbp::UnknownDataAndSize*), IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(Essenbp::UnknownDataAndSize*) << " Byes Of Memory for ArrayOfArgumentData In: cl_KernelArgumentSendStruct!\n";
				std::cout << "\n Error Construction Failed cl_KernelArgumentSendStruct!";
				free(ArrayOfOverWriteDataSetBoolean);
				free(ArrayOfIsArgumentAlreadySetBoolean);
				free(ArrayOfUsePreviouslyAllocatedMemoryBoolean);
				return;
			}
			else
			{
				for (int i = 0; i < ArugmentList->TotalNumberOfArugments; ++i)
				{
					ArrayOfArgumentData[i] = new Essenbp::UnknownDataAndSize();
					ArrayOfOverWriteDataSetBoolean[i] = new bool(false);
					ArrayOfIsArgumentAlreadySetBoolean[i] = new bool(false);
					ArrayOfUsePreviouslyAllocatedMemoryBoolean[i] = new bool(false);

					IsSuccesful = (ArrayOfArgumentData[i] != nullptr) && (ArrayOfIsArgumentAlreadySetBoolean[i] != nullptr) && (ArrayOfOverWriteDataSetBoolean[i] != nullptr);

					if (!IsSuccesful)
					{
						if (ArrayOfArgumentData[i] != nullptr)
						{
							delete ArrayOfArgumentData[i];
						}
						else
						{
							std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(Essenbp::UnknownDataAndSize) << " Byes Of Memory for ArrayOfArgumentData[" << i << "] In: cl_KernelArgumentSendStruct!\n";
						}

						if (ArrayOfOverWriteDataSetBoolean[i] != nullptr)
						{
							delete ArrayOfOverWriteDataSetBoolean[i];
						}
						else
						{
							std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(bool) << " Byes Of Memory for ArrayOfOverWriteDataSetBoolean[" << i << "] In: cl_KernelArgumentSendStruct!\n";
						}

						if (ArrayOfIsArgumentAlreadySetBoolean[i] != nullptr)
						{
							delete ArrayOfIsArgumentAlreadySetBoolean[i];
						}
						else
						{
							std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(bool) << " Byes Of Memory for ArrayOfIsArgumentAlreadySetBoolean[" << i << "] In: cl_KernelArgumentSendStruct!\n";
						}	

						if (ArrayOfUsePreviouslyAllocatedMemoryBoolean[i] != nullptr)
						{
							delete ArrayOfUsePreviouslyAllocatedMemoryBoolean[i];
						}
						else
						{
							std::cout << "\n Error Allocating " << ArugmentList->TotalNumberOfArugments * sizeof(bool) << " Byes Of Memory for ArrayOfUsePreviouslyAllocatedMemoryBoolean[" << i << "] In: cl_KernelArgumentSendStruct!\n";
						}

						for (int j = 0; j < i; j++)
						{
							delete ArrayOfArgumentData[j];
							delete ArrayOfOverWriteDataSetBoolean[i];
							delete ArrayOfIsArgumentAlreadySetBoolean[j];
							delete ArrayOfUsePreviouslyAllocatedMemoryBoolean[j];
						}

						free(ArrayOfArgumentData);
						free(ArrayOfOverWriteDataSetBoolean);
						free(ArrayOfIsArgumentAlreadySetBoolean);
						free(ArrayOfUsePreviouslyAllocatedMemoryBoolean);
						std::cout << "\n Error Construction Failed cl_KernelArgumentSendStruct!";
						return;
					}
				}
			}
			IsConstructionSuccesful = true;
		}
		
		//NOTE: For LOCAL Data ArgData should be nullptr
		//NOTE: For PRIVATE Data ArgData should be not nullptr And ArgSizeOfData should match the size first inputed, If ArgData is nullptr and ArgSizeOfData is 0 then previously written memory will be reused
		//NOTE: For READ, WRITE and READ_AND_WRITE Data ArgData should NOT be nullptr and ArgSizeOfData should be great than 0 
		//NOTE: For READ, WRITE and READ_AND_WRITE Data If ArgData is nullptr and ArgSizeOfData is 0 then previously written memory will be reused
		//NOTE: 5th argument OverWriteMemory if set to true previous allocated memory location is reused NOTE: ArgSizeOfData should be less than or equal to previous memory! by default it is set to false
		//NOTE: 5th argument OverWriteMemory if set to true previous allocated memory location is reused NOTE: ArgSizeOfData should be less than or equal to previous memory! by default it is set to false
		void StoreDataForKernelArgument(unsigned int ArgumentNumber, void* ArgData, size_t ArgSizeOfData, bool& IsSuccesful , bool OverWriteMemory_NOTForLOCAL_PRIVATE = false, bool UsePreviouslyAllocatedMemoryOnBuffer = false)
		{
			IsSuccesful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling StoreDataForKernelArgument Without Constructing the struct In: cl_KernelArgumentSendStruct!\n";
				return;
			}
			else
			{				
				if(ArgumentNumber >= ArugmentList->TotalNumberOfArugments)
				{
					std::cout << "\n Error ArgumentNumber + 1 Exceeds the Total Number Of Arugments Present! in StoreDataForKernelArgument In: PassDataStructToKernel!\n";
					return;
				}
				else
				{
					if (*(ArugmentList->KernelArgumentsInOrder[ArgumentNumber]) == cl_Memory_Type::CL_LOCALENUM)
					{
						if (OverWriteMemory_NOTForLOCAL_PRIVATE)
						{
							std::cout << "\n Error OverWriteMemory_NOTForLOCAL_PRIVATE is set to true, cl_Memory_Type::CL_LOCALENUM can not be written or overwritten StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
							std::cout << "NOTE: For PRIVATE and LOCAL Memory set OverWriteMemory_NOTForLOCAL_PRIVATE to false\n";
						}
						else
						{
							if (UsePreviouslyAllocatedMemoryOnBuffer)
							{
								std::cout << "\n Error UsePreviouslyAllocatedMemoryOnBuffer is set to true, cl_Memory_Type::CL_LOCALENUM can not be accesed from host StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
								std::cout << "NOTE: For LOCAL Memory set UsePreviouslyAllocatedMemoryOnBuffer to false\n";
							}
							else
							{
								if (ArgData == nullptr)
								{
									ArrayOfArgumentData[ArgumentNumber]->CopyAndStoreData(nullptr, ArgSizeOfData, IsSuccesful, true);
									if (!IsSuccesful)
									{
										std::cout << "\n Error CopyAndStoreData Failed in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
									}
								}
								else
								{
									std::cout << "\n Error Trying to Send Data to Local Memory, ArgData should be nullptr in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
								}
							}
						}
					}
					else
					{
						if (*(ArugmentList->KernelArgumentsInOrder[ArgumentNumber]) == cl_Memory_Type::CL_PRIVATE)
						{
							if (OverWriteMemory_NOTForLOCAL_PRIVATE)
							{
								std::cout << "\n Error OverWriteMemory_NOTForLOCAL_PRIVATE is set to true, cl_Memory_Type::CL_PRIVATE is compatable with OverWriteMemory_NOTForLOCAL_PRIVATE StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
								std::cout << "NOTE: For PRIVATE and LOCAL Memory set OverWriteMemory_NOTForLOCAL_PRIVATE to false\n";
							}
							else
							{
								if (*(ArrayOfIsArgumentAlreadySetBoolean[ArgumentNumber]))// After the First time(sencond and so on), the values are initialized to false at construction
								{
									if (ArgData == nullptr)
									{									
										if (ArgSizeOfData == 0)
										{
											if (*(ArrayOfIsArgumentAlreadySetBoolean[ArgumentNumber]))
											{
												// Previous Data Will be reused
												IsSuccesful = true;// setting it to succesful
											}
											else
											{
												std::cout << "\n Error Trying to Reuse previous Data There is no previous Data stored in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
											}
										}
										else
										{
											std::cout << "\n Error Trying to Reuse previous Data But ArgSizeOfData is Not Set to 0 in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
											std::cout << "NOTE: To Reuse Previous Data set ArgData to nullptr, and ArgSizeOfData to 0\n";
										}
									}										
									else
									{
										if (ArgSizeOfData != ArrayOfArgumentData[ArgumentNumber]->GetDataSize())
										{
											std::cout << "\n Error Trying to Change the Size of cl_Memory_Type::CL_PRIVATE DataType in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
											std::cout << "Size of Private Data type can only be set once, because Variable types do not changes their Data type sizes...\n";
										}
										else
										{
											if (ArgSizeOfData > 0)
											{
												ArrayOfArgumentData[ArgumentNumber]->CopyAndStoreData(ArgData, ArgSizeOfData, IsSuccesful, false);
												//ArrayOfArgumentData[ArgumentNumber]->CopyAndStoreData(nullptr, ArrayOfArgumentData[ArgumentNumber]->GetDataSize(), IsSuccesful, true);// Same as above since ArgSizeOfData == ArrayOfArgumentData[ArgumentNumber]->GetDataSize()
												if (!IsSuccesful)
												{
													std::cout << "\n Error CopyAndStoreData Failed in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
												}
											}
											else
											{
												if (UsePreviouslyAllocatedMemoryOnBuffer)// The difference is that This useses data ON THE BUFFER(May or may not have been changed by the kerel)
												{
													// Previous Data On the BUFFER Will be reused
													IsSuccesful = true;// setting it to succesful
													//NOTE: This does not verify whether there is data on the buffer or not...
												}
												else
												{
													std::cout << "\n Error Size of Data for PRIVATE data can not be Less than 1 Bytes in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
													std::cout << "NOTE: To Reuse Previous Data set ArgData to nullptr, and ArgSizeOfData to 0\n";
												}
											}
										}										
									}	
								}
								else
								{
									if (ArgData != nullptr)
									{
										if (ArgSizeOfData > 0)
										{
											ArrayOfArgumentData[ArgumentNumber]->CopyAndStoreData(ArgData, ArgSizeOfData, IsSuccesful, false);// For First time use
											if (!IsSuccesful)
											{
												std::cout << "\n Error CopyAndStoreData Failed in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
											}
										}
										else
										{
											std::cout << "\n Error Size of Data for PRIVATE data can not be Less than 1 Bytes in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
										}
									}
									else
									{
										if (ArgSizeOfData == 0)
										{
											if (UsePreviouslyAllocatedMemoryOnBuffer)// The difference is that This useses data ON THE BUFFER(May or may not have been changed by the kerel)
											{
												// Previous Data On the BUFFER Will be reused
												IsSuccesful = true;// setting it to succesful
												//NOTE: This does not verify whether there is data on the buffer or not...
											}
											else
											{
												std::cout << "\n Error First time Setting Private Data but ArgData Is set to nullptr and size is 0 In: cl_KernelArgumentSendStruct!\n";
												std::cout << "NOTE: First Set data using valid pointer and size then you can reuse\n";
												std::cout << "NOTE: To Reuse Previous Data set ArgData to nullptr, and ArgSizeOfData to 0\n";
											}
										}
										else
										{
											std::cout << "\n Error First time Setting Private Data but ArgData Is set to nullptr In: cl_KernelArgumentSendStruct!\n";
											std::cout << "NOTE: First Set data using valid pointer and size then you can reuse\n";
											std::cout << "NOTE: To Reuse Previous Data set ArgData to nullptr, and ArgSizeOfData to 0\n";
										}
									}
								}
							}
							return;
						}
						else
						{
							if ((*(ArugmentList->KernelArgumentsInOrder[ArgumentNumber]) == cl_Memory_Type::CL_READ_ONLY) || (*(ArugmentList->KernelArgumentsInOrder[ArgumentNumber]) == cl_Memory_Type::CL_WRITE_ONLY) || (*(ArugmentList->KernelArgumentsInOrder[ArgumentNumber]) == cl_Memory_Type::CL_READ_AND_WRITE))
							{
								if (ArgData != nullptr)
								{	
									if (OverWriteMemory_NOTForLOCAL_PRIVATE)
									{
										if (!UsePreviouslyAllocatedMemoryOnBuffer)
										{
											if (*(ArrayOfIsArgumentAlreadySetBoolean[ArgumentNumber]))
											{
												if (ArgSizeOfData > ArrayOfArgumentData[ArgumentNumber]->GetDataSize())
												{
													std::cout << "\n Error Trying to OverWrite previous Data But ArgSizeOfData " << ArgSizeOfData << " Bytes Is Greater than Previous Data Size " << ArrayOfArgumentData[ArgumentNumber]->GetDataSize() << " Bytes in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
												}
												else
												{
													// Previous Data will be OverWritten
													ArrayOfArgumentData[ArgumentNumber]->CopyAndStoreData(ArgData, ArgSizeOfData, IsSuccesful, false);
													if (!IsSuccesful)
													{
														std::cout << "\n Error CopyAndStoreData Failed in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
													}
												}
											}
											else
											{
												std::cout << "\n Error Trying to OverWrite previous Data but There is no previous Data stored in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
											}
										}
										else
										{
											std::cout << "\n Error Trying to OverWrite previous Data and UsePreviouslyAllocatedMemoryOnBuffer cannot be used at the same time, in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
										}
									}
									else
									{
										ArrayOfArgumentData[ArgumentNumber]->CopyAndStoreData(ArgData, ArgSizeOfData, IsSuccesful, false);
										if (!IsSuccesful)
										{
											std::cout << "\n Error CopyAndStoreData Failed in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
										}
									}									
								}
								else
								{
									if (ArgSizeOfData == 0)
									{
										if (UsePreviouslyAllocatedMemoryOnBuffer)// The difference is that This useses data ON THE BUFFER(May or may not have been changed by the kerel)
										{
											// Previous Data On the BUFFER Will be reused
											IsSuccesful = true;// setting it to succesful
											//NOTE: This does not verify whether there is data on the buffer or not...
										}
										else// While This Copys Data(Most definitely not modified) ON THE HOST to the buffer
										{
											if (*(ArrayOfIsArgumentAlreadySetBoolean[ArgumentNumber]))
											{
												// Previous Data on the HOST Will be reused
												IsSuccesful = true;// setting it to succesful
											}
											else
											{
												std::cout << "\n Error Trying to Reuse previous Data on the HOST, But There is no previous Data stored in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
											}
										}										
									}
									else
									{
										std::cout << "\n Error Trying to Reuse previous Data But ArgSizeOfData is Not Set to 0 in StoreDataForKernelArgument In: cl_KernelArgumentSendStruct!\n";
										std::cout << "NOTE: To Reuse Previous Data set ArgData to nullptr, and ArgSizeOfData to 0\n";
									}
								}
							}
						}
					}					
					//cl_Memory_Type::Uninitialized_cl_Memory or undefined memorytype is not possible here since list is declared fully initialized
				}
				if (!IsSuccesful)
				{
					std::cout << "\n Error StoreDataForKernelArgument Failed In: cl_KernelArgumentSendStruct!\n";
				}
				else
				{
					DataPassedToKernel = false;// New data is stored so it means that data is not passed
					*(ArrayOfIsArgumentAlreadySetBoolean[ArgumentNumber]) = true;
					if (OverWriteMemory_NOTForLOCAL_PRIVATE)
					{
						*(ArrayOfIsArgumentAlreadySetBoolean[ArgumentNumber]) = true;
					}
					else
					{
						if (UsePreviouslyAllocatedMemoryOnBuffer)
						{
							*(ArrayOfUsePreviouslyAllocatedMemoryBoolean[ArgumentNumber]) = true;
						}
						else
						{
							*(ArrayOfUsePreviouslyAllocatedMemoryBoolean[ArgumentNumber]) = false;
						}
					}
				}
			}			
		}

		//This ArgumentNumberWhichIsNotReady -1 if true, else anywhere between 0 to (Total Numbers of Arguments - 1) which is the argument of the kernel function, to whom Data has not been sent
		void IsDataReadyForSend(bool& IsSuccesful, cl_int& ArgumentNumberWhichIsNotReady)
		{
			IsSuccesful = false;
			ArgumentNumberWhichIsNotReady = -1;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling IsDataReadyForSend Without Constructing the struct In: cl_KernelArgumentSendStruct!\n";
				return;
			}
			else
			{
				for (int i = 0; i < ArugmentList->TotalNumberOfArugments; ++i)
				{
					if (!(*(ArrayOfIsArgumentAlreadySetBoolean[i])))
					{
						ArgumentNumberWhichIsNotReady = i;
						return;
					}
				}
				IsSuccesful = true;
				return;
			}
		}

		void PassAllDataToKernel(cl_KernelMultipleArgumentStruct* TheKernelMultiArgStruct, bool& IsSuccesful, cl_int& ArgumentNumberWhichIsNotReady)
		{
			IsSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling PassAllDataToKernel Without Constructing the struct In: cl_KernelArgumentSendStruct!\n";
				return;
			}
			if (TheKernelMultiArgStruct == nullptr)
			{
				std::cout << "\n Error 'TheKernelMultiArgStruct Argument of type cl_KernelMultipleArgumentStruct Is a nullptr in PassAllDataToKernel In: cl_KernelArgumentSendStruct!\n";
			}

			if (TheKernelMultiArgStruct->IsConstructionSuccesful)
			{
				std::cout << "\n Error 'TheKernelMultiArgStruct Argument of type cl_KernelMultipleArgumentStruct Is Not Constructed in PassAllDataToKernel In: cl_KernelArgumentSendStruct!\n";
				return;
			}

			if (DataPassedToKernel)
			{
				//std::cout << "\n Error Same Data already passed to kernel in PassAllDataToKernel In: cl_KernelArgumentSendStruct!\n";
				//std::cout << "NOTE: Change any or all of argument data(or pass the same data) by using StoreDataForKernelArgument()\n";
				IsSuccesful = true;// Since the Data is already passed and it was already succesful... this entire thing is succesful
				return;
			}

			IsDataReadyForSend(IsSuccesful, ArgumentNumberWhichIsNotReady);// Data is Sent only if it is ready
			if (!IsSuccesful)
			{
				if (ArgumentNumberWhichIsNotReady == -1)
				{
					std::cout << "\n Error Calling PassAllDataToKernel Without Constructing the struct In: cl_KernelArgumentSendStruct!\n";
				}
				else
				{
					std::cout << "\n Error ArgumentList's argument Number " << ArgumentNumberWhichIsNotReady << " Is not Set! in PassDataStructToKernel In: cl_KernelArgumentSendStruct!\n";
				}
			}
			else
			{
				for (int i = 0; i < ArugmentList->TotalNumberOfArugments; ++i)
				{
					if (*(ArrayOfUsePreviouslyAllocatedMemoryBoolean[i]))// Uses previously allocated memory on buffer(if it exists)
					{
						TheKernelMultiArgStruct->AllocateMemoryAndPassToKernel(i, nullptr, 0, 0, false, true, IsSuccesful);
					}
					else
					{
						TheKernelMultiArgStruct->AllocateMemoryAndPassToKernel(i, ArrayOfArgumentData[i]->GetData(), ArrayOfArgumentData[i]->GetDataSize(), (*(ArrayOfOverWriteDataSetBoolean[i]) ? 0 : ArrayOfArgumentData[i]->GetDataSize()), *(ArrayOfOverWriteDataSetBoolean[i]), false, IsSuccesful);
					}
					
					if (!IsSuccesful)
					{
						std::cout << "\n Error :AllocateMemoryAndPassToKernel failed in PassAllDataToKernel In: cl_KernelArgumentSendStruct!\n";
						return;
					}
					else//On Success
					{
						*(ArrayOfOverWriteDataSetBoolean[i]) = false;
						*(ArrayOfUsePreviouslyAllocatedMemoryBoolean[i]) = false;
					}
				}
				DataPassedToKernel = true;
			}			
		}

		~cl_KernelArgumentSendStruct()
		{
			std::cout << "\n Destructing cl_KernelArgumentSendStruct(!";
			if (IsConstructionSuccesful)
			{
				for (int i = 0; i < ArugmentList->TotalNumberOfArugments; ++i)
				{
					delete ArrayOfArgumentData[i];
					delete ArrayOfIsArgumentAlreadySetBoolean[i];
					delete ArrayOfUsePreviouslyAllocatedMemoryBoolean[i];
				}
				free(ArrayOfArgumentData);
				free(ArrayOfIsArgumentAlreadySetBoolean);
				free(ArrayOfUsePreviouslyAllocatedMemoryBoolean);
			}
			IsConstructionSuccesful = false;
		}
	};

	// NOTE: Call Constructor Before Using, This is a Helper for multi devices(single too)
	struct cl_MultiDevice_KernelArgumentSendStruct
	{
		unsigned int NumberOfDevices = 0;
		cl_KernelArgumentSendStruct** MultiArgumentSendStructList = nullptr;
		cl_KernelFunctionArgumentOrderListStruct* ArugmentList;

		bool IsConstructionSuccesful = false;
		cl_MultiDevice_KernelArgumentSendStruct(cl_KernelFunctionArgumentOrderListStruct* ArgArugmentList, bool& IsSuccesful)
		{
			NumberOfDevices = 0;
			MultiArgumentSendStructList = nullptr;

			ArugmentList = ArgArugmentList;
			if (ArugmentList == nullptr)
			{
				std::cout << "\n Error Construction Failed cl_MultiDevice_KernelArgumentSendStruct!";
				IsSuccesful = false;
				return;
			}
			
			IsConstructionSuccesful = true;
			IsSuccesful = true;
			
		}

		void SetNumberOfDevices(unsigned int ArgNumberOfDevices, bool& IsSuccesful)
		{
			IsSuccesful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling SetNumberOfDevices Without Constructing the struct In: cl_MultiDevice_KernelArgumentSendStruct!\n";
				return;
			}

			if (MultiArgumentSendStructList != nullptr)
			{
				for (int i = 0; i < NumberOfDevices; ++i)
				{
					delete MultiArgumentSendStructList[i];
				}
				free(MultiArgumentSendStructList);
			}

			Essenbp::Malloc_PointerToArrayOfPointers((void***)&MultiArgumentSendStructList, ArgNumberOfDevices, sizeof(cl_KernelArgumentSendStruct*), IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Allocating " << ArgNumberOfDevices * sizeof(cl_KernelArgumentSendStruct*) << " Byes Of Memory for MultiArgumentSendStructList in SetNumberOfDevices In: cl_MultiDevice_KernelArgumentSendStruct!\n";
				std::cout << "\n Error Construction Failed cl_KernelArgumentSendStruct!";
				return;
			}

			for (int i = 0; i < NumberOfDevices; ++i)
			{
				MultiArgumentSendStructList[i] = new cl_KernelArgumentSendStruct(ArugmentList, IsSuccesful);
				if (!IsSuccesful)
				{
					std::cout << "\n Error Allocating " << ArgNumberOfDevices * sizeof(cl_KernelArgumentSendStruct*) << " Byes Of Memory for MultiArgumentSendStructList["<<i<<"] in SetNumberOfDevices In: cl_MultiDevice_KernelArgumentSendStruct!\n";
					std::cout << "\n Error Construction Failed cl_KernelArgumentSendStruct!";
					for (int j = 0; j < i; ++j)
					{
						delete MultiArgumentSendStructList[j];
					}
					free(MultiArgumentSendStructList);
					return;
				}
			}
			NumberOfDevices = ArgNumberOfDevices;
			IsSuccesful = true;
		}

		void StoreDataForKernelArgument(unsigned int DeviceNumber, unsigned int ArgumentNumber, void* ArgData, size_t ArgSizeOfData, bool& IsSuccesful, bool OverWriteMemory_NOTForLOCAL_PRIVATE = false, bool UsePreviouslyAllocatedMemoryOnBuffer = false)
		{
			IsSuccesful = false;
			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error :Calling SetNumberOfDevices Without Constructing the struct In: cl_MultiDevice_KernelArgumentSendStruct!\n";
				return;
			}

			if (DeviceNumber < NumberOfDevices)
			{
				MultiArgumentSendStructList[DeviceNumber]->StoreDataForKernelArgument(ArgumentNumber, ArgData, ArgSizeOfData, IsSuccesful, OverWriteMemory_NOTForLOCAL_PRIVATE, UsePreviouslyAllocatedMemoryOnBuffer);
				if (!IsSuccesful)
				{
					std::cout << "\n Error cl_KernelArgumentSendStruct::StoreDataForKernelArgument failed in StoreDataForKernelArgument In: cl_MultiDevice_KernelArgumentSendStruct!\n";
					return;
				}
			}
			else
			{
				std::cout << "\n Error DeviceNumber + 1 '"<< DeviceNumber + 1 <<"' is greater than NumberOfDevices in StoreDataForKernelArgument In: cl_MultiDevice_KernelArgumentSendStruct!\n";
				return;
			}
		}

		~cl_MultiDevice_KernelArgumentSendStruct()
		{
			std::cout << "\n Destructing cl_MultiDevice_KernelArgumentSendStruct!";
			if (IsConstructionSuccesful)
			{
				for (int i = 0; i < NumberOfDevices; ++i)
				{
					delete MultiArgumentSendStructList[i];
				}
				free(MultiArgumentSendStructList);
			}
			IsConstructionSuccesful = false;
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
		cl_MultiKernelFunction_MultiDeviceStruct** MultiKernelFunction_MultiDevice = nullptr;	// Initialization And Construction functions will take care of it	

	public:
		bool IsConstructionSuccesful = false;// Same as before, Manual changes = memory leaks, Automatic(constructor) Only changes will Obliterate the chances of possible memory leaks

		//Initialization
		void InitializeOpenCLProgram(bool& IsSuccesful, cl_uint PlatformToUse)//, cl_uint PlatformToUse = 1)
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

			Essenbp::Malloc_PointerToArrayOfPointers((void***)&PerDeviceValueStruct, NumberOfGPUs, sizeof(cl_PerDeviceValuesStruct*), IsSuccesful);
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
						if (ClErrorResult != CL_SUCCESS)
						{
							std::cout << "\n CLError " << ClErrorResult << " : Constructing cl_PerDeviceValuesStruc in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						}

						std::cout << "\n Error Allocating " << sizeof(cl_PerDeviceValuesStruct) << " Byes Of Memory for PerDeviceValueStruct[" << i << "] In InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						for (int j = 0; j < i; j++)
						{
							delete(PerDeviceValueStruct[j]);
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
			Essenbp::GetFileContent(ClSourceFilePath, ClSourceFileInString, IsSuccesful);
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
				std::cout << "\n Error The value of the variable 'ArgOrderedKernelArgumentList' Is not nullptr, It should be empty, In FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
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

		void cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper(std::string ClSourceFilePath, cl_KernelFunctionArgumentOrderListStruct** ArgOrderedKernelArgumentList, unsigned int ArgTotalNumberOfKernelFunctions, bool& IsSuccesful)
		{
			IsSuccesful = false;
			Essenbp::Malloc_PointerToArrayOfPointers((void***)&MultiKernelFunction_MultiDevice, TotalNumberOfKernelFunctions, sizeof(cl_MultiKernelFunction_MultiDeviceStruct*), IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Allocating " << (TotalNumberOfKernelFunctions * sizeof(cl_MultiKernelFunction_MultiDeviceStruct*)) << " Byes Of Memory for MultiKernelFunction_MultiDevice In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}
			else
			{
				for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
				{
					MultiKernelFunction_MultiDevice[i] = new cl_MultiKernelFunction_MultiDeviceStruct(NumberOfDevices, OrderedKernelArgumentList[i], &SingleContext, PerDeviceValueStruct[i], &SingleProgram, IsSuccesful);

					if (MultiKernelFunction_MultiDevice[i] != nullptr)
					{
						if (IsSuccesful)
						{
							continue;
						}
						else
						{
							delete MultiKernelFunction_MultiDevice[i];
						}
					}
					//else the code below will clean up
					std::cout << "\n Error Allocating " << (sizeof(cl_MultiKernelFunction_MultiDeviceStruct)) << " Byes Of Memory for MultiKernelFunction_MultiDevice[i] In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					IsConstructionSuccesful = false;

					for (int j = 0; j < i; ++j)
					{
						delete MultiKernelFunction_MultiDevice[j];
					}
					free(MultiKernelFunction_MultiDevice);
					return;
				}
				IsSuccesful = true;
			}
		}

		// Manual
		cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct(std::string ClSourceFilePath, cl_KernelFunctionsStruct* KernelFunctionsStruct, unsigned int ArgTotalNumberOfKernelFunctions, cl_PlatformVendorStruct* PlatformToUse, bool& IsSuccesful) : ClSourceFilePath(ClSourceFilePath)
		{
			std::cout << "\n Constructing cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct! Using Manual Constructor";

			ChosenPlatform = nullptr;
			SingleProgram = nullptr;
			SingleContext = nullptr;
			NumberOfDevices = 0;
			ChosenDevices = nullptr;
			PerDeviceValueStruct = nullptr;
			TotalNumberOfKernelFunctions = ArgTotalNumberOfKernelFunctions;
			OrderedKernelArgumentList = nullptr;
			MultiKernelFunction_MultiDevice = nullptr;

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
				std::cout << "\n Error PlatformToUse->GetChosenPlatform() Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}

			// Single Program Is First Initialized
			InitializeOpenCLProgram(IsSuccesful, PlatformNumberToUse);

			if (IsSuccesful && KernelFunctionsStruct->IsConstructionSuccesful)
			{
				IsSuccesful = false;// reseting

				Essenbp::Malloc_PointerToArrayOfPointers((void***)&OrderedKernelArgumentList, TotalNumberOfKernelFunctions, sizeof(cl_KernelFunctionArgumentOrderListStruct*), IsSuccesful);
				if (!IsSuccesful)
				{
					std::cout << "\n Error Allocating " << (TotalNumberOfKernelFunctions * sizeof(cl_KernelFunctionArgumentOrderListStruct*)) << " Byes Of Memory for OrderedKernelArgumentList In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					ReleaseAndFreeClData();
					return;
				}
				else
				{
					for (int i = 0; i < ArgTotalNumberOfKernelFunctions; ++i)
					{
						OrderedKernelArgumentList[i] = new cl_KernelFunctionArgumentOrderListStruct(KernelFunctionsStruct->OpenCL_KernelFunctions[i]);
						if (OrderedKernelArgumentList[i] == nullptr)
						{
							std::cout << "\n Error Allocating " << sizeof(cl_KernelFunctionArgumentOrderListStruct) << " Byes Of Memory for OrderedKernelArgumentList[" << i << "] In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
							ReleaseAndFreeClData();
							for (int j = 0; j < i; ++j)
							{
								delete OrderedKernelArgumentList[j];
							}
							free(OrderedKernelArgumentList);
							return;
						}
					}
					TotalNumberOfKernelFunctions = ArgTotalNumberOfKernelFunctions;

					cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper(ClSourceFilePath, OrderedKernelArgumentList, TotalNumberOfKernelFunctions, IsSuccesful);
					if (!IsSuccesful)
					{
						std::cout << "\n Error Construction Of cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
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

		// Automatic
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
			MultiKernelFunction_MultiDevice = nullptr;

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
				std::cout << "\n Error PlatformToUse->GetChosenPlatform() Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}

			// Single Program Is First Initialized	
			InitializeOpenCLProgram(IsSuccesful, PlatformNumberToUse);

			if (IsSuccesful)
			{
				std::string ClSourceFileInString;
				Essenbp::GetFileContent(ClSourceFilePath, ClSourceFileInString, IsSuccesful);
				if (IsSuccesful)
				{
					FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode(ClSourceFileInString, &OrderedKernelArgumentList, TotalNumberOfKernelFunctions, IsSuccesful);
					if (!IsSuccesful)
					{
						std::cout << "\n Error FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
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
									std::cout << "\n Error FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
									IsSuccesful = false;
								}
							}
							else
							{
								std::cout << "\n Error FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
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
					std::cout << "\n Error Construction Of cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
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
				std::cout << "\n Error Initilization Of cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				ReleaseAndFreeClData();
				return;
			}
			IsConstructionSuccesful = true;
		}

		// Single Line Automatic, First Platform Is chosen by default
		cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct(const std::string ClSourceFilePath, bool& IsSuccesful) : ClSourceFilePath(ClSourceFilePath)
		{
			std::cout << "\n Constructing cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct! Using Single Line Automatic Automatic Constructor(NOTE: First Platform Is chosen by default)";

			ChosenPlatform = nullptr;
			SingleProgram = nullptr;
			SingleContext = nullptr;
			NumberOfDevices = 0;
			ChosenDevices = nullptr;
			PerDeviceValueStruct = nullptr;
			TotalNumberOfKernelFunctions = 0;
			OrderedKernelArgumentList = nullptr;
			MultiKernelFunction_MultiDevice = nullptr;

			IsConstructionSuccesful = false;
			IsSuccesful = false;

			OCLW_P::cl_PlatformVendorStruct PlatformToUse(IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Construction of 'PlatformToUse' Variable of type 'OCLW_P::cl_PlatformVendorStruct' Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				std::cout << "\n Error Construction Failed cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}

			int TotalNumberOfPlatforms;
			PlatformToUse.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error GetTotalNumberOfPlatformVendors() Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				std::cout << "\n Error Construction Failed cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}
			PlatformToUse.PrintAllAvailablePlatformVendorNames(IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error PrintAllAvailablePlatformVendorNames() Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				std::cout << "\n Error Construction Failed cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}
			PlatformToUse.SetChosenPlatform(1, IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error SetChosenPlatform() Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				std::cout << "\n Error Construction Failed cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}

			cl_uint PlatformNumberToUse = 0;
			PlatformToUse.GetChosenPlatform(PlatformNumberToUse, IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error PlatformToUse->GetChosenPlatform() Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}

			// Single Program Is First Initialized	
			InitializeOpenCLProgram(IsSuccesful, PlatformNumberToUse);

			if (IsSuccesful)
			{
				std::string ClSourceFileInString;
				Essenbp::GetFileContent(ClSourceFilePath, ClSourceFileInString, IsSuccesful);
				if (IsSuccesful)
				{
					FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode(ClSourceFileInString, &OrderedKernelArgumentList, TotalNumberOfKernelFunctions, IsSuccesful);
					if (!IsSuccesful)
					{
						std::cout << "\n Error FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
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
									std::cout << "\n Error FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
									IsSuccesful = false;
								}
							}
							else
							{
								std::cout << "\n Error FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode Failed In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
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
					std::cout << "\n Error Construction Of cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
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
				std::cout << "\n Error Initilization Of cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				ReleaseAndFreeClData();
				return;
			}
			IsConstructionSuccesful = true;
		}

		void PassDataStructToKernel(unsigned int KernelToRunNumber, unsigned int DevicesToSendData_From, unsigned int DevicesToSendData_To, cl_MultiDevice_KernelArgumentSendStruct* MultiDeviceSendStructList, bool& IsSuccesful)
		{
			IsSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling PassDataStructToKernel Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}
			else
			{
				if (MultiDeviceSendStructList == nullptr)
				{
					std::cout << "\n Error :Passed Argument MultiDeviceSendStructList Is nullptr in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					return;
				}

				if (MultiDeviceSendStructList->IsConstructionSuccesful)
				{
					std::cout << "\n Error :Passed Argument MultiDeviceSendStructList Is Not Consturcted in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					return;
				}

				if (KernelToRunNumber < TotalNumberOfKernelFunctions)
				{
					cl_int PossiblePendingArgument = -1;
					
					for (int i = DevicesToSendData_From; i <= DevicesToSendData_To; ++i)
					{
						MultiDeviceSendStructList->MultiArgumentSendStructList[i]->IsDataReadyForSend(IsSuccesful, PossiblePendingArgument);
						if (!IsSuccesful)
						{
							if (PossiblePendingArgument == -1)
							{
								std::cout << "\n Error Unconstructed ArgumentList variable of type cl_KernelArgumentSendStruct& Passed in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
							}
							else
							{
								std::cout << "\n Error ArgumentList's argument Number " << PossiblePendingArgument << " Is not Set! in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
							}
							return;
						}
					}

					IsSuccesful = false;
					if (DevicesToSendData_From > DevicesToSendData_To)
					{
						std::cout << "\n Error DevicesToSendData_From Is greater than DevicesToSendData_To! in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						return;
					}
					else
					{
						if (DevicesToSendData_To < NumberOfDevices)
						{
							for (int i = DevicesToSendData_From; i <= DevicesToSendData_To; ++i)
							{
								MultiDeviceSendStructList->MultiArgumentSendStructList[i]->PassAllDataToKernel(MultiKernelFunction_MultiDevice[KernelToRunNumber]->MultiDeviceKernelArgumentsArray[i],IsSuccesful, PossiblePendingArgument);
								if (!IsSuccesful)
								{
									std::cout << "\n Error cl_KernelArgumentSendStruct::PassAllDataToKernel failed in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
									return;
								}
							}
							return;// Finish
						}
						else
						{
							std::cout << "\n Error DevicesToSendData_To Exceeds the Number Of Devices Present! in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
							return;
						}
					}					
				}
				else
				{
					std::cout << "\n Error KernelToRunNumber Exceeds the Total Number Of Kernel Functions Present! in PassDataStructToKernel In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					return;
				}
			}
		}

		void PassDataStructToKernel(std::string NameOfTheKernelToRun, unsigned int DevicesToSendData_From, unsigned int DevicesToSendData_To, cl_MultiDevice_KernelArgumentSendStruct* MultiDeviceSendStructList, bool& IsSuccesful)
		{
			IsSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling PassDataStructToKernel Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}
			unsigned int KernelNumber = 0;
			GetKernelNumberByKernelName(KernelNumber, NameOfTheKernelToRun, IsSuccesful);
			if (IsSuccesful)
			{
				PassDataStructToKernel(KernelNumber, DevicesToSendData_From, DevicesToSendData_To, MultiDeviceSendStructList, IsSuccesful);
				return;
			}
			else
			{
				//std::cout << "\n OpenCL Kernel by the name " << NameOfTheKernelToRun << " Is Not Found In the Provided Program!\n";
				//std::cout << "\n Please Check for missed letters, symbols or numbers in the Function Name!\n";
			}
		}

		void RunKernelFunction(unsigned int KernelToRunNumber, unsigned int DevicesToRunKernel_From, unsigned int DevicesToRunKernel_To, cl_MultiDevice_NDRangeStruct* MultiDeviceNDRange, bool& IsSuccesful, cl_MultiDevice_KernelArgumentSendStruct* MultiDeviceSendStructList = nullptr)
		{
			IsSuccesful = false;

			if (!IsConstructionSuccesful)
			{
				std::cout << "\n Error Calling RunKernelFunction Without Constructing the struct In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				return;
			}
			else
			{
				if (KernelToRunNumber < TotalNumberOfKernelFunctions)
				{					
					if (DevicesToRunKernel_From > DevicesToRunKernel_To)
					{
						std::cout << "\n Error DevicesToRunKernel_From Is greater than DevicesToRunKernel_To! in RunKernelFunction In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
						return;
					}
					else
					{
						if (DevicesToRunKernel_To < NumberOfDevices)
						{
							if (MultiDeviceSendStructList != nullptr)
							{
								PassDataStructToKernel(KernelToRunNumber, DevicesToRunKernel_From, DevicesToRunKernel_To, MultiDeviceSendStructList, IsSuccesful);
							}

							//PENDING
							cl_NDRangeStruct* PointerToNDRangeStruct = nullptr;
							for (int i = DevicesToRunKernel_From; i <= DevicesToRunKernel_To; ++i)
							{
								PointerToNDRangeStruct = MultiDeviceNDRange->GetNDRangeOfDevice(i, IsSuccesful);
								if (!IsSuccesful)
								{
									std::cout << "\n Error MultiDeviceNDRange.GetNDRangeOfDevice() for Device '"<< i <<"' Failed in RunKernelFunction In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
									return;
								}
								MultiKernelFunction_MultiDevice[KernelToRunNumber]->RunKernel(i, PointerToNDRangeStruct, IsSuccesful);
							}
							
							IsSuccesful = false;
						}
						else
						{
							std::cout << "\n Error DevicesToRunKernel_To Exceeds the Number Of Devices Present! in RunKernelFunction In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
							return;
						}
					}
				}
				else
				{
					std::cout << "\n Error KernelToRunNumber Exceeds the Total Number Of Kernel Functions Present! in RunKernelFunction In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
					return;
				}
			}
		}

		void RunKernelFunction(std::string NameOfTheKernelToRun, unsigned int DevicesToRunKernel_From, unsigned int DevicesToRunKernel_To, cl_MultiDevice_NDRangeStruct* MultiDeviceNDRange, bool& IsSuccesful, cl_MultiDevice_KernelArgumentSendStruct* MultiDeviceSendStructList = nullptr)
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
				RunKernelFunction(KernelNumber, DevicesToRunKernel_From, DevicesToRunKernel_To, MultiDeviceNDRange, IsSuccesful, MultiDeviceSendStructList);
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
				std::cout << "\n Error Allocating " << sizeof(char) << " Byes Of Memory for CharBinary_Of_CLProgram In GetBinaryInformationOfProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
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
		cl_KernelFunctionArgumentOrderListStruct* GetKernelInformation(unsigned int KernelNumber, bool& IsSuccesful)
		{
			IsSuccesful = false;
			if (IsConstructionSuccesful)
			{
				if (KernelNumber < TotalNumberOfKernelFunctions)
				{
					return OrderedKernelArgumentList[KernelNumber];
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
			return nullptr;
		}
		cl_KernelFunctionArgumentOrderListStruct* GetKernelInformation(std::string NameOfTheKernel, bool& IsSuccesful)
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
				ReleaseAndFreeClData();
				for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
				{
					delete PerDeviceValueStruct[i];
					delete OrderedKernelArgumentList[i];
					delete MultiKernelFunction_MultiDevice[i];
				}
				free(PerDeviceValueStruct);
				free(OrderedKernelArgumentList);
				free(MultiKernelFunction_MultiDevice);
			}
			IsConstructionSuccesful = false;
		}
	};

	// NOTE: This Is the shortend Name, Does not matter if you use the struct name directly
	typedef cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct OpenCLWrapper;
};
#else
#endif // !OPENCL_WRAPPER_BY_PUNAL