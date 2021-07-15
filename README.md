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

# How to use
## Initialization:
```c++
#include "OCLW_P.h"

int main()
{	
	//Error Checking is omitted
	bool Issuccessful = false;//NOTE: Checking for failures is a good practice!
	
	//int N = "Total_Number_Of_Functions";//Example 2 functions namely "Add_Integers" & "Sub_Integers"//Function Names are Case and Symbol Sensitive
	//int K1 = "Total_Number_Of_Arugmnets_Of_Each_Specific_Function"//Example: 3 Argument: 2 input And 1 Output
	//int K2 = "Total_Number_Of_Arugmnets_Of_Each_Specific_Function"//Example: 3 Argument: 2 input And 1 Output
	
	OCLW_P::cl_KernelFunctionsStruct Functions_List(N, Issuccessful);// Specify The Total Number of functions to add
	
	//NOTE: Function Names and Arugmnets are Stored in Order Like this: First->Second...->Last == (1,2...,N)
	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Add_Integers", K1, Issuccessful);// 1st Function
	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Sub_Integers", K2, Issuccessful);// 2nd Function

	//First Function "Add_Integers"
	Functions_List.SetMemoryTypeOfArugment(0, K1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);//Input
	Functions_List.SetMemoryTypeOfArugment(0, K1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);//Input
	Functions_List.SetMemoryTypeOfArugment(0, K1, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);//OutPut
	
	//Second Function "Sub_Integers"
	Functions_List.SetMemoryTypeOfArugment(1, K2, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);//Input
	Functions_List.SetMemoryTypeOfArugment(1, K2, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);//Input
	Functions_List.SetMemoryTypeOfArugment(1, K2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);//OutPut
	
	//This Returns ALL the Available Platforms(Example: Intel, AMD, Nvdia)
	OCLW_P::cl_PlatformVendorStruct AvailablePlatformVendors(Issuccessful);

	int TotalNumberOfPlatforms = 0;
	AvailablePlatformVendors.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, Issuccessful);
	AvailablePlatformVendors.PrintAllAvailablePlatformVendorNames(Issuccessful);
	AvailablePlatformVendors.SetChosenPlatform(1, Issuccessful);//For Me AMD is the First Platform(GPU), Intel is Second Platform(CPU)

	//std::string FilePath = "C:\\OpenCL Folder\\PunalOpenclFunctionsProgram.cl";// Is Direct Path
	std::string FilePath = "OpenclFunctionsProgram.cl";// This looks for the file in program location
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("PunalOpenclFunctionsProgram.cl", &Functions_List, &AvailablePlatformVendors, Issuccessful);
	
	//Initialization End
}
```
