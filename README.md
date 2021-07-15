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
1. OpenCL Minimum Version 1.2
2. "Essenbp.h" Header(Provided)

# How to use
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

## Run Kernel
```c++
int main()
{	
	//Initialization Is Ommited from Above

	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStruct = nullptr;// No need to free it since it is only pointing to The One Inside
	EntireOpenCLProgram.GetKernelInformation("Add_Integers", &OrderedStruct, Issuccessful);//This Gets the Pointer to Argument List Order Created in Initialization

	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceData(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStruct, Issuccessful);
	
	
	cl_int Read[X];//Creation Of Global Variables are the Same
	cl_int Write[X];//NOTE: Every Buffer Should Be Initialized even if Write[X] Buffer's Element's value are 0
	cl_int ReadWrite[X];
	cl_int Local = nullptr;
	cl_int Private = Y;

	//StoreDataForKernelArgument(/*Kernel Numbter*/, /*Argument Number*/, /*Buffer Number*/, /*Pointer To Data*/, /*Size Of Data*/, /*Is Succesful Boolean*/);
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 0, Read, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 0, Write, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 0, ReadWrite, 10 * sizeof(cl_int), Issuccessful);

	//Setting our Range
	OCLW_P::cl_MultiDevice_NDRangeStruct MultiNDRange(EntireOpenCLProgram.GetTotalNumberOfDevices(), Issuccessful);
	OCLW_P::cl_NDRangeStruct* NDRangeStructPtr = nullptr;
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtr, Issuccessful);
	NDRangeStructPtr->SetNDRange(10, 1, 0);

	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceData);

	Essenbp::UnknownDataAndSizeStruct RetreivedData;
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
