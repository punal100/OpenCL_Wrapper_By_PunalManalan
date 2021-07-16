# OpenCL_Wrapper By Punal Manalan
Welcome To OpenCL Wrapper Source Code!
Solely Written By Punal Manalan.

Free to Use, Whenever and whereever needed!

# Information About This Wrapper
1. LightWeight, Easy to use **Header-Only** "OCLW_P.h" Wrapper for OpenCL.
2. 'OCLW_P::OpenCLWrapper' Is Entire OpenCl Program which as every information of the devices.
3. Written Using only "CL/cl.h" and has backward compatblity upto OpenCL 1.2!
4. Adding kernel function and argument types Only Once.
5. Information about each kernel function and it's argument types can be Accesed with **getter**functions().
6. Works On **Multi-Platforms** and **Multi-GPUs**.
7. Sharing of Work-Load between **Multi-GPUs** Can be acheived anytime during runtime!
9. Compiles and Runs on Windows, Linux and Mac.
10. This Wrapper specializes in Heavy Computation on GPU.
11. From Constrction to Destruction everything is Done automaticaly!
12. No need for manual destruction and releasing cl resources!
13. No need to write Tedious amount of code to counter memory leaks!
14. Upon going out of scope or exiting the program Every resource that is used is safely Released and Deleted.
15. Details and Errors(If any) are Logged as a Log.txt file Upon Exit (**NOTE**:"OCLW_P.h" is tested bug free!).

# Requirements
1. OpenCL Minimum Version 1.2 //cl.h & OpenCL.lib
2. "Essenbp.h" Header(Provided)

# How to use
**NOTE: Example Code Available as Examples.cpp & ExampleOpenCLKernels.cl**
## Initialization:
```c++
#include "OCLW_P.h"

int main()
{	
	//Error Checking is omitted
	bool Issuccessful = false;//NOTE: Checking for failures is a good practice!
	
	//int N = "Total_Number_Of_Functions";//Example 2 functions namely "KernelOne" & "KernelTwo"//Function Names are Case and Symbol Sensitive
	//int K1 = "Total_Number_Of_Arugmnets_Of_Each_Specific_Function"//Example: 5 Argument: 2 input And 1 Output
	//int K2 = "Total_Number_Of_Arugmnets_Of_Each_Specific_Function"//Example: 5 Argument: 2 input And 1 Output
	
	//OCLW_P::cl_KernelFunctionsStruct [Constructor](/*Total Number Of Functions*/, /*Is Succesful Boolean*/);
	OCLW_P::cl_KernelFunctionsStruct Functions_List(N, Issuccessful);// Specify The Total Number of functions to add
	
	//SetTheNameAndNumberOfArgumentsForNextKernelFunction(/*Kernel Name*/, /*Total Number Of Arguments*/, /*Is Succesful Boolean*/);
	//NOTE: Function Names and Arugmnets are Stored in Order Like this: First->Second...->Last == (1,2...,N)
	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("KernelOne", K1, Issuccessful);// 1st Function
	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("KernelTwo", K2, Issuccessful);// 2nd Function

	//SetMemoryTypeOfArugment(/*Kernel Numbter*/, /*Argument Number*/, /*Argument Type*/, /*Is Succesful Boolean*/);
	//First Function "KernelOne"
	Functions_List.SetMemoryTypeOfArugment(0, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 1, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 2, OCLW_P::cl_Memory_Type::CL_READ_AND_WRITE, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 3, OCLW_P::cl_Memory_Type::CL_LOCALENUM, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 4, OCLW_P::cl_Memory_Type::CL_PRIVATE, Issuccessful);
	
	//Second Function "KernelTwo"
	Functions_List.SetMemoryTypeOfArugment(1, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 1, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 2, OCLW_P::cl_Memory_Type::CL_READ_AND_WRITE, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 3, OCLW_P::cl_Memory_Type::CL_LOCALENUM, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 4, OCLW_P::cl_Memory_Type::CL_PRIVATE, Issuccessful);
	
	//OCLW_P::cl_PlatformVendorStruct(/*Is Succesful Boolean*/);
	//NOTE:This Returns ALL the Available Platforms(Example: Intel, AMD, Nvdia)
	OCLW_P::cl_PlatformVendorStruct AvailablePlatformVendors(Issuccessful);

	int TotalNumberOfPlatforms = 0;
	//GetTotalNumberOfPlatformVendors(/*Return Total Number Of Platforms*/, /*Is Succesful Boolean*/);
	AvailablePlatformVendors.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, Issuccessful);
	AvailablePlatformVendors.PrintAllAvailablePlatformVendorNames(Issuccessful);// Prints These In Log.txt
	//SetChosenPlatform(/*Platfrom Number From 1 to TotalNumberOfPlatforms*/, /*Is Succesful Boolean*/);
	AvailablePlatformVendors.SetChosenPlatform(1, Issuccessful);//For Me AMD is the First Platform(GPU), Intel is Second Platform(CPU)

	//std::string FilePath = "C:\\OpenCL Folder\\PunalOpenclFunctionsProgram.cl";// Is Direct Path
	std::string FilePath = "OpenclFunctionsProgram.cl";// This looks for the file in program location
	//OCLW_P::OpenCLWrapper(/*File Path*/,
      ///*Reference to Constructed OCLW_P::cl_KernelFunctionsStruct*/,
      ///*Reference to Constructed OCLW_P::cl_PlatformVendorStruct*/,
      ///*Is Succesful Boolean*/);
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("PunalOpenclFunctionsProgram.cl", &Functions_List, &AvailablePlatformVendors, Issuccessful);
	
	//Initialization End
}
```

## Runing Kernel:
```c++
int main()
{	
	//Initialization Is Ommited from Above

	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStruct = nullptr;// No need to free it since it is only pointing to The One Inside
	EntireOpenCLProgram.GetKernelInformation("Add_Integers", &OrderedStruct, Issuccessful);//This Gets the Pointer to Ordered Argument List Created in Initialization
	
	//OCLW_P::cl_MultiDevice_KernelArgumentSendStruct [Constructor](/*Total Number Of Devices*/, 
      ///*Pointer to Constructed OCLW_P::cl_KernelFunctionArgumentOrderListStruct*/,
      ///*Is Succesful Boolean*/);
	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceData(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStruct, Issuccessful);
	
	//int X = "Total Elements";// Anything of Choice, like Read[Y], ReadWrite[Z] etc
	cl_int Read[X];//Creation Of Global Variables are the Same
	cl_int Write[X];//NOTE: Every Buffer Should Be Initialized even if Write[X] Buffer's Element's value are 0
	cl_int ReadWrite[X];
	cl_int Local = nullptr;//Let the Size Be Y
	cl_int Private = Z;//Z being Some Value

	//StoreDataForKernelArgument(/*Kernel Numbter*/,
      ///*Argument Number*/, /*Buffer Number*/,
      ///*Pointer To Data*/, /*Size Of Data*/, /*Is Succesful Boolean*/,
      //OverWriteMemory_NOTForLOCAL_PRIVATE = false, bool UsePreviouslyAllocatedMemoryOnBuffer = false);//These Two Are Optional, To be Used Only after Storing Data Atleast Once
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 0, Read, X * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 0, Write,     X * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 0, ReadWrite, X * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 3, 0, nullptr,   Y * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 4, 0, Private, 	   sizeof(cl_int), Issuccessful);

	//Setting our Range
	//OCLW_P::cl_MultiDevice_NDRangeStruct [Constructor](/*Total Number Of Devices*/, 
      ///*Is Succesful Boolean*/);
	OCLW_P::cl_MultiDevice_NDRangeStruct MultiNDRange(EntireOpenCLProgram.GetTotalNumberOfDevices(), Issuccessful);
	OCLW_P::cl_NDRangeStruct* NDRangeStructPtr = nullptr;//Single Device
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtr, Issuccessful);//Pointer to NDRange of Device 0
	NDRangeStructPtr->SetNDRange(10, 1, 0);
	
	//RunKernelFunction(/*Kernel Number or Name*/,
      ///*Devices to Run From*/, /*Devices to Run to*/,// Example From and To is Set to 2,5 it means Kernel Will be run on 2,3,4,5(Assuming NDRange Is Set) 
      ///*Multi NDRange Struct*/, /*Is Succesful Boolean*/,
      //OCLW_P::cl_MultiDevice_KernelArgumentSendStruct = nullptr);//Should be Set Atleast Once before Use
	EntireOpenCLProgram.RunKernelFunction("KernelOne", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceData);// Here Only Device 0 Will run

	Essenbp::UnknownDataAndSizeStruct RetreivedData;//A Simple Struct of Void* Data and size_t Size
	
	//RetreiveDataFromKernel(/*Device Number*/, /*Kernel Number*/, /*Argument Number*/
      ///*Buffer Number*/, /*Is Succesful Boolean*/)
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 0, 0, RetreivedData, Issuccessful);
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 0:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 1, 0, RetreivedData, Issuccessful);
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 1:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 2, 0, RetreivedData, Issuccessful);	
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	return 0;
}
```

## Add/Remove/Reuse/Overwrite/Share/UnShare/Interchange for Buffer:
```c++
int main()
{
	//Initialization Is Ommitted
	//Run Kernel Is Ommitted
	
	//AddBufferForArgument(unsigned int DeviceNumber, unsigned int KernelNumber, unsigned int KernelArgumentNumber, bool& IsSuccessful)
	EntireOpenCLProgram.AddBufferForArgument(0, 0, 0, Issuccessful);
	
	//RemoveBufferForArgument(unsigned int DeviceNumber, unsigned int KernelNumber, unsigned int KernelArgumentNumber, unsigned int BufferNumber, bool& IsSuccessful)
	EntireOpenCLProgram.RemoveBufferForArgument(0, 0, 0, 0, Issuccessful);
	
	//StoreDataForKernelArgument(unsigned int DeviceNumber, unsigned int ArgumentNumber, unsigned int BufferNumber, void* ArgData, size_t ArgSizeOfData, bool& IsSuccessful, bool OverWriteMemory_NOTForLOCAL_PRIVATE = false, bool UsePreviouslyAllocatedMemoryOnBuffer = false)
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 0, nullptr, 0, Issuccessful, false, true);
	
	//ShareBufferWithAnotherKernel(unsigned int DeviceNumber, unsigned int KernelNumber, unsigned int ArgumentNumber, unsigned int ArgBufferNumber, unsigned int ChildKernelNumber, unsigned int ChildArgumentNumber, unsigned int ChildBufferNumber, bool& IsSuccessful)
	EntireOpenCLProgram.ShareBufferWithAnotherKernel(0, 0, 0, 0, 1, 0, 0, Issuccessful);//Sharing Arugment0Buffer0 of Kernel0(KenrelOne) with Arugment0Buffer0 of Kernel1(KernelTwo)
	
	//StopSharingBufferWithAnotherKernel(unsigned int DeviceNumber, unsigned int KernelNumber, unsigned int ArgumentNumber, unsigned int ArgBufferNumber, unsigned int ChildKernelNumber, unsigned int ChildArgumentNumber, unsigned int ChildBufferNumber, bool& IsSuccessful)
	EntireOpenCLProgram.StopSharingBufferWithAnotherKernel(0, 0, 0, 0, 1, 0, 0, Issuccessful);
	
	//InterchangeBufferWithinSameDevice(unsigned int DeviceNumber, unsigned int KernelNumber, unsigned int ArgumentNumber, unsigned int BufferNumber, unsigned int TargetKernelNumber, unsigned int TargetArgumentNumber, unsigned int TargetBufferNumber, bool& IsSuccessful)
	EntireOpenCLProgram.InterchangeBufferWithinSameDevice(0, 0, 0, 0, 1, 0, 0, Issuccessful);//Inter Changing Arugment0Buffer0 of Kernel0(KernelOne) with Arugment0Buffer0 of Kernel1(KernelTwo)
	
	//InterchangeBufferWithAnotherDevice(unsigned int DeviceNumber, unsigned int KernelNumber, unsigned int ArgumentNumber, unsigned int BufferNumber, unsigned int TargetDeviceNumber, unsigned int TargetKernelNumber, unsigned int TargetArgumentNumber, unsigned int TargetBufferNumber, bool& IsSuccessful)
	EntireOpenCLProgram.InterchangeBufferWithAnotherDevice(0, 0, 0, 0, 1 ,1, 0, 0, Issuccessful);//Inter Changing Arugment0Buffer0 of Kernel0(KernelOne) with Arugment0Buffer0 of Kernel1(KernelTwo)
}
```

## Additonal Functions:
```c++
int main()
{
	unsigned int GetTotalNumberOfDevices()
	unsigned int GetTotalNumberOfKernelFunctions()
	void GetBinaryInformationOfProgram(Essenbp::ArrayOfUnknownDataAndSize& Binary_Program_OfEachDevice, bool& IsSuccessful)
	void GetKernelNumberByKernelName(unsigned int& KernelNumber, std::string NameOfTheKernel, bool& IsSuccessful)
	void GetKernelInformation(unsigned int KernelNumber, cl_KernelFunctionArgumentOrderListStruct** OrderedStruct, bool& IsSuccessful)
	void GetBufferNumberBeingUsed(unsigned int DeviceNumber, unsigned int KernelNumber, unsigned int ArgumentNumber, unsigned int& ArgBufferNumber, bool& IsSuccessful)


	void GetDeviceInfoStruct(unsigned int DeviceNumber, cl_PerDeviceValuesStruct** DeviceInfo, bool& IsSuccessful)
	//Device Info Contains
		const cl_device_id SelectedDevice;
		cl_command_queue DeviceClCommandQueue = NULL;
		size_t MaxComputeUnitPerGPU = 0;
		size_t MaxWorkItemPerGroup = 0;
		size_t MaxGlobalMemoryOfDevice = 0;
		size_t MaxPrivateMemoryBytesPerWorkGroup = 0;
		size_t MaxLocalMemoryBytesPerWorkGroup = 0;


	//Gets Binary Information About Program 
	void GetBinaryInformationOfProgram(Essenbp::ArrayOfUnknownDataAndSize& Binary_Program_OfEachDevice, bool& IsSuccessful)
}
```

# About Me
I am Punal Manalan, I Spend My Free Time in C++ Mostly Optimizing(Pointer & Less Code) my Code. Programming for Over 2 years as of 2021, I also Use Unreal Engine 4(C++).
Took me 4 Months to Complete **OCLW_P Wrapper** Mainly Due to me spending Almost all my free time Playing **Black Mesa, Metro Redux, Metro LastLight, Metro Exodus, Halo CE, Halo 2, Halo 3, Enlisted, Warthunder, Planetside 2, swtor(returned after 6 years).** in thee Last 3 Months...
In Fact i should have already completed this in a month at max...
