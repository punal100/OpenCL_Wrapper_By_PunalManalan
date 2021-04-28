/**********************************************************************
// This Code is Written by Punal Manalan
**********************************************************************/

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
	std::cout << "Error: Unable to Open File to Initialize OpenCL Program!" << std::endl;
	return false;
}

struct cl_KernelMemoryStruct
{
	bool IsInitialized = false;// False When no Buffer in on Device, True is when Buffer is stored on Device
	cl_mem MemoryInDevice;

	~cl_KernelMemoryStruct()
	{
		std::cout << "\n Destroying cl_KernelMemoryStruct!";
		if (IsInitialized)
		{
			int ClErrorResult;
			ClErrorResult = clReleaseMemObject(MemoryInDevice);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n clError " << ClErrorResult << " : Releasing cl_KernelMemoryStruct's Device Memory!";
			}
		}
	}
};

//NOTE: DO NOT CALL WITHOUT THE CONSTRUCTOR
struct cl_KernelSingleArgumentStruct
{
	const bool IsLocalMemory;
	const bool TrueForReadOnlyFalseForWriteOnly;
	bool TrueForCreateFalseForOverWrite;//When True Creates The Buffer
	size_t CL_MemorySizeToCreate;//For Local And Global //NOTE: NO NEED TO Allocate memory for local from host side... memory allocation from host is only required for global	
	void* DataFromHost;//For Global and Private Only , Pass NULL for Local...
	size_t INPUTorOUTPUTDataHostSizeToCopySize;//For Global ONLY, Reads Data When TrueForReadOnlyFalseForWriteOnly is true, else writes to buffer on creation
	cl_KernelMemoryStruct BufferOnDevice;//Example: Buffer on GPU device
	//NOTE:IF TrueForCreateFalseForOverWrite is false And INPUTorOUTPUTDataHostSizeToCopySize > CL_MemorySizeToCreate
	//     Memory is released and reallocaed

	//Contructor
	cl_KernelSingleArgumentStruct(bool InitializeIsLocalMemory, bool InitializeTrueForReadOnlyFalseForWriteOnly) : IsLocalMemory(InitializeIsLocalMemory), TrueForReadOnlyFalseForWriteOnly(InitializeTrueForReadOnlyFalseForWriteOnly) {}//const bool Are Initialized in Initialization list 

	//Destructor
	~cl_KernelSingleArgumentStruct()
	{
		std::cout << "\n Destroying cl_KernelSingleArgumentStruct!";
		//BufferOnDevice.~cl_KernelMemoryStruct(); Automatically called upon destruction
	}
};

//NOTE: DO NOT CALL WITHOUT THE CONSTRUCTOR
struct cl_KernelMultipleArgumentStruct
{
	const int NumberOfArugments;//Example:	SingleKernelFunctionMultiArgumentsArray[Min: 0,				Max: NumberOfArugments					 - 1]// Is the Maximum Arguments access limit
	const int NumberOfReads;//				SingleKernelFunctionMultiArgumentsArray[Min: 0,				Max: NumberOfReads					     - 1]// Is the Where the
	const int NumberOfWrites;//				SingleKernelFunctionMultiArgumentsArray[Min: NumberOfReads, Max: NumberOfReads + NumberOfWrites		 - 1]// Is the Where the
	const int NumberOfLocals;//				SingleKernelFunctionMultiArgumentsArray[Min: NumberOfWrites,Max: NumberOfWrites + NumberOfLocals	 - 1]// Is the Where the
	//NOTE: For (NumberOfArugments - 1),(NumberOfReads - 1),(NumberOfWrites - 1),(NumberOfLocals - 1) If they are less than Zero than  Force the output of Zero

	bool IsConstructionSuccesful = false;// If you kate Memory leaks, then Don't Even try to manualy change it... If you love memory leaks Please do so!

	cl_KernelSingleArgumentStruct** SingleKernelFunctionMultiArgumentsArray;// Arguments stored here

	cl_KernelMultipleArgumentStruct(int ArgNumberOfReads, int ArgNumberOfWrites, int ArgNumberOfLocals) : NumberOfArugments((ArgNumberOfReads + ArgNumberOfWrites + ArgNumberOfLocals)), NumberOfReads(ArgNumberOfReads), NumberOfWrites(ArgNumberOfWrites), NumberOfLocals(ArgNumberOfLocals)
	{
		SingleKernelFunctionMultiArgumentsArray = (cl_KernelSingleArgumentStruct**)malloc(NumberOfArugments * sizeof(cl_KernelSingleArgumentStruct*));
		if (SingleKernelFunctionMultiArgumentsArray == NULL)
		{
			std::cout << "\n Error Allocating " << (NumberOfArugments * sizeof(cl_KernelSingleArgumentStruct*)) << " Byes Of Memory for SingleKernelFunctionMultiArgumentsArray In cl_KernelMultipleArgumentStruct!";
			IsConstructionSuccesful = false;
		}
		else
		{
			IsConstructionSuccesful = true;
			int i = 0;
			int j = ArgNumberOfReads;
			for (i = 0; i < j; ++i)
			{
				SingleKernelFunctionMultiArgumentsArray[i] = new cl_KernelSingleArgumentStruct({ false, true });
				if (SingleKernelFunctionMultiArgumentsArray[i] == NULL)
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
				j = ArgNumberOfReads + ArgNumberOfWrites;
				for (i = i; i < j; ++i)
				{
					SingleKernelFunctionMultiArgumentsArray[i] = new cl_KernelSingleArgumentStruct({ false, false });
					if (SingleKernelFunctionMultiArgumentsArray[i] == NULL)
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
					j = ArgNumberOfReads + ArgNumberOfWrites + ArgNumberOfLocals;
					for (i = i; i < j; ++i)
					{
						SingleKernelFunctionMultiArgumentsArray[i] = new cl_KernelSingleArgumentStruct({ true, false });// Second arge can either be true of false... because it is a local memory so it does not matter wether second arg is true or false
						if (SingleKernelFunctionMultiArgumentsArray[i] == NULL)
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
		}		
	}
};

//NOTE: DO NOT CALL WITHOUT THE CONSTRUCTOR
struct cl_KernelAndArgumentsStruct
{
	const int NumberOfDevices;
	cl_kernel** MultiDeviceKernelFunctionArray;
	cl_KernelMultipleArgumentStruct** MultiDeviceKernelArgumentsArray;
	bool IsConstructionSuccesful = false;// Once again i tell you this If you hate memory leaks, Don't change value. If you Love memory leaks change them!

	//NOTE: Kernel Arguments are Ordered from left to right, Read only memory in left, write only in right, Local in middle of them
	//NOTE: any memory which is plain int, float etc etc those which are NOT cl_mem, Will NOT BE INCLUDED HERE instead they will be given as parameters in kernel Host function
	cl_KernelAndArgumentsStruct(int ArgNumberOfDevices, int NumberOfReads, int NumberOfWrites, int NumberOfLocal, std::string KernelFunctionName, cl_program* BuiltClProgramContainingTheSpecifiedFunction) : NumberOfDevices(ArgNumberOfDevices)
	{		
		MultiDeviceKernelFunctionArray = (cl_kernel**)malloc(NumberOfDevices * sizeof(cl_kernel*));
		if (MultiDeviceKernelFunctionArray == NULL)
		{
			IsConstructionSuccesful = false;
			std::cout << "\n Error Allocating " << (NumberOfDevices * sizeof(cl_kernel*)) << " Byes Of Memory for MultiDeviceKernelFunctionArray In cl_KernelAndArgumentsStruct!";
		}
		else
		{
			IsConstructionSuccesful = true;
			cl_int ClErrorResult;
			int i = 0;
			for (i = 0; i < NumberOfDevices; ++i)
			{
				*(MultiDeviceKernelFunctionArray[i]) = clCreateKernel(*BuiltClProgramContainingTheSpecifiedFunction, KernelFunctionName.c_str(), &ClErrorResult);
				if (ClErrorResult != CL_SUCCESS)
				{
					IsConstructionSuccesful = false;
					std::cout << "\n clError " << ClErrorResult << " : Creating Kernel For Device Number: " << i << "in cl_KernelAndArgumentsStruct!";
					for (int j = 0; j < i; ++j)
					{
						ClErrorResult = clReleaseKernel(*(MultiDeviceKernelFunctionArray[j]));
						if (ClErrorResult != CL_SUCCESS)
						{
							std::cout << "\n clError " << ClErrorResult << " : Releasing Kernel For Device Number: " << j << "in cl_KernelAndArgumentsStruct!";
						}
					}
					break;
				}
			}
		}

		if (IsConstructionSuccesful)// When Kernel is Succesfully created then and only then will this code execute
		{
			MultiDeviceKernelArgumentsArray = (cl_KernelMultipleArgumentStruct**)malloc(NumberOfDevices * sizeof(cl_KernelMultipleArgumentStruct*));
			if (MultiDeviceKernelArgumentsArray == NULL)
			{
				IsConstructionSuccesful = false;
				std::cout << "\n Error Allocating " << (NumberOfDevices * sizeof(cl_KernelMultipleArgumentStruct*)) << " Byes Of Memory for MultiDeviceKernelArgumentsArray In cl_KernelAndArgumentsStruct!";
				cl_int ClErrorResult;
				int k = 0;
				for (k = 0; k < NumberOfDevices; ++k)
				{
					ClErrorResult = clReleaseKernel(*(MultiDeviceKernelFunctionArray[k]));
					if (ClErrorResult != CL_SUCCESS)
					{
						std::cout << "\n clError " << ClErrorResult << " : Releasing Kernel For Device Number: " << k << "in cl_KernelAndArgumentsStruct!";
					}
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
					if (MultiDeviceKernelArgumentsArray[i] != NULL)
					{
						if (MultiDeviceKernelArgumentsArray[i]->IsConstructionSuccesful)
						{
							continue;
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
							std::cout << "\n clError " << ClErrorResult << " : Releasing Kernel For Device Number: " << k << "in cl_KernelAndArgumentsStruct!";
						}
					}
					free(MultiDeviceKernelFunctionArray);

					int j = i;
					for (i = 0; i < j; ++i)
					{
						delete MultiDeviceKernelArgumentsArray[i];
					}
					free(MultiDeviceKernelArgumentsArray);
					std::cout << "\n Error Allocating " << sizeof(cl_KernelSingleArgumentStruct) << " Byes Of Memory for MultiDeviceKernelArgumentsArray[i] In cl_KernelAndArgumentsStruct!";
					break;
				}
			}
		}
	}

	~cl_KernelAndArgumentsStruct()
	{
		std::cout << "\n Destroying cl_KernelAndArgumentsStruct!";
		if (IsConstructionSuccesful)
		{
			int i = 0;
			cl_int ClErrorResult;
			for (i = 0; i < NumberOfDevices; ++i)
			{
				ClErrorResult = clReleaseKernel(*(MultiDeviceKernelFunctionArray[i]));
				if (ClErrorResult != CL_SUCCESS)
				{
					std::cout << "\n clError " << ClErrorResult << " : Releasing Kernel For Device Number: " << i << "in cl_KernelAndArgumentsStruct!";
				}
				delete MultiDeviceKernelArgumentsArray[i];
			}
			free(MultiDeviceKernelFunctionArray);
			free(MultiDeviceKernelArgumentsArray);
		}		
	}
};

cl_uint NumOfPlatforms;	//the NO. of platforms
cl_platform_id ChosenPlatform = NULL;	//the chosen platform
cl_uint	NumberOfGPUs = 0;//cl_uint	NumberOfGPUorCPU = 0;
cl_device_id* ChosenGPUs = NULL;
cl_context SingleClContext;
cl_command_queue* ClCommandQueues = NULL;
size_t* MaxComputeUnitPerGPU = NULL;
size_t* MaxWorkItemPerGroup = NULL;
size_t* MaxGlobalMemoryOfDevice = NULL;
size_t* MaxPrivateMemoryBytesPerWorkGroup = NULL;
size_t* MaxLocalMemoryBytesPerWorkGroup = NULL;
cl_program AllCustomClFunctionsProgram = NULL;

bool InitializeOpenCLProgram()
{
	bool ReturnResult = true;
	cl_int CLStatus = clGetPlatformIDs(0, NULL, &NumOfPlatforms);//clGetPlatformIDs(1, &platform, NULL);// One line code with no checks chooses first platform GPU/CPU(APU if available) in this
	if (CLStatus != CL_SUCCESS)
	{
		std::cout << "Error Code " << CLStatus << " : Getting platforms!" << std::endl;
		return false;
	}

	CLStatus = clGetPlatformIDs(1, &ChosenPlatform, NULL);// for the first platform //Single Line code
	if (CLStatus != CL_SUCCESS)
	{
		std::cout << "Error Code " << CLStatus << " : Platform Not set!" << std::endl;
		return false;
	}

	CLStatus = clGetDeviceIDs(ChosenPlatform, CL_DEVICE_TYPE_GPU, 0, NULL, &NumberOfGPUs);
	if (CLStatus != CL_SUCCESS)
	{
		std::cout << "Error Code " << CLStatus << " : Getting GPUs!" << std::endl;
		return false;
	}
	std::cout << "\nNumber Of GPUs: " << NumberOfGPUs << std::endl;

	ChosenGPUs = (cl_device_id*)malloc(NumberOfGPUs * sizeof(cl_device_id));
	CLStatus = clGetDeviceIDs(ChosenPlatform, CL_DEVICE_TYPE_GPU, NumberOfGPUs, ChosenGPUs, NULL);// Same line for code for Multi-GPU
	if (CLStatus != CL_SUCCESS)
	{
		std::cout << "Error Code " << CLStatus << " : GPUs Not set!" << std::endl;
		return false;
	}

	SingleClContext = clCreateContext(NULL, NumberOfGPUs, ChosenGPUs, NULL, NULL, &CLStatus);
	if (CLStatus != CL_SUCCESS)
	{
		std::cout << "Error Code " << CLStatus << " : Context Not Created!" << std::endl;
		return false;
	}

	ClCommandQueues = (cl_command_queue*)malloc(NumberOfGPUs * sizeof(cl_command_queue));
	MaxComputeUnitPerGPU = (size_t*)calloc(NumberOfGPUs, sizeof(size_t));
	MaxWorkItemPerGroup = (size_t*)calloc(NumberOfGPUs, sizeof(size_t));
	MaxGlobalMemoryOfDevice = (size_t*)calloc(NumberOfGPUs, sizeof(size_t));
	MaxPrivateMemoryBytesPerWorkGroup = (size_t*)calloc(NumberOfGPUs, sizeof(size_t));
	MaxLocalMemoryBytesPerWorkGroup = (size_t*)calloc(NumberOfGPUs, sizeof(size_t));

	for (int i = 0; i < NumberOfGPUs; i++)
	{
		//Create a command queue for every device
		ClCommandQueues[i] = clCreateCommandQueue(SingleClContext, ChosenGPUs[i], 0, &CLStatus);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error: Cl Command Queue Creation Failed!" << std::endl;
			return false;
		}

		cl_uint Temp1 = 0;
		size_t Temp2 = 0;
		cl_ulong Temp3 = 0;// Calloc Called Below

		CLStatus = clGetDeviceInfo(ChosenGPUs[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &Temp1, NULL);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error Code " << CLStatus << " : Device Get CL_DEVICE_MAX_COMPUTE_UNITS Info Failed!" << std::endl;
			return false;
		}
		MaxComputeUnitPerGPU[i] = (int)Temp1;
		std::cout << "\nMaxComputeUnitPerGPU[i]:" << std::endl;
		std::cout << MaxComputeUnitPerGPU[i] << std::endl;

		CLStatus = clGetDeviceInfo(ChosenGPUs[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &Temp2, NULL);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error Code " << CLStatus << " : Device Get CL_DEVICE_MAX_WORK_GROUP_SIZE Info Failed!" << std::endl;
			return false;
		}
		MaxWorkItemPerGroup[i] = (int)Temp2;
		std::cout << "\nMaxWorkItemPerGroup[i]:" << std::endl;
		std::cout << MaxWorkItemPerGroup[i] << std::endl;

		CLStatus = clGetDeviceInfo(ChosenGPUs[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &Temp3, NULL);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error Code " << CLStatus << " : Device Get CL_DEVICE_GLOBAL_MEM_SIZE Info Failed!" << std::endl;
			return false;
		}
		MaxGlobalMemoryOfDevice[i] = Temp3;
		std::cout << "\nMaxGlobalMemoryOfDevice[i]:" << std::endl;// Total GPU VRAM 
		std::cout << MaxGlobalMemoryOfDevice[i] << std::endl;

		CLStatus = clGetDeviceInfo(ChosenGPUs[i], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(cl_ulong), &Temp3, NULL);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error Code " << CLStatus << " : Device Get CL_DEVICE_GLOBAL_MEM_CACHE_SIZE Info Failed!" << std::endl;
			return false;
		}
		MaxPrivateMemoryBytesPerWorkGroup[i] = (int)Temp3;
		std::cout << "\nMaxPrivateMemoryBytesPerWorkGroup[i]:" << std::endl;// Per Work Item
		std::cout << MaxPrivateMemoryBytesPerWorkGroup[i] << std::endl;

		CLStatus = clGetDeviceInfo(ChosenGPUs[i], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &Temp3, NULL);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error Code " << CLStatus << " : Device Get CL_DEVICE_LOCAL_MEM_SIZE Info Failed!" << std::endl;
			return false;
		}
		MaxLocalMemoryBytesPerWorkGroup[i] = (int)Temp3;
		std::cout << "\nMaxLocalMemoryBytesPerWorkGroup[i]:" << std::endl;// Per Work Group
		std::cout << MaxLocalMemoryBytesPerWorkGroup[i] << std::endl << std::endl;
	}

	if (NumberOfGPUs > 0)// Totaly unnecessary but using this just for the scope so that these variable can be cleared from memory with ease
	{
		const char* ClSourceFileName = "D:\\C++ Test Projects\\TestOpenCL4\\PunalManalanOpenCLKernelFunctions.cl";
		std::string ClSourceFileInString;
		GetFileContent(ClSourceFileName, ClSourceFileInString);
		const char* ClSourceFileInChar = ClSourceFileInString.c_str();
		size_t ClSourceFileInCharSize[] = { strlen(ClSourceFileInChar) };
		AllCustomClFunctionsProgram = clCreateProgramWithSource(SingleClContext, 1, &ClSourceFileInChar, ClSourceFileInCharSize, NULL);
	}

	CLStatus = clBuildProgram(AllCustomClFunctionsProgram, 0, NULL, NULL, NULL, NULL);

	cl_KernelMemoryStruct*** TempGpuMemoryInitializationHelper = NULL;//NOTE: Free this BEFORE EVERY MALLOC

	/**************************************************************************************************************/
	/* GPU			Kernel			Functions			And			Parameter			Defination*/
	/*_________FirstTestFunction Kernel Function_________*/
	TempGpuMemoryInitializationHelper = (cl_KernelMemoryStruct***)malloc(2 * sizeof(cl_KernelMemoryStruct**));// Free this upon usage
	TempGpuMemoryInitializationHelper[0] = &FirstTestInputBuffer;
	TempGpuMemoryInitializationHelper[1] = &FirstTestOutputBuffer;
	ReturnResult = InitializeKernelWithFunctionAndInitializeBuffer(&FirstTestFunction_Kernel, "FirstTestFunction", TempGpuMemoryInitializationHelper, 2);
	if (ReturnResult != true)
	{
		std::cout << "Error Initializeing Kernel With Function And InitializeBuffer!" << std::endl;
		return false;
	}
	free(TempGpuMemoryInitializationHelper);

	/*_________SphericalHeadCapsuleTrace Kernel Function_________*/
	TempGpuMemoryInitializationHelper = (cl_KernelMemoryStruct***)malloc(5 * sizeof(cl_KernelMemoryStruct**));
	TempGpuMemoryInitializationHelper[0] = &SphericalHeadCapsuleTrace_INPUTCharacterLocationArray;				//Float3D Array			Global
	TempGpuMemoryInitializationHelper[1] = &SphericalHeadCapsuleTrace_INPUTCharacterVelocityArray;				//Float3D Array			Global
	TempGpuMemoryInitializationHelper[2] = &SphericalHeadCapsuleTrace_INPUTPlaneArray;							//Three Float3D Array	Global
	TempGpuMemoryInitializationHelper[3] = &SphericalHeadCapsuleTrace_OUTPUTCharacterImpactArray;				//Float3D Array			Global
	TempGpuMemoryInitializationHelper[4] = &SphericalHeadCapsuleTrace_OUTPUTCharacterLocationArray;				//Float3D Array			Global
	//float SphericalHeadCapsuleTrace_DeltaTime = NULL;					  This Does NOT Require Assignment		//Float					Private
	//uint32_t SphericalHeadCapsuleTrace_TotalNumberOfCharacters = NULL;  This Does NOT Require Assignment		//Unsigned Int			Private	
	//uint32_t SphericalHeadCapsuleTrace_ActualTotalNumberOfCharacters = NULL;This Does NOT Require Assignment  //Unsigned Int			Private
	//uint32_t SphericalHeadCapsuleTraceCharacterOffset = NULL;			  This Does NOT Require Assignment		//Float					Private
	//TempGpuMemoryInitializationHelper[5] = &SphericalHeadCapsuleTrace_CALCULATIONCharacterImpactSubArray;		//Float3D Array			Local
	//TempGpuMemoryInitializationHelper[6] = &SphericalHeadCapsuleTrace_CALCULATIONCharacterLocationSubArray;	//Float3D Array			Local
	ReturnResult = InitializeKernelWithFunctionAndInitializeBuffer(&SphericalHeadCapsuleTrace_Kernel, "SphericalHeadCapsuleTrace", TempGpuMemoryInitializationHelper, 5);
	if (ReturnResult != true)
	{
		std::cout << "Error Initializeing Kernel With Function And InitializeBuffer!" << std::endl;
		return false;
	}
	free(TempGpuMemoryInitializationHelper);
	/**************************************************************************************************************/

	if (ReturnResult == false)
	{
		return false;
	}
}

/*Clean the resources.*/
void OpenCLReleaseResources(bool ReleaseMemory)
{
	free(ChosenPlatform);
	clReleaseContext(SingleClContext);// Only one context to release
	/**************************************************************************************************************/
	/* GPU			Program			Resource			Release */
	clReleaseProgram(AllCustomClFunctionsProgram);//Only One Program
	/**************************************************************************************************************/

	/**************************************************************************************************************/
	/* GPU			Kernel			Resources			Release */
	for (int i = 0; i < NumberOfGPUs; i++)
	{
		clReleaseKernel(FirstTestFunction_Kernel[i]);//NO.1
		clReleaseKernel(SphericalHeadCapsuleTrace_Kernel[i]);//NO.2
	}
	/**************************************************************************************************************/

	if (ReleaseMemory)
	{
		/**************************************************************************************************************/
		/* GPU			Memory			Resources			Release */
		/*_________FirstTestFunction Kernel Function_________*/
		//ReleaseArrayOfMemoryObjects(FirstTestInputBuffer);//NO.1	
		/**************************************************************************************************************/
	}

	/**************************************************************************************************************/
	/* GPU			CommandQueue			Resources			Release */
	for (int i = 0; i < NumberOfGPUs; i++)
	{
		clReleaseDevice(ChosenGPUs[i]);
		clReleaseCommandQueue(ClCommandQueues[i]);//Only One Command queue array
	}
	free(ChosenGPUs);
	free(ClCommandQueues);
	free(MaxComputeUnitPerGPU);
	free(MaxWorkItemPerGroup);
	free(MaxGlobalMemoryOfDevice);
	free(MaxPrivateMemoryBytesPerWorkGroup);
	free(MaxLocalMemoryBytesPerWorkGroup);
	/**************************************************************************************************************/
	size_t* MaxWorkItemPerGroup = NULL;
	size_t* MaxGlobalMemoryOfDevice = NULL;
	size_t* MaxPrivateMemoryBytesPerWorkGroup = NULL;
	size_t* MaxLocalMemoryBytesPerWorkGroup = NULL;
	cl_program AllCustomClFunctionsProgram = NULL;
}

int main()
{

}