#include "OCLW_P.h"

//int main()
//{
//	bool IsSuccesful = false;
//
//	// Single Line Code, First platform is chosen by default 
//	OCLW_P::OpenCLWrapper EntireOpenCLProgram("PunalOpenclFunctionsProgram.cl", IsSuccesful);//NOTE: OCLW_P::cl_PlatformVendorStruct can be used to choose platform of your choice
//	if (IsSuccesful)
//	{
//		EntireOpenCLProgram.RunKernelFunction();
//		return 1;
//	}
//	return 0;
//}

int main()
{
	//Testing
	bool IsSuccesful = false;
	OCLW_P::cl_KernelFunctionsStruct Functions_List(1, IsSuccesful);
	if (!IsSuccesful)
	{
		std::cout << "\n Error: Variable construction Unsuccesful";
		return 1;
	}

	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Add_Integers", 3, IsSuccesful);
	if (!IsSuccesful)
	{
		std::cout << "\n Error: Function Unsuccesful";
		return 1;
	}
		
	//NOTE: Checking for failures is a good practice!
	Functions_List.SetMemoryTypeOfArugment(0, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, IsSuccesful);
	/*if (!IsSuccesful)
	{
		std::cout << "\n Error: Function Unsuccesful";
		return 1;
	}*/
	Functions_List.SetMemoryTypeOfArugment(0, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, IsSuccesful);
	/*if (!IsSuccesful)
	{
		std::cout << "\n Error: Function Unsuccesful";
		return 1;
	}*/
	Functions_List.SetMemoryTypeOfArugment(0, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, IsSuccesful);
	/*if (!IsSuccesful)
	{
		std::cout << "\n Error: Function Unsuccesful";
		return 1;
	}*/

	OCLW_P::cl_PlatformVendorStruct AvailablePlatformVendors(IsSuccesful);
	if (!IsSuccesful)
	{
		std::cout << "\n Error: Variable construction Unsuccesful";
		return 1;
	}

	int TotalNumberOfPlatforms;
	AvailablePlatformVendors.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, IsSuccesful);
	/*if (!IsSuccesful)
	{
		std::cout << "\n Error: Function Unsuccesful";
		return 1;
	}*/	
	AvailablePlatformVendors.PrintAllAvailablePlatformVendorNames(IsSuccesful);
	/*if (!IsSuccesful)
	{
		std::cout << "\n Error: Function Unsuccesful";
		return 1;
	}*/
	AvailablePlatformVendors.SetChosenPlatform(1, IsSuccesful);
	/*if (!IsSuccesful)
	{
		std::cout << "\n Error: Function Unsuccesful";
		return 1;
	}*/

	std::string FilePath = "D:\\C++ Projects\\Opencl Punal Wrapper\\OpenCL Wrapper by Punal\\PunalOpenclFunctionsProgram.cl";
	OCLW_P::OpenCLWrapper EntireOpenCLProgram(FilePath, &Functions_List, 1, &AvailablePlatformVendors, IsSuccesful);

	//std::cout << "\n Before Destruction";
	//EntireOpenCLProgram.~cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct();// Doing this just to show that the destructor is working properly
	//std::cout << "\n After  Destruction";

	return 0;
}