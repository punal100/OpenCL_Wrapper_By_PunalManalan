#include "OCLW_P.h"

int Test1()
{
	bool IsSuccesful = false;

	// Single Line Code, First platform is chosen by default 
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("PunalOpenclFunctionsProgram.cl", IsSuccesful);//NOTE: OCLW_P::cl_PlatformVendorStruct can be used to choose platform of your choice

	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStruct = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Add_Integers", &OrderedStruct, IsSuccesful);

	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceData(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStruct, IsSuccesful);

	int IntA[10] = { 1,2,3,4,5,6,7,8,9,10 };//Input
	int IntB[10] = { 11,12,13,14,15,16,17,18,19,20 };//Input
	int IntC[10] = { 0,0,0,0,0,0,0,0,0,0 };//OutPut

	//Device Number 0 for first Device, Argument Number 0 for first Argument
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 0, IntA, 10 * sizeof(int), IsSuccesful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 0, IntB, 10 * sizeof(int), IsSuccesful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 0, IntC, 10 * sizeof(int), IsSuccesful);

	//Setting our Range
	OCLW_P::cl_MultiDevice_NDRangeStruct MultiNDRange(EntireOpenCLProgram.GetTotalNumberOfDevices(), IsSuccesful);
	OCLW_P::cl_NDRangeStruct* NDRangeStructPtr = nullptr;
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtr, IsSuccesful);
	NDRangeStructPtr->SetNDRange(10, 1, 0);

	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, IsSuccesful);

	Essenbp::UnknownDataAndSize RetreivedData;
	EntireOpenCLProgram.RetreiveDataFromKernel(0, "Add_Integers", 2, 0, RetreivedData, IsSuccesful);

	std::cout << "\n\nRetreived Output Data";
	for (int i = 0; i < 10; ++i)
	{
		std::cout << "\nValue of ["<<i<<"] Is" << ((int*)RetreivedData.GetData())[i];
	}
	std::cout << "\n\n";
	return 0;
}

int Test2()
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

	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStruct = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Add_Integers", &OrderedStruct, IsSuccesful);

	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceData(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStruct, IsSuccesful);

	int IntA[10] = { 1,2,3,4,5,6,7,8,9,10 };			//Input   Arg 0
	int IntB[10] = { 11,12,13,14,15,16,17,18,19,20 };	//Input	  Arg 1
	int IntC[10] = { 0,0,0,0,0,0,0,0,0,0 };				//OutPut  Arg 2

	//Device Number 0 for first Device, Argument Number 0 for first Argument
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 0, IntA, 10 * sizeof(int), IsSuccesful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 0, IntB, 10 * sizeof(int), IsSuccesful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 0, IntC, 10 * sizeof(int), IsSuccesful);

	//Setting our Range
	OCLW_P::cl_MultiDevice_NDRangeStruct MultiNDRange(EntireOpenCLProgram.GetTotalNumberOfDevices(), IsSuccesful);
	OCLW_P::cl_NDRangeStruct* NDRangeStructPtr = nullptr;
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtr, IsSuccesful);
	NDRangeStructPtr->SetNDRange(10, 1, 0);

	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, IsSuccesful);

	//std::cout << "\n Before Destruction";
	//EntireOpenCLProgram.~cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct();// Doing this just to show that the destructor is working properly
	//std::cout << "\n After  Destruction";

	return 0;
}

int main()
{
	Test2();
}