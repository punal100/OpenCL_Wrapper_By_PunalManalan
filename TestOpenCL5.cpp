// TestOpenCL5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

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


=========
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

>>>>>>>>> Temporary merge branch 2
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
struct cl_KernelMemoryStruct
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

>>>>>>>>> Temporary merge branch 2
struct KernelArgumentStruct
{
	bool IsInitialized = false;// False When no Buffer in on Device, True is when Buffer is stored on Device
	cl_mem MemoryInDevice;
};

struct cl_KernelSingleArgumentStruct
{
	const bool& IsLocalMemory;
	const bool& TrueForReadOnlyFalseForWriteOnly;
	bool TrueForCreateFalseForOverWrite;//When True Creates The Buffer
	size_t CL_MemorySizeToCreate;//For Local And Global //NOTE: NO NEED TO Allocate memory for local from host side... memory allocation from host is only required for global	
	void* DataFromHost;//For Global and Private Only , Pass NULL for Local...
	size_t INPUTorOUTPUTDataHostSizeToCopySize;//For Global ONLY, Reads Data When TrueForReadOnlyFalseForWriteOnly is true, else writes to buffer on creation
	cl_KernelMemoryStruct BufferOnDevice;//Example: Buffer on GPU device
	//NOTE:IF TrueForCreateFalseForOverWrite is false And INPUTorOUTPUTDataHostSizeToCopySize > CL_MemorySizeToCreate
	//     Memory is released and reallocaed

	//Contructor
	cl_KernelSingleArgumentStruct(bool InitializeIsLocalMemory,bool InitializeTrueForReadOnlyFalseForWriteOnly) : IsLocalMemory(InitializeIsLocalMemory) ,TrueForReadOnlyFalseForWriteOnly(InitializeTrueForReadOnlyFalseForWriteOnly) {}//const bool Are Initialized in Initialization list 
	
	//Destructor
	~cl_KernelSingleArgumentStruct()
	{
		if (BufferOnDevice.IsInitialized)
		{
			try
			{
				int ClErrorResult;
				ClErrorResult = clReleaseMemObject(BufferOnDevice.MemoryInDevice);
				if (ClErrorResult != CL_SUCCESS)
				{
					throw ClErrorResult;
				}
			}
			catch (int CatchErrorNum)
			{
				std::cout << "\n Error " << CatchErrorNum << " : Releasing Device Memory!";
			}			
		}		
	}
};

//NOTE: Always call with the below constructor otherwise, DO NOT call
struct cl_KernelMultipleArgumentStruct
{
	const int NumberOfArugments;
	cl_KernelSingleArgumentStruct* SingleKernelFunctionMultiArgumentsArray;

	cl_KernelMultipleArgumentStruct(int NumberOfReads, int NumberOfWrites, int NumberOfLocal) : NumberOfArugments((NumberOfReads + NumberOfWrites + NumberOfLocal))
	{

	}

	~cl_KernelMultipleArgumentStruct()
	{

	}
}

struct cl_KernelAndArguments
{
	cl_kernel* MultiDeviceKernelFunctionArray;
	cl_kernel* MultiDeviceKernelArgumentsArray;
	//NOTE: Kernel Arguments are Ordered from left to right, Read only memory in left, write only in right, Local in middle of them
	//NOTE: any memory which is plain int, float etc etc those which are NOT cl_mem, Will NOT BE INCLUDED HERE instead they will be given as parameters in kernel Host function
	cl_KernelAndArguments(int NumberOfDevices, int NumberOfReads, int NumberOfLocal, int NumberOfWrites)
	{
		try
		{
			throw(10);
		}
		catch (int Num)
		{

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
// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
