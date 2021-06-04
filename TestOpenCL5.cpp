/********************************************************************************************************************************
OpenCL Wrapper By Punal Manalan
Use this code wherever and whenever you want to!

Information About This Wrapper:
1. This is a LightWeight, Easy to use Simple Wrapper for OpenCL.
2. This Wrapper specializes in Heavy Computation on GPU(or CPU).
3. From Constrction to Destruction everything is Done automaticaly!
4. No need for manual destruction and releasing cl resources!
5. No need to write Tedious amount of code to counter memory leaks!
6. Upon going out of scope or exiting the program Every resource that is used is safely Released and Deleted.
*********************************************************************************************************************************/

#include <iostream>
#include <fstream>// For file reading

#define __CL_ENABLE_EXCEPTIONS
//#define CL_API_SUFFIX__VERSION_1_2
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL/cl.h>// OpenCl

#include <chrono>// Mainly For FRAMERATE(FPS) LOCK
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

uint64_t StartTime;
void TimeCalculationInMilliseconds()
{
	std::cout << '\n';
	std::cout << "      StartTime :" << StartTime << '\n';
	std::cout << "        EndTime :" << TimeSinceEpochInMilliSecond() << '\n';
	std::cout << "Time Difference :" << TimeSinceEpochInMilliSecond() - StartTime << '\n' << '\n';
	StartTime = TimeSinceEpochInMilliSecond();
}

void TimeCalculationInNanoSeconds()
{
	std::cout << '\n';
	std::cout << "      StartTime :" << StartTime << '\n';
	std::cout << "        EndTime :" << TimeSinceEpochInNanoSecond() << '\n';
	std::cout << "Time Difference :" << TimeSinceEpochInNanoSecond() - StartTime << '\n' << '\n';
	StartTime = TimeSinceEpochInNanoSecond();
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

bool GetFileContent(const std::string Path, std::string& DataStorage)
{
	std::ifstream file(Path);
	if (file.is_open())
	{
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		DataStorage = content;
		return true;
	}
	std::cout << "\nError: Unable to Open File to Initialize OpenCL Program!\n";
	return false;
}

enum cl_Memory_Type//NOTE: This is a Enum Based On CL_MEM_READ_ONLY, CL_MEM_WRITE_ONLY, CL_MEM_READ_WRITE for buffer creation in the cl_device
{
	Uninitialized_cl_Memory = NULL,	
	CL_READ_ONLY = CL_MEM_READ_ONLY,
	CL_WRITE_ONLY = CL_MEM_WRITE_ONLY,
	CL_READ_AND_WRITE = CL_MEM_READ_WRITE,
	CL_LOCALENUM = 5,// Not To be confused with CL_LOCAL
	CL_PRIVATE = 3
};

struct cl_KernelFunctionArgumentOrderListStruct
{
public:
	bool IsConstructionSuccesful = false;// NOTE: Never Change this Manualy not worth the time...
	bool IsThisListUsable = false;

	const std::string KernelFunctionName;
	const unsigned int TotalNumberOfArugments;
	unsigned int NumberOfArgumentsSet = 0;
	int NumberOfReads;			//Min: 0						To		Max: (NumberOfReads			- 1) = Read			Only Data
	int NumberOfWrites;			//Min: NumberOfReads			To		Max: (NumberOfWrites		- 1) = Write		Only Data
	int NumberOfRead_Writes;	//Min: NumberOfWrites			To		Max: (NumberOfRead_Writes	- 1) = Read_Write	Only Data
	int NumberOfLocals;			//Min: NumberOfRead_Writes		To		Max: (NumberOfLocal			- 1) = Local		Only Data
	int NumberOfPrivates;		//Min: NumberOfLocal			To		Max: (NumberOfPrivate		- 1) = Private		Only Data

	cl_Memory_Type** KernelArgumentsInOrder = nullptr;// Manually Set the enum types or use 'FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode(std::string ProgramKernelCode, std::string FunctionName, cl_KernelFunctionArgumentOrderListStruct& OrderedKernelArgumentList)' Function

	cl_KernelFunctionArgumentOrderListStruct(const unsigned int ArgTotalNumberOfArugments, const std::string ArgKernelFunctionName) : TotalNumberOfArugments(ArgTotalNumberOfArugments), KernelFunctionName(ArgKernelFunctionName)
	{
		IsConstructionSuccesful = false;
		IsThisListUsable = false;
		KernelArgumentsInOrder = (cl_Memory_Type**)malloc(TotalNumberOfArugments * sizeof(cl_Memory_Type*));
		if (KernelArgumentsInOrder == nullptr)
		{
			std::cout << "\n Error Allocating " << (TotalNumberOfArugments * sizeof(cl_Memory_Type*)) << " Byes Of Memory for KernelArgumentsInOrder In KernelFunctionArgumentOrderListStruct!\n";
			return;
		}

		for (int i = 0; i < TotalNumberOfArugments; ++i)
		{
			KernelArgumentsInOrder[i] = (cl_Memory_Type*)malloc(TotalNumberOfArugments * sizeof(cl_Memory_Type));
			if (KernelArgumentsInOrder[i] == nullptr)
			{
				std::cout << "\n Error Allocating " << (TotalNumberOfArugments * sizeof(cl_Memory_Type*)) << " Byes Of Memory for KernelArgumentsInOrder In KernelFunctionArgumentOrderListStruct!\n";
				for (int j = 0; j < i; ++j)
				{
					free(KernelArgumentsInOrder[j]);
				}
				free(KernelArgumentsInOrder);
				return;
			}
		}
		IsConstructionSuccesful = true;
	}

	void SetMemoryTypeOfArugment(const unsigned int ArgumentNumber, cl_Memory_Type MemoryType, bool& IsSuccesful)
	{
		IsSuccesful = false;
		if (MemoryType != cl_Memory_Type::Uninitialized_cl_Memory)
		{
			std::cout << "\n Error Trying to Set The MemoryType of the Argument Number'" << ArgumentNumber << "' to cl_Memory_Type::Uninitialized_cl_Memory In SetMemoryTypeOfArugment In: cl_KernelFunctionArgumentOrderListStruct!\n";
			return;
		}
		if (ArgumentNumber < TotalNumberOfArugments)
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

	~cl_KernelFunctionArgumentOrderListStruct()
	{
		if (IsConstructionSuccesful)
		{
			if (KernelArgumentsInOrder != nullptr)
			{
				for (int i = 0; i < TotalNumberOfArugments; ++i)
				{
					free(KernelArgumentsInOrder[i]);
				}
				free(KernelArgumentsInOrder);
			}
		}
	}
};

//NOTE: Do not Use without calling the constructor first
struct cl_PerDeviceValuesStruct
{
	const cl_device_id SelectedDevice;
	cl_command_queue DeviceClCommandQueue;
	size_t MaxComputeUnitPerGPU = 0;
	size_t MaxWorkItemPerGroup = 0;
	size_t MaxGlobalMemoryOfDevice = 0;
	size_t MaxPrivateMemoryBytesPerWorkGroup = 0;
	size_t MaxLocalMemoryBytesPerWorkGroup = 0;
	bool IsConstructionSuccesful = false;// Constructor argument cl_int& ClErrorResult returns our succes value so 'IsConstructionSuccesful' Is only needed when exiting program

	cl_PerDeviceValuesStruct(cl_device_id ArgSelectedDevice, cl_context* TheClContext, cl_int& ClErrorResult) : SelectedDevice(ArgSelectedDevice)
	{
		std::cout << "\n Constructing cl_PerDeviceValuesStruct!";

		DeviceClCommandQueue = clCreateCommandQueue(*TheClContext, SelectedDevice, 0, &ClErrorResult);
		if (ClErrorResult != CL_SUCCESS)
		{
			std::cout << "Error: Cl Command Queue Creation Failed!\n";
			return;
		}

		cl_uint Temp1 = 0;
		size_t Temp2 = 0;
		cl_ulong Temp3 = 0;// Calloc Called Below

		ClErrorResult = clGetDeviceInfo(SelectedDevice, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &Temp1, NULL);
		if (ClErrorResult != CL_SUCCESS)
		{
			std::cout << "Error Code " << ClErrorResult << " : Device Get CL_DEVICE_MAX_COMPUTE_UNITS Info Failed!\n";
			ClErrorResult = clReleaseCommandQueue(DeviceClCommandQueue);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n clError " << ClErrorResult << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!";
			}
			return;
		}
		MaxComputeUnitPerGPU = (int)Temp1;
		std::cout << "\nMaxComputeUnitPerGPU:\n";
		std::cout << MaxComputeUnitPerGPU << "\n";

		ClErrorResult = clGetDeviceInfo(SelectedDevice, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &Temp2, NULL);
		if (ClErrorResult != CL_SUCCESS)
		{
			std::cout << "Error Code " << ClErrorResult << " : Device Get CL_DEVICE_MAX_WORK_GROUP_SIZE Info Failed!\n";
			ClErrorResult = clReleaseCommandQueue(DeviceClCommandQueue);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n clError " << ClErrorResult << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!";
			}
			return;
		}
		MaxWorkItemPerGroup = (int)Temp2;
		std::cout << "\nMaxWorkItemPerGroup:\n";
		std::cout << MaxWorkItemPerGroup << "\n";

		ClErrorResult = clGetDeviceInfo(SelectedDevice, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &Temp3, NULL);
		if (ClErrorResult != CL_SUCCESS)
		{
			std::cout << "Error Code " << ClErrorResult << " : Device Get CL_DEVICE_GLOBAL_MEM_SIZE Info Failed!\n";
			ClErrorResult = clReleaseCommandQueue(DeviceClCommandQueue);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n clError " << ClErrorResult << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!";
			}
			return;
		}
		MaxGlobalMemoryOfDevice = Temp3;
		std::cout << "\nMaxGlobalMemoryOfDevice:\n";// Total GPU VRAM 
		std::cout << MaxGlobalMemoryOfDevice << "\n";

		ClErrorResult = clGetDeviceInfo(SelectedDevice, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(cl_ulong), &Temp3, NULL);
		if (ClErrorResult != CL_SUCCESS)
		{
			std::cout << "Error Code " << ClErrorResult << " : Device Get CL_DEVICE_GLOBAL_MEM_CACHE_SIZE Info Failed!\n";
			ClErrorResult = clReleaseCommandQueue(DeviceClCommandQueue);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n clError " << ClErrorResult << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!";
			}
			return;
		}
		MaxPrivateMemoryBytesPerWorkGroup = (int)Temp3;
		std::cout << "\nMaxPrivateMemoryBytesPerWorkGroup:\n";// Per Work Item
		std::cout << MaxPrivateMemoryBytesPerWorkGroup << "\n";

		ClErrorResult = clGetDeviceInfo(SelectedDevice, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &Temp3, NULL);
		if (ClErrorResult != CL_SUCCESS)
		{
			std::cout << "Error Code " << ClErrorResult << " : Device Get CL_DEVICE_LOCAL_MEM_SIZE Info Failed!\n";
			ClErrorResult = clReleaseCommandQueue(DeviceClCommandQueue);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n clError " << ClErrorResult << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!";
			}
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
			cl_int ClErrorResult = clReleaseCommandQueue(DeviceClCommandQueue);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n clError " << ClErrorResult << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!";
			}
		}
	}

};

//NOTE: Always call this with new operator
struct cl_MemoryStruct
{
private:
	const cl_kernel* TheKernel;
	const cl_uint KernelArgumentNumber;
	const cl_context* cl_ContextForThisArgument;
	const cl_command_queue* cl_CommandQueueForThisArgument;
	const cl_Memory_Type clMemory_Type_Of_Argument;
	cl_mem GlobalMemoryInDevice = nullptr;// Local not needed as it is inaccessible by host..
	void* COPY_OF_PrivateMemoryType = nullptr;
	size_t MemoryInDeviceTotalSizeInBytes = 0;//NOTE: for private pass the sizeof(variable_type)
	size_t MemoryInDevice_Occupied_SizeInBytes = 0;//NOTE: Actually 'MemoryInDeviceTotalSizeInBytes' occupies full space... but by 'MemoryInDevice_Occupied_SizeInBytes' I mean the memory YOU "use". I know this might sound confusing, but here is a simple example; Say you have a box that which can hold 20 pieces of bottles, but we only put in 10 pieces, meaning we used only 10 slot instead fully using 20 slots. 
	
	void BufferCreation(size_t BUFFER_CREATION_ONLY_SizeOfBuffer, bool& IsFunctionSuccesful)
	{
		cl_int ClErrorResult;
		IsFunctionSuccesful = false;
		if (BUFFER_CREATION_ONLY_SizeOfBuffer < 1)
		{
			std::cout << "\n Error Supplied Size of " << BUFFER_CREATION_ONLY_SizeOfBuffer << " Bytes Is Less than 1 Bytes, Pass atleast 1 byte of Data for Buffer Creation" << ": BufferCreation In: cl_MemoryStruct!\n";
			return;
		}

		if (IsConstructionSuccesful)
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
					ClErrorResult = clReleaseMemObject(GlobalMemoryInDevice);// releasing Memory object every time this function is called	
					if (ClErrorResult != CL_SUCCESS)
					{
						std::cout << "\n ClError Code " << ClErrorResult << " : Releasing Memory On device In: cl_MemoryStruct!\n";
						return;
					}
				}

				MemoryInDeviceTotalSizeInBytes = 0;
				MemoryInDevice_Occupied_SizeInBytes = 0;
			}
			else
			{
				std::cout << "\n Error Trying to Releasing Uninitialized_cl_Memory Memory On device In BufferCreation In: cl_MemoryStruct!\n";
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
				if (IsConstructionSuccesful)
				{
					if ((MemoryInDeviceTotalSizeInBytes != 0) || (MemoryInDevice_Occupied_SizeInBytes != 0))
					{
						std::cout << "\n Error Trying to Change the Size of CL_PRIVATE Memory On device In BufferCreation In: cl_MemoryStruct!\n";
						return;
					}
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
				if ((clMemory_Type_Of_Argument == cl_Memory_Type::CL_READ_ONLY) || (clMemory_Type_Of_Argument == cl_Memory_Type::CL_WRITE_ONLY) || (clMemory_Type_Of_Argument == cl_Memory_Type::CL_READ_AND_WRITE))
				{
					GlobalMemoryInDevice = clCreateBuffer(*cl_ContextForThisArgument, clMemory_Type_Of_Argument, BUFFER_CREATION_ONLY_SizeOfBuffer, NULL, &ClErrorResult);
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
				}
				else
				{
					std::cout << "\n Error undefined Enum Passed! Please pass any of these Enums CL_PRIVATE, CL_LOCALENUM, CL_READ_ONLY, CL_WRITE_ONLY, CL_READ_AND_WRITE In BufferCreation In: cl_MemoryStruct!\n";					
				}
				return;
				//break;// Not needed as return is done
			}
		}		
	}

public:
	bool IsConstructionSuccesful = false;	

	cl_MemoryStruct(const cl_Memory_Type ArgclMemory_Type_Of_Argument, const cl_context* Argcl_ContextForThisArgument, const cl_command_queue* Argcl_CommandQueueForThisArgument, const cl_kernel* ArgTheKernel, const cl_uint ArgKernelArgumentNumber) : clMemory_Type_Of_Argument(ArgclMemory_Type_Of_Argument), cl_ContextForThisArgument(Argcl_ContextForThisArgument), cl_CommandQueueForThisArgument(Argcl_CommandQueueForThisArgument), TheKernel(ArgTheKernel), KernelArgumentNumber(ArgKernelArgumentNumber)
	{
		std::cout << "\n Constructing cl_MemoryStruct!\n";
		IsConstructionSuccesful = false;
		bool IsSuccesful = false;
		BufferCreation(1, IsSuccesful);
		if(!IsSuccesful)
		{
			std::cout << "\n Unsuccesful Construction Of cl_MemoryStruct!\n";
			return;
		}
		IsConstructionSuccesful = true;			
	}

	void MemoryAllocationOnDevice(void* PointerToMemoryToCopyFrom, size_t SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, size_t BUFFER_CREATION_ONLY_SizeOfBuffer, bool OverWriteMemory, bool& IsFunctionSuccesful)//Note: If MemorySize is 0, Then no memory is written on the device but memory is created, Note: for CreateOnlyOptionalLargerBufferSize to take effect pass a size larger than SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type
	{
		IsFunctionSuccesful = false;
		cl_int ClErrorResult;

		if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type < 1)
		{
			std::cout << "\n Error Supplied Memory Size of " << SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type << " Bytes Is Less than 1 Bytes, Pass atleast 1 byte of Data" << ": MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
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
				////		std::cout << "Error " << ClErrorResult << " : Default 'Uninitialized_cl_Memory' Enum passed! Please pass any of these Enums CL_PRIVATE, CL_LOCALENUM, CL_READ_ONLY, CL_WRITE_ONLY, CL_READ_AND_WRITE\n";
				////	}
				////}
				if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type <= MemoryInDeviceTotalSizeInBytes)
				{
					if (clMemory_Type_Of_Argument == cl_Memory_Type::CL_PRIVATE)
					{
						if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type != MemoryInDeviceTotalSizeInBytes)
						{
							std::cout << "\n Error Trying to change the Size of Private Variable(NOTE: This is impossible to happen. Because size of variables type does not change...)" << ": OverWriting Buffer In: cl_MemoryStruct!\n";
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
								std::cout << "\n Error Allocating" << SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type * sizeof(char) << "TempDataCarryHelper Variable In: cl_MemoryStruct\n";
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
								std::cout << "\n CRITICAL WARNING Trying To OverWrite Local Memory(This type can not be written or overwritten from host...) So Pass nullptr to the parameter 'PointerToMemoryToCopyFrom'" << ": OverWriting Buffer In: cl_MemoryStruct!\n";
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
					std::cout << "\n Error Size Passed is larger than the Available buffer Space" << ": OverWriting Buffer In: cl_MemoryStruct!\n";
					return;
				}
			}
			else
			{
				std::cout << "\n Error Trying To OverWrite Uninitialized Memory" << ": OverWriting Buffer In: cl_MemoryStruct!\n";
				return;
			}
		}
		else
		{
			if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type > BUFFER_CREATION_ONLY_SizeOfBuffer)
			{
				std::cout << "\n Error Supplied Size Of SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type Excedes the size maximum size specified by BUFFER_CREATION_ONLY_SizeOfBuffer" << " : Creating Buffer In: cl_MemoryStruct!\n";
				return;
			}

			BufferCreation(BUFFER_CREATION_ONLY_SizeOfBuffer, IsFunctionSuccesful);
			if (IsFunctionSuccesful)
			{
				IsFunctionSuccesful = false;// Yes it is set to false So that it can be used for below code
			}
			else
			{
				std::cout << "\n Error Buffer Creation Unsuccesful In MemoryAllocationOnDevice In: cl_MemoryStruct!\n";
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

	void PassBufferToKernel(bool& IsFunctionSuccesful)
	{
		IsFunctionSuccesful = false;
		cl_int ClErrorResult;
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

	void AllocateMemoryAndPassToKernel(void* PointerToMemoryToCopyFrom, size_t SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, size_t BUFFER_CREATION_ONLY_SizeOfBuffer, bool OverWriteMemory, bool& IsFunctionSuccesful)
	{
		bool IsFunctionSuccesful;
		MemoryAllocationOnDevice(PointerToMemoryToCopyFrom, SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, BUFFER_CREATION_ONLY_SizeOfBuffer, OverWriteMemory, IsFunctionSuccesful);
		if (!IsFunctionSuccesful)
		{
			std::cout << "\n Error Memory Allocation On Device was Unsuccesful In AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
		}
		else
		{

			PassBufferToKernel(IsFunctionSuccesful);
			if (!IsFunctionSuccesful)
			{
				std::cout << "\n Error sending buffer to kernel failed In AllocateMemoryAndPassToKernel In: cl_MemoryStruct!\n";
			}
		}
	}

	~cl_MemoryStruct()
	{
		std::cout << "\n Destructing cl_MemoryStruct!";
		if (IsConstructionSuccesful)
		{		
			IsConstructionSuccesful = false;
			if (clMemory_Type_Of_Argument != cl_Memory_Type::Uninitialized_cl_Memory)
			{
				if (MemoryInDeviceTotalSizeInBytes > 0)
				{
					cl_int ClErrorResult;
					ClErrorResult = clReleaseMemObject(GlobalMemoryInDevice);
					if (ClErrorResult != CL_SUCCESS)
					{
						std::cout << "\n clError " << ClErrorResult << " : Releasing cl_MemoryStruct's Device Memory In: cl_MemoryStruct!";
					}
					if (COPY_OF_PrivateMemoryType != nullptr)
					{
						free(COPY_OF_PrivateMemoryType);
					}
				}
			}
		}
	}
};

//NOTE: DO NOT USE WITHOUT CALLING THE CONSTRUCTOR FIRST
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
		IsConstructionSuccesful = false;
		IsDataSendCompleted = false;
		BufferOnDevice = new cl_MemoryStruct(ArgclMemory_Type_Of_Argument, Argcl_ContextForThisArgument, Argcl_CommandQueueForThisArgument, TheKernel, KernelArgumentNumber);
		if (BufferOnDevice == nullptr)
		{			
			std::cout << "\n Error Allocating " << sizeof(cl_MemoryStruct) << " Byes Of Memory for BufferOnDevice In cl_KernelSingleArgumentStruct!";
			return;
		}	
		if (!BufferOnDevice->IsConstructionSuccesful)
		{
			std::cout << "\n Unsuccesful Construction Of BufferOnDevice of type cl_MemoryStruct In cl_KernelSingleArgumentStruct!\n";
			delete BufferOnDevice;
			return;
		}
		IsConstructionSuccesful = true;
	}

	//Do not call this without calling PassDataToThisKernelArgument() first
	void PassDataToDeviceBuffer()
	{
		if (!IsDataSendCompleted)
		{
			BufferOnDevice->MemoryAllocationOnDevice(COPY_OF_DataFromHost, DataFromHostSize, CL_MemorySizeToCreate, !TrueForCreateFalseForOverWrite, IsDataSendCompleted);
			if (!IsDataSendCompleted)
			{
				std::cout << "\n Error New Data Was not sent to buffer In PassDataToDeviceBuffer In: cl_KernelSingleArgumentStruct!\n";
			}
		}
		else
		{
			std::cout << "\n Error Same Data Was already sent to buffer In PassDataToDeviceBuffer In: cl_KernelSingleArgumentStruct!\n";
		}
	}
	
	//NOTE: AUTO_AUDJUST_AND_FIX_SIZE_AND_DATA_COMPLETE_ERROR is by default false, 
	void PassDataToThisKernelArgument(void* DataToPass, size_t SizeOfData,const bool If_Data_Empty_PassZeroForSizeOfData_And_SetThisToFalse, size_t BufferSizeToCreate, bool ARGUMENT_TrueForCreateFalseForOverWrite, bool& IsSuccesful, const bool /*AUTO_AUDJUST_AND_FIX_SIZE_AND_DATA_COMPLETE_ERROR*/ = false)
	{
		const bool AUTO_AUDJUST_AND_FIX_SIZE_AND_DATA_COMPLETE_ERROR = false;//NOTE: This parameter is Deprecated, So in place of it a dummy variable is put
		//NOTE: This Deprecated version works perfectly fine but i do not recommend using it, as you MAY make some kind of mistake while using it
		IsSuccesful = false;
		if (If_Data_Empty_PassZeroForSizeOfData_And_SetThisToFalse)//Yes, Variable name too long, but it Explains what it does
		{
			if (SizeOfData > 0)
			{
				// Deprecated 
				if (AUTO_AUDJUST_AND_FIX_SIZE_AND_DATA_COMPLETE_ERROR)
				{
					DataToPass = calloc(1, sizeof(char));
					SizeOfData = 1;
				}
				else// Error Code
				{
					std::cout << "\n Error Is_Data_Empty is set to true but SizeOfData is Greater than Zero In PassDataToThisKernelArgument In: cl_KernelSingleArgumentStruct!\n";
					return;
				}				
			}
			else
			{
				if (DataToPass != nullptr)
				{
					std::cout << "\n Error Is_Data_Empty is set to true but DataToPass is not set to nullptr In PassDataToThisKernelArgument In: cl_KernelSingleArgumentStruct!\n";
					return;
				}
				else
				{
					// Correct
					DataToPass = calloc(1, sizeof(char));
					SizeOfData = 1;
				}				
			}
		}
		else
		{
			if (SizeOfData < 2)
			{
				std::cout << "\n Error SizeOfData of " << SizeOfData << " bytes Is Less than 2 Bytes In PassDataToThisKernelArgument In: cl_KernelSingleArgumentStruct!\n";
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
						std::cout << "\n Error SizeOfData Exceeds BufferSize OverWrite Buffer Not Possible In PassDataToThisKernelArgument In: cl_KernelSingleArgumentStruct!\n";
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
					std::cout << "\n Error OverWrite Not Possible as IsDataSendCompleted is set to false In PassDataToThisKernelArgument In: cl_KernelSingleArgumentStruct!\n";
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
					std::cout << "\n Error SizeOfData Exceeds BufferSize Create Buffer Not Possible In PassDataToThisKernelArgument cl_KernelSingleArgumentStruct!\n";
					return;
				}				
			}
		}

		void* TempDataCarryHelper = calloc(SizeOfData, sizeof(char));// malloc works great too, but i prefer to use calloc here, NOTE: Char is 1 Byte so using char
		if (TempDataCarryHelper == nullptr)
		{
			std::cout << "\n Error Allocating" << SizeOfData * sizeof(char) << "TempDataCarryHelper Variable In PassDataToThisKernelArgument In: cl_KernelSingleArgumentStruct!\n";
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
			IsConstructionSuccesful = false;
			delete BufferOnDevice;
			if (COPY_OF_DataFromHost != nullptr)
			{
				free(COPY_OF_DataFromHost);
			}			
		}
	}
};

// Always call using new operator
struct cl_KernelMultipleArgumentStruct
{
private:
	bool IsConstructionSuccesful = false;// NOTE: Memory Leaks bad, so do not change this manualy...
	
	const cl_KernelFunctionArgumentOrderListStruct* OrderedListOfArugments;
	cl_KernelSingleArgumentStruct** SingleKernelFunctionMultiArgumentsArray = nullptr;// Arguments stored here

	void CreateKernelSingleArgumentStruct(const cl_context* cl_ContextForThisKernel, const cl_command_queue* Argcl_CommandQueueForThisKernel, const cl_kernel* TheKernel, bool& IsSuccesful)
	{
		IsSuccesful = false;
		for (int i = 0; i < OrderedListOfArugments->TotalNumberOfArugments; ++i)
		{
			SingleKernelFunctionMultiArgumentsArray[i] = new cl_KernelSingleArgumentStruct(*(OrderedListOfArugments->KernelArgumentsInOrder[i]), cl_ContextForThisKernel, Argcl_CommandQueueForThisKernel, TheKernel, i);
			if (SingleKernelFunctionMultiArgumentsArray[i] != nullptr)
			{
				if (SingleKernelFunctionMultiArgumentsArray[i]->IsConstructionSuccesful == false)// ! works great but, '== false' make it readable
				{
					std::cout << "\n Unsuccesful Construction Of SingleKernelFunctionMultiArgumentsArray["<< i <<"] of type CreateKernelSingleArgumentStruct In cl_KernelMultipleArgumentStruct!\n";
					for (int j = 0; j <= i; ++i)// Greater than or equal to i
					{
						delete SingleKernelFunctionMultiArgumentsArray[i];
					}
					free(SingleKernelFunctionMultiArgumentsArray);
					return;
				}
			}
			else
			{
				std::cout << "\n Error Allocating " << sizeof(cl_KernelSingleArgumentStruct) << " Byes Of Memory for CreateKernelSingleArgumentStruct In cl_KernelMultipleArgumentStruct!\n";
				for (int j = 0; j < i; ++i)// Greater than i
				{
					delete SingleKernelFunctionMultiArgumentsArray[i];
				}
				free(SingleKernelFunctionMultiArgumentsArray);
				return;
			}
		}
		IsSuccesful = true;
	}

public:
	cl_KernelMultipleArgumentStruct(const cl_KernelFunctionArgumentOrderListStruct* ArgOrderedListOfArugments,
		const cl_context* Argcl_ContextForThisKernel, const cl_command_queue* Argcl_CommandQueueForThisKernel, const cl_kernel* TheKernel) : OrderedListOfArugments(ArgOrderedListOfArugments)
	{
		std::cout << "\n Constructing cl_KernelMultipleArgumentStruct!";
		IsConstructionSuccesful = false;// Yes this is set to false

		if (OrderedListOfArugments->IsThisListUsable)
		{
			SingleKernelFunctionMultiArgumentsArray = (cl_KernelSingleArgumentStruct**)malloc(OrderedListOfArugments->TotalNumberOfArugments * sizeof(cl_KernelSingleArgumentStruct*));
			if (SingleKernelFunctionMultiArgumentsArray == nullptr)
			{
				std::cout << "\n Error Allocating " << (OrderedListOfArugments->TotalNumberOfArugments * sizeof(cl_KernelSingleArgumentStruct*)) << " Byes Of Memory for SingleKernelFunctionMultiArgumentsArray In cl_KernelMultipleArgumentStruct!";
				return;
			}
		}
		else
		{
			std::cout << "\n Error The OrderedListOfArugments Is Unusable In cl_KernelMultipleArgumentStruct In: cl_KernelMultipleArgumentStruct!\n";
			return;
		}			

		bool IsSuccesful = false;
		int TotalArgumentsCreated = 0;
		CreateKernelSingleArgumentStruct(Argcl_ContextForThisKernel, Argcl_CommandQueueForThisKernel, TheKernel, IsSuccesful);
		if (!IsSuccesful)
		{
			std::cout << "\n Error Single Kernel Argument Failed In: cl_KernelMultipleArgumentStruct!\n";
			return;
		}
		IsConstructionSuccesful = true;
	}

	~cl_KernelMultipleArgumentStruct()
	{
		std::cout << "\n Destructing cl_KernelMultipleArgumentStruct!";
		if (IsConstructionSuccesful)
		{
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

// ALWAYS CALL CONSTRUCTOR(if you are not using 'new' keyword)
// NOTE: Single Device Works Perfectly
struct cl_MultiDevice_KernelFunctionStruct
{
private:
	const unsigned int NumberOfDevices;
	const std::string KernelFunctionName;
	cl_kernel** MultiDeviceKernelFunction;
	cl_KernelMultipleArgumentStruct** MultiDeviceKernelArgumentsArray = nullptr;

public:
	bool IsConstructionSuccesful = false;// Once again i tell you this If you hate memory leaks, Don't change value. If you Love memory leaks change them!
	//NOTE: Kernel Arguments are Ordered from left to right, Read only memory in left, write only in right, Local in middle of them
	//NOTE: any memory which is plain int, float etc etc those which are NOT cl_mem, Will NOT BE INCLUDED HERE instead they will be given as parameters in kernel Host function
	cl_MultiDevice_KernelFunctionStruct(const unsigned int ArgNumberOfDevices, const cl_KernelFunctionArgumentOrderListStruct* ArgOrderedListOfArugments,
		const cl_context* Argcl_ContextForThisKernel, const cl_command_queue* Argcl_CommandQueueForThisKernel,
		std::string ArgKernelFunctionName, cl_program* BuiltClProgramContainingTheSpecifiedFunction) : NumberOfDevices(ArgNumberOfDevices), KernelFunctionName(ArgKernelFunctionName)
	{
		IsConstructionSuccesful = false;
		std::cout << "\n Constructing cl_MultiDevice_KernelFunctionStruct!";
		if (NumberOfDevices > 0)
		{
			MultiDeviceKernelArgumentsArray = (cl_KernelMultipleArgumentStruct**)malloc(NumberOfDevices * sizeof(cl_KernelMultipleArgumentStruct*));
			if (MultiDeviceKernelArgumentsArray == nullptr)
			{
				std::cout << "\n Error Allocating " << (NumberOfDevices * sizeof(cl_KernelMultipleArgumentStruct*)) << " Byes Of Memory for MultiDeviceKernelArgumentsArray In cl_MultiDevice_KernelFunctionStruct!";
				return;
			}

			MultiDeviceKernelFunction = (cl_kernel**)malloc(NumberOfDevices * sizeof(cl_kernel*));
			if (MultiDeviceKernelFunction == nullptr)
			{
				std::cout << "\n Error Allocating " << (NumberOfDevices * sizeof(cl_kernel*)) << " Byes Of Memory for MultiDeviceKernelFunction In cl_MultiDevice_KernelFunctionStruct!";
				free(MultiDeviceKernelArgumentsArray);
				return;
			}

			for (int i = 0; i < NumberOfDevices; ++i)
			{
				MultiDeviceKernelFunction[i] = (cl_kernel*)malloc(sizeof(cl_kernel));
				if(MultiDeviceKernelFunction[i] == nullptr)
				{
					std::cout << "\n Error Allocating " << sizeof(cl_kernel) << " Byes Of Memory for MultiDeviceKernelFunction["<< i <<"] In cl_MultiDevice_KernelFunctionStruct!";
					for (int j = 0; j < i; ++j)
					{
						free(MultiDeviceKernelFunction[j]);
					}
					free(MultiDeviceKernelFunction);
					free(MultiDeviceKernelArgumentsArray);
					return;
				}
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
			*(MultiDeviceKernelFunction[i]) = clCreateKernel(*BuiltClProgramContainingTheSpecifiedFunction, KernelFunctionName.c_str(), &ClErrorResult);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n clError " << ClErrorResult << " : Kernel Creation Failed in cl_MultiDevice_KernelAndArgumentsStruct!\n";
				for (int j = 0; j < i; ++j)
				{
					clReleaseKernel(*(MultiDeviceKernelFunction[j]));
					delete MultiDeviceKernelArgumentsArray[j];					
				}

				for (int j = 0; j < NumberOfDevices; ++j)
				{
					free(MultiDeviceKernelFunction[j]);
				}
				free(MultiDeviceKernelFunction);
				free(MultiDeviceKernelArgumentsArray);
				return;
			}	
			MultiDeviceKernelArgumentsArray[i] = new cl_KernelMultipleArgumentStruct(ArgOrderedListOfArugments, Argcl_ContextForThisKernel, Argcl_CommandQueueForThisKernel, MultiDeviceKernelFunction[i]);
			if (MultiDeviceKernelArgumentsArray[i] == nullptr)
			{
				std::cout << "\n Error Allocating " << sizeof(cl_KernelMultipleArgumentStruct) << " Byes Of Memory for MultiDeviceKernelArgumentsArray[" << i << "] In cl_MultiDevice_KernelFunctionStruct!";
				
				clReleaseKernel(*(MultiDeviceKernelFunction[i]));// Because the above code succesfuly ran so we are releasing this
				for (int j = 0; j < i; ++j)
				{
					clReleaseKernel(*(MultiDeviceKernelFunction[j]));
					delete MultiDeviceKernelArgumentsArray[j];					
				}				

				for (int j = 0; j < NumberOfDevices; ++j)
				{
					free(MultiDeviceKernelFunction[j]);
				}
				free(MultiDeviceKernelFunction);
				free(MultiDeviceKernelArgumentsArray);
				return;
			}
		}
		IsConstructionSuccesful = true;
	}

	~cl_MultiDevice_KernelFunctionStruct()
	{
		if (IsConstructionSuccesful)
		{
			IsConstructionSuccesful = false;
			for (int i = 0; i < NumberOfDevices; ++i)
			{
				clReleaseKernel(*(MultiDeviceKernelFunction[i]));
				delete MultiDeviceKernelArgumentsArray[i];
				free(MultiDeviceKernelFunction[i]);
			}
			free(MultiDeviceKernelFunction);
			free(MultiDeviceKernelArgumentsArray);
		}
	}
};

struct cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct
{
	const std::string ClSourceFilePath;
	cl_program SingleProgram;
	cl_context SingleContext;
	unsigned int NumberOfDevices;
	cl_device_id* ChosenDevices = nullptr;
	unsigned int TotalNumberOfKernelFunctions = 0;											// Don't try to manually change this unless you know how to do it properly
	cl_KernelFunctionArgumentOrderListStruct** OrderedKernelArgumentList = nullptr;		// This Contains All the Kernel Functions information
	cl_MultiDevice_KernelFunctionStruct** MultiDevice_And_MultiKernel = nullptr;		// Initialization And Construction functions will take care of it	
	cl_PerDeviceValuesStruct** PerDeviceValueStruct = nullptr;							// Initalized and Constructed in  InitializeOpenCLProgram()

	bool IsConstructionSuccesful = false;// Same as before, Manual changes = memory leaks, Automatic(constructor) Only changes will Obliterate the chances of possible memory leaks

	//Initialization
	void InitializeOpenCLProgram(bool &IsSuccesful)
	{
		IsSuccesful = false;
		cl_uint NumOfPlatforms;						//the NO. of platforms
		cl_platform_id ChosenPlatform = nullptr;	//the chosen platform Will only be choosing one platform which will be the first one
		cl_uint	NumberOfGPUs = 0;					//cl_uint	NumberOfGPUorCPU = 0;// We will Only be using GPU here

		bool ReturnResult = true;
		cl_int ClErrorResult = clGetPlatformIDs(0, NULL, &NumOfPlatforms);//clGetPlatformIDs(1, &platform, NULL);// One line code with no checks chooses first platform GPU/CPU(APU if available) in this
		if (ClErrorResult != CL_SUCCESS)
		{
			std::cout << "ClError Code " << ClErrorResult << " : Getting platforms in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
		}

		ClErrorResult = clGetPlatformIDs(1, &ChosenPlatform, NULL);// for the first platform //Single Line code
		if (ClErrorResult != CL_SUCCESS)
		{
			std::cout << "ClError Code " << ClErrorResult << " : Platform Not set in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
		}

		ClErrorResult = clGetDeviceIDs(ChosenPlatform, CL_DEVICE_TYPE_GPU, 0, NULL, &NumberOfGPUs);
		if (ClErrorResult != CL_SUCCESS)
		{
			std::cout << "ClError Code " << ClErrorResult << " : Getting GPUs in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
		}
		std::cout << "\n\nTotal Number Of GPUs Available: " << NumberOfGPUs << "\n";

		if (NumberOfGPUs < 1)
		{
			std::cout << "\n : Error NO GPUs Available in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
		}

		ChosenDevices = (cl_device_id*)malloc(NumberOfGPUs * sizeof(cl_device_id));
		ClErrorResult = clGetDeviceIDs(ChosenPlatform, CL_DEVICE_TYPE_GPU, NumberOfGPUs, ChosenDevices, NULL);// Same line for code for Multi-GPU
		if (ClErrorResult != CL_SUCCESS)
		{
			std::cout << "ClError Code " << ClErrorResult << " : GPUs Not set in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
		}

		SingleContext = clCreateContext(NULL, NumberOfGPUs, ChosenDevices, NULL, NULL, &ClErrorResult);
		if (ClErrorResult != CL_SUCCESS)
		{
			free(ChosenDevices);
			std::cout << "ClError Code " << ClErrorResult << " : Context Not Created in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
		}

		PerDeviceValueStruct = (cl_PerDeviceValuesStruct**)malloc(NumberOfGPUs * sizeof(cl_PerDeviceValuesStruct*));
		for (int i = 0; i < NumberOfGPUs; i++)
		{
			//Create a command queue for every device
			PerDeviceValueStruct[i] = new cl_PerDeviceValuesStruct({ ChosenDevices[i], &SingleContext, ClErrorResult });
			if ((ClErrorResult != CL_SUCCESS) || (PerDeviceValueStruct[i] == nullptr))
			{
				std::cout << "\n Error Allocating " << (NumberOfGPUs * sizeof(cl_PerDeviceValuesStruct*)) << " Byes Of Memory for PerDeviceValueStruct In InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				for (int j = 0; j < i; i++)
				{
					delete(PerDeviceValueStruct[i]);
				}
				free(PerDeviceValueStruct);
				free(ChosenDevices);
				ClErrorResult = clReleaseContext(SingleContext);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n clError " << ClErrorResult << " : Releasing cl_context in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				}
			}
		}

		//const char* ClSourceFilePath = "D:\\C++ Test Projects\\TestOpenCL4\\PunalManalanOpenCLKernelFunctions.cl";
		std::string ClSourceFileInString;
		if (GetFileContent(ClSourceFilePath, ClSourceFileInString))
		{
			const char* ClSourceFileInChar = ClSourceFileInString.c_str();
			size_t ClSourceFileInCharSize[] = { strlen(ClSourceFileInChar) };
			SingleProgram = clCreateProgramWithSource(SingleContext, 1, &ClSourceFileInChar, ClSourceFileInCharSize, &ClErrorResult);

			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "ClError Code " << ClErrorResult << " : Cl Program Not Created with Source!\n";
				for (int i = 0; i < NumberOfGPUs; i++)
				{
					delete(PerDeviceValueStruct[i]);
				}
				free(PerDeviceValueStruct);
				free(ChosenDevices);				
				ClErrorResult = clReleaseContext(SingleContext);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n clError " << ClErrorResult << " : Releasing cl_context in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				}
			}

			ClErrorResult = clBuildProgram(SingleProgram, 0, NULL, NULL, NULL, NULL);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "ClError Code " << ClErrorResult << " : Cl Program Not Build in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
				for (int i = 0; i < NumberOfGPUs; i++)
				{
					delete(PerDeviceValueStruct[i]);
				}
				free(PerDeviceValueStruct);
				free(ChosenDevices);
				ClErrorResult = clReleaseContext(SingleContext);
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n clError " << ClErrorResult << " : Releasing cl_context in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				}
			}
			NumberOfDevices = NumberOfGPUs;			
			IsSuccesful = true;
			return;
		}
		//else
		IsSuccesful = false;
	}	

	void ReleaseAndFreeClData()
	{
		cl_int ClErrorResult;
		ClErrorResult = clReleaseProgram(SingleProgram);
		if (ClErrorResult != CL_SUCCESS)
		{
			std::cout << "\n clError " << ClErrorResult << " : Releasing cl_program " << "in cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
		}
		ClErrorResult = clReleaseContext(SingleContext);
		if (ClErrorResult != CL_SUCCESS)
		{
			std::cout << "\n clError " << ClErrorResult << " : Releasing cl_context in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
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
			std::cout << "\n Error The value of the variable 'ArgOrderedKernelArgumentList' Is not nullptr, It should be empty, In FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
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

	void cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper(const std::string ClSourceFilePath, const cl_KernelFunctionArgumentOrderListStruct** ArgOrderedKernelArgumentList, const unsigned int& ArgTotalNumberOfKernelFunctions, bool &IsSuccesful)
	{
		MultiDevice_And_MultiKernel = (cl_MultiDevice_KernelAndArgumentsStruct**)malloc(ArrayOfKernelsAndTheirArugmentsNeededToAdd.NumbersOfKernelStored * sizeof(cl_MultiDevice_KernelAndArgumentsStruct*));
		if (MultiDevice_And_MultiKernel == nullptr)
		{
			std::cout << "\n Error Allocating " << (ArrayOfKernelsAndTheirArugmentsNeededToAdd.NumbersOfKernelStored * sizeof(cl_MultiDevice_KernelAndArgumentsStruct*)) << " Byes Of Memory for MultiDevice_And_MultiKernel In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			IsConstructionSuccesful = false;
		}
		else
		{
			IsConstructionSuccesful = true;
			NumberOfKernelFunctions = ArrayOfKernelsAndTheirArugmentsNeededToAdd.NumbersOfKernelStored;
			for (int i = 0; i < NumberOfKernelFunctions; ++i)
			{
				MultiDevice_And_MultiKernel[i] = new cl_MultiDevice_KernelAndArgumentsStruct({ NumberOfDevices , ArrayOfKernelsAndTheirArugmentsNeededToAdd.ArrayOfKernelFunction[i]->NumberOfReads, ArrayOfKernelsAndTheirArugmentsNeededToAdd.ArrayOfKernelFunction[i]->NumberOfWrites, ArrayOfKernelsAndTheirArugmentsNeededToAdd.ArrayOfKernelFunction[i]->NumberOfLocals, ArrayOfKernelsAndTheirArugmentsNeededToAdd.ArrayOfKernelFunction[i]->KernelFunctionName, &SingleProgram });

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
				std::cout << "\n Error Allocating " << (sizeof(cl_MultiDevice_KernelAndArgumentsStruct)) << " Byes Of Memory for MultiDevice_And_MultiKernel[i] In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				IsConstructionSuccesful = false;

				for (int j = 0; j < i; ++j)
				{
					delete MultiDevice_And_MultiKernel[j];
				}
				free(MultiDevice_And_MultiKernel);
				NumberOfKernelFunctions = 0;
				IsConstructionSuccesful = false;
				break;
			}
		}
	}

	//Manual
	cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct(const std::string ClSourceFilePath, const cl_KernelFunctionArgumentOrderListStruct** ArgOrderedKernelArgumentList, const unsigned int& ArgTotalNumberOfKernelFunctions) : ClSourceFilePath(ClSourceFilePath)
	{
		std::cout << "\n Constructing cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct! Using Manual Constructor";
	}

	//Automatic
	cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct(const std::string ClSourceFilePath) : ClSourceFilePath(ClSourceFilePath)
	{
		std::cout << "\n Constructing cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct! Using Automatic Constructor";
		IsConstructionSuccesful = false;
		bool IsSuccesful = false;
		// Single Program Is First Initialized
		InitializeOpenCLProgram(IsSuccesful);
		//CONTINUE THIS	

		if (IsSuccesful)
		{
			std::string ClSourceFileInString;
			if (GetFileContent(ClSourceFilePath, ClSourceFileInString))
			{				
				FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode(ClSourceFileInString, &OrderedKernelArgumentList, TotalNumberOfKernelFunctions, IsSuccesful);
				if (!IsSuccesful)
				{
					std::cout << "\n Error FindTotalNumberOfKernelsAndNameOfKernelsInTheCLProgramCode Failed In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				}
				for (int i = 0; i < TotalNumberOfKernelFunctions; ++i)
				{
					if (OrderedKernelArgumentList[i]->IsConstructionSuccesful)
					{
						if (OrderedKernelArgumentList[i]->IsThisListUsable)
						{
							FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode(ClSourceFileInString, OrderedKernelArgumentList[i]->KernelFunctionName, *(OrderedKernelArgumentList[i]), IsSuccesful);
						}
						else
						{
							//PENDING RESOURCE RELEASE
							std::cout << "\n Error FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode Failed In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
							ReleaseAndFreeClData();
							return;
						}
					}
					else
					{
						//PENDING RESOURCE RELEASE
						std::cout << "\n Error FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode Failed In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
						return;
					}
					if (!IsSuccesful)
					{
						//PENDING RESOURCE RELEASE
						std::cout << "\n Error FindTheTotalNumberAndTypesOfDataTypeInKernelFunctionCode Failed In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
						ReleaseAndFreeClData();
						return;
					}
				}
			}
			else
			{
				std::cout << "\n Error GetFileContent Did returned false In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
				return;
			}
			cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct_ConstructionHelper(ClSourceFilePath, OrderedKernelArgumentList, TotalNumberOfKernelFunctions, IsSuccesful);
			if (!IsSuccesful)
			{
				std::cout << "\n Error Construction Of cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";
			}
		}
		else
		{
			ReleaseAndFreeClData();
			std::cout << "\n Error Initilization Of cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct In cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!";			
			return;
		}
		IsConstructionSuccesful = true;
	}

	//If Either of DevicesToRunKernelFrom/To is set to -1 Then All the Devices(GPU) Will Run
	void RunKernelFunction(unsigned int KernelToRunNumber, int DevicesToRunKernelFrom, int DevicesToRunKernelTo, bool ChooseOptimalTotalWorkItemsAndGroupSize, int TotalWorkItems, int TotalWorkGroups)
	{

	}

	void RunKernelFunction(std::string NameOfTheKernelToRun, int DevicesToRunKernelFrom, int DevicesToRunKernelTo)
	{
		for (int i = 0; i < NumberOfKernelFunctions; ++i)
		{
			if (MultiDevice_And_MultiKernel[i]->KernelFunctionName == NameOfTheKernelToRun)
			{
				RunKernelFunction(i, DevicesToRunKernelFrom, DevicesToRunKernelTo);
				return;
			}
		}	
		std::cout << "\n Cl Kernel by the name " << NameOfTheKernelToRun << " Is Not Found!";
		std::cout << "\n Please Check whether you added the function in AddKernelFunction()!";
	}

	void GetBinaryInformationOfProgram(size_t& Binary_Size_Of_CLProgram, std::string& Binary_Of_CLProgram, bool& IsSuccesful)
	{
		IsSuccesful = false;
		cl_int ClErrorResult;		
		clGetProgramInfo(SingleProgram, CL_PROGRAM_BINARY_SIZES, sizeof(size_t), &Binary_Size_Of_CLProgram, NULL);
		if (ClErrorResult != CL_SUCCESS)
		{
			std::cout << "ClError Code " << ClErrorResult << " : clGetProgramInfo to get Binary Size Failed in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			return;
		}
		char* CharBinary_Of_CLProgram = (char*)malloc(sizeof(char) * Binary_Size_Of_CLProgram);
		clGetProgramInfo(SingleProgram, CL_PROGRAM_BINARIES, Binary_Size_Of_CLProgram, &CharBinary_Of_CLProgram, NULL);
		if (ClErrorResult != CL_SUCCESS)
		{
			std::cout << "ClError Code " << ClErrorResult << " : clGetProgramInfo to get Binary Information Failed in InitializeOpenCLProgram In: cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct!\n";
			free(CharBinary_Of_CLProgram);
			return;
		}
		Binary_Of_CLProgram = CharBinary_Of_CLProgram;
		free(CharBinary_Of_CLProgram);
		IsSuccesful = true;
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
				delete MultiDevice_And_MultiKernel[i];
			}
			free(MultiDevice_And_MultiKernel);
		}
	}
};

int main()
{
	//Testing 
	cl_MultiDevice_KernelFunctionStruct TestKernels;
	TestKernels.AddKernelFunction("Add_Integers",2,1,0);
	cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct TestProgram{ TestKernels , "D:\\c++ test projects\\testopencl5\\PunalOpenclFunctionsProgram.cl"};

	std::cout << "\nBefore End";
	std::cout << "\nEnd Of program exe";
}