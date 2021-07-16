#include "OCLW_P.h"

/*//Single Line Auto, Automatically Chooses First Platform, and Kernel Functions are Found And Added Automatically
int Example1()
{
	bool Issuccessful = false;

	// Single Line Code, First platform is chosen by default 
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("ExampleOpenCLKernels.cl", Issuccessful);//NOTE: OCLW_P::cl_PlatformVendorStruct can be used to choose platform of your choice

	// Choosing which Function To Use
	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStruct = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Add_Integers", &OrderedStruct, Issuccessful);
	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceData(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStruct, Issuccessful);

	// In This Test The Chosen Function Has 3 Arugments
	cl_int IntA[10] = { 1 , 2, 3, 4, 5, 6, 7, 8, 9,10 };			//Input   Arg 0
	cl_int IntB[10] = { 11,12,13,14,15,16,17,18,19,20 };			//Input	  Arg 1
	cl_int IntC[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };			//OutPut  Arg 2 //This Is Optonal since Write only memory can't be read from inside kernel()...

	//Device Number 0 for first Device, Argument Number 0 for first Argument , 1 to second argument, 2 for thrid argument
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 0, IntA, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 0, IntB, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 0, IntC, 10 * sizeof(cl_int), Issuccessful);

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

	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	return 0;
}

//Automatic only Set which platform To use, and Kernel Functions found And Added Automatically
int Example2()
{
	bool Issuccessful = false;

	//Querying all the available platform(Example: Intel, AMD, Nvdia)
	OCLW_P::cl_PlatformVendorStruct PlatformToUse(Issuccessful);
	PlatformToUse.PrintAllAvailablePlatformVendorNames(Issuccessful);//Logged in File
	PlatformToUse.SetChosenPlatform(1, Issuccessful);//Choosing The First Platform

	OCLW_P::OpenCLWrapper EntireOpenCLProgram("ExampleOpenCLKernels.cl", &PlatformToUse, Issuccessful);//NOTE: OCLW_P::cl_PlatformVendorStruct can be used to choose platform of your choice

	// Choosing which Function To Use
	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStruct = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Add_Integers", &OrderedStruct, Issuccessful);
	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceData(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStruct, Issuccessful);

	// In This Test The Chosen Function Has 3 Arugments
	cl_int IntA[10] = { 1 , 2, 3, 4, 5, 6, 7, 8, 9,10 };			//Input   Arg 0
	cl_int IntB[10] = { 11,12,13,14,15,16,17,18,19,20 };			//Input	  Arg 1
	cl_int IntC[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };			//OutPut  Arg 2 //This Is Optonal since Write only memory can't be read from inside kernel()...

	//Device Number 0 for first Device, Argument Number 0 for first Argument , 1 to second argument, 2 for thrid argument
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 0, IntA, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 0, IntB, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 0, IntC, 10 * sizeof(cl_int), Issuccessful);

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

	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	return 0;
}*/

//Manual, Set which Platform To Use and Manually Add specic Kernel Functions to Use and and its arguments
int Example1()
{
	bool Issuccessful = false;
	//Initialization Start
	OCLW_P::cl_KernelFunctionsStruct Functions_List(1, Issuccessful);

	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Add_Integers", 3, Issuccessful);

	//NOTE: Checking for failures is a good practice!
	Functions_List.SetMemoryTypeOfArugment(0, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);

	OCLW_P::cl_PlatformVendorStruct AvailablePlatformVendors(Issuccessful);

	int TotalNumberOfPlatforms = 0;
	AvailablePlatformVendors.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, Issuccessful);
	AvailablePlatformVendors.PrintAllAvailablePlatformVendorNames(Issuccessful);
	AvailablePlatformVendors.SetChosenPlatform(1, Issuccessful);

	//std::string FilePath = "D:\\C++ Projects\\Opencl Punal Wrapper\\OpenCL Wrapper by Punal\\ExampleOpenCLKernels.cl";
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("ExampleOpenCLKernels.cl", &Functions_List, &AvailablePlatformVendors, Issuccessful);
	//Initialization End

	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStruct = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Add_Integers", &OrderedStruct, Issuccessful);

	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceData(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStruct, Issuccessful);

	cl_int IntA[10] = { 1 , 2, 3, 4, 5, 6, 7, 8, 9,10 };			//Input   Arg 0
	cl_int IntB[10] = { 11,12,13,14,15,16,17,18,19,20 };			//Input	  Arg 1
	cl_int IntC[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };			//OutPut  Arg 2

	//Device Number 0 for first Device, Argument Number 0 for first Argument
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 0, IntA, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 0, IntB, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 0, IntC, 10 * sizeof(cl_int), Issuccessful);

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

//Add More Buffers(and remove buffer)
int Example2()
{
	bool Issuccessful = false;
	//Initialization Start
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
	Functions_List.SetMemoryTypeOfArugment(0, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);

	OCLW_P::cl_PlatformVendorStruct AvailablePlatformVendors(Issuccessful);

	int TotalNumberOfPlatforms = 0;
	AvailablePlatformVendors.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, Issuccessful);
	AvailablePlatformVendors.PrintAllAvailablePlatformVendorNames(Issuccessful);
	AvailablePlatformVendors.SetChosenPlatform(1, Issuccessful);

	//std::string FilePath = "D:\\C++ Projects\\Opencl Punal Wrapper\\OpenCL Wrapper by Punal\\ExampleOpenCLKernels.cl";
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("ExampleOpenCLKernels.cl", &Functions_List, &AvailablePlatformVendors, Issuccessful);
	//Initialization End

	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStruct = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Add_Integers", &OrderedStruct, Issuccessful);

	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceData(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStruct, Issuccessful);

	cl_int IntA[10] = { 1 , 2, 3, 4, 5, 6, 7, 8, 9,10 };			//Input   Arg 0
	cl_int IntB[10] = { 11,12,13,14,15,16,17,18,19,20 };			//Input	  Arg 1
	cl_int IntC[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };			//OutPut  Arg 2

	//Device Number 0 for first Device, Argument Number 0 for first Argument
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 0, IntA, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 0, IntB, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 0, IntC, 10 * sizeof(cl_int), Issuccessful);

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

	//ADD ANOTHER BUFFER
	EntireOpenCLProgram.AddBufferForArgument(0, 0, 0, Issuccessful);
	EntireOpenCLProgram.AddBufferForArgument(0, 0, 1, Issuccessful);
	EntireOpenCLProgram.AddBufferForArgument(0, 0, 2, Issuccessful);

	cl_int IntA2[10] = { 10 , 20, 30, 40, 50, 60, 70, 80, 90,100 };		//Input   Arg 0
	cl_int IntB2[10] = { 911,812,713,14,615,516,417,318,219,120 };			//Input	  Arg 1
	cl_int IntC2[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };					//OutPut  Arg 2

	//Device Number 0 for first Device, Argument Number 0 for first Argument and Buffer 1(the 2nd Buffer) For each Arugment
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 1, IntA2, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 1, IntB2, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 1, IntC2, 10 * sizeof(cl_int), Issuccessful);

	//Setting our Range
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtr, Issuccessful);
	NDRangeStructPtr->SetNDRange(10, 1, 0);

	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceData);

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 0, 1, RetreivedData, Issuccessful);

	Essenbp::WriteLogToFile("\n Retreived Data of Arg 0:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 1, 1, RetreivedData, Issuccessful);
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 1:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 2, 1, RetreivedData, Issuccessful);
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	//EntireOpenCLProgram.RemoveBufferForArgument(0, 0, 0, 0, Issuccessful);
	//EntireOpenCLProgram.RemoveBufferForArgument(0, 0, 1, 0, Issuccessful);//This is used to remove buffers
	//EntireOpenCLProgram.RemoveBufferForArgument(0, 0, 2, 0, Issuccessful);
	return 0;
}

//Reuse Same Buffer or OverWrite in Same Buffer(Whithout Creating New Buffer)
int Example3()
{
	bool Issuccessful = false;
	//Initialization Start
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
	Functions_List.SetMemoryTypeOfArugment(0, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);

	OCLW_P::cl_PlatformVendorStruct AvailablePlatformVendors(Issuccessful);

	int TotalNumberOfPlatforms = 0;
	AvailablePlatformVendors.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, Issuccessful);
	AvailablePlatformVendors.PrintAllAvailablePlatformVendorNames(Issuccessful);
	AvailablePlatformVendors.SetChosenPlatform(1, Issuccessful);

	//std::string FilePath = "D:\\C++ Projects\\Opencl Punal Wrapper\\OpenCL Wrapper by Punal\\ExampleOpenCLKernels.cl";
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("ExampleOpenCLKernels.cl", &Functions_List, &AvailablePlatformVendors, Issuccessful);
	//Initialization End

	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStruct = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Add_Integers", &OrderedStruct, Issuccessful);

	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceData(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStruct, Issuccessful);

	cl_int IntA[10] = { 1 , 2, 3, 4, 5, 6, 7, 8, 9,10 };			//Input   Arg 0
	cl_int IntB[10] = { 11,12,13,14,15,16,17,18,19,20 };			//Input	  Arg 1
	cl_int IntC[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };			//OutPut  Arg 2

	//Device Number 0 for first Device, Argument Number 0 for first Argument
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 0, IntA, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 0, IntB, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 0, IntC, 10 * sizeof(cl_int), Issuccessful);

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

	//ADD ANOTHER BUFFER
	EntireOpenCLProgram.AddBufferForArgument(0, 0, 0, Issuccessful);
	EntireOpenCLProgram.AddBufferForArgument(0, 0, 1, Issuccessful);
	EntireOpenCLProgram.AddBufferForArgument(0, 0, 2, Issuccessful);

	cl_int IntA2[10] = { 10 , 20, 30, 40, 50, 60, 70, 80, 90,100 };		//Input   Arg 0
	cl_int IntB2[10] = { 911,812,713,14,615,516,417,318,219,120 };			//Input	  Arg 1
	cl_int IntC2[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };					//OutPut  Arg 2

	//Device Number 0 for first Device, Argument Number 0 for first Argument and Buffer 1(the 2nd Buffer) For each Arugment
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 1, IntA2, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 1, IntB2, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 1, IntC2, 10 * sizeof(cl_int), Issuccessful);

	//Setting our Range
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtr, Issuccessful);
	NDRangeStructPtr->SetNDRange(10, 1, 0);

	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceData);

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 0, 1, RetreivedData, Issuccessful);

	Essenbp::WriteLogToFile("\n Retreived Data of Arg 0:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 1, 1, RetreivedData, Issuccessful);
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 1:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 2, 1, RetreivedData, Issuccessful);
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	//UsePrevious Memory
	//Device Number 0 for first Device, Argument Number 0 for first Argument and Buffer 0(the 1st Buffer) For each Arugment
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 0, nullptr, 0, Issuccessful, false, true);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 0, nullptr, 0, Issuccessful, false, true);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 0, nullptr, 0, Issuccessful, false, true);

	//Setting our Range
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtr, Issuccessful);
	NDRangeStructPtr->SetNDRange(10, 1, 0);

	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceData);

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 0, 1, RetreivedData, Issuccessful);

	Essenbp::WriteLogToFile("\n Retreived Data of Arg 0:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 1, 1, RetreivedData, Issuccessful);
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 1:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 2, 1, RetreivedData, Issuccessful);
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	//OverWrite Portion Of Memory of Buffer 2 with Half of Buffer 1
	//Device Number 0 for first Device, Argument Number 0 for first Argument and Buffer 0(the 1st Buffer) For each Arugment
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 1, IntA, 5 * sizeof(cl_int), Issuccessful, true, false);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 1, IntB, 5 * sizeof(cl_int), Issuccessful, true, false);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 1, IntC, 5 * sizeof(cl_int), Issuccessful, true, false);

	//Setting our Range
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtr, Issuccessful);
	NDRangeStructPtr->SetNDRange(10, 1, 0);

	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceData);

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 0, 1, RetreivedData, Issuccessful);

	Essenbp::WriteLogToFile("\n Retreived Data of Arg 0:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 1, 1, RetreivedData, Issuccessful);
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 1:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 2, 1, RetreivedData, Issuccessful);
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
		
	//This Below Code Is Optional As Upon Destruction of Structs this is automatically executed
	//MultiDeviceData.FreeDataInSendStruct(0, 0);
	//MultiDeviceData.FreeDataInSendStruct(0, 1);//This is Used to Remove(free) Data from the Send Struct when not needed
	//MultiDeviceData.FreeDataInSendStruct(0, 2);
	//EntireOpenCLProgram.RemoveBufferForArgument(0, 0, 0, 0, Issuccessful);
	//EntireOpenCLProgram.RemoveBufferForArgument(0, 0, 1, 0, Issuccessful);//This is used to remove buffers
	//EntireOpenCLProgram.RemoveBufferForArgument(0, 0, 2, 0, Issuccessful);
	return 0;
}

//Share Buffer Within Same Device
int Example4()
{
	bool Issuccessful = false;
	//Initialization Start
	//NOTE: Checking for failures is a good practice!
	OCLW_P::cl_KernelFunctionsStruct Functions_List(2, Issuccessful);

	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Add_Integers", 3, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);
	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Sub_Integers", 3, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);

	//Querying all the available platform(Example: Intel, AMD, Nvdia)
	OCLW_P::cl_PlatformVendorStruct AvailablePlatformVendors(Issuccessful);
	int TotalNumberOfPlatforms = 0;
	AvailablePlatformVendors.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, Issuccessful);
	AvailablePlatformVendors.PrintAllAvailablePlatformVendorNames(Issuccessful);//Logged in File
	AvailablePlatformVendors.SetChosenPlatform(1, Issuccessful);//Choosing The First Platform

	//std::string FilePath = "D:\\C++ Projects\\Opencl Punal Wrapper\\OpenCL Wrapper by Punal\\ExampleOpenCLKernels.cl";
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("ExampleOpenCLKernels.cl", &Functions_List, &AvailablePlatformVendors, Issuccessful);
	//Initialization End

	// Choosing which Function To Use
	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStructAdd = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Add_Integers", &OrderedStructAdd, Issuccessful);
	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceDataAdd(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStructAdd, Issuccessful);

	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStructSub = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Sub_Integers", &OrderedStructSub, Issuccessful);
	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceDataSub(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStructSub, Issuccessful);

	// In This Test The Chosen Function Has 3 Arugments
	//Add function
	cl_int IntA[10] = { 1 , 2, 3, 4, 5, 6, 7, 8, 9,10 };			//Input   Arg 0
	cl_int IntB[10] = { 11,12,13,14,15,16,17,18,19,20 };			//Input	  Arg 1
	cl_int IntC[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };			//OutPut  Arg 2 //This Is Optonal since Write only memory can't be read from inside kernel()...

	//Sub fucntion
	cl_int IntD[10] = { 99 , 88, 77, 66, 55, 44, 33, 22, 11,00 };	//Input   Arg 0
	cl_int IntE[10] = { 1,12,23,34,45,56,67,78,89,100 };			//Input	  Arg 1
	cl_int IntF[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };			//OutPut  Arg 2 //This Is Optonal since Write only memory can't be read from inside kernel()...

	//Device Number 0 for first Device, Argument Number 0 for first Argument , 1 to second argument, 2 for thrid argument
	//Add
	MultiDeviceDataAdd.StoreDataForKernelArgument(0, 0, 0, IntA, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataAdd.StoreDataForKernelArgument(0, 1, 0, IntB, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataAdd.StoreDataForKernelArgument(0, 2, 0, IntC, 10 * sizeof(cl_int), Issuccessful);
	//Sub Same as Add
	MultiDeviceDataSub.StoreDataForKernelArgument(0, 0, 0, IntD, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataSub.StoreDataForKernelArgument(0, 1, 0, IntE, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataSub.StoreDataForKernelArgument(0, 2, 0, IntF, 10 * sizeof(cl_int), Issuccessful);

	//Setting our Range
	OCLW_P::cl_MultiDevice_NDRangeStruct MultiNDRange(EntireOpenCLProgram.GetTotalNumberOfDevices(), Issuccessful);
	OCLW_P::cl_NDRangeStruct* NDRangeStructPtr = nullptr;
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtr, Issuccessful);//Chose Device Number
	//NDRangeStructPtr->SetNDRange(10, 1, 0);// 10 Work Items, 1 Per WorkGroup, 0 Offset
	NDRangeStructPtr->SetNDRangeWithoutLocal(10, 0);// 10 Work Items, Automatic Per WorkGroup, 0 Offset

	//Running the Kernel
	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceDataAdd);
	EntireOpenCLProgram.RunKernelFunction("Sub_Integers", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceDataSub);

	//Retreiving the Data
	Essenbp::UnknownDataAndSizeStruct RetreivedData;

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 1, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	//Share the Buffer with same Device
	EntireOpenCLProgram.ShareBufferWithAnotherKernel(0, 0, 0, 0, 1, 0, 0, Issuccessful);//Sharing Arugment0Buffer0 of Kernel0(Add) with Arugment0Buffer0 of Kernel1(Sub)
	EntireOpenCLProgram.ShareBufferWithAnotherKernel(0, 0, 1, 0, 1, 1, 0, Issuccessful);//Sharing Arugment1Buffer0 of Kernel0(Add) with Arugment1Buffer0 of Kernel1(Sub)	

	//Running the Kernel
	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, Issuccessful);
	EntireOpenCLProgram.RunKernelFunction("Sub_Integers", 0, 0, &MultiNDRange, Issuccessful);

	//Retreiving the Data Again
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 0, 0, RetreivedData, Issuccessful);// Retrieved Data of 1rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 0 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 1, 0, RetreivedData, Issuccessful);// Retrieved Data of 2rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 1 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 1, 0, 0, RetreivedData, Issuccessful);// Retrieved Data of 1rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 0 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 1, 1, 0, RetreivedData, Issuccessful);// Retrieved Data of 2rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 1 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 1, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	//EntireOpenCLProgram.StopSharingBufferWithAnotherKernel(0, 0, 0, 0, 1, 0, 0, Issuccessful);
	//EntireOpenCLProgram.StopSharingBufferWithAnotherKernel(0, 0, 1, 0, 1, 1, 0, Issuccessful);
	return 0;
}

//Interchange Buffer Within Same Device
int Example5()
{
	bool Issuccessful = false;
	//Initialization Start
	//NOTE: Checking for failures is a good practice!
	OCLW_P::cl_KernelFunctionsStruct Functions_List(2, Issuccessful);

	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Add_Integers", 3, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);
	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Sub_Integers", 3, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);

	//Querying all the available platform(Example: Intel, AMD, Nvdia)
	OCLW_P::cl_PlatformVendorStruct AvailablePlatformVendors(Issuccessful);
	int TotalNumberOfPlatforms = 0;
	AvailablePlatformVendors.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, Issuccessful);
	AvailablePlatformVendors.PrintAllAvailablePlatformVendorNames(Issuccessful);//Logged in File
	AvailablePlatformVendors.SetChosenPlatform(1, Issuccessful);//Choosing The First Platform

	//std::string FilePath = "D:\\C++ Projects\\Opencl Punal Wrapper\\OpenCL Wrapper by Punal\\ExampleOpenCLKernels.cl";
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("ExampleOpenCLKernels.cl", &Functions_List, &AvailablePlatformVendors, Issuccessful);
	//Initialization End

	// Choosing which Function To Use
	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStructAdd = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Add_Integers", &OrderedStructAdd, Issuccessful);
	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceDataAdd(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStructAdd, Issuccessful);
	
	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStructSub = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Sub_Integers", &OrderedStructSub, Issuccessful);
	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceDataSub(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStructSub, Issuccessful);

	// In This Test The Chosen Function Has 3 Arugments
	//Add function
	cl_int IntA[10] = { 1 , 2, 3, 4, 5, 6, 7, 8, 9,10 };			//Input   Arg 0
	cl_int IntB[10] = { 11,12,13,14,15,16,17,18,19,20 };			//Input	  Arg 1
	cl_int IntC[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };			//OutPut  Arg 2 //This Is Optonal since Write only memory can't be read from inside kernel()...

	//Sub fucntion
	cl_int IntD[10] = { 99 , 88, 77, 66, 55, 44, 33, 22, 11,00 };	//Input   Arg 0
	cl_int IntE[10] = { 1,12,23,34,45,56,67,78,89,100 };			//Input	  Arg 1
	cl_int IntF[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };			//OutPut  Arg 2 //This Is Optonal since Write only memory can't be read from inside kernel()...

	//Device Number 0 for first Device, Argument Number 0 for first Argument , 1 to second argument, 2 for thrid argument
	//Add
	MultiDeviceDataAdd.StoreDataForKernelArgument(0, 0, 0, IntA, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataAdd.StoreDataForKernelArgument(0, 1, 0, IntB, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataAdd.StoreDataForKernelArgument(0, 2, 0, IntC, 10 * sizeof(cl_int), Issuccessful);
	//Sub Same as Add
	MultiDeviceDataSub.StoreDataForKernelArgument(0, 0, 0, IntD, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataSub.StoreDataForKernelArgument(0, 1, 0, IntE, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataSub.StoreDataForKernelArgument(0, 2, 0, IntF, 10 * sizeof(cl_int), Issuccessful);

	//Setting our Range
	OCLW_P::cl_MultiDevice_NDRangeStruct MultiNDRange(EntireOpenCLProgram.GetTotalNumberOfDevices(), Issuccessful);
	OCLW_P::cl_NDRangeStruct* NDRangeStructPtr = nullptr;
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtr, Issuccessful);//Chose Device Number
	//NDRangeStructPtr->SetNDRange(10, 1, 0);// 10 Work Items, 1 Per WorkGroup, 0 Offset
	NDRangeStructPtr->SetNDRangeWithoutLocal(10, 0);// 10 Work Items, Automatic Per WorkGroup, 0 Offset

	//Running the Kernel
	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceDataAdd);
	EntireOpenCLProgram.RunKernelFunction("Sub_Integers", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceDataSub);

	//Retreiving the Data
	Essenbp::UnknownDataAndSizeStruct RetreivedData;

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 1, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	//InterChange the Buffer with same Device
	EntireOpenCLProgram.InterchangeBufferWithinSameDevice(0, 0, 0, 0, 1, 0, 0, Issuccessful);//Inter Changing Arugment0Buffer0 of Kernel0(Add) with Arugment0Buffer0 of Kernel1(Sub)
	EntireOpenCLProgram.InterchangeBufferWithinSameDevice(0, 0, 1, 0, 1, 1, 0, Issuccessful);//Inter Changing Arugment1Buffer0 of Kernel0(Add) with Arugment1Buffer0 of Kernel1(Sub)	

	//Running the Kernel
	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, Issuccessful);
	EntireOpenCLProgram.RunKernelFunction("Sub_Integers", 0, 0, &MultiNDRange, Issuccessful);

	//Retreiving the Data Again
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 0, 0, RetreivedData, Issuccessful);// Retrieved Data of 1rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 0 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 1, 0, RetreivedData, Issuccessful);// Retrieved Data of 2rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 1 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 1, 0, 0, RetreivedData, Issuccessful);// Retrieved Data of 1rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 0 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 1, 1, 0, RetreivedData, Issuccessful);// Retrieved Data of 2rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 1 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 1, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	return 0;
}

//Interchange Buffer Within Another Device //NOTE: ONLY FOR MULTI-GPU
int Example6()
{
	bool Issuccessful = false;
	//Initialization Start
	//NOTE: Checking for failures is a good practice!
	OCLW_P::cl_KernelFunctionsStruct Functions_List(2, Issuccessful);

	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Add_Integers", 3, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);
	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Sub_Integers", 3, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);

	//Querying all the available platform(Example: Intel, AMD, Nvdia)
	OCLW_P::cl_PlatformVendorStruct AvailablePlatformVendors(Issuccessful);
	int TotalNumberOfPlatforms = 0;
	AvailablePlatformVendors.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, Issuccessful);
	AvailablePlatformVendors.PrintAllAvailablePlatformVendorNames(Issuccessful);//Logged in File
	AvailablePlatformVendors.SetChosenPlatform(1, Issuccessful);//Choosing The First Platform

	//std::string FilePath = "D:\\C++ Projects\\Opencl Punal Wrapper\\OpenCL Wrapper by Punal\\ExampleOpenCLKernels.cl";
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("ExampleOpenCLKernels.cl", &Functions_List, &AvailablePlatformVendors, Issuccessful);
	//Initialization End

	// Choosing which Function To Use
	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStructAdd = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Add_Integers", &OrderedStructAdd, Issuccessful);
	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceDataAdd(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStructAdd, Issuccessful);

	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStructSub = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Sub_Integers", &OrderedStructSub, Issuccessful);
	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceDataSub(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStructSub, Issuccessful);

	// In This Test The Chosen Function Has 3 Arugments
	//Add function
	cl_int IntA[10] = { 1 , 2, 3, 4, 5, 6, 7, 8, 9,10 };			//Input   Arg 0
	cl_int IntB[10] = { 11,12,13,14,15,16,17,18,19,20 };			//Input	  Arg 1
	cl_int IntC[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };			//OutPut  Arg 2 //This Is Optonal since Write only memory can't be read from inside kernel()...

	//Sub fucntion
	cl_int IntD[10] = { 99 , 88, 77, 66, 55, 44, 33, 22, 11,00 };	//Input   Arg 0
	cl_int IntE[10] = { 1,12,23,34,45,56,67,78,89,100 };			//Input	  Arg 1
	cl_int IntF[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };			//OutPut  Arg 2 //This Is Optonal since Write only memory can't be read from inside kernel()...

	//Device Number 0 for first Device, Argument Number 0 for first Argument , 1 to second argument, 2 for thrid argument
	//Add
	MultiDeviceDataAdd.StoreDataForKernelArgument(0, 0, 0, IntA, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataAdd.StoreDataForKernelArgument(0, 1, 0, IntB, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataAdd.StoreDataForKernelArgument(0, 2, 0, IntC, 10 * sizeof(cl_int), Issuccessful);
	//Sub Same as Add
	MultiDeviceDataSub.StoreDataForKernelArgument(1, 0, 0, IntD, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataSub.StoreDataForKernelArgument(1, 1, 0, IntE, 10 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataSub.StoreDataForKernelArgument(1, 2, 0, IntF, 10 * sizeof(cl_int), Issuccessful);

	//Setting our Range
	OCLW_P::cl_MultiDevice_NDRangeStruct MultiNDRange(EntireOpenCLProgram.GetTotalNumberOfDevices(), Issuccessful);
	OCLW_P::cl_NDRangeStruct* NDRangeStructPtrDeviceOne = nullptr;
	OCLW_P::cl_NDRangeStruct* NDRangeStructPtrDeviceTwo = nullptr;
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtrDeviceOne, Issuccessful);//Chose Device Number
	//NDRangeStructPtrDeviceOne->SetNDRange(10, 1, 0);// 10 Work Items, 1 Per WorkGroup, 0 Offset
	NDRangeStructPtrDeviceOne->SetNDRangeWithoutLocal(10, 0);// 10 Work Items, Automatic Per WorkGroup, 0 Offset
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtrDeviceTwo, Issuccessful);//Chose Device Number
	//NDRangeStructPtrDeviceTwo->SetNDRange(10, 1, 0);// 10 Work Items, 1 Per WorkGroup, 0 Offset
	NDRangeStructPtrDeviceTwo->SetNDRangeWithoutLocal(10, 0);// 10 Work Items, Automatic Per WorkGroup, 0 Offset

	//Running the Kernel
	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceDataAdd);
	EntireOpenCLProgram.RunKernelFunction("Sub_Integers", 1, 1, &MultiNDRange, Issuccessful, &MultiDeviceDataSub);

	//Retreiving the Data
	Essenbp::UnknownDataAndSizeStruct RetreivedData;

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(1, 1, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	//InterChange the Buffer with Another Device
	EntireOpenCLProgram.InterchangeBufferWithAnotherDevice(0, 0, 0, 0, 1 ,1, 0, 0, Issuccessful);//Inter Changing Arugment0Buffer0 of Kernel0(Add) with Arugment0Buffer0 of Kernel1(Sub)
	EntireOpenCLProgram.InterchangeBufferWithAnotherDevice(0, 0, 1, 0, 1, 1, 1, 0, Issuccessful);//Inter Changing Arugment1Buffer0 of Kernel0(Add) with Arugment1Buffer0 of Kernel1(Sub)	

	//Running the Kernel
	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, Issuccessful);
	EntireOpenCLProgram.RunKernelFunction("Sub_Integers", 1, 0, &MultiNDRange, Issuccessful);

	//Retreiving the Data Again
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 0, 0, RetreivedData, Issuccessful);// Retrieved Data of 1rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 0 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 1, 0, RetreivedData, Issuccessful);// Retrieved Data of 2rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 1 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(1, 1, 0, 0, RetreivedData, Issuccessful);// Retrieved Data of 1rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 0 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(1, 1, 1, 0, RetreivedData, Issuccessful);// Retrieved Data of 2rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 1 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(1, 1, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	return 0;
}

//Get Binary Data Of Program
int Example7()
{
	bool Issuccessful = false;
	//Initialization Start
	OCLW_P::cl_KernelFunctionsStruct Functions_List(1, Issuccessful);

	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Add_Integers", 3, Issuccessful);

	//NOTE: Checking for failures is a good practice!
	Functions_List.SetMemoryTypeOfArugment(0, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);

	OCLW_P::cl_PlatformVendorStruct AvailablePlatformVendors(Issuccessful);

	int TotalNumberOfPlatforms = 0;
	AvailablePlatformVendors.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, Issuccessful);
	AvailablePlatformVendors.PrintAllAvailablePlatformVendorNames(Issuccessful);
	AvailablePlatformVendors.SetChosenPlatform(1, Issuccessful);

	//std::string FilePath = "D:\\C++ Projects\\Opencl Punal Wrapper\\OpenCL Wrapper by Punal\\ExampleOpenCLKernels.cl";
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("ExampleOpenCLKernels.cl", &Functions_List, &AvailablePlatformVendors, Issuccessful);
	//Initialization End

	Essenbp::ArrayOfUnknownDataAndSize ArrayOfRetreivedData;
	EntireOpenCLProgram.GetBinaryInformationOfProgram(ArrayOfRetreivedData, Issuccessful);

	Essenbp::UnknownDataAndSizeStruct* ReturnValue;
	ArrayOfRetreivedData.GetData(0, &ReturnValue, Issuccessful);

	Essenbp::WriteBytesToFile("BinaryReturn", "txt", *ReturnValue, Issuccessful);
	Essenbp::WriteLogToFile("\n\n Before Destruction");
	//EntireOpenCLProgram.~cl_Program_With_MultiDevice_With_MultiKernelFunctionsStruct();// Doing this just to show that the destructor is working properly
	//Essenbp::WriteLogToFile("\n After  Destruction";

	return 0;
}

void FillIntArray(int* ArrayPtr, int Size, int ValueOffset, int Value)
{
	for (int i = 0; i < Size; ++i)
	{
		ArrayPtr[i] = (Value * i) + ValueOffset;
	}
}

//Basic Add
int StressExample1()
{
	bool Issuccessful = false;
	//Initialization Start
	OCLW_P::cl_KernelFunctionsStruct Functions_List(1, Issuccessful);

	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Add_Integers", 3, Issuccessful);

	//NOTE: Checking for failures is a good practice!
	Functions_List.SetMemoryTypeOfArugment(0, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);

	OCLW_P::cl_PlatformVendorStruct AvailablePlatformVendors(Issuccessful);

	int TotalNumberOfPlatforms = 0;
	AvailablePlatformVendors.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, Issuccessful);
	AvailablePlatformVendors.PrintAllAvailablePlatformVendorNames(Issuccessful);
	AvailablePlatformVendors.SetChosenPlatform(1, Issuccessful);

	//std::string FilePath = "D:\\C++ Projects\\Opencl Punal Wrapper\\OpenCL Wrapper by Punal\\ExampleOpenCLKernels.cl";
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("ExampleOpenCLKernels.cl", &Functions_List, &AvailablePlatformVendors, Issuccessful);
	//Initialization End

	OCLW_P::cl_PerDeviceValuesStruct* DeviceInfo = nullptr;
	EntireOpenCLProgram.GetDeviceInfoStruct(0, &DeviceInfo, Issuccessful);

	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStruct = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Add_Integers", &OrderedStruct, Issuccessful);

	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceData(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStruct, Issuccessful);

	//cl_int IntA[2048] = { 0 };			//Input   Arg 0
	//cl_int IntB[2048] = { 0 };			//Input	  Arg 1
	//cl_int IntC[2048] = { 0 };			//OutPut  Arg 2
	cl_int* IntA = (int*)malloc(2048 * sizeof(cl_int));			//Input   Arg 0
	cl_int* IntB = (int*)malloc(2048 * sizeof(cl_int));			//Input	  Arg 1
	cl_int* IntC = (int*)malloc(2048 * sizeof(cl_int));			//OutPut  Arg 2
	FillIntArray(IntA, 2048, 0, 1);
	FillIntArray(IntB, 2048, 2048, 2);

	//Device Number 0 for first Device, Argument Number 0 for first Argument
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 0, IntA, 2048 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 0, IntB, 2048 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 0, IntC, 2048 * sizeof(cl_int), Issuccessful);

	//Setting our Range
	OCLW_P::cl_MultiDevice_NDRangeStruct MultiNDRange(EntireOpenCLProgram.GetTotalNumberOfDevices(), Issuccessful);
	OCLW_P::cl_NDRangeStruct* NDRangeStructPtr = nullptr;
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtr, Issuccessful);
	//NDRangeStructPtr->SetNDRange(2048, DeviceInfo->MaxWorkItemPerGroup, 0);
	NDRangeStructPtr->SetNDRange(2048, 256, 0);//2048(Total Work Items)/256(Work Items Per Work Group)

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

//Multiply Using Local
int StressExample2()
{
	bool Issuccessful = false;
	//Initialization Start
	OCLW_P::cl_KernelFunctionsStruct Functions_List(1, Issuccessful);

	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Multiply_Integers", 5, Issuccessful);

	//NOTE: Checking for failures is a good practice!
	Functions_List.SetMemoryTypeOfArugment(0, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 3, OCLW_P::cl_Memory_Type::CL_LOCALENUM, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 4, OCLW_P::cl_Memory_Type::CL_PRIVATE, Issuccessful);

	OCLW_P::cl_PlatformVendorStruct AvailablePlatformVendors(Issuccessful);

	int TotalNumberOfPlatforms = 0;
	AvailablePlatformVendors.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, Issuccessful);
	AvailablePlatformVendors.PrintAllAvailablePlatformVendorNames(Issuccessful);
	AvailablePlatformVendors.SetChosenPlatform(1, Issuccessful);

	//std::string FilePath = "D:\\C++ Projects\\Opencl Punal Wrapper\\OpenCL Wrapper by Punal\\ExampleOpenCLKernels.cl";
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("ExampleOpenCLKernels.cl", &Functions_List, &AvailablePlatformVendors, Issuccessful);
	//Initialization End

	OCLW_P::cl_PerDeviceValuesStruct* DeviceInfo = nullptr;
	EntireOpenCLProgram.GetDeviceInfoStruct(0, &DeviceInfo, Issuccessful);

	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStruct = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Multiply_Integers", &OrderedStruct, Issuccessful);

	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceData(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStruct, Issuccessful);

	//cl_int IntA[2048] = { 0 };			//Input   Arg 0
	//cl_int IntB[2048] = { 0 };			//Input	  Arg 1
	//cl_int IntC[2048] = { 0 };			//OutPut  Arg 2
	//cl_int IntD[2048] = { 0 };			//Local	  Arg 3
	//cl_int IntPrivate = 0;				//Private Arg 4 //Total Number Of Local Needed per work item(For this Kernel I wrote)
	cl_int* IntA = (int*)malloc(2048 * sizeof(cl_int));			//Input   Arg 0
	cl_int* IntB = (int*)malloc(2048 * sizeof(cl_int));			//Input	  Arg 1
	cl_int* IntC = (int*)malloc(2048 * sizeof(cl_int));			//OutPut  Arg 2
	//cl_int* IntD = nullptr;									//Local	  Arg 3
	cl_int IntPrivate = 0;										//Private Arg 4
	FillIntArray(IntA, 2048, 0, 1);
	FillIntArray(IntB, 2048, 2048, 2);
	FillIntArray(IntC, 2048, 0, 0);
	//IntD = nullptr;//This is Local
	IntPrivate = ((DeviceInfo->MaxLocalMemoryBytesPerWorkGroup/sizeof(cl_int))/ 2048) - 0;

	//Device Number 0 for first Device, Argument Number 0 for first Argument
	MultiDeviceData.StoreDataForKernelArgument(0, 0, 0, IntA,					2048 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, 0, IntB,					2048 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, 0, IntC,					2048 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 3, 0, nullptr,	IntPrivate *2048 * sizeof(cl_int), Issuccessful);
	MultiDeviceData.StoreDataForKernelArgument(0, 4, 0, &IntPrivate,				   sizeof(cl_int), Issuccessful);

	//Setting our Range
	OCLW_P::cl_MultiDevice_NDRangeStruct MultiNDRange(EntireOpenCLProgram.GetTotalNumberOfDevices(), Issuccessful);
	OCLW_P::cl_NDRangeStruct* NDRangeStructPtr = nullptr;
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtr, Issuccessful);
	//NDRangeStructPtr->SetNDRange(2048, DeviceInfo->MaxWorkItemPerGroup, 0);
	NDRangeStructPtr->SetNDRange(2048, 256, 0);//2048(Total Work Items)/256(Work Items Per Work Group)//For RX 570

	EntireOpenCLProgram.RunKernelFunction("Multiply_Integers", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceData);

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

//Interchange Buffer Within Same Device //Multiply Using Local
int StressExample3()
{
	bool Issuccessful = false;
	//Initialization Start
	//NOTE: Checking for failures is a good practice!
	OCLW_P::cl_KernelFunctionsStruct Functions_List(2, Issuccessful);

	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Multiply_Integers", 5, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 3, OCLW_P::cl_Memory_Type::CL_LOCALENUM, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(0, 4, OCLW_P::cl_Memory_Type::CL_PRIVATE, Issuccessful);;
	Functions_List.SetTheNameAndNumberOfArgumentsForNextKernelFunction("Multiply_IntegersTestTwo", 5, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 0, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 1, OCLW_P::cl_Memory_Type::CL_READ_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 2, OCLW_P::cl_Memory_Type::CL_WRITE_ONLY, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 3, OCLW_P::cl_Memory_Type::CL_LOCALENUM, Issuccessful);
	Functions_List.SetMemoryTypeOfArugment(1, 4, OCLW_P::cl_Memory_Type::CL_PRIVATE, Issuccessful);

	//Querying all the available platform(Example: Intel, AMD, Nvdia)
	OCLW_P::cl_PlatformVendorStruct AvailablePlatformVendors(Issuccessful);
	int TotalNumberOfPlatforms = 0;
	AvailablePlatformVendors.GetTotalNumberOfPlatformVendors(TotalNumberOfPlatforms, Issuccessful);
	AvailablePlatformVendors.PrintAllAvailablePlatformVendorNames(Issuccessful);//Logged in File
	AvailablePlatformVendors.SetChosenPlatform(1, Issuccessful);//Choosing The First Platform

	//std::string FilePath = "D:\\C++ Projects\\Opencl Punal Wrapper\\OpenCL Wrapper by Punal\\ExampleOpenCLKernels.cl";
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("ExampleOpenCLKernels.cl", &Functions_List, &AvailablePlatformVendors, Issuccessful);
	//Initialization End
	OCLW_P::cl_PerDeviceValuesStruct* DeviceInfo = nullptr;
	EntireOpenCLProgram.GetDeviceInfoStruct(0, &DeviceInfo, Issuccessful);


	// Choosing which Function To Use
	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStructAdd = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Multiply_Integers", &OrderedStructAdd, Issuccessful);
	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceDataAdd(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStructAdd, Issuccessful);

	OCLW_P::cl_KernelFunctionArgumentOrderListStruct* OrderedStructSub = nullptr;
	EntireOpenCLProgram.GetKernelInformation("Multiply_IntegersTestTwo", &OrderedStructSub, Issuccessful);
	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceDataSub(EntireOpenCLProgram.GetTotalNumberOfDevices(), OrderedStructSub, Issuccessful);

	// In This Test The Chosen Function Has 3 Arugments
	//Multiply_Integers
	//cl_int IntA[2048] = { 0 };			//Input   Arg 0
	//cl_int IntB[2048] = { 0 };			//Input	  Arg 1
	//cl_int IntC[2048] = { 0 };			//OutPut  Arg 2
	//cl_int IntD[2048] = { 0 };			//Local	  Arg 3
	//cl_int IntPrivate = 0;				//Private Arg 4 //Total Number Of Local Needed per work item(For this Kernel I wrote)
	cl_int* IntA = (int*)malloc(2048 * sizeof(cl_int));			//Input   Arg 0
	cl_int* IntB = (int*)malloc(2048 * sizeof(cl_int));			//Input	  Arg 1
	cl_int* IntC = (int*)malloc(2048 * sizeof(cl_int));			//OutPut  Arg 2
	//cl_int* IntD = nullptr;									//Local	  Arg 3
	cl_int IntPrivate = 0;										//Private Arg 4
	FillIntArray(IntA, 2048, 0, 1);
	FillIntArray(IntB, 2048, 2048, 2);
	FillIntArray(IntC, 2048, 0, 0);
	//IntD = nullptr;//This is Local
	IntPrivate = ((DeviceInfo->MaxLocalMemoryBytesPerWorkGroup / sizeof(cl_int)) / 2048) + -3;// (32768/4) - 1 for RX 570

	//Device Number 0 for first Device, Argument Number 0 for first Argument
	MultiDeviceDataAdd.StoreDataForKernelArgument(0, 0, 0, IntA, 2048 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataAdd.StoreDataForKernelArgument(0, 1, 0, IntB, 2048 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataAdd.StoreDataForKernelArgument(0, 2, 0, IntC, 2048 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataAdd.StoreDataForKernelArgument(0, 3, 0, nullptr, IntPrivate * 2048 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataAdd.StoreDataForKernelArgument(0, 4, 0, &IntPrivate, sizeof(cl_int), Issuccessful);

	//Multiply_Integers_Test
	//cl_int IntA[2048] = { 0 };			//Input   Arg 0
	//cl_int IntB[2048] = { 0 };			//Input	  Arg 1
	//cl_int IntC[2048] = { 0 };			//OutPut  Arg 2
	//cl_int IntD[2048] = { 0 };			//Local	  Arg 3
	//cl_int IntPrivate = 0;				//Private Arg 4 //Total Number Of Local Needed per work item(For this Kernel I wrote)
	cl_int* IntA1 = (int*)malloc(2048 * sizeof(cl_int));			//Input   Arg 0
	cl_int* IntB1 = (int*)malloc(2048 * sizeof(cl_int));			//Input	  Arg 1
	cl_int* IntC1 = (int*)malloc(2048 * sizeof(cl_int));			//OutPut  Arg 2
	//cl_int* IntD = nullptr;									//Local	  Arg 3
	cl_int IntPrivate1 = 0;										//Private Arg 4
	FillIntArray(IntA1, 2048, 4096, 1);
	FillIntArray(IntB1, 2048, 8192, 2);
	FillIntArray(IntC1, 2048, 0, 0);
	//IntD = nullptr;//This is Local
	IntPrivate1 = ((DeviceInfo->MaxLocalMemoryBytesPerWorkGroup / sizeof(cl_int)) / 2048) + -2;// (32768/4) - 1 for RX 570

	//Device Number 0 for first Device, Argument Number 0 for first Argument
	MultiDeviceDataSub.StoreDataForKernelArgument(0, 0, 0, IntA1, 2048 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataSub.StoreDataForKernelArgument(0, 1, 0, IntB1, 2048 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataSub.StoreDataForKernelArgument(0, 2, 0, IntC1, 2048 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataSub.StoreDataForKernelArgument(0, 3, 0, nullptr, IntPrivate1 * 2048 * sizeof(cl_int), Issuccessful);
	MultiDeviceDataSub.StoreDataForKernelArgument(0, 4, 0, &IntPrivate1, sizeof(cl_int), Issuccessful);

	//Setting our Range
	OCLW_P::cl_MultiDevice_NDRangeStruct MultiNDRange(EntireOpenCLProgram.GetTotalNumberOfDevices(), Issuccessful);
	OCLW_P::cl_NDRangeStruct* NDRangeStructPtr = nullptr;
	MultiNDRange.GetNDRangeOfDevice(0, &NDRangeStructPtr, Issuccessful);//Chose Device Number
	//NDRangeStructPtr->SetNDRange(10, 1, 0);// 10 Work Items, 1 Per WorkGroup, 0 Offset
	NDRangeStructPtr->SetNDRange(2048, 256, 0);//2048(Total Work Items)/256(Work Items Per Work Group)//For RX 570

	//Running the Kernel
	EntireOpenCLProgram.RunKernelFunction("Multiply_Integers", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceDataAdd);
	EntireOpenCLProgram.RunKernelFunction("Multiply_IntegersTestTwo", 0, 0, &MultiNDRange, Issuccessful, &MultiDeviceDataSub);

	//Retreiving the Data
	Essenbp::UnknownDataAndSizeStruct RetreivedData;

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 1, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	//InterChange the Buffer with same Device
	EntireOpenCLProgram.InterchangeBufferWithinSameDevice(0, 0, 0, 0, 1, 0, 0, Issuccessful);//Inter Changing Arugment0Buffer0 of Kernel0(Add) with Arugment0Buffer0 of Kernel1(Sub)
	EntireOpenCLProgram.InterchangeBufferWithinSameDevice(0, 0, 1, 0, 1, 1, 0, Issuccessful);//Inter Changing Arugment1Buffer0 of Kernel0(Add) with Arugment1Buffer0 of Kernel1(Sub)		
	EntireOpenCLProgram.InterchangeBufferWithinSameDevice(0, 0, 4, 0, 1, 4, 0, Issuccessful);//Inter Changing Arugment4Buffer0 of Kernel0(Add) with Arugment4Buffer0 of Kernel1(Sub)	

	//Running the Kernel
	EntireOpenCLProgram.RunKernelFunction("Multiply_Integers", 0, 0, &MultiNDRange, Issuccessful);
	EntireOpenCLProgram.RunKernelFunction("Multiply_IntegersTestTwo", 0, 0, &MultiNDRange, Issuccessful);

	//Retreiving the Data Again
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 0, 0, RetreivedData, Issuccessful);// Retrieved Data of 1rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 0 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 1, 0, RetreivedData, Issuccessful);// Retrieved Data of 2rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 1 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 0, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Add
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Add:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}

	EntireOpenCLProgram.RetreiveDataFromKernel(0, 1, 0, 0, RetreivedData, Issuccessful);// Retrieved Data of 1rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 0 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 1, 1, 0, RetreivedData, Issuccessful);// Retrieved Data of 2rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 1 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	EntireOpenCLProgram.RetreiveDataFromKernel(0, 1, 2, 0, RetreivedData, Issuccessful);// Retrieved Data of 3rd Argument() of Sub
	Essenbp::WriteLogToFile("\n Retreived Data of Arg 2 of Sub:-");
	for (int i = 0; i < RetreivedData.GetDataSize() / sizeof(cl_int); ++i)
	{
		Essenbp::WriteLogToFile("\n Data[" + std::to_string(i) + "] == " + std::to_string(((int*)RetreivedData.GetData())[i]));
	}
	return 0;
}

int main()
{
	//Basic Quick Test
	Example1();
	Example2();
	Example3();
	Example4();
	Example5();
	////Example6();//NOTE: ONLY FOR MULTI-GPU
	Example7();
	
	//Heavy Stress Test
	StressExample1();
	StressExample2();
	StressExample3();
}