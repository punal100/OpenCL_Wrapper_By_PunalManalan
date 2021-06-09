# OpenCL_Wrapper By Punal Manalan
Welcome To OpenCL Wrapper Source Code!
Solely Written By Punal Manalan.

Free to Use, Whenever and whereever needed!

# Information About This Wrapper
1. LightWeight, Easy to use **Header-Only** "OCLW_P.h" Wrapper for OpenCL.
2. 'OCLW_P::OpenCLWrapper' This Single line of code does Everything(Including Adding Functions) Automaticaly(*NOTE*: Everything is Automatic, but can also be done manually).
3. Written Using only "CL/cl.h" and has backward compatblity upto OpenCL 1.2!
3. No need to maually Add kernel function or figure out argument types.
4. The Entire OpenCL Program File is Read and automaticaly Assignes Kernel Functions Ready for use!
5. Information about each kernel function and it's argument types can be Accesed with **getter**functions().
6. Works On **Multi-Platforms** and **Multi-GPUs**.
7. Sharing of Work-Load between **Multi-GPUs** Can be acheived anytime during runtime!
9. Compiles and Runs on Windows, Linux and Mac.
10. This Wrapper specializes in Heavy Computation on GPU.
11. From Constrction to Destruction everything is Done automaticaly!
12. No need for manual destruction and releasing cl resources!
13. No need to write Tedious amount of code to counter memory leaks!
14. Upon going out of scope or exiting the program Every resource that is used is safely Released and Deleted.
15. Errors are Logged as a .txt file Upon Exit (**NOTE**:"OCLW_P.h" is tested bug free!).

# Minimal working Exampless
## Example 1:
```c++
#include "OCLW_P.h"

int main()
{
	bool IsSuccesful = false;

	// Single Line Code, First platform is chosen by default 
	OCLW_P::OpenCLWrapper EntireOpenCLProgram("PunalOpenclFunctionsProgram.cl", IsSuccesful);//NOTE: OCLW_P::cl_PlatformVendorStruct can be used to choose platform of your choice
	if (IsSuccesful)
	{
		EntireOpenCLProgram.RunKernelFunction();//Pending
		return 1;
	}
	return 0;
}
```
