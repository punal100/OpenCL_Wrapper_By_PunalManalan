#include "OCLW_P.h"

int Test1()
{
	bool Issuccessful = false;

	// Single Line Code, First platform is chosen by default 
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("PunalOpenclFunctionsProgram.cl", Issuccessful);//NOTE: OCLW_P::cl_PlatformVendorStruct can be used to choose platform of your choice

	// Choosing which Function To Use
	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStruct = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Add_Integers", &OrderedStruct, Issuccessful);
	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceData(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStruct, Issuccessful);

	// In This Test The Chosen Function Has 3 Arugments
	int IntA[10] = { 1 , 2, 3, 4, 5, 6, 7, 8, 9,10 };			//Input   Arg 0
	int IntB[10] = { 11,12,13,14,15,16,17,18,19,20 };			//Input	  Arg 1
	int IntC[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };			//OutPut  Arg 2 //This Is Optonal since Write only memory can't be read from kernel...

	//Device Number 0 for first Device, Argument Number 0 for first Argument , 1 to second argument, 2 for thrid argument
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 0, IntA, 10 * sizeof(int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 0, IntB, 10 * sizeof(int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 0, IntC, 10 * sizeof(int), Issuccessful);

	//Setting our Range
	OCLW_P::cl_MultiDevice_NDRangeStruct MultiNDRange(EntireOpenCLProgram.GetTotalNumberOfDevices(), Issuccessful);
	OCLW_P::cl_NDRangeStruct* NDRangeStructPtr = nullptr;
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtr, Issuccessful);//Chose Device Number
	//NDRangeStructPtr->SetNDRange(10, 1, 0);// 10 Work Items, 1 Per WorkGroup, 0 Offset
	NDRangeStructPtr->SetNDRangeWithoutLocal(10, 0);// 10 Work Items, Automatic Per WorkGroup, 0 Offset

	//Running the Kernel
	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceData);

	//Retreiving the Data
	Essenbp::UnknownDataAndSizeStruct RetreivedData;
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument()

	Essenbp::WriteLogToFile("\n Retreived Data of Arg 0:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	return 0;
}

int Test2()
{
	//Testing
	bool Issuccessful = false;
	OCLW_P::cl_KernelFunctionsStruct Functions_List(1, Issuccessful);
	if (!Issuccessful)
	{
		Essenbp::WriteLogToFile("\n Error: Variable construction Unsuccesful");
		return 1;
	}

	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Add_Integers", 3, Issuccessful);
	if (!Issuccessful)
	{
		Essenbp::WriteLogToFile("\n Error: Function Unsuccesful");
		return 1;
	}

	//NOTE: Checking for failures is a good practice!
	Functions_List.SetMemoryTypeOfArugment(0, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	/*if (!Issuccessful)
	{
		Essenbp::WriteLogToFile("\n Error: Function Unsuccesful";
		return 1;
	}*/
	Functions_List.SetMemoryTypeOfArugment(0, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	/*if (!Issuccessful)
	{
		Essenbp::WriteLogToFile("\n Error: Function Unsuccesful";
		return 1;
	}*/
	Functions_List.SetMemoryTypeOfArugment(0, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);
	/*if (!Issuccessful)
	{
		Essenbp::WriteLogToFile("\n Error: Function Unsuccesful";
		return 1;
	}*/

	OCLW_P::cl_PlatformVendorStruct AvailablePlatformVendors(Issuccessful);
	if (!Issuccessful)
	{
		Essenbp::WriteLogToFile("\n Error: Variable construction Unsuccesful");
		return 1;
	}

	int TotalNumberOfPlatforms;
	AvailablePlatformVendors.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, Issuccessful);
	/*if (!Issuccessful)
	{
		Essenbp::WriteLogToFile("\n Error: Function Unsuccesful";
		return 1;
	}*/
	AvailablePlatformVendors.PrintAllAvailablePlatformVendorNames(Issuccessful);
	/*if (!Issuccessful)
	{
		Essenbp::WriteLogToFile("\n Error: Function Unsuccesful";
		return 1;
	}*/
	AvailablePlatformVendors.SetChosenPlatform(1, Issuccessful);
	/*if (!Issuccessful)
	{
		Essenbp::WriteLogToFile("\n Error: Function Unsuccesful";
		return 1;
	}*/

	std::string FilePath = "D:\\C++ Projects\\Opencl Punal Wrapper\\OpenCL Wrapper by Punal\\PunalOpenclFunctionsProgram.cl";
	OCLW_P::OpenCLWrapper EntireOpenCLProgram(FilePath, &Functions_List, 1, &AvailablePlatformVendors, Issuccessful);

	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStruct = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Add_Integers", &OrderedStruct, Issuccessful);

	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceData(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStruct, Issuccessful);

	int IntA[10] = { 1 , 2, 3, 4, 5, 6, 7, 8, 9,10 };			//Input   Arg 0
	int IntB[10] = { 11,12,13,14,15,16,17,18,19,20 };			//Input	  Arg 1
	int IntC[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };			//OutPut  Arg 2

	//Device Number 0 for first Device, Argument Number 0 for first Argument
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 0, IntA, 10 * sizeof(int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 0, IntB, 10 * sizeof(int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 0, IntC, 10 * sizeof(int), Issuccessful);

	//Setting our Range
	OCLW_P::cl_MultiDevice_NDRangeStruct MultiNDRange(EntireOpenCLProgram.GetTotalNumberOfDevices(), Issuccessful);
	OCLW_P::cl_NDRangeStruct* NDRangeStructPtr = nullptr;
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtr, Issuccessful);
	NDRangeStructPtr->SetNDRange(10, 1, 0);

	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceData);

	Essenbp::UnknownDataAndSizeStruct RetreivedData;
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 0, 0, RetreivedData, Issuccessful);

	Essenbp::WriteLogToFile("\n Retreived Data of Arg 0:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 1, 0, RetreivedData, Issuccessful);
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 1:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 2, 0, RetreivedData, Issuccessful);	
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	Essenbp::ArrayOfUnknownDataAndSize ArrayOfRetreivedData;
	EntireOpenCLProgram.GetBinaryInformationOfProgram(ArrayOfRetreivedData, Issuccessful);

	Essenbp::UnknownDataAndSizeStruct* ReturnValue;
	ArrayOfRetreivedData.GetData(0, &ReturnValue, Issuccessful);

	Essenbp::WriteBytesToFile("D:\\TestFolder\\BinaryReturn", "txt", *ReturnValue, Issuccessful);
	Essenbp::WriteLogToFile("\n\n Before Destruction");
	//EntireOpenCLProgram.~cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct();// Doing this just to show that the destructor is working properly
	//Essenbp::WriteLogToFile("\n After  Destruction";

	return 0;
}

int main()
{
	Test1();
	//Test2();
}