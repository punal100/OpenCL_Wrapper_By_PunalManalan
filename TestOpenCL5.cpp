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
	CL_PRIVATE = 3,
	CL_LOCALENUM = 5,// Not To be confused with CL_LOCAL
	CL_READ_ONLY = CL_MEM_READ_ONLY,
	CL_WRITE_ONLY = CL_MEM_WRITE_ONLY,
	CL_READ_AND_WRITE = CL_MEM_READ_WRITE
};

//NOTE: Always call this with new operator
struct cl_KernelMemoryStruct
{
private:
	//bool IsInitialized = false;// False When no Buffer in on Device, True is when Buffer is stored on Device
	cl_mem GlobalMemoryInDevice = nullptr;// Local not needed as it is inaccessible by host..
	cl_Memory_Type MemoryTypeOfThisMemoryInDevice = Uninitialized_cl_Memory;
	size_t MemoryInDeviceTotalSizeInBytes = 0;//NOTE: for private pass the sizeof(variable_type)
	size_t MemoryInDevice_Occupied_SizeInBytes = 0;//NOTE: Actually 'MemoryInDeviceTotalSizeInBytes' occupies full space... but by 'MemoryInDevice_Occupied_SizeInBytes' I mean the memory YOU "use". I know this might sound confusing, but here is a simple example; Say you have a box that which can hold 20 pieces of bottles, but we only put in 10 pieces, meaning we used only 10 slot instead fully using 20 slots. 

public:
	cl_KernelMemoryStruct()
	{
		std::cout << "\n Constructing cl_KernelMemoryStruct!";
		//IsInitialized = false;
		MemoryTypeOfThisMemoryInDevice = Uninitialized_cl_Memory;
	}

	void cl_MemoryAllocationOnDevice(cl_context The_clContext_For_BufferCreation, cl_command_queue The_clCommandQueue_For_BufferCreation, cl_Memory_Type TheMemoryType, void* PointerToMemoryToCopyFrom, size_t SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, size_t BUFFER_CREATION_ONLY_SizeOfBuffer, bool OverWriteMemory, bool& IsFunctionSuccesful)//Note: If MemorySize is 0, Then no memory is written on the device but memory is created, Note: for CreateOnlyOptionalLargerBufferSize to take effect pass a size larger than SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type
	{
		IsFunctionSuccesful = false;
		cl_int CLStatus;

		if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type < 1)
		{
			std::cout << "\n Error Supplied Memory Size of " << SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type << " Bytes Is Less than 1 Bytes, Pass atleast 1 byte of Data" << ": cl_MemoryAllocationOnDevice In: cl_KernelMemoryStruct!\n";
			return;
		}

		if (OverWriteMemory)
		{
			//if (IsInitialized)
			if(MemoryTypeOfThisMemoryInDevice != cl_Memory_Type::Uninitialized_cl_Memory)
			{
				if (MemoryTypeOfThisMemoryInDevice != TheMemoryType)
				{
					std::cout << "\n Error Supplied MemoryType Does not Match with Type Of MemoryInDevice" << ": OverWriting Buffer In: cl_KernelMemoryStruct!\n";
					return;
				}
				//else// This is unlikely to happen see the above two if stement, MemoryTypeOfThisMemoryInDevice != cl_Memory_Type::Uninitialized_cl_Memory, Then MemoryTypeOfThisMemoryInDevice != TheMemoryType
				//{
				//	if (TheMemoryType == cl_Memory_Type::Uninitialized_cl_Memory)
				//	{
				//		std::cout << "Error " << CLStatus << " : Default 'Uninitialized_cl_Memory' Enum passed! Please pass any of these Enums CL_PRIVATE, CL_LOCALENUM, CL_READ_ONLY, CL_WRITE_ONLY, CL_READ_AND_WRITE\n";
				//	}
				//}
				if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type <= MemoryInDeviceTotalSizeInBytes)
				{	
					if (MemoryTypeOfThisMemoryInDevice == cl_Memory_Type::CL_PRIVATE)
					{
						if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type != MemoryInDeviceTotalSizeInBytes)
						{
							std::cout << "\n Error Trying to change the Size of Private Variable(NOTE: This is impossible to happen. Because size of variables type does not change...)" << ": OverWriting Buffer In: cl_KernelMemoryStruct!\n";
							return;
						}
					}
					else
					{
						if (MemoryTypeOfThisMemoryInDevice != cl_Memory_Type::CL_LOCALENUM)// Uninitialized_cl_Memory Is imposible in this case, reason being if IsInitialized == true, then MemoryTypeOfThisMemoryInDevice is definitely not Uninitialized_cl_Memory
						{
							void* TempDataCarryHelper = calloc(SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, sizeof(char));// malloc works great too, but i prefer to use calloc here, NOTE: Char is 1 Byte so using char
							if (TempDataCarryHelper == nullptr)
							{
								std::cout << "\n Error Allocating" << SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type * sizeof(char) << "TempDataCarryHelper Variable In: cl_KernelMemoryStruct\n";
								return;
							}

							if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type > 0)
							{
								for (int i = 0; i < SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type; ++i)// Memccpy bad
								{
									((char*)TempDataCarryHelper)[i] = ((char*)PointerToMemoryToCopyFrom)[i];// I could simply convert void* to char*... but i left it as void* for the purpose of 'readability'
								}

								CLStatus = clEnqueueWriteBuffer(The_clCommandQueue_For_BufferCreation, GlobalMemoryInDevice, CL_TRUE, 0, SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, TempDataCarryHelper, 0, NULL, NULL);
								free(TempDataCarryHelper);// Free the data

								if (CLStatus != CL_SUCCESS)
								{
									std::cout << "\n CL_Error Code " << CLStatus << " : OverWriting Buffer In: cl_KernelMemoryStruct!\n";
									return;
								}
							}
							MemoryInDevice_Occupied_SizeInBytes = SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type;
						}
						else
						{
							if (PointerToMemoryToCopyFrom != nullptr)
							{
								std::cout << "\n CRITICAL WARNING Trying To OverWrite Local Memory(This type can not be written or overwritten from host...) So Pass nullptr to the parameter 'PointerToMemoryToCopyFrom'" << ": OverWriting Buffer In: cl_KernelMemoryStruct!\n";
							}
							else
							{
								MemoryInDeviceTotalSizeInBytes = SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type;
								MemoryInDevice_Occupied_SizeInBytes = MemoryInDeviceTotalSizeInBytes;
							}
						}
					}		
				}
				else
				{
					std::cout << "\n Error Size Passed is larger than the Available buffer Space" << ": OverWriting Buffer In: cl_KernelMemoryStruct!\n";
					return;
				}
			}
			else
			{
				std::cout << "\n Error Trying To OverWrite Uninitialized Memory" << ": OverWriting Buffer In: cl_KernelMemoryStruct!\n";
				return;
			}
		}
		else
		{
			//if (IsInitialized)
			if (MemoryTypeOfThisMemoryInDevice != cl_Memory_Type::Uninitialized_cl_Memory)
			{
				CLStatus = clReleaseMemObject(GlobalMemoryInDevice);// releasing Memory object every time this function is called	
				if (CLStatus != CL_SUCCESS)
				{
					std::cout << "\n ClError Code " << CLStatus << " : Releasing Memory On device In: cl_KernelMemoryStruct!\n";
					return;
				}

				//IsInitialized = false;
				MemoryTypeOfThisMemoryInDevice = cl_Memory_Type::Uninitialized_cl_Memory;
				MemoryInDeviceTotalSizeInBytes = 0;
				MemoryInDevice_Occupied_SizeInBytes = 0;
			}

			switch (TheMemoryType)
			{
				case cl_Memory_Type::CL_LOCALENUM:
				{
					//No Need for Buffer creation as this is a local memory...
					MemoryTypeOfThisMemoryInDevice = TheMemoryType;
					MemoryInDeviceTotalSizeInBytes = SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type;
					MemoryInDevice_Occupied_SizeInBytes = MemoryInDeviceTotalSizeInBytes;
					IsFunctionSuccesful = true;
					return;
					//break;// Not needed as return is done
				}

				case cl_Memory_Type::CL_PRIVATE:
				{
					MemoryTypeOfThisMemoryInDevice = TheMemoryType;
					MemoryInDeviceTotalSizeInBytes = SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type;
					MemoryInDevice_Occupied_SizeInBytes = MemoryInDeviceTotalSizeInBytes;
					IsFunctionSuccesful = true;
					return;
					//break;// Not needed as return is done
				}

				case cl_Memory_Type::Uninitialized_cl_Memory:
				{
					std::cout << "\n Error " << CLStatus << " : Default 'Uninitialized_cl_Memory' Enum Passed In: cl_KernelMemoryStruct! Please pass any of these Enums CL_PRIVATE, CL_LOCALENUM, CL_READ_ONLY, CL_WRITE_ONLY, CL_READ_AND_WRITE" << " : Creating Buffer In: cl_KernelMemoryStruct!\n";
					return;
					//break;// Not needed as return is done
				}

				default://CL_MEM_READ_ONLY, CL_MEM_WRITE_ONLY And CL_MEM_READ_WRITE
				{
					if ((TheMemoryType == CL_READ_ONLY) || (TheMemoryType == CL_WRITE_ONLY) || (TheMemoryType == CL_READ_AND_WRITE))
					{
						if (BUFFER_CREATION_ONLY_SizeOfBuffer >= SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type)
						{
							GlobalMemoryInDevice = clCreateBuffer(The_clContext_For_BufferCreation, TheMemoryType, BUFFER_CREATION_ONLY_SizeOfBuffer, NULL, &CLStatus);
							if (CLStatus != CL_SUCCESS)
							{
								std::cout << "\n ClError Code " << CLStatus << " : Creating Buffer On device In: cl_KernelMemoryStruct!\n";
								return;
							}
							//IsInitialized = true;
							MemoryTypeOfThisMemoryInDevice = TheMemoryType;
							MemoryInDeviceTotalSizeInBytes = 0;
							MemoryInDevice_Occupied_SizeInBytes = 0;
							break;// NOTE: The Code Below will execute only if this break is accessed
						}
						else
						{
							std::cout << "\n Error Supplied Size Of SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type Excedes the size maximum size specified by BUFFER_CREATION_ONLY_SizeOfBuffer" << " : Creating Buffer In: cl_KernelMemoryStruct!\n";
							return;
						}
					}
					else
					{
						std::cout << "\n Error Undefined Enum Passed In: cl_KernelMemoryStruct! Please pass any of these Enums CL_PRIVATE, CL_LOCALENUM, CL_READ_ONLY, CL_WRITE_ONLY, CL_READ_AND_WRITE" << " : Creating Buffer In: cl_KernelMemoryStruct!\n";
						return;
					}
				}
			}		

			if (SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type > 0)
			{
				CLStatus = clEnqueueWriteBuffer(The_clCommandQueue_For_BufferCreation, GlobalMemoryInDevice, CL_TRUE, 0, SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type, PointerToMemoryToCopyFrom, 0, NULL, NULL);
				if (CLStatus != CL_SUCCESS)
				{
					std::cout << "\n Error Code " << CLStatus << " : Writing Buffer In: cl_KernelMemoryStruct!\n";
					CLStatus = clReleaseMemObject(GlobalMemoryInDevice);
					if (CLStatus != CL_SUCCESS)
					{
						std::cout << "\n CL_Error Code " << CLStatus << " : Releasing Memory On device In: cl_KernelMemoryStruct!\n";
						return;
					}
					//IsInitialized = false;
					MemoryTypeOfThisMemoryInDevice = cl_Memory_Type::Uninitialized_cl_Memory;
					MemoryInDeviceTotalSizeInBytes = 0;
					MemoryInDevice_Occupied_SizeInBytes = 0;
					return;
				}
				MemoryInDeviceTotalSizeInBytes = BUFFER_CREATION_ONLY_SizeOfBuffer;
				MemoryInDevice_Occupied_SizeInBytes = SizeOfMemoryInBytes_ForPrivatePassSizeofVariable_Type;
			}
		}
		IsFunctionSuccesful = true;
	}

	~cl_KernelMemoryStruct()
	{
		std::cout << "\n Destructing cl_KernelMemoryStruct!";
		//if (IsInitialized)
		if (MemoryTypeOfThisMemoryInDevice != cl_Memory_Type::Uninitialized_cl_Memory)
		{
			cl_int ClErrorResult;
			ClErrorResult = clReleaseMemObject(GlobalMemoryInDevice);
			if (ClErrorResult != CL_SUCCESS)
			{
				std::cout << "\n clError " << ClErrorResult << " : Releasing cl_KernelMemoryStruct's Device Memory In: cl_KernelMemoryStruct!";
			}
		}
	}
};

//NOTE: DO NOT USE WITHOUT CALLING THE CONSTRUCTOR FIRST
struct cl_KernelSingleArgumentStruct
{
private:
	const cl_context* cl_ContextForThisArgument;
	const cl_command_queue* cl_CommandQueueForThisArgument;
	const cl_Memory_Type clMemory_Type_Of_Argument;
	bool IsDataSendCompleted = false;
	bool TrueForCreateFalseForOverWrite = true;//When True Creates The Buffer
	void* COPY_OF_DataFromHost = nullptr;//For Global and Private Only , Pass NULL for Local...
	size_t DataFromHostSize = 0;//For Global ONLY, Reads Data When IsReadMemory is true and IsWriteMemory is true or false And IsLocalMemory is false, Memory Can be Written to the buffer
	size_t CL_MemorySizeToCreate = 0;//For Local And Global //NOTE: NO NEED TO Allocate memory for local from host side... memory allocation from host is only required for global			
	cl_KernelMemoryStruct* BufferOnDevice = nullptr;//Example: Buffer on GPU device	

public:
	bool IsConstructionSuccesful = false;// NOTE: Do not change this manualy! unless you know what you are doing

	//Contructor
	cl_KernelSingleArgumentStruct(const cl_Memory_Type ArgclMemory_Type_Of_Argument, const cl_context* Argcl_ContextForThisArgument, const cl_command_queue* Argcl_CommandQueueForThisArgument) : clMemory_Type_Of_Argument(ArgclMemory_Type_Of_Argument), cl_ContextForThisArgument(Argcl_ContextForThisArgument), cl_CommandQueueForThisArgument(Argcl_CommandQueueForThisArgument)
	{
		std::cout << "\n Constructing cl_KernelSingleArgumentStruct!";
		IsConstructionSuccesful = false;
		IsDataSendCompleted = false;
		BufferOnDevice = new cl_KernelMemoryStruct;
		if (BufferOnDevice == nullptr)
		{			
			std::cout << "\n Error Allocating " << sizeof(cl_KernelMemoryStruct) << " Byes Of Memory for BufferOnDevice In cl_KernelSingleArgumentStruct!";
			return;
		}		
		IsConstructionSuccesful = true;
	}//const bool Are Initialized in Initialization list 

	//Do not call this without calling PassDataToThisKernelArgument() first
	void PassDataToDeviceBuffer()
	{
		if (!IsDataSendCompleted)
		{
			BufferOnDevice->cl_MemoryAllocationOnDevice(*cl_ContextForThisArgument, *cl_CommandQueueForThisArgument, clMemory_Type_Of_Argument, COPY_OF_DataFromHost, DataFromHostSize, CL_MemorySizeToCreate, !TrueForCreateFalseForOverWrite, IsDataSendCompleted);
			if (!IsDataSendCompleted)
			{
				std::cout << "\n Error New Data Was not sent to buffer" << ": Passing Data To DeviceBuffer In: cl_KernelSingleArgumentStruct!\n";
			}
		}
		else
		{
			std::cout << "\n Error Same Data Was already sent to buffer" << ": Passing Data To DeviceBuffer In: cl_KernelSingleArgumentStruct!\n";
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
					std::cout << "\n Error Is_Data_Empty is set to true but SizeOfData is Greater than Zero" << " : Passing Data To KernelArgument In: cl_KernelSingleArgumentStruct!\n";
					return;
				}				
			}
			else
			{
				if (DataToPass != nullptr)
				{
					std::cout << "\n Error Is_Data_Empty is set to true but DataToPass is not set to nullptr" << " : Passing Data To KernelArgument In: cl_KernelSingleArgumentStruct!\n";
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
				std::cout << "\n Error SizeOfData of " << SizeOfData << " bytes Is Less than 2 Bytes" << " : Passing Data To KernelArgument In: cl_KernelSingleArgumentStruct!\n";
				std::cout << "NOTE: If you are trying to pass Empty/Null Data set the 'DataToPass' to nullptr, 'SizeOfData' to 0 and 'If_Data_Empty_PassZeroForSizeOfData_And_SetThisToFalse' to true\n";
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
						std::cout << "\n Error SizeOfData Exceeds BufferSize OverWrite Buffer Not Possible" << " : Passing Data To KernelArgument In: cl_KernelSingleArgumentStruct!\n";
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
					std::cout << "\n Error OverWrite Not Possible as IsDataSendCompleted is set to false" << " : Passing Data To KernelArgument In: cl_KernelSingleArgumentStruct!\n";
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
					std::cout << "\n Error SizeOfData Exceeds BufferSize Create Buffer Not Possible" << " : Passing Data To KernelArgument In: cl_KernelSingleArgumentStruct!\n";
					return;
				}				
			}
		}

		void* TempDataCarryHelper = calloc(SizeOfData, sizeof(char));// malloc works great too, but i prefer to use calloc here, NOTE: Char is 1 Byte so using char
		if (TempDataCarryHelper == nullptr)
		{
			std::cout << "\n Error Allocating" << SizeOfData * sizeof(char) << "TempDataCarryHelper Variable In: cl_KernelSingleArgumentStruct!\n";
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
		}
	}
};

//Always call using new operator
struct cl_KernelMultipleArgumentStruct
{
private:
	bool IsConstructionSuccesful = false;// NOTE: Never Change this not worth the time...
	const unsigned int TotalNumberOfArugments;
	const unsigned int NumberOfReads;		//Min: 0						To		Max: (NumberOfReads			- 1) = Read			Only Data
	const unsigned int NumberOfWrites;		//Min: NumberOfReads			To		Max: (NumberOfWrites		- 1) = Write		Only Data
	const unsigned int NumberOfRead_Writes;	//Min: NumberOfWrites			To		Max: (NumberOfRead_Writes	- 1) = Read_Write	Only Data
	const unsigned int NumberOfLocals;		//Min: NumberOfRead_Writes		To		Max: (NumberOfLocal			- 1) = Local		Only Data
	const unsigned int NumberOfPrivates;	//Min: NumberOfLocal			To		Max: (NumberOfPrivate		- 1) = Private		Only Data

	cl_KernelSingleArgumentStruct** SingleKernelFunctionMultiArgumentsArray = nullptr;// Arguments stored here

	void CreateKernelSingleArgumentStruct(int &ArgumentsCreated, int NumberOfArgumentsToCreate, cl_Memory_Type ArgumentTypeToCreate, const cl_context* cl_ContextForThisKernel, const cl_command_queue* Argcl_CommandQueueForThisKernel, bool& IsSuccesful)
	{
		IsSuccesful = false;
		for (int i = ArgumentsCreated; i < NumberOfArgumentsToCreate; ++i)
		{
			SingleKernelFunctionMultiArgumentsArray[i] = new cl_KernelSingleArgumentStruct(ArgumentTypeToCreate, cl_ContextForThisKernel, Argcl_CommandQueueForThisKernel);
			if (SingleKernelFunctionMultiArgumentsArray[i] != nullptr)
			{
				if (SingleKernelFunctionMultiArgumentsArray[i]->IsConstructionSuccesful == false)// ! works great but, '== false' make it readable
				{
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
				for (int j = 0; j < i; ++i)// Greater than i
				{
					delete SingleKernelFunctionMultiArgumentsArray[i];
				}
				free(SingleKernelFunctionMultiArgumentsArray);
				return;
			}
		}
		ArgumentsCreated = ArgumentsCreated + NumberOfArgumentsToCreate;
		IsSuccesful = true;
	}

public:
	cl_KernelMultipleArgumentStruct(
		const unsigned int ArgNumberOfReads				,			const unsigned int ArgNumberOfWrites,
		const unsigned int ArgNumberOfRead_Writes		,
		const unsigned int ArgNumberOfLocals			,			const unsigned int ArgNumberOfPrivates,
		const cl_context* Argcl_ContextForThisKernel	,			const cl_command_queue* Argcl_CommandQueueForThisKernel):
		NumberOfReads(ArgNumberOfReads)					,			NumberOfWrites(ArgNumberOfWrites),
		NumberOfRead_Writes(ArgNumberOfRead_Writes)		,
		NumberOfLocals(ArgNumberOfLocals)				,			NumberOfPrivates(ArgNumberOfPrivates),
		TotalNumberOfArugments(NumberOfReads + NumberOfWrites + NumberOfRead_Writes + NumberOfLocals + NumberOfPrivates)
	{
		std::cout << "\n Constructing cl_KernelMultipleArgumentStruct!";
		IsConstructionSuccesful = false;// Yes this is set to false

		if (TotalNumberOfArugments > 0)
		{
			SingleKernelFunctionMultiArgumentsArray = (cl_KernelSingleArgumentStruct**)malloc(TotalNumberOfArugments * sizeof(cl_KernelSingleArgumentStruct*));
		}
		else
		{
			std::cout << "\n Error The TotalNumberOfArugments Is Set to Zero In: cl_KernelMultipleArgumentStruct!\n";
			return;
		}

		if (SingleKernelFunctionMultiArgumentsArray == nullptr)
		{
			std::cout << "\n Error Allocating " << (TotalNumberOfArugments * sizeof(cl_KernelSingleArgumentStruct*)) << " Byes Of Memory for SingleKernelFunctionMultiArgumentsArray In cl_KernelMultipleArgumentStruct!";
			return;
		}	

		bool IsSuccesful = false;
		int TotalArgumentsCreated = 0;
		CreateKernelSingleArgumentStruct(TotalArgumentsCreated, NumberOfReads, cl_Memory_Type::CL_READ_ONLY, Argcl_ContextForThisKernel, Argcl_CommandQueueForThisKernel, IsSuccesful);
		if (!IsSuccesful)
		{
			std::cout << "\n Error Single Kernel Argument Creation of type cl_Memory_Type::CL_READ_ONLY Failed In: cl_KernelMultipleArgumentStruct!\n";
			return;
		}
		CreateKernelSingleArgumentStruct(TotalArgumentsCreated, NumberOfWrites, cl_Memory_Type::CL_WRITE_ONLY, Argcl_ContextForThisKernel, Argcl_CommandQueueForThisKernel, IsSuccesful);
		if (!IsSuccesful)
		{
			std::cout << "\n Error Single Kernel Argument Creation of type cl_Memory_Type::CL_WRITE_ONLY Failed In: cl_KernelMultipleArgumentStruct!\n";
			return;
		}
		CreateKernelSingleArgumentStruct(TotalArgumentsCreated, NumberOfRead_Writes, cl_Memory_Type::CL_READ_AND_WRITE, Argcl_ContextForThisKernel, Argcl_CommandQueueForThisKernel, IsSuccesful);
		if (!IsSuccesful)
		{
			std::cout << "\n Error Single Kernel Argument Creation of type cl_Memory_Type::CL_READ_AND_WRITE Failed In: cl_KernelMultipleArgumentStruct!\n";
			return;
		}
		CreateKernelSingleArgumentStruct(TotalArgumentsCreated, NumberOfLocals, cl_Memory_Type::CL_LOCALENUM, Argcl_ContextForThisKernel, Argcl_CommandQueueForThisKernel, IsSuccesful);
		if (!IsSuccesful)
		{
			std::cout << "\n Error Single Kernel Argument Creation of type cl_Memory_Type::CL_LOCALENUM Failed In: cl_KernelMultipleArgumentStruct!\n";
			return;
		}
		CreateKernelSingleArgumentStruct(TotalArgumentsCreated, NumberOfPrivates, cl_Memory_Type::CL_PRIVATE, Argcl_ContextForThisKernel, Argcl_CommandQueueForThisKernel, IsSuccesful);
		if (!IsSuccesful)
		{
			std::cout << "\n Error Single Kernel Argument Creation of type cl_Memory_Type::CL_PRIVATE Failed In: cl_KernelMultipleArgumentStruct!\n";
			return;
		}

		IsConstructionSuccesful = true;
	}
};

//NOTE: DO NOT USE WITHOUT CALLING THE CONSTRUCTOR FIRST
struct cl_KernelMultipleArgumentStructA
{
	const unsigned int TotalNumberOfArugments;//Example:		SingleKernelFunctionMultiArgumentsArray[Min: 0,					 Max: TotalNumberOfArugments						 - 1]// Minimum and Maximum range to access particular arguments
	const unsigned int NumberOfReads;//					SingleKernelFunctionMultiArgumentsArray[Min: 0,					 Max: NumberOfReads							 - 1]
	const unsigned int NumberOfWrites;//				SingleKernelFunctionMultiArgumentsArray[Min: NumberOfReads,		 Max: NumberOfReads + NumberOfWrites		 - 1]
	const unsigned int NumberOfReadWrites;//			SingleKernelFunctionMultiArgumentsArray[Min: NumberOfWrites,	 Max: NumberOfWrites + NumberOfReadWrites	 - 1]
	const unsigned int NumberOfLocals;//				SingleKernelFunctionMultiArgumentsArray[Min: NumberOfReadWrites, Max: NumberOfReadWrites + NumberOfLocals	 - 1]
	//NOTE: For (TotalNumberOfArugments - 1),(NumberOfReads - 1),(NumberOfWrites - 1),(NumberOfLocals - 1) If they are less than Zero than  Force the output of Zero

	bool IsConstructionSuccesful = false;// If you hate Memory leaks, then Don't Even try to manualy change it... If you love memory leaks Please do so!

	cl_KernelSingleArgumentStruct** SingleKernelFunctionMultiArgumentsArray = nullptr;// Arguments stored here

	cl_KernelMultipleArgumentStructA(const unsigned int ArgNumberOfReads, const unsigned int ArgNumberOfWrites, const unsigned int ArgNumberOfReadWrites, const unsigned int ArgNumberOfLocals, const unsigned int ArgNumberOfPrivates) : TotalNumberOfArugments((ArgNumberOfReads + ArgNumberOfWrites + ArgNumberOfReadWrites + ArgNumberOfLocals)), NumberOfReads(ArgNumberOfReads), NumberOfWrites(ArgNumberOfWrites), NumberOfReadWrites(ArgNumberOfReadWrites), NumberOfLocals(ArgNumberOfLocals)
	{
		std::cout << "\n Constructing cl_KernelMultipleArgumentStruct!";
		if (TotalNumberOfArugments > 0)
		{
			SingleKernelFunctionMultiArgumentsArray = (cl_KernelSingleArgumentStruct**)malloc(TotalNumberOfArugments * sizeof(cl_KernelSingleArgumentStruct*));
		}
		
		if (SingleKernelFunctionMultiArgumentsArray == nullptr)
		{
			std::cout << "\n Error Allocating " << (TotalNumberOfArugments * sizeof(cl_KernelSingleArgumentStruct*)) << " Byes Of Memory for SingleKernelFunctionMultiArgumentsArray In cl_KernelMultipleArgumentStruct!";
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

	~cl_KernelMultipleArgumentStructA()
	{
		std::cout << "\n Destructing cl_KernelMultipleArgumentStruct!";
		if (IsConstructionSuccesful)
		{
			for (int i = 0; i < TotalNumberOfArugments; ++i)
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
		std::cout << "\n Destructing cl_MultiDevice_KernelAndArgumentsStruct!";
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
		std::cout << "\n Destructing cl_SingleKernelFunctionConstructionHelper!";
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
		std::cout << "\n Destructing cl_MultiKernelFunctionConstructionHelper!";
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
			CLStatus = clReleaseCommandQueue(DeviceClCommandQueue);
			if (CLStatus != CL_SUCCESS)
			{
				std::cout << "\n clError " << CLStatus << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!";
			}
			return;
		}
		MaxComputeUnitPerGPU = (int)Temp1;
		std::cout << "\nMaxComputeUnitPerGPU:\n";
		std::cout << MaxComputeUnitPerGPU << "\n";

		CLStatus = clGetDeviceInfo(SelectedDevice, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &Temp2, NULL);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error Code " << CLStatus << " : Device Get CL_DEVICE_MAX_WORK_GROUP_SIZE Info Failed!\n";
			CLStatus = clReleaseCommandQueue(DeviceClCommandQueue);
			if (CLStatus != CL_SUCCESS)
			{
				std::cout << "\n clError " << CLStatus << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!";
			}
			return;
		}
		MaxWorkItemPerGroup = (int)Temp2;
		std::cout << "\nMaxWorkItemPerGroup:\n";
		std::cout << MaxWorkItemPerGroup << "\n";

		CLStatus = clGetDeviceInfo(SelectedDevice, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &Temp3, NULL);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error Code " << CLStatus << " : Device Get CL_DEVICE_GLOBAL_MEM_SIZE Info Failed!\n";
			CLStatus = clReleaseCommandQueue(DeviceClCommandQueue);
			if (CLStatus != CL_SUCCESS)
			{
				std::cout << "\n clError " << CLStatus << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!";
			}
			return;
		}
		MaxGlobalMemoryOfDevice = Temp3;
		std::cout << "\nMaxGlobalMemoryOfDevice:\n";// Total GPU VRAM 
		std::cout << MaxGlobalMemoryOfDevice << "\n";

		CLStatus = clGetDeviceInfo(SelectedDevice, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(cl_ulong), &Temp3, NULL);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error Code " << CLStatus << " : Device Get CL_DEVICE_GLOBAL_MEM_CACHE_SIZE Info Failed!\n";
			CLStatus = clReleaseCommandQueue(DeviceClCommandQueue);
			if (CLStatus != CL_SUCCESS)
			{
				std::cout << "\n clError " << CLStatus << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!";
			}
			return;
		}
		MaxPrivateMemoryBytesPerWorkGroup = (int)Temp3;
		std::cout << "\nMaxPrivateMemoryBytesPerWorkGroup:\n";// Per Work Item
		std::cout << MaxPrivateMemoryBytesPerWorkGroup << "\n";

		CLStatus = clGetDeviceInfo(SelectedDevice, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &Temp3, NULL);
		if (CLStatus != CL_SUCCESS)
		{
			std::cout << "Error Code " << CLStatus << " : Device Get CL_DEVICE_LOCAL_MEM_SIZE Info Failed!\n";
			CLStatus = clReleaseCommandQueue(DeviceClCommandQueue);
			if (CLStatus != CL_SUCCESS)
			{
				std::cout << "\n clError " << CLStatus << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!";
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
			cl_int CLStatus = clReleaseCommandQueue(DeviceClCommandQueue);
			if (CLStatus != CL_SUCCESS)
			{
				std::cout << "\n clError " << CLStatus << " : Releasing CommandQueue: " << "in cl_PerDeviceValuesStruct!";
			}
		}		
	}

};

struct cl_ProgramWith_MultiDevice_Kernel
{
	const std::string ClSourceFilePath;
	cl_program SingleProgram;
	cl_context SingleContext;
	unsigned int NumberOfDevices;
	cl_device_id* ChosenDevices = nullptr;
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
			free(ChosenDevices);
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
				free(ChosenDevices);
				CLStatus = clReleaseContext(SingleContext);
				if (CLStatus != CL_SUCCESS)
				{
					std::cout << "\n clError " << CLStatus << " : Releasing cl_context" << "in cl_ProgramWith_MultiDevice_Kernel!";
				}
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
				free(ChosenDevices);				
				CLStatus = clReleaseContext(SingleContext);
				if (CLStatus != CL_SUCCESS)
				{
					std::cout << "\n clError " << CLStatus << " : Releasing cl_context" << "in cl_ProgramWith_MultiDevice_Kernel!";
				}
				return false;
			}

			CLStatus = clBuildProgram(SingleProgram, 0, NULL, NULL, NULL, NULL);
			if (CLStatus != CL_SUCCESS)
			{
				std::cout << "ClError Code " << CLStatus << " : Cl Program in  InitializeOpenCLProgram Not Built!\n";
				for (int i = 0; i < NumberOfGPUs; i++)
				{
					delete(PerDeviceValueStruct[i]);
				}
				free(PerDeviceValueStruct);
				free(ChosenDevices);
				CLStatus = clReleaseContext(SingleContext);
				if (CLStatus != CL_SUCCESS)
				{
					std::cout << "\n clError " << CLStatus << " : Releasing cl_context" << "in cl_ProgramWith_MultiDevice_Kernel!";
				}
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
		//	//
		//	//size_t binary_size;
		//	//clGetProgramInfo(SingleProgram, CL_PROGRAM_BINARY_SIZES, sizeof(size_t), &binary_size, NULL);
		//	//char* binary = (char*)malloc(binary_size);// sizeof(char) * binary_size would be not be needed
		//	//clGetProgramInfo(SingleProgram, CL_PROGRAM_BINARIES, binary_size, &binary, NULL);
		//	//		
		//	//clBuildProgram(binary_program, 1, &device, NULL, NULL, NULL);
		//	//binary_kernel = clCreateKernel(binary_program, "kmain", &errcode_ret);
		//	//f = fopen(YOUR_BIN_STORAGE_PATH, "w");// Path to write the binary data into...
		//	//fwrite(binary, binary_size, 1, f);
		//	//fclose(f);
		//}		
	}	

	cl_ProgramWith_MultiDevice_Kernel(cl_MultiKernelFunctionConstructionHelper& ArrayOfKernelsAndTheirArugmentsNeededToAdd, std::string ClSourceFilePath) : ClSourceFilePath(ClSourceFilePath)
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
	void RunKernelFunction(unsigned int KernelToRunNumber, int DevicesToRunKernelFrom, int DevicesToRunKernelTo, bool ChooseOptimalTotalWorkItemsAndGroupSize, int TotalWorkItems, int TotalWorkGroups)
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
		std::cout << "\n Destructing cl_ProgramWith_MultiDevice_Kernel!";
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
			for (int i = 0; i < NumberOfDevices; i++)
			{
				delete(PerDeviceValueStruct[i]);
			}
			free(PerDeviceValueStruct);
			IsConstructionSuccesful = false;
		}
	}
};

int main()
{
	//Testing 
	cl_MultiKernelFunctionConstructionHelper TestKernels;
	TestKernels.AddKernelFunction("Add_Integers",2,1,0);
	cl_ProgramWith_MultiDevice_Kernel TestProgram{ TestKernels , "D:\\c++ test projects\\testopencl5\\PunalOpenclFunctionsProgram.cl"};

	std::cout << "\nBefore End";
	std::cout << "\nEnd Of program exe";
}