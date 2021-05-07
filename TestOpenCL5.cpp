/********************************************************************************************************************************
// This Code is Written by Punal Manalan
// Use this code wherever and whenever you want to!

// Information About the Code:
// This is a LightWeight, Easy to use Simple Wrapper for OpenCL
// From the Constrction to Destruction everything is Done automaticaly!
// No need for manual destruction and releasing cl resources!
// No need to write tedious amount of code to care memory leaks!
// Upoing going out of scope or exiting the program Every resource used is safely released and deleted
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
	std::cout << "Error: Unable to Open File to Initialize OpenCL Program!\n";
	return false;
}

struct cl_KernelMemoryStruct
{
	bool IsInitialized = false;// False When no Buffer in on Device, True is when Buffer is stored on Device
	cl_mem MemoryInDevice;

	cl_KernelMemoryStruct()
	{
		std::cout << "\n Constructing cl_KernelMemoryStruct!";		
	}

	~cl_KernelMemoryStruct()
	{
		std::cout << "\n Destroying cl_KernelMemoryStruct!";
		if (IsInitialized)
		{
			cl_int ClErrorResult;
			ClErrorResult = clReleaseMemObject(MemoryInDevice);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n clError " << ClErrorResult << " : Releasing cl_KernelMemoryStruct's Device Memory!";
			}
		}
	}
};

//NOTE: DO NOT USE WITHOUT CALLING THE CONSTRUCTOR FIRST
struct cl_KernelSingleArgumentStruct
{
	const bool IsLocalMemory;
	const bool TrueForReadOnlyFalseForWriteOnly;
	bool TrueForCreateFalseForOverWrite = true;//When True Creates The Buffer
	size_t CL_MemorySizeToCreate = 0;//For Local And Global //NOTE: NO NEED TO Allocate memory for local from host side... memory allocation from host is only required for global	
	void* DataFromHost = nullptr;//For Global and Private Only , Pass NULL for Local...
	size_t INPUTorOUTPUTDataHostSizeToCopySize = 0;//For Global ONLY, Reads Data When TrueForReadOnlyFalseForWriteOnly is true, else writes to buffer on creation
	cl_KernelMemoryStruct BufferOnDevice;//Example: Buffer on GPU device
	//NOTE:IF TrueForCreateFalseForOverWrite is false And INPUTorOUTPUTDataHostSizeToCopySize > CL_MemorySizeToCreate
	//     Memory is released and reallocaed

	//Contructor
	cl_KernelSingleArgumentStruct(bool InitializeIsLocalMemory, bool InitializeTrueForReadOnlyFalseForWriteOnly) : IsLocalMemory(InitializeIsLocalMemory), TrueForReadOnlyFalseForWriteOnly(InitializeTrueForReadOnlyFalseForWriteOnly)
	{
		std::cout << "\n Constructing cl_KernelSingleArgumentStruct!";

		//TrueForCreateFalseForOverWrite = true;
		//CL_MemorySizeToCreate = 0;
		//DataFromHost = nullptr;
		//INPUTorOUTPUTDataHostSizeToCopySize = 0;

	}//const bool Are Initialized in Initialization list 

	//Destructor
	~cl_KernelSingleArgumentStruct()
	{
		std::cout << "\n Destroying cl_KernelSingleArgumentStruct!";
		//BufferOnDevice.~cl_KernelMemoryStruct(); Automatically called upon destruction
	}
};

//NOTE: DO NOT USE WITHOUT CALLING THE CONSTRUCTOR FIRST
struct cl_KernelMultipleArgumentStruct
{
	const unsigned int NumberOfArugments;//Example:	SingleKernelFunctionMultiArgumentsArray[Min: 0,				Max: NumberOfArugments					 - 1]// Is the Maximum Arguments access limit
	const unsigned int NumberOfReads;//				SingleKernelFunctionMultiArgumentsArray[Min: 0,				Max: NumberOfReads					     - 1]// Is the Where the
	const unsigned int NumberOfWrites;//				SingleKernelFunctionMultiArgumentsArray[Min: NumberOfReads, Max: NumberOfReads + NumberOfWrites		 - 1]// Is the Where the
	const unsigned int NumberOfLocals;//				SingleKernelFunctionMultiArgumentsArray[Min: NumberOfWrites,Max: NumberOfWrites + NumberOfLocals	 - 1]// Is the Where the
	//NOTE: For (NumberOfArugments - 1),(NumberOfReads - 1),(NumberOfWrites - 1),(NumberOfLocals - 1) If they are less than Zero than  Force the output of Zero

	bool IsConstructionSuccesful = false;// If you hate Memory leaks, then Don't Even try to manualy change it... If you love memory leaks Please do so!

	cl_KernelSingleArgumentStruct** SingleKernelFunctionMultiArgumentsArray = nullptr;// Arguments stored here

	cl_KernelMultipleArgumentStruct(unsigned int ArgNumberOfReads, unsigned int ArgNumberOfWrites, unsigned int ArgNumberOfLocals) : NumberOfArugments((ArgNumberOfReads + ArgNumberOfWrites + ArgNumberOfLocals)), NumberOfReads(ArgNumberOfReads), NumberOfWrites(ArgNumberOfWrites), NumberOfLocals(ArgNumberOfLocals)
	{
		std::cout << "\n cl_KernelMultipleArgumentStruct!";
		if (NumberOfArugments > 0)
		{
			SingleKernelFunctionMultiArgumentsArray = (cl_KernelSingleArgumentStruct**)malloc(NumberOfArugments * sizeof(cl_KernelSingleArgumentStruct*));
		}
		
		if (SingleKernelFunctionMultiArgumentsArray == nullptr)
		{
			std::cout << "\n Error Allocating " << (NumberOfArugments * sizeof(cl_KernelSingleArgumentStruct*)) << " Byes Of Memory for SingleKernelFunctionMultiArgumentsArray In cl_KernelMultipleArgumentStruct!";
			IsConstructionSuccesful = false;
		}
		else
		{
			IsConstructionSuccesful = true;
			int i = 0;
			int j = NumberOfReads;
			for (i = 0; i < j; ++i)
			{
				SingleKernelFunctionMultiArgumentsArray[i] = new cl_KernelSingleArgumentStruct({ false, true });
				if (SingleKernelFunctionMultiArgumentsArray[i] == nullptr)
				{
					j = i;
					for (i = 0; i < j; ++i)
					{
						delete SingleKernelFunctionMultiArgumentsArray[i];
					}
					free(SingleKernelFunctionMultiArgumentsArray);
					IsConstructionSuccesful = false;
					std::cout << "\n Error Allocating " << sizeof(cl_KernelSingleArgumentStruct) << " Byes Of Memory for SingleKernelFunctionMultiArgumentsArray[i] In cl_KernelMultipleArgumentStruct!";
				}
			}

			if (IsConstructionSuccesful)
			{
				j = NumberOfReads + NumberOfWrites;
				for (i = i; i < j; ++i)
				{
					SingleKernelFunctionMultiArgumentsArray[i] = new cl_KernelSingleArgumentStruct({ false, false });
					if (SingleKernelFunctionMultiArgumentsArray[i] == nullptr)
					{
						j = i;
						for (i = 0; i < j; ++i)
						{
							delete SingleKernelFunctionMultiArgumentsArray[i];
						}
						free(SingleKernelFunctionMultiArgumentsArray);
						IsConstructionSuccesful = false;
						std::cout << "\n Error Allocating " << sizeof(cl_KernelSingleArgumentStruct) << " Byes Of Memory for SingleKernelFunctionMultiArgumentsArray[i] In cl_KernelMultipleArgumentStruct!";
					}
				}

				if (IsConstructionSuccesful)
				{
					j = NumberOfReads + NumberOfWrites + NumberOfLocals;
					for (i = i; i < j; ++i)
					{
						SingleKernelFunctionMultiArgumentsArray[i] = new cl_KernelSingleArgumentStruct({ true, false });// Second arge can either be true of false... because it is a local memory so it does not matter wether second arg is true or false
						if (SingleKernelFunctionMultiArgumentsArray[i] == nullptr)
						{
							j = i;
							for (i = 0; i < j; ++i)
							{
								delete SingleKernelFunctionMultiArgumentsArray[i];
							}
							free(SingleKernelFunctionMultiArgumentsArray);
							IsConstructionSuccesful = false;
							std::cout << "\n Error Allocating " << sizeof(cl_KernelSingleArgumentStruct) << " Byes Of Memory for SingleKernelFunctionMultiArgumentsArray[i] In cl_KernelMultipleArgumentStruct!";
						}
					}
				}
			}						
		}
	}

	~cl_KernelMultipleArgumentStruct()
	{
		std::cout << "\n Destroying cl_KernelMultipleArgumentStruct!";
		if (IsConstructionSuccesful)
		{
			for (int i = 0; i < NumberOfArugments; ++i)
			{
				delete SingleKernelFunctionMultiArgumentsArray[i];
			}
			free(SingleKernelFunctionMultiArgumentsArray);
			IsConstructionSuccesful = false;
		}		
	}
};

//NOTE: DO NOT USE WITHOUT CALLING THE CONSTRUCTOR FIRST
struct cl_MultiDevice_KernelAndArgumentsStruct
{
	const unsigned int NumberOfDevices;
	const std::string KernelFunctionName;
	cl_kernel** MultiDeviceKernelFunctionArray = nullptr;
	cl_KernelMultipleArgumentStruct** MultiDeviceKernelArgumentsArray = nullptr;
	bool IsConstructionSuccesful = false;// Once again i tell you this If you hate memory leaks, Don't change value. If you Love memory leaks change them!

	//NOTE: Kernel Arguments are Ordered from left to right, Read only memory in left, write only in right, Local in middle of them
	//NOTE: any memory which is plain int, float etc etc those which are NOT cl_mem, Will NOT BE INCLUDED HERE instead they will be given as parameters in kernel Host function
	cl_MultiDevice_KernelAndArgumentsStruct(unsigned int ArgNumberOfDevices, unsigned int NumberOfReads, unsigned int NumberOfWrites, unsigned int NumberOfLocal, std::string ArgKernelFunctionName, cl_program* BuiltClProgramContainingTheSpecifiedFunction) : NumberOfDevices(ArgNumberOfDevices),  KernelFunctionName(ArgKernelFunctionName)
	{		
		std::cout << "\n Constructing cl_MultiDevice_KernelAndArgumentsStruct!";
		if (NumberOfDevices > 0)
		{
			MultiDeviceKernelFunctionArray = (cl_kernel**)malloc(NumberOfDevices * sizeof(cl_kernel*));
		}
		
		if (MultiDeviceKernelFunctionArray == nullptr)
		{
			IsConstructionSuccesful = false;
			std::cout << "\n Error Allocating " << (NumberOfDevices * sizeof(cl_kernel*)) << " Byes Of Memory for MultiDeviceKernelFunctionArray In cl_MultiDevice_KernelAndArgumentsStruct!";
		}
		else
		{
			IsConstructionSuccesful = true;

			cl_int ClErrorResult;
			int i = 0;
			for (i = 0; i < NumberOfDevices; ++i)
			{
				MultiDeviceKernelFunctionArray[i] = (cl_kernel*)malloc(sizeof(cl_kernel));				
				if (MultiDeviceKernelFunctionArray[i] != nullptr)
				{
					*(MultiDeviceKernelFunctionArray[i]) = clCreateKernel(*BuiltClProgramContainingTheSpecifiedFunction, KernelFunctionName.c_str(), &ClErrorResult);
				}	
				else
				{
					ClErrorResult = -1;
					std::cout << "\n Malloc Error Allocating" << sizeof(cl_kernel) << "Bytes of Data for Kernel creation For the Device Number: " << i << "in cl_MultiDevice_KernelAndArgumentsStruct! ClError Is Manualy Set to -1, So it is not an actual Cl_Error";
				}

				if (ClErrorResult != CL_SUCCESS)
				{
					IsConstructionSuccesful = false;
					std::cout << "\n clError " << ClErrorResult << " : Creating Kernel For Device Number: " << i << "in cl_MultiDevice_KernelAndArgumentsStruct!";
					for (int j = 0; j < i; ++j)
					{
						ClErrorResult = clReleaseKernel(*(MultiDeviceKernelFunctionArray[j]));
						if (ClErrorResult != CL_SUCCESS)
						{
							std::cout << "\n clError " << ClErrorResult << " : Releasing Kernel For Device Number: " << j << "in cl_MultiDevice_KernelAndArgumentsStruct!";
						}
						free(MultiDeviceKernelFunctionArray[j]);
					}
					free(MultiDeviceKernelFunctionArray[i]);// free the memory of the last one
					free(MultiDeviceKernelFunctionArray);
					break;
				}
			}
		}

		if (IsConstructionSuccesful)// When Kernel is Succesfully created then and only then will this code execute
		{
			MultiDeviceKernelArgumentsArray = (cl_KernelMultipleArgumentStruct**)malloc(NumberOfDevices * sizeof(cl_KernelMultipleArgumentStruct*));
			if (MultiDeviceKernelArgumentsArray == nullptr)
			{
				IsConstructionSuccesful = false;
				std::cout << "\n Error Allocating " << (NumberOfDevices * sizeof(cl_KernelMultipleArgumentStruct*)) << " Byes Of Memory for MultiDeviceKernelArgumentsArray In cl_MultiDevice_KernelAndArgumentsStruct!";
				cl_int ClErrorResult;
				int k = 0;
				for (k = 0; k < NumberOfDevices; ++k)
				{
					ClErrorResult = clReleaseKernel(*(MultiDeviceKernelFunctionArray[k]));
					if (ClErrorResult != CL_SUCCESS)
					{
						std::cout << "\n clError " << ClErrorResult << " : Releasing Kernel For Device Number: " << k << "in cl_MultiDevice_KernelAndArgumentsStruct!";
					}
					free(MultiDeviceKernelFunctionArray[k]);
				}
				free(MultiDeviceKernelFunctionArray);
				free(MultiDeviceKernelArgumentsArray);
			}
			else
			{
				int i = 0;
				for (i = 0; i < NumberOfDevices; ++i)
				{
					MultiDeviceKernelArgumentsArray[i] = new cl_KernelMultipleArgumentStruct({ NumberOfReads, NumberOfWrites, NumberOfLocal });
					if (MultiDeviceKernelArgumentsArray[i] != nullptr)
					{
						if (MultiDeviceKernelArgumentsArray[i]->IsConstructionSuccesful)
						{
							continue;
						}
						else
						{
							delete MultiDeviceKernelArgumentsArray[i];
						}
					}
					//If the return value is a null pointer Or The construction is not Succesful the below code will execute
					IsConstructionSuccesful = false;

					cl_int ClErrorResult;
					for (int k = 0; k < NumberOfDevices; ++k)
					{
						ClErrorResult = clReleaseKernel(*(MultiDeviceKernelFunctionArray[k]));
						if (ClErrorResult != CL_SUCCESS)
						{
							std::cout << "\n clError " << ClErrorResult << " : Releasing Kernel For Device Number: " << k << "in cl_MultiDevice_KernelAndArgumentsStruct!";
						}
						free(MultiDeviceKernelFunctionArray[k]);
					}
					free(MultiDeviceKernelFunctionArray);

					for (int j = 0; j < i; ++j)
					{
						delete MultiDeviceKernelArgumentsArray[j];
					}
					free(MultiDeviceKernelArgumentsArray);
					std::cout << "\n Error Allocating " << sizeof(cl_KernelSingleArgumentStruct) << " Byes Of Memory for MultiDeviceKernelArgumentsArray[i] In cl_MultiDevice_KernelAndArgumentsStruct!";
					break;
				}
			}
		}
	}

	~cl_MultiDevice_KernelAndArgumentsStruct()
	{
		std::cout << "\n Destroying cl_MultiDevice_KernelAndArgumentsStruct!";
		if (IsConstructionSuccesful)
		{
			int i = 0;
			cl_int ClErrorResult;
			for (i = 0; i < NumberOfDevices; ++i)
			{
				ClErrorResult = clReleaseKernel(*(MultiDeviceKernelFunctionArray[i]));
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n clError " << ClErrorResult << " : Releasing Kernel For Device Number: " << i << "in cl_MultiDevice_KernelAndArgumentsStruct!";
				}
				delete MultiDeviceKernelArgumentsArray[i];
				free(MultiDeviceKernelFunctionArray[i]);
			}
			free(MultiDeviceKernelFunctionArray);
			free(MultiDeviceKernelArgumentsArray);
			IsConstructionSuccesful = false;
		}		
	}
};

//A Helper Struct for kernel Creation
struct cl_SingleKernelFunctionConstructionHelper
{
	const std::string KernelFunctionName;
	const unsigned int NumberOfReads;
	const unsigned int NumberOfWrites;
	const unsigned int NumberOfLocals;
	bool IsConstructionSuccesful = false;// This time no memory leaks because this has no pointer

	cl_SingleKernelFunctionConstructionHelper(std::string ArgKernelFunctionName, unsigned int ArgNumberOfReads, unsigned int ArgNumberOfWrites, unsigned int ArgNumberOfLocals) : KernelFunctionName(ArgKernelFunctionName), NumberOfReads(ArgNumberOfReads), NumberOfWrites(ArgNumberOfWrites), NumberOfLocals(ArgNumberOfLocals)
	{
		std::cout << "\n Constructing cl_SingleKernelFunctionConstructionHelper!";
		IsConstructionSuccesful = true;
		//KernelFunctionName = ArgKernelFunctionName;
		//NumberOfReads = ArgNumberOfReads;
		//NumberOfWrites = ArgNumberOfWrites;
		//NumberOfLocals = ArgNumberOfLocals;
	}

	~cl_SingleKernelFunctionConstructionHelper()
	{
		std::cout << "\n Destroying cl_SingleKernelFunctionConstructionHelper!";
	}
};

//NOTE: DO NOT STORE POINTER TO ArrayOfKernelFunction because It is reallocated everytime AddKernelFunction() is called!
struct cl_MultiKernelFunctionConstructionHelper
{
	cl_SingleKernelFunctionConstructionHelper** ArrayOfKernelFunction = nullptr;
	unsigned int NumbersOfKernelStored = 0;

	cl_MultiKernelFunctionConstructionHelper()
	{
		std::cout << "\n Constructing cl_MultiKernelFunctionConstructionHelper!";
	}

	//NOTE: Provide Correct Number and type of Arguments, If provided with incorrect number or type of argument the kernel may or may not run depending on the Code you wrote int he Kernel
	void AddKernelFunction(std::string ArgKernelFunctionName, unsigned int NumberOfReads, unsigned int NumberOfWrites, unsigned int NumberOfLocal)
	{
		if (NumbersOfKernelStored > 0)
		{
			cl_SingleKernelFunctionConstructionHelper** TempReallocationHolder = ArrayOfKernelFunction;
			ArrayOfKernelFunction = (cl_SingleKernelFunctionConstructionHelper**)malloc((NumbersOfKernelStored + 1) * sizeof(cl_SingleKernelFunctionConstructionHelper*));
			if (ArrayOfKernelFunction == nullptr)
			{
				std::cout << "\n Error Allocating " << ((NumbersOfKernelStored + 1) * sizeof(cl_SingleKernelFunctionConstructionHelper*)) << " Byes Of Memory for ArrayOfKernelFunction In cl_MultiKernelFunctionConstructionHelper!";
				ArrayOfKernelFunction = TempReallocationHolder;// Reseting
				return;
			}
			for (int i = 0; i < NumbersOfKernelStored; ++i)
			{
				ArrayOfKernelFunction[i] = TempReallocationHolder[i];// Reallocating pointer
			}
			free(TempReallocationHolder);//Delete Temporary Pointer to Pointer array
			
			ArrayOfKernelFunction[NumbersOfKernelStored] = new cl_SingleKernelFunctionConstructionHelper({ ArgKernelFunctionName , NumberOfReads, NumberOfWrites, NumberOfLocal });
			if (ArrayOfKernelFunction[NumbersOfKernelStored] != nullptr)
			{
				if (ArrayOfKernelFunction[NumbersOfKernelStored]->IsConstructionSuccesful)
				{
					NumbersOfKernelStored = NumbersOfKernelStored + 1;
					return;
				}
				else
				{
					delete ArrayOfKernelFunction[NumbersOfKernelStored];
				}
			}	
			std::cout << "\n Error Allocating " << (sizeof(cl_SingleKernelFunctionConstructionHelper)) << " Byes Of Memory for ArrayOfKernelFunction[NumbersOfKernelStored] In cl_MultiKernelFunctionConstructionHelper!";
			//NOTE: Don't Worry ArrayOfKernelFunction[NumbersOfKernelStored] will be freed on the next call of AddKernelFunction()			
		}
		else
		{
			ArrayOfKernelFunction = (cl_SingleKernelFunctionConstructionHelper**)malloc(sizeof(cl_SingleKernelFunctionConstructionHelper*));
			if (ArrayOfKernelFunction == nullptr)
			{
				std::cout << "\n Error Allocating " << sizeof(cl_SingleKernelFunctionConstructionHelper*) << " Byes Of Memory for ArrayOfKernelFunction] In cl_MultiKernelFunctionConstructionHelper!";
				return;
			}
			ArrayOfKernelFunction[0] = new cl_SingleKernelFunctionConstructionHelper({ ArgKernelFunctionName , NumberOfReads, NumberOfWrites, NumberOfLocal });
			if (ArrayOfKernelFunction[0] != nullptr)
			{
				if (ArrayOfKernelFunction[0]->IsConstructionSuccesful)
				{
					NumbersOfKernelStored = 1;
					return;
				}
				else
				{
					delete ArrayOfKernelFunction[0];
				}
			}
			NumbersOfKernelStored = 0;
			std::cout << "\n Error Allocating " << (sizeof(cl_SingleKernelFunctionConstructionHelper)) << " Byes Of Memory for ArrayOfKernelFunction[0] In cl_MultiKernelFunctionConstructionHelper!";
		}
	}

	~cl_MultiKernelFunctionConstructionHelper()
	{
		std::cout << "\n Destroying cl_MultiKernelFunctionConstructionHelper!";
		for (int i = 0; i < NumbersOfKernelStored; ++i)//No Need for IsInitialized because NumbersOfKernelStored = 0 would always be zero when not Initialized, unless we are calling the destructor From a malloc of this... please don't do it because it will cause a memory leak that can't be fixed... 
		{
			delete ArrayOfKernelFunction[i];
		}
		if (ArrayOfKernelFunction != nullptr)
		{
			free(ArrayOfKernelFunction);
		}
		NumbersOfKernelStored = 0;
	}
};

//NOTE: Do not Use without constructing first
struct cl_PerDeviceValuesStruct
{
	const cl_device_id SelectedDevice;
	cl_command_queue DeviceClCommandQueue;
	size_t MaxComputeUnitPerGPU = 0;
	size_t MaxWorkItemPerGroup = 0;
	size_t MaxGlobalMemoryOfDevice = 0;
	size_t MaxPrivateMemoryBytesPerWorkGroup = 0;
	size_t MaxLocalMemoryBytesPerWorkGroup = 0;
	bool IsConstructionSuccesful = false;// Constructor argument cl_int& CLStatus returns our succes value so 'IsConstructionSuccesful' Is only needed when exiting program

	cl_PerDeviceValuesStruct(cl_device_id ArgSelectedDevice, cl_context* TheClContext, cl_int& CLStatus) : SelectedDevice(ArgSelectedDevice)
	{
		std::cout << "\n Constructing cl_PerDeviceValuesStruct!";

		DeviceClCommandQueue = clCreateCommandQueue(*TheClContext, SelectedDevice, 0, &CLStatus);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error: Cl Command Queue Creation Failed!\n";
			return;
		}

		cl_uint Temp1 = 0;
		size_t Temp2 = 0;
		cl_ulong Temp3 = 0;// Calloc Called Below

		CLStatus = clGetDeviceInfo(SelectedDevice, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &Temp1, NULL);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error Code " << CLStatus << " : Device Get CL_DEVICE_MAX_COMPUTE_UNITS Info Failed!\n";
			clReleaseCommandQueue(DeviceClCommandQueue);
			return;
		}
		MaxComputeUnitPerGPU = (int)Temp1;
		std::cout << "\nMaxComputeUnitPerGPU:\n";
		std::cout << MaxComputeUnitPerGPU << "\n";

		CLStatus = clGetDeviceInfo(SelectedDevice, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &Temp2, NULL);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error Code " << CLStatus << " : Device Get CL_DEVICE_MAX_WORK_GROUP_SIZE Info Failed!\n";
			clReleaseCommandQueue(DeviceClCommandQueue);
			return;
		}
		MaxWorkItemPerGroup = (int)Temp2;
		std::cout << "\nMaxWorkItemPerGroup:\n";
		std::cout << MaxWorkItemPerGroup << "\n";

		CLStatus = clGetDeviceInfo(SelectedDevice, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &Temp3, NULL);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error Code " << CLStatus << " : Device Get CL_DEVICE_GLOBAL_MEM_SIZE Info Failed!\n";
			clReleaseCommandQueue(DeviceClCommandQueue);
			return;
		}
		MaxGlobalMemoryOfDevice = Temp3;
		std::cout << "\nMaxGlobalMemoryOfDevice:\n";// Total GPU VRAM 
		std::cout << MaxGlobalMemoryOfDevice << "\n";

		CLStatus = clGetDeviceInfo(SelectedDevice, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(cl_ulong), &Temp3, NULL);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error Code " << CLStatus << " : Device Get CL_DEVICE_GLOBAL_MEM_CACHE_SIZE Info Failed!\n";
			clReleaseCommandQueue(DeviceClCommandQueue);
			return;
		}
		MaxPrivateMemoryBytesPerWorkGroup = (int)Temp3;
		std::cout << "\nMaxPrivateMemoryBytesPerWorkGroup:\n";// Per Work Item
		std::cout << MaxPrivateMemoryBytesPerWorkGroup << "\n";

		CLStatus = clGetDeviceInfo(SelectedDevice, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &Temp3, NULL);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error Code " << CLStatus << " : Device Get CL_DEVICE_LOCAL_MEM_SIZE Info Failed!\n";
			clReleaseCommandQueue(DeviceClCommandQueue);
			return;
		}
		MaxLocalMemoryBytesPerWorkGroup = (int)Temp3;
		std::cout << "\nMaxLocalMemoryBytesPerWorkGroup:\n";// Per Work Group
		std::cout << MaxLocalMemoryBytesPerWorkGroup << "\n" << "\n";
		IsConstructionSuccesful = true;
	}

	~cl_PerDeviceValuesStruct()
	{
		std::cout << "\n Destructing cl_PerDeviceValuesStruct!";
		if (IsConstructionSuccesful)
		{
			clReleaseCommandQueue(DeviceClCommandQueue);// Always be sure to Clrelease cl variables!
		}		
	}

};

struct cl_ProgramWith_MultiDevice_Kernel
{
	const char* ClSourceFilePath;
	cl_program SingleProgram;
	cl_context SingleContext;
	unsigned int NumberOfDevices;
	unsigned int NumberOfKernelFunctions = 0;												// Don't try to manually change this unless you know how to do it properly
	cl_MultiDevice_KernelAndArgumentsStruct** MultiDeviceKernelAndArgumentStruct;	// Initialization And Construction functions will take care of it	
	cl_PerDeviceValuesStruct** PerDeviceValueStruct;// Initalized and Constructed in  InitializeOpenCLProgram()

	bool IsConstructionSuccesful = false;// Same as before, Manual changes = memory leaks, Automatic(constructor) Only changes will Obliterate the chances of possible memory leaks

	//Initialization
	bool InitializeOpenCLProgram()
	{
		cl_uint NumOfPlatforms;						//the NO. of platforms
		cl_platform_id ChosenPlatform = nullptr;	//the chosen platform Will only be choosing one platform which will be the first one
		cl_uint	NumberOfGPUs = 0;					//cl_uint	NumberOfGPUorCPU = 0;// We will Only be using GPU here
		cl_device_id* ChosenDevices = nullptr;		// Temporary Variable

		bool ReturnResult = true;
		cl_int CLStatus = clGetPlatformIDs(0, NULL, &NumOfPlatforms);//clGetPlatformIDs(1, &platform, NULL);// One line code with no checks chooses first platform GPU/CPU(APU if available) in this
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "ClError Code " << CLStatus << " : Getting platforms!\n";
			return false;
		}

		CLStatus = clGetPlatformIDs(1, &ChosenPlatform, NULL);// for the first platform //Single Line code
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "ClError Code " << CLStatus << " : Platform Not set!\n";
			return false;
		}

		CLStatus = clGetDeviceIDs(ChosenPlatform, CL_DEVICE_TYPE_GPU, 0, NULL, &NumberOfGPUs);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "ClError Code " << CLStatus << " : Getting GPUs!\n";
			return false;
		}
		std::cout << "\n\nNumber Of GPUs: " << NumberOfGPUs << "\n";

		if (NumberOfGPUs < 1)
		{
			std::cout << " : NO GPUs Available!!\n";
			return false;
		}

		ChosenDevices = (cl_device_id*)malloc(NumberOfGPUs * sizeof(cl_device_id));
		CLStatus = clGetDeviceIDs(ChosenPlatform, CL_DEVICE_TYPE_GPU, NumberOfGPUs, ChosenDevices, NULL);// Same line for code for Multi-GPU
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "ClError Code " << CLStatus << " : GPUs Not set!\n";
			return false;
		}

		SingleContext = clCreateContext(NULL, NumberOfGPUs, ChosenDevices, NULL, NULL, &CLStatus);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "ClError Code " << CLStatus << " : Context Not Created!\n";
			return false;
		}

		PerDeviceValueStruct = (cl_PerDeviceValuesStruct**)malloc(NumberOfGPUs * sizeof(cl_PerDeviceValuesStruct*));
		for (int i = 0; i < NumberOfGPUs; i++)
		{
			//Create a command queue for every device
			PerDeviceValueStruct[i] = new cl_PerDeviceValuesStruct({ ChosenDevices[i], &SingleContext, CLStatus });
			if ((CLStatus != CL_SUCCESS) || (PerDeviceValueStruct[i] == nullptr))
			{
				std::cout << "\n Error Allocating " << (NumberOfGPUs * sizeof(cl_PerDeviceValuesStruct*)) << " Byes Of Memory for PerDeviceValueStruct In InitializeOpenCLProgram In cl_ProgramWith_MultiDevice_Kernel!";
				for (int j = 0; j < i; i++)
				{
					delete(PerDeviceValueStruct[i]);
				}
				free(PerDeviceValueStruct);
				free(ChosenDevices);//Temp Variable Is Freed
				return false;
			}
		}

		//const char* ClSourceFilePath = "D:\\C++ Test Projects\\TestOpenCL4\\PunalManalanOpenCLKernelFunctions.cl";
		std::string ClSourceFileInString;
		if (GetFileContent(ClSourceFilePath, ClSourceFileInString))
		{
			const char* ClSourceFileInChar = ClSourceFileInString.c_str();
			size_t ClSourceFileInCharSize[] = { strlen(ClSourceFileInChar) };
			SingleProgram = clCreateProgramWithSource(SingleContext, 1, &ClSourceFileInChar, ClSourceFileInCharSize, &CLStatus);

			if (CLStatus != CL_SUCCESS)
			{
				std::cout << "ClError Code " << CLStatus << " : Cl Program Not Created with Source!\n";
				for (int i = 0; i < NumberOfGPUs; i++)
				{
					delete(PerDeviceValueStruct[i]);
				}
				free(PerDeviceValueStruct);
				free(ChosenDevices);//Temp Variable Is Freed
				return false;
			}

			CLStatus = clBuildProgram(SingleProgram, 0, NULL, NULL, NULL, NULL);
			free(ChosenDevices);//Temp Variable Is Freed
			if (CLStatus != CL_SUCCESS)
			{
				std::cout << "ClError Code " << CLStatus << " : Cl Program Not Built!\n";
				for (int i = 0; i < NumberOfGPUs; i++)
				{
					delete(PerDeviceValueStruct[i]);
				}
				free(PerDeviceValueStruct);
				return false;
			}
			NumberOfDevices = NumberOfGPUs;
			return true;
		}
		//else
		return false;

		//Use this code below if you want to have binary information
		//if (false)
		//{
		//	//std::string ClSourceFileInString;
		//	//GetFileContent(ClSourceFilePath, ClSourceFileInString);
		//	//const char* ClSourceFileInChar = ClSourceFileInString.c_str();
		//	//SingleProgram = clCreateProgramWithSource(SingleContext, 1, &ClSourceFileInChar, NULL, NULL);
		//	//clBuildProgram(SingleProgram, 1, AnyDevice , "", NULL, NULL);\
		//
		//	//size_t binary_size;
		//	//clGetProgramInfo(SingleProgram, CL_PROGRAM_BINARY_SIZES, sizeof(size_t), &binary_size, NULL);
		//	//char* binary = (char*)malloc(binary_size);// sizeof(char) * binary_size would be not be needed
		//	//clGetProgramInfo(SingleProgram, CL_PROGRAM_BINARIES, binary_size, &binary, NULL);
		//			
		//	//clBuildProgram(binary_program, 1, &device, NULL, NULL, NULL);
		//	//binary_kernel = clCreateKernel(binary_program, "kmain", &errcode_ret);
		//	//f = fopen(YOUR_BIN_STORAGE_PATH, "w");// Path to write the binary data into...
		//	//fwrite(binary, binary_size, 1, f);
		//	//fclose(f);
		//}		
	}

	cl_ProgramWith_MultiDevice_Kernel(cl_MultiKernelFunctionConstructionHelper& ArrayOfKernelsAndTheirArugmentsNeededToAdd, std::string ClSourceFilePath) : ClSourceFilePath(ClSourceFilePath.c_str())
	{
		std::cout << "\n Constructing cl_ProgramWith_MultiDevice_Kernel!";

		// Single Program Is First Initialized
		IsConstructionSuccesful = InitializeOpenCLProgram();		
		//CONTINUE THIS

		if (IsConstructionSuccesful)
		{
			MultiDeviceKernelAndArgumentStruct = (cl_MultiDevice_KernelAndArgumentsStruct**)malloc(ArrayOfKernelsAndTheirArugmentsNeededToAdd.NumbersOfKernelStored * sizeof(cl_MultiDevice_KernelAndArgumentsStruct*));
			if (MultiDeviceKernelAndArgumentStruct == nullptr)
			{
				std::cout << "\n Error Allocating " << (ArrayOfKernelsAndTheirArugmentsNeededToAdd.NumbersOfKernelStored * sizeof(cl_MultiDevice_KernelAndArgumentsStruct*)) << " Byes Of Memory for MultiDeviceKernelAndArgumentStruct In cl_ProgramWith_MultiDevice_Kernel!";
				IsConstructionSuccesful = false;
			}
			else
			{
				IsConstructionSuccesful = true;
				NumberOfKernelFunctions = ArrayOfKernelsAndTheirArugmentsNeededToAdd.NumbersOfKernelStored;
				for (int i = 0; i < NumberOfKernelFunctions; ++i)
				{
					MultiDeviceKernelAndArgumentStruct[i] = new cl_MultiDevice_KernelAndArgumentsStruct({ NumberOfDevices , ArrayOfKernelsAndTheirArugmentsNeededToAdd.ArrayOfKernelFunction[i]->NumberOfReads, ArrayOfKernelsAndTheirArugmentsNeededToAdd.ArrayOfKernelFunction[i]->NumberOfWrites, ArrayOfKernelsAndTheirArugmentsNeededToAdd.ArrayOfKernelFunction[i]->NumberOfLocals, ArrayOfKernelsAndTheirArugmentsNeededToAdd.ArrayOfKernelFunction[i]->KernelFunctionName, &SingleProgram });

					if (MultiDeviceKernelAndArgumentStruct[i] != nullptr)
					{
						if (MultiDeviceKernelAndArgumentStruct[i]->IsConstructionSuccesful)
						{
							continue;
						}
						else
						{
							delete MultiDeviceKernelAndArgumentStruct[i];
						}
					}
					//else the code below will clean up
					std::cout << "\n Error Allocating " << (sizeof(cl_MultiDevice_KernelAndArgumentsStruct)) << " Byes Of Memory for MultiDeviceKernelAndArgumentStruct[i] In cl_ProgramWith_MultiDevice_Kernel!";
					IsConstructionSuccesful = false;

					for (int j = 0; j < i; ++j)
					{
						delete MultiDeviceKernelAndArgumentStruct[j];
					}
					free(MultiDeviceKernelAndArgumentStruct);
					NumberOfKernelFunctions = 0;
					IsConstructionSuccesful = false;
					break;
				}
			}
		}
		else
		{
			std::cout << "\n Construction Of cl_ProgramWith_MultiDevice_Kernel Failed creating cl_program from source for SingleProgram!";
		}
	}

	//If Either of DevicesToRunKernelFrom/To is set to -1 Then All the Devices(GPU) Will Run
	void RunKernelFunction(unsigned int KernelToRunNumber, int DevicesToRunKernelFrom, int DevicesToRunKernelTo)
	{

	}

	void RunKernelFunction(std::string NameOfTheKernelToRun, int DevicesToRunKernelFrom, int DevicesToRunKernelTo)
	{
		for (int i = 0; i < NumberOfKernelFunctions; ++i)
		{
			if (MultiDeviceKernelAndArgumentStruct[i]->KernelFunctionName == NameOfTheKernelToRun)
			{
				RunKernelFunction(i, DevicesToRunKernelFrom, DevicesToRunKernelTo);
				return;
			}
		}	
		std::cout << "\n Cl Kernel by the name " << NameOfTheKernelToRun << " Is Not Found!";
		std::cout << "\n Please Check whether you added the function in AddKernelFunction()!";
	}

	~cl_ProgramWith_MultiDevice_Kernel()
	{
		std::cout << "\n Destroying cl_ProgramWith_MultiDevice_Kernel!";
		if (IsConstructionSuccesful)
		{
			cl_int ClErrorResult;
			ClErrorResult = clReleaseProgram(SingleProgram);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n clError " << ClErrorResult << " : Releasing cl_program " << "in cl_ProgramWith_MultiDevice_Kernel!";
			}
			ClErrorResult = clReleaseContext(SingleContext);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n clError " << ClErrorResult << " : Releasing cl_context" << "in cl_ProgramWith_MultiDevice_Kernel!";
			}
			for (int i = 0; i < NumberOfKernelFunctions; ++i)
			{
				delete MultiDeviceKernelAndArgumentStruct[i];
			}
			free(MultiDeviceKernelAndArgumentStruct);
			IsConstructionSuccesful = false;
		}
	}
};

int main()
{
	//Testing 
	cl_MultiKernelFunctionConstructionHelper TestKernels;
	TestKernels.AddKernelFunction("Add_Integers",2,1,0);
	cl_ProgramWith_MultiDevice_Kernel TestProgram{ TestKernels , "ASD"};

	std::cout << "Before End";	
	std::cout << "End Of program exe";
}