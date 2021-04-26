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

struct KernelArgumentStruct
{
	bool FirstTimeBufferCreation;//For Global
	void* DataFromHost;//For Global
	size_t INPUTorOUTPUTDataHostSizeToCopySize;//For Global
	size_t CL_MemorySizeToCreate;//For Local And Global //NOTE: NO NEED TO Allocate memory for local from host side... memory allocation from host is only required for global
	bool IntialReleaseBuffer;//For Global
	bool OverWriteBuffer;//For Global Input
};

cl_uint NumOfPlatforms;	//the NO. of platforms
cl_platform_id ChosenPlatform = NULL;	//the chosen platform
cl_uint	NumberOfGPUs = 0;//cl_uint	NumberOfGPUorCPU = 0;
cl_device_id* ChosenGPUs = NULL;
cl_context SingleClContext;
cl_command_queue* ClCommandQueues = NULL;
size_t* MaxComputeUnitPerGPU = NULL;
//size_t* MaxNumberOfWorkGroup = NULL;
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

	cl_mem*** TempGpuMemoryInitializationHelper = NULL;//NOTE: Free this BEFORE EVERY MALLOC

	/**************************************************************************************************************/
	/* GPU			Kernel			Functions			And			Parameter			Defination*/
	/*_________FirstTestFunction Kernel Function_________*/
	TempGpuMemoryInitializationHelper = (cl_mem***)malloc(2 * sizeof(cl_mem**));// Free this upon usage
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
	TempGpuMemoryInitializationHelper = (cl_mem***)malloc(5 * sizeof(cl_mem**));
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