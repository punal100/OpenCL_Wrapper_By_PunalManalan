#include "OCLW_P.h"

int main()
{
	bool IsSuccesful = false;

	// Single Line Code, First platform is chosen by default 
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("PunalOpenclFunctionsProgram.cl", IsSuccesful);//NOTE: OCLW_P::cl_PlatformVendorStruct can be used to choose platform of your choice
	OCLW_P::cl_MultiDevice_KernelArgumentSendStruct MultiDeviceData(EntireOpenCLProgram.GetKernelInformation("Add_Integers", IsSuccesful), IsSuccesful);	
	MultiDeviceData.SetNumberOfDevices(1,IsSuccesful);// We use only 1 GPU right now

	int IntA[10] = { 1,2,3,4,5,6,7,8,9,10};//Input
	int IntB[10] = { 11,12,13,14,15,16,17,18,19,20};//Input
	int IntC[10] = { 0,0,0,0,0,0,0,0,0,0 };//OutPut

	//Device Number 0 for first Device, Argument Number 0 for first Argument
	MultiDeviceData.StoreDataForKernelArgument(0, 0, IntA, 10 * sizeof(int), IsSuccesful);
	MultiDeviceData.StoreDataForKernelArgument(0, 1, IntB, 10 * sizeof(int), IsSuccesful);
	MultiDeviceData.StoreDataForKernelArgument(0, 2, IntC, 10 * sizeof(int), IsSuccesful);

	//Setting our Range
	OCLW_P::cl_MultiDevice_NDRangeStruct MultiNDRange(EntireOpenCLProgram.GetTotalNumberOfDevices(), IsSuccesful);
	MultiNDRange.GetNDRangeOfDevice(0, IsSuccesful)->SetNDRange(10, 1, 0);

	EntireOpenCLProgram.RunKernelFunction("Add_Integers", 0, 0, &MultiNDRange, IsSuccesful);
	return 0;
}